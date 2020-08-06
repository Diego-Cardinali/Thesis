#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <array>
#include <iterator>
#include <algorithm>

#include <jsoncons/json.hpp>

#include "DataToJson.h"
#include "CartesianToSpherical.h"
#include "Variations.h"
#include "Histogram.h"

#include "../Libraries/Pipeline.h"
#include "../Libraries/FileManip.h"
#include "../Libraries/ExploreDirectory.h"  //requires Boost

#define DATATOJSON   0u
#define CARTTOSPH    1u
#define VARONDELTAT  2u
#define VARIATIONS   3u
#define HISTOGRAMS   4u
#define JOINFILES    5u

using DataContainer = std::array<std::vector<double>, 3>;

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

DataContainer MassReadData (const std::filesystem::path& InPath, 
    const std::vector<std::filesystem::path>& InFiles, const std::wstring& TypeOfDataIn) {
    DataContainer MergedData;
    std::vector<DataContainer> DataToMerge;
    DataToMerge.reserve(InFiles.size());
    for (const auto & InFile: InFiles) {
        DataToMerge.push_back(ReadData(InPath/InFile, TypeOfDataIn));
	}
    size_t Size = 0;
    for (const auto & Datum : DataToMerge) {
        Size+=Datum[0].size();
    }
    MergedData[0].reserve(Size);
    MergedData[1].reserve(Size);
    MergedData[2].reserve(Size);
    for (unsigned long int I = 0; I != DataToMerge.size(); ++I) {
        MergedData[0].insert(MergedData[0].end(), DataToMerge[I][0].begin(), DataToMerge[I][0].end());
        DataToMerge[I][0].clear();
        MergedData[1].insert(MergedData[1].end(), DataToMerge[I][1].begin(), DataToMerge[I][1].end());
        DataToMerge[I][1].clear();
        MergedData[2].insert(MergedData[2].end(), DataToMerge[I][2].begin(), DataToMerge[I][2].end());
        DataToMerge[I][2].clear();
    }
    return MergedData;
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

void WriteData (const DataContainer& Data, const std::filesystem::path& OutPath, 
    const std::wstring& OutputName, const std::wstring& TypeOfDataOut) {
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
    std::filesystem::path Output =  OutPath/std::wstring(OutputName+L".json");
    std::wofstream OutStream (Output);
    OutStream << JData;
    OutStream.close();
}

void WriteData (const jsoncons::wojson& JData, const std::filesystem::path& OutPath, 
    const std::wstring& OutputName, const int FileNumber) {
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
        const std::vector<unsigned int> TaskList = Instructions[L"TaskList"].as<std::vector<unsigned int>>();
        const std::filesystem::path InstructionsPath = Instructions[L"InstructionsPath"].as<std::wstring>();
        const std::vector<std::wstring> InstructionsFiles = Instructions[L"InstructionsFiles"].as<std::vector<std::wstring>>();
        const bool Verbose = Instructions[L"Verbose"].as<bool>();
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
            if (Verbose) {
                Output << L"Read instructions "<<InstructionsPath.wstring()<<InstructionsFiles[I]<<L'\n';
            }
        	const std::filesystem::path InputPath = TaskInstructions[L"InputPath"].as<std::wstring>();
    		std::vector<std::wstring> InputNames = TaskInstructions[L"InputNames"].as<std::vector<std::wstring>>();
        	const std::filesystem::path OutputPath = TaskInstructions[L"OutputPath"].as<std::wstring>();
        	const std::vector<std::wstring> OutputNames = TaskInstructions[L"OutputNames"].as<std::vector<std::wstring>>();
            const std::wstring TypeOfDataIn = TaskInstructions[L"TypeOfDataIn"].as<std::wstring>();
    		const std::wstring TypeOfDataOut = TaskInstructions[L"TypeOfDataOut"].as<std::wstring>();
	        if (InputNames.size() != OutputNames.size()) {
	            std::wcerr << L"Error: Number of input and output names do not match."<<std::endl;
	            return 2;
	        }
            if (TaskList[I] == HISTOGRAMS and  (TaskInstructions[L"ArrayOfIntParameters"].size() != InputNames.size())) {
                std::wcerr << L"Error: Number of Bins do not match with number of files."<<std::endl;
                return 2;
            }
            for (unsigned long int J = 0; J != InputNames.size(); ++J) {
                std::sort(InputNames.begin(), InputNames.end());
                std::vector<std::filesystem::path> WorkingFiles = Explore::ExploreDirectoryByName(InputNames[J], InputPath);
                std::sort(WorkingFiles.begin(), WorkingFiles.end());
                for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                    DataContainer Data;
                    DataContainer ProcessedData;
                    jsoncons::wojson ProcessedJData;
                    //Each I is associated with one and only one function.
                    switch (TaskList[I]) {
                        case DATATOJSON:
                            ProcessedJData = DataToJson(InputPath/WorkingFiles[K]);
                            WriteData(ProcessedJData, OutputPath, OutputNames[J], K);
                            break;
                        case CARTTOSPH:                      
                            Data = ReadData (InputPath/WorkingFiles[K], TypeOfDataIn);
                            ProcessedData = CartesianToSpherical(Data);
                            WriteData(ProcessedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                            break;
                        case VARONDELTAT:
                            Data = ReadData (InputPath/WorkingFiles[K], TypeOfDataIn);
                            ProcessedData = VariationsOnDeltaT (Data, TaskInstructions[L"DoubleParameter"].as<double>());
                            WriteData(ProcessedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                            break;
                        case VARIATIONS:
                            Data = ReadData (InputPath/WorkingFiles[K], TypeOfDataIn);
                            ProcessedData = Variations (Data);
                            WriteData(ProcessedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                            break;
                        case HISTOGRAMS:
                            Data = ReadData (InputPath/WorkingFiles[K], TypeOfDataIn);
                            ProcessedJData = Histograms (Data, TaskInstructions[L"ArrayOfIntParameters"][J].as<size_t>());
                            WriteData(ProcessedJData, OutputPath, OutputNames[J], K);
                            break;
                        case JOINFILES:
                            Data = MassReadData (InputPath, WorkingFiles, TypeOfDataIn);
                            WriteData (Data, OutputPath, OutputNames[J], TypeOfDataOut);
                            //Questa è una porcata ma uso molto più spesso le altre funzioni
                            //quindi conviene tenerla qui dentro
                            K = WorkingFiles.size()-1;
                            break;
                        default:
                            std::wcerr << L"Invalid function ID selected."<<std::endl;
                            return 3;
                    }
			    }
		    }
       	}
    }
	return 0;
}