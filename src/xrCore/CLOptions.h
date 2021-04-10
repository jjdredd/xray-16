#pragma once

#if !defined(_CPPUNWIND)
#error Please enable exceptions...
#endif

#include <exception>

#include "xrCommon/xr_list.h"
#include "xrstring.h"
#include "_std_extensions.h"

class XRCORE_API CLOptionBase {

public:
	CLOptionBase(char *opname, bool req);

private:
	char* option_name;
	bool required;
	bool provided = false;
};


template<typename T>
class XRCORE_API CLOption : public CLOptionBase {
	
	struct ArgMissingException : public exception {
		const char * what() const throw() {
			return "Missing Argument";
		}
	}

	struct ArgParamException : public exception {
		const char * what() const throw() {
			return "Missing Argument Parameter";
		}
	}
	
public:
	CLOption(char *flag_name, T defval);
	CLOption(char *flag_name, bool req);
	void CheckArguments();
	bool IsProvided();
	T OptionValue();

private:
	static ParseCommandLine(int argc, char *argv[]);
	static xr_list<OptionBase *> options;
	T argument;
};
