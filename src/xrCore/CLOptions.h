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
public:
	CLOption(const char *flag_name, const T defval, bool req);
	CLOption(const char *flag_name, bool req);
    ~CLOption();
	bool IsProvided() const;          // was the option provided in the CLI?
	T OptionValue() const;            // value provided with the option

    static void CheckArguments();
	friend void ParseCommandLine(int argc, char *argv[]);

private:
	xr_string option_name;
	bool required;
	bool provided = false;

	T argument;

    static typename xr_list<CLOption<T> *>::iterator find_option(const xr_string &flag_name);
    static bool parse_option(const char *option, const char *arg);
    static xr_list<CLOption<T> *> options;
};

// exception

struct CLOptionMissing : public std::exception
{
    xr_string option_error;

    CLOptionMissing(xr_string &opt_err) : option_error(opt_err) {}
    ~CLOptionMissing() = default;
    const char * what() const throw()
    {
        return option_error.c_str(); // missing argument
    }
};

struct CLOptionParam : public std::exception
{
    xr_string option_error;

    CLOptionParam(xr_string &opt_err) : option_error(opt_err) {}
    ~CLOptionParam() = default;
    const char * what() const throw()
    {
        return option_error.c_str(); // missing argument parameter
    }
};

inline static bool IsOptionFlag(const char *buf)
{
    return (buf && buf[0] == '-');
}

void XRCORE_API ParseCommandLine(int argc, char **argv);
void XRCORE_API CLCheckAllArguments();
