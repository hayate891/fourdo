#include <string>
#include <wx/string.h>

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
			for (int i = 0; i < argc; i++)
			{
				size_t start        = 0;
				size_t substitution = format_str.find('%');

				if (substitution != std::string::npos)
				{
					// 
					// ignore this % if it's followed by another %
					// 
					if (format_str.at(substitution + 1) != '%')				
					{
						// 
						// find out what type we're dealing with
						// and append this substr along with it's arg
						// onto the final string
						// 
						// TODO: theres more to this
						switch (format_str.at(substitution + 1))
						{
							case 'i':
								// the +2 is so we can get past both the % and the type identifier
								final_str += wxString::Format(
									format_str.substr(start, ((substitution + 2) - start)).c_str(), 
									*(static_cast<int *>(argv[i]))
								);

								break;
							case 'f':
								final_str += wxString::Format(
									format_str.substr(start, (substitution - start)).c_str(), 
									*(static_cast<float *>(argv[i]))
								);

								break;
							case 'c':
								final_str += wxString::Format(
									format_str.substr(start, (substitution - start)).c_str(), 
									*(static_cast<char *>(argv[i]))
								);

								break;
							case 's':
								final_str += wxString::Format(
									format_str.substr(start, (substitution - start)).c_str(), 
									static_cast<char *>(argv[i])
								);

								break;
							default:
								final_str += wxString::Format(
									format_str.substr(start, (substitution - start)).c_str(), 
									argv[i]
								);

								break;
						}

						// + 2 to get past the % and the type identifier
						start = substitution + 2;
					}

					substitution = format_str.find('%', substitution + 2);
				}
				
				// 
				// if we have extra characters at the end of the string,
				// append them now
				// 
				if (start < substitution)
					final_str += wxString::Format(format_str.substr(start).c_str());

				std::printf(final_str.c_str());
			}
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
		int arg_count       = 0;

		if (substitution != std::string::npos)
		{
			// 
			// we don't want to count this as an arg if it's two %'s in a row.
			// skip the % if we find it
			// 
			if (format.at(substitution + 1) == '%')
				substitution++;
			else
				arg_count++;

			substitution = format.find('%', substitution + 1);
		}

		// 
		// now grab all the arguments we need from
		// the registers and/or the stack
		// 
		void **args = NULL;
		if (arg_count > 0)
		{
			args = new void*[arg_count];

			// 
			// note that we have to add 1 when we grab our arguments
			// from the registers.  this is because the format str is 
			// in the first register, and we don't need that
			// 
			for (int i = 0; i < arg_count; i++)
			{
				// TODO: double check this
				if (i <= 5)
					args[i] = static_cast<void *>(&registers->USER[i + 1]);
				else
					// TODO: check this
					args[i] = static_cast<void *>(dma->GetRAMPointer(registers->USER[13] - ((i - 6) * 4)));
			}
		}

		// 
		// execute our swi
		// 
		execute_printf(format.c_str(), arg_count, args);

		// 
		// cleanup
		// 
		if (args)
			delete[] args;
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
