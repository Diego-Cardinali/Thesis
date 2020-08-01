#include <jsoncons/json.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <iterator>

#include "[IO]Pipeline.h"
#include "ExploreDirectory.h"  //requires Boost
#include "FileManip.h"

/**************************************************************
|		This code converts specifically formatted .txt        |
|		documents to .json. It aws hardcoded for The          |
|		type of files I need it to convert.					  |
**************************************************************/

jsoncons::wojson ProcessData (std::filesystem::path FileName) {
	jsoncons::wojson ProcessedData;
	ProcessedData.insert_or_assign(L"List",  jsoncons::json_array_arg);
	std::wstringstream DataStream = FileManip::DataInput(FileName);
	std::wstring Line;
	while (std::getline(DataStream, Line)) {
		//{
     		std::wistringstream Temp(Line);
      		//std::copy(std::istream_iterator<double>(Temp), std::istream_iterator<double>(), std::back_inserter(Numbers));
		//}
		std::vector<double> Numbers {std::istream_iterator<double, wchar_t>{Temp}, std::istream_iterator<double, wchar_t>()}; 
		jsoncons::wojson Coordinates;
		Coordinates.insert_or_assign(L"Coordinates", jsoncons::json_array_arg);
		Coordinates[L"Coordinates"].push_back(Numbers[1]);
		Coordinates[L"Coordinates"].push_back(Numbers[2]);
		Coordinates[L"Coordinates"].push_back(Numbers[3]);
		ProcessedData[L"List"].push_back(Coordinates);
		//Numbers.clear();
	}
	return ProcessedData;
}

int main (int Nargs, char** Args) {
	std::vector<std::wstring> IOArgs;
	if (Nargs > 1) {
		//The paths are necessary for conversion.
		IOArgs.push_back(std::filesystem::path(Args[1]).wstring());
	}
	if (Nargs > 2) {
		//The paths are necessary for conversion.
		IOArgs.push_back(std::filesystem::path(Args[2]).wstring());
	}
	int IOCheck = Pipeline::MakePipe (IOArgs);
	if (IOCheck >= 0) {
		std::wistream & Input = ((IOCheck == 3) or (IOCheck == 1)) ? *Pipeline::PipeInput : std::wcin;
		//std::wostream & Output = ((IOCheck == 3) or (IOCheck == 2)) ? *Pipeline::PipeOutput : std::wcout;

		//Process instructions
		jsoncons::wojson Instructions;
		Instructions = jsoncons::wojson::parse(Input);
		std::filesystem::path InputPath = Instructions[L"InputPath"].as<std::wstring>();
		std::vector<std::wstring> InputNames = Instructions[L"InputNames"].as<std::vector<std::wstring>>();
		std::filesystem::path OutputPath = Instructions[L"OutputPath"].as<std::wstring>();
		std::vector<std::wstring> OutputNames = Instructions[L"OutputNames"].as<std::vector<std::wstring>>();

		if (InputNames.size() != OutputNames.size()) {
			std::wcerr << L"Error: Number of input and output names do not match."<<std::endl;
			return 1;
		}
		for (unsigned long int I = 0; I != InputNames.size(); ++I) {
			std::vector<std::filesystem::path> FilesToConvert = Explore::ExploreDirectoryByName(InputNames[I], InputPath);
			for (unsigned long int J = 0; J != FilesToConvert.size(); ++J) {
				jsoncons::wojson ProcessedData = ProcessData (InputPath/FilesToConvert[J]);
				std::wstring Number;
				{
					std::wstringstream Temp;
					Temp << std::setw(3) << std::setfill(L'0') << J+1;
					Number = Temp.str();
				}
				std::filesystem::path Output =  OutputPath/std::wstring(OutputNames[I]+L"_"+Number+L".json");
				std::wofstream OutStream (Output);
				OutStream << ProcessedData;
				OutStream.close();
			}
		}
	}
	return 0;
}