#include "SWI_Kernel.hpp"

#include <string>
#include <wx/string.h>
#include "BitMath.h"

void KRN_kprintf(const char *format, int argc, void **argv)
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
