#ifndef IOPIPELINE_H
#define IOPIPELINE_H
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace Pipeline {
	//If arguments are good returns a cleaned version of them
	//First one is input, second one is output
	std::array <std::wstring, 2> CheckArguments (const std::wstring Arg1, const std::wstring Arg2);
	//If argument is good for selected mode returns a cleaned version of it
	//True = Input
	//False = Output
	std::wstring CheckArgument (const std::wstring Arg, bool Mode);

	extern std::unique_ptr<std::wistream> PipeInput;
	extern std::unique_ptr<std::wostream> PipeOutput;

	//1 = Only Input is set
	//2 = Only Output is set
	//3 = Both are set
	//0 = Nothing was changed
	//-1 = Bad Input (Output is never checked in this case)
	//-2 = Bad Output
	//-3 = Invalid number of arguments
	// -4 = Unforseen error
	int MakePipe (const std::vector<std::wstring> & Args);
}
#endif
