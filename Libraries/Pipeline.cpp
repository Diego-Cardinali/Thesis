#include "Pipeline.h"

//If arguments are good returns a cleaned version of them
//First one is input, second one is output
std::array <std::wstring, 2> Pipeline::CheckArguments (const std::wstring Arg1, const std::wstring Arg2) {
	std::array<std::wstring, 2> Arguments;
	//Input
	if (!Arg1.rfind(L"-i", 0)) {
		Arguments[0] = Arg1.substr(2, Arg1.size());
	}
	else if (!Arg2.rfind(L"-i", 0)) {
		Arguments[0] = Arg2.substr(2, Arg2.size());
	}
	//Output
	if (!Arg1.rfind(L"-o", 0)) {
		Arguments[1] = Arg1.substr(2, Arg1.size());
	}
	else if (!Arg2.rfind(L"-o", 0)) {
		Arguments[1] = Arg2.substr(2, Arg2.size());
	}
	return Arguments;
}

std::unique_ptr<std::wistream> Pipeline::PipeInput;
std::unique_ptr<std::wostream> Pipeline::PipeOutput;

//If argument is good for selected mode returns a cleaned version of it
//True = Input
//False = Output
std::wstring Pipeline::CheckArgument (const std::wstring Arg, bool Mode) {
	std::wstring Argument;
	if (Mode) {
		if (!Arg.rfind(L"-i", 0)) {
			Argument = Arg.substr(2, Arg.size());
		}
	}
	else {
		if (!Arg.rfind(L"-o", 0)) {
			Argument = Arg.substr(2, Arg.size());
		}
	}
	return Argument;
}

//1 = Only Input is set
//2 = Only Output is set
//3 = Both are set
//0 = Nothing was changed
//-1 = Bad Input (Output is never checked in this case)
//-2 = Bad Output
//-3 = Invalid number of arguments
// -4 = Unforseen error
int Pipeline::MakePipe (const std::vector<std::wstring> & Args) {
	if (!Args.size()) {
		return 0;
	}
	else if (Args.size() == 1) {
		std::wstring Argument = Pipeline::CheckArgument (Args[0], true);
		if (Argument.size()) {
			Pipeline::PipeInput.reset(new std::wifstream (std::filesystem::path(Argument)));
			if (!Pipeline::PipeInput) {
				std::wcerr << L"Bad input file\n"<< std::flush;
				return -1;
			}
			return 1;
		}
		else {
			Argument = CheckArgument (Args[0], false);
			if (Argument.size()) {
				Pipeline::PipeOutput.reset(new std::wofstream (std::filesystem::path(Argument)));
				if (!Pipeline::PipeOutput) {
					std::wcerr << L"Bad output file\n"<< std::flush;
					return -2;
				}
				return 2;
			}
		}
	}
	else if (Args.size() == 2) {
		int Counter = 0;
		std::array<std::wstring, 2> IOArgs = Pipeline::CheckArguments (Args[0], Args[1]);
		if (IOArgs[0].size()) {
			Pipeline::PipeInput.reset(new std::wifstream (std::filesystem::path(IOArgs[0])));
			if (!Pipeline::PipeInput) {
				std::wcerr << L"Bad input file\n"<< std::flush;
				return -1;
			}
			++Counter;
		}
		if (IOArgs[1].size()) {
			Pipeline::PipeOutput.reset(new std::wofstream (std::filesystem::path(IOArgs[1])));
			if (!Pipeline::PipeOutput) {
				std::wcerr << L"Bad output file\n"<< std::flush;
				return -2;
			}
			Counter += 2;
		}
		if (Counter) {return Counter;}
	}
	else {
		std::wcerr << L"Invalid number of arguments\n"<< std::flush;
		return -3;
	}
return -4;
}