#pragma once

#if !defined(_CPPUNWIND)
#error Please enable exceptions...
#endif

#include <exception>

#include "xrCommon/xr_list.h"
#include "xrstring.h"
#include "_std_extensions.h"

template<typename T> class XRCORE_API CLOption;

template<typename T>
class XRCORE_API CLOption
{

	struct ArgMissingException : public std::exception
    {
        char *option_error;

        ArgMissingException(char *opt_err)
        {
            option_error = xr_strdup(opt_err);
        }

        ~ArgMissingException()
        {
            xr_free(option_error);
        }

        const char * what() const throw()
        {
			return option_error; // missing argument
		}
	};

	struct ArgParamException : public std::exception
    {
        char *option_error;

        ArgParamException(char *opt_err)
        {
            option_error = xr_strdup(opt_err);
        }

        ~ArgParamException()
        {
            xr_free(option_error);
        }

        const char * what() const throw()
        {
			return option_error; // missing argument parameter
		}
	};
	
public:
	CLOption(char *flag_name, T defval, bool req);
	CLOption(char *flag_name, bool req);
    ~CLOption();
	bool IsProvided();          // was the option provided in the CLI?
	T OptionValue();            // value provided with the option

    static void CheckArguments();
	friend void ParseCommandLine(int argc, char *argv[]);

private:
	char *option_name = nullptr;
	bool required;
	bool provided = false;

	T argument;

    static typename xr_list<CLOption<T> *>::iterator find_option(char *flag_name);
	static xr_list<CLOption<T> *> options;
};


void CLCheckAllArguments()
{
    CLOption<int>::CheckArguments();
    CLOption<bool>::CheckArguments();
    CLOption<char *>::CheckArguments();
}
