#include <string>
#include <vector>
#include <wx/string.h>

#include "BitMath.h"
#include "KernelFaker.h"
#include "SWIHandler.h"

#include "SWI_ARM.hpp"
#include "SWI_Meta.hpp"

namespace fourdo { namespace swi
{
	
	void execute_printf(const char *format, int argc, void **argv)
	{
		// 
		// it's only difficult when we have params
		// 
		if (argc > 0)
		{
			std::string format_str(format);
			wxString final_str;

			// 
			// loop through each of our params and find it's associated 
			// % symbol and interpolate it into the string
			// 
			size_t start        = 0;
			size_t position     = 0;
			for (int i = 0; i < argc; )
			{
				position = format_str.find('%', start);

				if (position == std::string::npos)
					break;
				
				// 
				// move past the % we just found
				// 
				position++;

				// 
				// ignore this % if it's followed by another %
				// 
				if (format_str.at(position) != '%')				
				{
					// 
					// the length of the substring we're substituting (i.e. %lu)
					// 
					size_t substr_length = position - start;

					// 
					// the var we will be interpolating into this substr
					// 
					void   *var = NULL;
					uint32 stack_value;
					
					if (i < 3)
						var = argv[i];
					else
					{
						stack_value = ByteSwap(*(static_cast<uint32 *>(argv[i])));
						var = &stack_value;
					}

					bool finished    = false;
					bool long_int    = false;
					bool short_int   = false;
					bool long_double = false;
					for (;;)
					{
						// 
						// find out what type we're dealing with
						// and append this substr along with it's arg
						// onto the final string
						// 
						switch (format_str.at(position))
						{
							// 
							// int
							// 
							case 'i':
							case 'd':
							case 'o':
							{
								finished = true;
								substr_length++;

								final_str += wxString::Format(
									reinterpret_cast<const wxChar *>(format_str.substr(start, substr_length).c_str()), 
									*(static_cast<int *>(var))
								);

								break;
							}
							// 
							// unsigned int
							// 
							case 'u':
							case 'x':
							case 'X':
							{
								finished = true;
								substr_length++;

								final_str += wxString::Format(
									reinterpret_cast<const wxChar *>(format_str.substr(start, substr_length).c_str()),
									*(static_cast<unsigned int *>(var))
								);

								break;
							}
							// 
							// float
							// 
							case 'f':
							{
								finished = true;
								substr_length++;

								final_str += wxString::Format(
									reinterpret_cast<const wxChar *>(format_str.substr(start, substr_length).c_str()),
									*(static_cast<float *>(var))
								);

								break;
							}
							// 
							// char
							// 
							case 'c':
							{
								finished = true;
								substr_length++;

								final_str += wxString::Format(
									reinterpret_cast<const wxChar *>(format_str.substr(start, substr_length).c_str()),
									*(static_cast<char *>(var))
								);

								break;
							}
							// 
							// c string
							// 
							case 's':
							{
								finished = true;
								substr_length++;

								final_str += wxString::Format(
									reinterpret_cast<const wxChar *>(format_str.substr(start, substr_length).c_str()),
									static_cast<char *>(var)
								);

								break;
							}
							// 
							// long width
							// 
							case 'l':
							{
								substr_length++;
								long_int = true;
								break;
							}
							// 
							// short int
							// 
							case 'h':
							{
								substr_length++;
								short_int = true;
								break;
							}
							// 
							// long double
							// 
							case 'L':
							{
								substr_length++;
								long_double = true;
								break;
							}
							// 
							// formatting that we can ignore
							// 
							case '+':
							case '-':
							case ' ':
							case '#':
							case '*':
							case '.':
							case '0':
							case '1':
							case '2':
							case '3':
							case '4':
							case '5':
							case '6':
							case '7':
							case '8':
							case '9':
							{
								substr_length++;
								break;
							}
							default:
							{
								break;
							}
						}

						if (finished)
							break;
						else
							position++;
					}

					// 
					// increment our argument counter
					// 
					i++;
				}

				// 
				// start our search from the next character
				// 
				start = position + 1;
			}
				
			// 
			// if we have extra characters at the end of the string,
			// append them now
			// 
			if (start < format_str.size())
				final_str += wxString::Format(reinterpret_cast<const wxChar *>(format_str.substr(start).c_str()));

			std::printf(reinterpret_cast<const char *>(final_str.c_str()));
		}
		else
			std::printf(format);
	}

	void prepare_printf(ARMRegisters *registers, DMAController *dma)
	{
		std::string format(reinterpret_cast<const char *>(dma->GetRAMPointer(registers->USER[0])));

		// 
		// parse the format string to determine how many more arguments 
		// we need to get from registers and the stack
		// 
		size_t substitution = format.find('%');
		std::vector<void *> argv;

		while (substitution != std::string::npos)
		{
			switch (format.at(substitution + 1))
			{
				case 's':
					if (argv.size() >= 3)
						argv.push_back(
							static_cast<void *>(
								dma->GetRAMPointer(registers->USER[13] + ((argv.size() - 3) * 4))
							)
						);
					else
						argv.push_back(
							static_cast<void *>(
								dma->GetRAMPointer(registers->USER[argv.size() + 1])
							)
						);
					break;
				case '%':
					break;
				default:
					if (argv.size() >= 3)
					{
						argv.push_back(
							static_cast<void *>(
								dma->GetRAMPointer(registers->USER[13] + ((argv.size() - 3) * 4))
							)
						);
					}
					else
						argv.push_back(static_cast<void *>(&registers->USER[argv.size() + 1]));

					break;
			}

			substitution = format.find('%', substitution + 1);
		}

		// 
		// execute our swi
		// 
		execute_printf(
			format.c_str(), 
			argv.size(), 
			(argv.size() ? &argv.front() : NULL));
	}

	// 
	// namespace scope initialization
	// 

	std::map<uint32, swiHandler> initSwiMap()
	{
		std::map<uint32, swiHandler> map;

		map.insert(std::pair<uint32, swiHandler>(0x00010, &fourdo::swi::SWI_ARM_GetMemoryBaseAddress));
		map.insert(std::pair<uint32, swiHandler>(0x00011, &fourdo::swi::SWI_ARM_HaltExecution));
		
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IMAGE_ENTRY_POINT, &fourdo::swi::SWI_META_ImageEntryPoint));
		
		map.insert(std::pair<uint32, swiHandler>(0x1000e, &fourdo::swi::prepare_printf));

		return map;
	}

	std::map<uint32, swiHandler> swiToHandlerMap = initSwiMap();
	
	///////////////////////////////////////////////////////////////
	// ARM Interrupts
	// 
	void SWI_ARM_GetMemoryBaseAddress(ARMRegisters *registers, DMAController *dma)
	{
		//registers->USER[0] = ARM_GetMemoryBaseAddress();
	}
	
	void SWI_ARM_HaltExecution(ARMRegisters *registers, DMAController *dma)
	{
		//ARM_HaltExecution();
	}

	///////////////////////////////////////////////////////////////
	// Meta Interrupts
	//
	void SWI_META_ImageEntryPoint(ARMRegisters *registers, DMAController *dma)
	{
		//META_ImageEntryPoint();
	}
}
}
