#include "CLOptions.h"
#include "xrMemory.h"
#include "log.h"


// CLOption

// when adding make sure it's beginning with a '-'
template<typename T>
CLOption<T>::CLOption(const char *opname, const T defval, bool req)
    : option_name(opname), provided(true), required(false), argument(defval)
{
    if(IsOptionFlag(option_name.c_str())) option_name.insert(option_name.begin(), '-');
    options.push_back(this);
}

template<typename T>
CLOption<T>::CLOption(const char *opname, bool req)
    : option_name(opname), provided(false)
{
    options.push_back(this);
}

template<typename T>
typename xr_list<CLOption<T> *>::iterator CLOption<T>::find_option(const char *flag_name)
{
    typename xr_list<CLOption<T> *>::iterator it;
    for(it = CLOption<T>::options.begin(); it != CLOption<T>::options.end(); it++)
    {
        if (!(*it)->option_name.compare(flag_name)) break;
    }
    return it;
}

template<typename T>
CLOption<T>::~CLOption()
{
    auto it = find_option(option_name);
    if (it != options.end()) options.erase(it);
}

template<typename T>
bool CLOption<T>::IsProvided() const
{
    return provided;
}

template<typename T>
T CLOption<T>::OptionValue() const
{
    return argument;
}

template<typename T>
void CLOption<T>::CheckArguments()
{
    typename xr_list<CLOption<T> *>::iterator it;
    for(it = CLOption<T>::options.begin(); it != CLOption<T>::options.end(); it++)
    {
        if((*it)->required && !(*it)->provided)
        {
            throw CLOptionMissing((*it)->option_name);
        }
    }
}


template<>
bool CLOption<bool>::parse_option(const char *option, const char *arg)
{
    auto it = find_option(option);
    if(it != options.end()) return false; // not found

    // found -> must be of type bool
    CLOption<bool> *o = *it;

    o->argument = true;
    o->provided = true;
    return true;                // found and set
}

template<>
bool CLOption<int>::parse_option(const char *option, const char *arg)
{
    auto it = find_option(option);
    if(it != options.end()) return false; // not found

    // found -> must be of type int
    CLOption<int> *o = *it;

    o->argument = std::stoi(arg); // may throw
    o->provided = true;
    return true;
}

template<>
bool CLOption<xr_string>::parse_option(const char *option, const char *arg)
{
    auto it = find_option(option);
    if(it != options.end()) return false; // not found

    // found -> must be of type string
    CLOption<xr_string> *o = *it;

    o->argument = arg;
    o->provided = true;
    return true;
}

void ParseCommandLine(const int argc, const char *argv[])
{
    // put these back into class methods?
    for(int n = 0; n < argc; n++)
    {
        if(IsOptionFlag(argv[n + 1]))
        {
            Msg("Unknown option <%s>", argv[n]);
            continue;
        }

        // is this a bool option?
        if(CLOption<bool>::parse_option(argv[n], nullptr))
        {
            continue;
        }
        // the rest of the flags will require an argument
        else if(n + 1 >= argc || IsOptionFlag(argv[n + 1]))
        {
            xr_string s(argv[n]);
            throw CLOptionParam(s);
        }
        else if(CLOption<int>::parse_option(argv[n], argv[n + 1])
                || CLOption<xr_string>::parse_option(argv[n], argv[n + 1]))
        {
            n++;
            continue;
        }

        Msg("Unknown option <%s>", argv[n]);
    }
}
