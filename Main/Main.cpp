#include <iostream>
#include <jsoncons/json.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <array>
#include <iterator>
#include <functional>
#include <variant>
#include <utility>
#include <algorithm>

#include "../Libraries/Pipeline.h"
#include "../Libraries/FileManip.h"
#include "../Libraries/ExploreDirectory.h"  //requires Boost

#define N_FUNCS 3

using DataContainer = std::array<std::vector<double>, 3>;
using Task = std::variant<
	std::function<DataContainer(const DataContainer&)>,
	std::function<DataContainer(const DataContainer&, const double)>
	>;

extern DataContainer CartesianToSpherical (const DataContainer&);
extern DataContainer VariationsOnDeltaT (const DataContainer&, const double);
extern DataContainer Variations (const DataContainer&);
///*extern*/ DataContainer AngleVariationOnUnitSphere (const DataContainer& DC) {std::cout << 2 <<std::endl; return DC;};
//DataContainer Placeholder (const DataContainer& DC, const double) {std::cout << 3 <<std::endl; return DC;};

const std::array <Task, N_FUNCS> Tasks = {
		&CartesianToSpherical,
		&VariationsOnDeltaT,
		&Variations
	};

struct TaskLauncher {
public:
	explicit TaskLauncher(const DataContainer & DC, double Parameter = {}) : Data_{DC}, Parameter_{Parameter} {}

	DataContainer operator() (std::function<DataContainer(const DataContainer&)> F) {return F(Data_);}
	DataContainer operator() (std::function<DataContainer(const DataContainer&, const double)> F) {
        return F(Data_, Parameter_);}
	
private:	
	const DataContainer & Data_;
	double Parameter_;
};

DataContainer ReadData (const std::filesystem::path& InFile, const std::wstring& TypeOfDataIn) {
    std::array <std::vector <double>, 3> Data;
    {
        jsoncons::wojson JData;
        {
            std::wstringstream Temp = FileManip::DataInput(InFile);
            JData = jsoncons::wojson::parse(Temp);
        }
        //Put data in vectors
        for (int K = 0; K != 3; ++K) {
            Data[K].reserve(JData[L"List"].size());
        }
        for (long unsigned int K = 0ul; K != JData[L"List"].size(); ++K) {
            Data[0].push_back(JData[L"List"][K][TypeOfDataIn][0].as<double>());
            Data[1].push_back(JData[L"List"][K][TypeOfDataIn][1].as<double>());
            Data[2].push_back(JData[L"List"][K][TypeOfDataIn][2].as<double>());
        }
	}
	return Data;
}

void WriteData (const DataContainer& Data, const std::filesystem::path& OutPath, 
    const std::wstring& OutputName, const std::wstring& TypeOfDataOut, const int FileNumber) {
    jsoncons::wojson JData;
    JData.insert_or_assign(L"List", jsoncons::json_array_arg);
    for (unsigned long int I = 0; I != Data[0].size(); ++I) {
        jsoncons::wojson Coordinates;
        Coordinates.insert_or_assign(TypeOfDataOut, jsoncons::json_array_arg);
        Coordinates[TypeOfDataOut].push_back(Data[0][I]);
        Coordinates[TypeOfDataOut].push_back(Data[1][I]);
        Coordinates[TypeOfDataOut].push_back(Data[2][I]);
        JData[L"List"].push_back(Coordinates);
    }
    //Save Data
    std::wstring Number;
    {
        std::wstringstream Temp;
        Temp << std::setw(3) << std::setfill(L'0') << FileNumber+1;
        Number = Temp.str();
    }
    std::filesystem::path Output =  OutPath/std::wstring(OutputName+L"_"+Number+L".json");
    std::wofstream OutStream (Output);
    OutStream << JData;
    OutStream.close();
}

int main(int Nargs, char** Args) {
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
        std::wostream & Output = ((IOCheck == 3) or (IOCheck == 2)) ? *Pipeline::PipeOutput : std::wcout;
        
        //Process instructions
		jsoncons::wojson Instructions;
        Instructions = jsoncons::wojson::parse(Input);
        const std::vector<int> TaskList = Instructions[L"TaskList"].as<std::vector<int>>();
        const std::filesystem::path InstructionsPath = Instructions[L"InstructionsPath"].as<std::wstring>();
        const std::vector<std::wstring> InstructionsFiles = Instructions[L"InstructionsFiles"].as<std::vector<std::wstring>>();
        if (TaskList.size() != InstructionsFiles.size()) {
            std::wcerr << L"Error: Number of tasks and instructions do not match."<<std::endl;
            return 2;
        }        
        for (unsigned long int I = 0 ; I != TaskList.size() ; ++I) {
			jsoncons::wojson TaskInstructions;
			{
       			std::wstringstream Temp = FileManip::DataInput(InstructionsPath/InstructionsFiles[I]);
       			TaskInstructions = jsoncons::wojson::parse(Temp);
        	}
        	const std::filesystem::path InputPath = TaskInstructions[L"InputPath"].as<std::wstring>();
    		std::vector<std::wstring> InputNames = TaskInstructions[L"InputNames"].as<std::vector<std::wstring>>();
        	const std::filesystem::path OutputPath = TaskInstructions[L"OutputPath"].as<std::wstring>();
        	const std::vector<std::wstring> OutputNames = TaskInstructions[L"OutputNames"].as<std::vector<std::wstring>>();
            const std::wstring TypeOfDataIn = TaskInstructions[L"TypeOfDataIn"].as<std::wstring>();
    		const std::wstring TypeOfDataOut = TaskInstructions[L"TypeOfDataOut"].as<std::wstring>();
            const double Parameter = TaskInstructions[L"Parameter"].as<double>();
	        if (InputNames.size() != OutputNames.size()) {
	            std::wcerr << L"Error: Number of input and output names do not match."<<std::endl;
	            return 2;
	        }
            for (unsigned long int J = 0; J != InputNames.size(); ++J) {
                std::sort(InputNames.begin(), InputNames.end());
            //std::wcerr << InputNames[0]<<std::endl;
                std::vector<std::filesystem::path> WorkingFiles = Explore::ExploreDirectoryByName(InputNames[J], InputPath);
                for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                    std::sort(WorkingFiles.begin(), WorkingFiles.end());
            //std::wcerr << WorkingFiles[0]<<std::endl;
                    DataContainer Data = ReadData (InputPath/WorkingFiles[K], TypeOfDataIn);
                    auto NewData = std::visit(TaskLauncher{Data, Parameter}, Tasks[TaskList[I]]);
                    WriteData(NewData, OutputPath, OutputNames[J], TypeOfDataOut, K);
			    }
		    }
       	}
    }
	return 0;
}
