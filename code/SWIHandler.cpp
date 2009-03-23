#include <string>
#include <vector>
#include <wx/string.h>

#include "BitMath.h"
#include "SWIHandler.h"

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
			size_t substitution = 0;
			for (int i = 0; i < argc; i++)
			{
				substitution = format_str.find('%', start);

				if (substitution != std::string::npos)
				{
					// 
					// ignore this % if it's followed by another %
					// 
					if (format_str.at(substitution + 1) != '%')				
					{
						// TODO: handle flags, width, precision, and length

						// 
						// the substring that we'll be appending to our final string
						// 
						std:: string temp_format = format_str.substr(start, ((substitution + 2) - start));

						// 
						// find out what type we're dealing with
						// and append this substr along with it's arg
						// onto the final string
						// 
						switch (format_str.at(substitution + 1))
						{
							// 
							// int
							// 
							case 'i':
							case 'd':
							case 'o':
							{
								final_str += wxString::Format(
									temp_format.c_str(), 
									(i < 3 ? *(static_cast<int *>(argv[i])) : static_cast<int>(ByteSwap(*(static_cast<uint32 *>(argv[i])))))
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
								final_str += wxString::Format(
									temp_format.c_str(),
									(i < 3 ? *(static_cast<unsigned int *>(argv[i])) : static_cast<unsigned int>(ByteSwap(*(static_cast<uint32 *>(argv[i])))))
								);

								break;
							}
							// 
							// float
							// 
							case 'f':
							{
								final_str += wxString::Format(
									temp_format.c_str(),
									(i < 3 ? *(static_cast<float *>(argv[i])) : static_cast<float>(ByteSwap(*(static_cast<uint32 *>(argv[i])))))
								);

								break;
							}
							// 
							// char
							// 
							case 'c':
							{
								final_str += wxString::Format(
									temp_format.c_str(),
									(i < 3 ? *(static_cast<char *>(argv[i])) : static_cast<char>(ByteSwap(*(static_cast<uint32 *>(argv[i])))))
								);

								break;
							}
							// 
							// c string
							// 
							case 's':
							{
								final_str += wxString::Format(
									temp_format.c_str(),
									static_cast<char *>(argv[i])
								);

								break;
							}
							default:
							{
								final_str += wxString::Format(
									temp_format.c_str(), 
									argv[i]
								);

								break;
							}
						}

						// + 2 to get past the % and the type identifier
						start = substitution + 2;
					}

					substitution = format_str.find('%', substitution + 2);
				}
			}
				
			// 
			// if we have extra characters at the end of the string,
			// append them now
			// 
			if (start < substitution)
				final_str += wxString::Format(format_str.substr(start).c_str());

			std::printf(final_str.c_str());
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

		map.insert(std::pair<uint32, swiHandler>(0x1000e, &fourdo::swi::prepare_printf));

		return map;
	}

	std::map<uint32, swiHandler> swiToHandlerMap = initSwiMap();
}
}
