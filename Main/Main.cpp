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
#include "MixData.h"
#include "DivideData.h"
#include "TrackLength.h"
#include "RunningAverage.h"
#include "Displacement.h"
#include "DivideFastSlow.h"

#include "Pipeline.h"
#include "FileManip.h"
#include "ExploreDirectory.h"  //requires Boost

#define DATATOJSON         0u
#define CARTTOSPH          1u
#define VARONDELTAT        2u
#define VARIATIONS         3u
#define HISTOGRAMS         4u
#define JOINFILES3D        5u
#define MIXDATA            6u
#define DIVIDEDATA         7u
#define REDUCEINRANGE      8u
#define TRACKLENGTH        9u
#define JOINTRACKS        10u
#define RUNNINGAVERAGE    11u
#define DISPLACEMENT      12u
#define FASTSLOWLIMITS    13u
#define JOINDISPLACEMENTS 14u
#define VARSTEPN          15u
#define DISPLACEMENTOVERT 16u
#define MEANDISPLACEMENT  17u
#define DATAMEAN          18u
#define DIVIDEFASTSLOW    19u
#define COORDTOVELMODULE  20u

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

std::vector<size_t> ReadData1D (const std::filesystem::path& InFile, const std::wstring& TypeOfDataIn) {
    std::vector <size_t> Data;
    jsoncons::wojson JData;
    {
        std::wstringstream Temp = FileManip::DataInput(InFile);
        JData = jsoncons::wojson::parse(Temp);
    }
    Data.reserve(JData[TypeOfDataIn].size());
    for (const auto Datum : JData[TypeOfDataIn].array_range()) {
        Data.push_back(Datum.as<size_t>());
    }
	return Data;
}

std::vector<size_t> MassReadData1D (const std::filesystem::path& InPath, 
    const std::vector<std::filesystem::path>& InFiles, const std::wstring& TypeOfDataIn) {
    std::vector<size_t> MergedData;
    std::vector<std::vector<size_t>> DataToMerge;
    DataToMerge.reserve(InFiles.size());
    for (const auto & InFile: InFiles) {
        DataToMerge.push_back(ReadData1D(InPath/InFile, TypeOfDataIn));
	}
    size_t Size = InFiles.size(); //Worst case possible
    for (const auto & Datum : DataToMerge) {
        Size+=Datum.size();
    }
    MergedData.reserve(Size);
    for (unsigned long int I = 0; I != DataToMerge.size(); ++I) {
        MergedData.insert(MergedData.end(), DataToMerge[I].begin(), DataToMerge[I].end());
        if (MergedData.size()%2) {
            MergedData.push_back(0);
        }
        DataToMerge[I].clear();
    }
    return MergedData;
}

jsoncons::wojson MergeJsons (const std::filesystem::path& InPath, 
    const std::vector<std::filesystem::path>& InFiles, const std::vector<std::wstring> & Keys) {
    jsoncons::wojson MergedData;
    for (const auto & Key : Keys) {
        MergedData.insert_or_assign(Key, jsoncons::json_array_arg);
    }
    for (const auto & InFile: InFiles) {
        jsoncons::wojson JData;
        {
            std::wstringstream Temp = FileManip::DataInput(InPath/InFile);
            JData = jsoncons::wojson::parse(Temp);
        }
        for (const auto & Key : Keys) {
            if (JData.contains(Key)) {
                for (const auto & Datum: JData[Key].array_range()) {
                    MergedData[Key].push_back(Datum);
                }
            }
        }
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

void WriteData (const jsoncons::wojson& JData, const std::filesystem::path& OutPath,
    const std::wstring& OutputName) {
    //Save Data
    std::filesystem::path Output =  OutPath/std::wstring(OutputName+L".json");
    std::wofstream OutStream (Output);
    OutStream << JData;
    OutStream.close();
}

template <typename T>
void WriteData (const std::vector<T>& Data, const std::filesystem::path& OutPath, 
    const std::wstring& OutputName, const std::wstring& TypeOfDataOut, const int FileNumber) {
    jsoncons::wojson JData;
    JData.insert_or_assign(TypeOfDataOut, jsoncons::json_array_arg);
    for (const auto & Datum : Data) {
        JData[TypeOfDataOut].push_back(Datum);
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

template <typename T>
void WriteData (const std::vector<T>& Data, const std::filesystem::path& OutPath, 
    const std::wstring& OutputName, const std::wstring& TypeOfDataOut) {
    jsoncons::wojson JData;
    JData.insert_or_assign(TypeOfDataOut, jsoncons::json_array_arg);
    for (const auto & Datum : Data) {
        JData[TypeOfDataOut].push_back(Datum);
    }
    std::filesystem::path Output =  OutPath/std::wstring(OutputName+L".json");
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
    		const std::vector<std::wstring> InputNames = TaskInstructions[L"InputNames"].as<std::vector<std::wstring>>();
            const std::filesystem::path OutputPath = TaskInstructions[L"OutputPath"].as<std::wstring>();
        	const std::vector<std::wstring> OutputNames = TaskInstructions[L"OutputNames"].as<std::vector<std::wstring>>();
            const std::wstring TypeOfDataIn = TaskInstructions[L"TypeOfDataIn"].as<std::wstring>();
    		const std::wstring TypeOfDataOut = TaskInstructions[L"TypeOfDataOut"].as<std::wstring>();
	        if (InputNames.size() != OutputNames.size()) {
	            std::wcerr << L"Error: Number of input and output names do not match."<<std::endl;
	            return 2;
	        }
            for (unsigned long int J = 0; J != InputNames.size(); ++J) {
                std::vector<std::filesystem::path> WorkingFiles = Explore::ExploreDirectoryByName(InputNames[J], InputPath);
                std::sort(WorkingFiles.begin(), WorkingFiles.end());
                std::vector<std::filesystem::path> ExtraWorkingFiles = (TaskInstructions.contains(L"ExtraInputNames") and TaskInstructions.contains(L"ExtraInputPath")) ? Explore::ExploreDirectoryByName(TaskInstructions[L"ExtraInputNames"][J].as<std::wstring>(), TaskInstructions[L"ExtraInputPath"].as<std::wstring>()) : std::vector<std::filesystem::path>();
                std::sort(ExtraWorkingFiles.begin(), ExtraWorkingFiles.end());
                //Each I is associated with one and only one function.
                if (TaskList[I] == DATATOJSON) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        jsoncons::wojson ProcessedJData = DataToJson(InputPath/WorkingFiles[K]);
                        WriteData(ProcessedJData, OutputPath, OutputNames[J], K);
                    }
                }
                else if (TaskList[I] == CARTTOSPH) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        DataContainer ProcessedData = CartesianToSpherical(Data);
                        WriteData(ProcessedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                    }
                }
                else if (TaskList[I] == VARONDELTAT) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        DataContainer ProcessedData = VariationsOnDeltaT(Data, TaskInstructions[L"DoubleParameter"].as<double>());
                        WriteData(ProcessedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                    }
                }
                else if (TaskList[I] == VARIATIONS) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        DataContainer ProcessedData = Variations(Data);
                        WriteData(ProcessedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                    }
                }
                else if (TaskList[I] == HISTOGRAMS) {
                    if (TaskInstructions[L"ArrayOfIntParameters"].size() != InputNames.size()) {
                        std::wcerr << L"Error: Number of Bins do not match with number of files." << std::endl;
                        return 2;
                    }
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        jsoncons::wojson ProcessedJData = Histograms <double> (Data, TaskInstructions[L"ArrayOfIntParameters"][J].as<size_t>());
                        WriteData(ProcessedJData, OutputPath, OutputNames[J], K);
                    }
                }
                else if (TaskList[I] == JOINFILES3D) {
                    DataContainer Data = MassReadData(InputPath, WorkingFiles, TypeOfDataIn);
                    WriteData (Data, OutputPath, OutputNames[J], TypeOfDataOut);
                }
                else if (TaskList[I] == MIXDATA) {
                    if (TaskInstructions[L"ExtraInputNames"].size() != InputNames.size()) {
                        std::wcerr << L"Error: Number of files to mix do not match." << std::endl;
                        return 2;
                    }
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer DataA = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        DataContainer DataB = ReadData(TaskInstructions[L"ExtraInputPath"].as<std::wstring>()/ExtraWorkingFiles[K], TaskInstructions[L"ExtraTypeOfDataIn"].as<std::wstring>());
                        //If true take data from DataA, if false from DataB
                        auto ColumnZero = TaskInstructions[L"ArrayOfBoolParameters"][0].as<bool>() ? &DataA[0] : &DataB[0];
                        auto ColumnOne  = TaskInstructions[L"ArrayOfBoolParameters"][1].as<bool>() ? &DataA[1] : &DataB[1];
                        auto ColumnTwo  = TaskInstructions[L"ArrayOfBoolParameters"][2].as<bool>() ? &DataA[2] : &DataB[2];
                        DataContainer ProcessedData = MixData(*ColumnZero, *ColumnOne, *ColumnTwo);
                        WriteData (ProcessedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                    }
                }
                else if (TaskList[I] == DIVIDEDATA) {
                    if (TaskInstructions[L"ArrayOfDoubleParameters"].size() != InputNames.size()) {
                        std::wcerr << L"Error: Number of parameters and number of inputs do not match." << std::endl;
                        return 2;
                    }
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer DataA = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        if (Verbose) {
                            Output << L"For file "<<WorkingFiles[K]<<L":\n\tTotal number of elements: "<<DataA[0].size()<<L'\n';
                        }
                        auto DataB = DivideData(DataA, TaskInstructions[L"ArrayOfDoubleParameters"][J].as<double>(), TaskInstructions[L"IntParameter"].as<int>());
                        std::wstring NameA = OutputNames[J]+L"_Geq_"+std::to_wstring(TaskInstructions[L"ArrayOfDoubleParameters"][J].as<double>());
                        std::wstring NameB = OutputNames[J]+L"_L_"+std::to_wstring(TaskInstructions[L"ArrayOfDoubleParameters"][J].as<double>());
                        WriteData(DataA, OutputPath, NameA, TypeOfDataOut, K);
                        WriteData(DataB, OutputPath, NameB, TypeOfDataOut, K);
                        if (Verbose) {
                            Output <<L"\tNumber of elements in Geq: "<< DataA[0].size()<< L"\n\tNumber of elements in L: "<<DataB[0].size()<<L'\n';
                        }
                    }
                }
                else if (TaskList[I] == REDUCEINRANGE) {
                    if (TaskInstructions[L"ArrayOfArraysOfDoubleParameters"].size() != InputNames.size()) {
                        std::wcerr << L"Error: Number of parameters and number of inputs do not match." << std::endl;
                        return 2;
                    }
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        if (Verbose) {
                            Output << L"For file "<<WorkingFiles[K]<<L":\n\tTotal number of elements: "<<Data[0].size()<<L'\n';
                        }
                        std::array<double, 2> Parameters = TaskInstructions[L"ArrayOfArraysOfDoubleParameters"][J].as<std::array<double, 2>>();
                        auto ReducedData = ReduceInRange(Data, Parameters, TaskInstructions[L"IntParameter"].as<int>());
                        WriteData(ReducedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                        if (Verbose) {
                            Output <<L"\tNumber of elements after reduction: "<< ReducedData[0].size() <<L'\n';
                        }
                    }
                }
                else if (TaskList[I] == TRACKLENGTH) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        std::vector<size_t> Tracks;
                        TrackLength(
                            std::begin(Data[TaskInstructions[L"IntParameter"].as<size_t>()]),
                            std::end(Data[TaskInstructions[L"IntParameter"].as<size_t>()]),
                            std::back_inserter(Tracks),
                            TaskInstructions[L"ArrayOfDoubleParameters"][J].as<double>(),
                            TaskInstructions[L"ExtraIntParameter"].as<size_t>()
                        );
                        WriteData(Tracks, OutputPath, OutputNames[J], TypeOfDataOut, K);
                    }
                }
                else if (TaskList[I] == JOINTRACKS) {
                    std::vector<size_t> Data = MassReadData1D(InputPath, WorkingFiles, TypeOfDataIn);
                    WriteData (Data, OutputPath, OutputNames[J], TypeOfDataOut);
                }
                else if (TaskList[I] == RUNNINGAVERAGE) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        //ExtraIntParameter is index on which RunAvg will be computed
                        //IntParameter is number of elements in running average
                        DataContainer ProcessedData;
                        ProcessedData[TaskInstructions[L"ExtraIntParameter"].as<size_t>()] = RunningAverage(Data[TaskInstructions[L"ExtraIntParameter"].as<size_t>()], TaskInstructions[L"IntParameter"].as<size_t>());
                        std::vector<size_t> Indexes = {0, 1, 2};
                        Indexes.erase(std::remove(Indexes.begin(), Indexes.end(), TaskInstructions[L"ExtraIntParameter"].as<size_t>()), Indexes.end());
                        for (const auto & Index : Indexes) {
                            ProcessedData[Index] = std::vector<double>(Data[Index].begin()+TaskInstructions[L"IntParameter"].as<size_t>(), Data[Index].end()-TaskInstructions[L"IntParameter"].as<size_t>());
                        }
                        WriteData(ProcessedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                    }
                }
                else if (TaskList[I] == DISPLACEMENT) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        jsoncons::wojson JData;
                        {
                            std::wstringstream Temp = FileManip::DataInput(InputPath/WorkingFiles[K]);
                            JData = jsoncons::wojson::parse(Temp);
                        }
                        ComputeDisplacement (JData);
                        WriteData(JData, OutputPath, OutputNames[J], K);
                        }
                }
                else if (TaskList[I] == FASTSLOWLIMITS) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        if (Verbose) {
                            Output << L"Working on " << WorkingFiles[K]<<L" - "<<ExtraWorkingFiles[K]<<L'\n';
                        }
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        std::vector<size_t> Tracks = ReadData1D(TaskInstructions[L"ExtraInputPath"].as<std::wstring>()/ExtraWorkingFiles[K], TaskInstructions[L"ExtraTypeOfDataIn"].as<std::wstring>());
                        auto JData = GetFastSlowLimits(Data, Tracks);
                        WriteData(JData, OutputPath, OutputNames[J], K);
                    }
                }
                else if (TaskList[I] == JOINDISPLACEMENTS) {
                    auto Keys = TaskInstructions[L"ArrayOfStringParameters"].as<std::vector<std::wstring>>();
                    auto Data = MergeJsons (InputPath, WorkingFiles, Keys);
                    WriteData (Data, OutputPath, OutputNames[J]);
                }
                else if (TaskList[I] == VARSTEPN) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        DataContainer ProcessedData = VariationsStepN(Data, TaskInstructions[L"DoubleParameter"].as<double>(), TaskInstructions[L"IntParameter"].as<size_t>());
                        WriteData(ProcessedData, OutputPath, OutputNames[J], TypeOfDataOut, K);
                    }
                }
                else if (TaskList[I] == DISPLACEMENTOVERT) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        if (Verbose) {
                            Output << L"Working on " << WorkingFiles[K]<<L" - "<<ExtraWorkingFiles[K]<<L'\n';
                        }
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        std::vector<size_t> Tracks = ReadData1D(TaskInstructions[L"ExtraInputPath"].as<std::wstring>()/ExtraWorkingFiles[K], TaskInstructions[L"ExtraTypeOfDataIn"].as<std::wstring>());
                        auto JData = DisplacementOverTime(Data, Tracks);
                        WriteData(JData, OutputPath, OutputNames[J], K);
                    }
                }
                else if (TaskList[I] == MEANDISPLACEMENT) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        jsoncons::wojson JData;
                        {
                            std::wstringstream Temp = FileManip::DataInput(InputPath/WorkingFiles[K]);
                            JData = jsoncons::wojson::parse(Temp);
                        }
                        MeanDisplacement (JData, TaskInstructions[L"ArrayOfStringParameters"].as<std::vector<std::wstring>>(), TaskInstructions[L"ExtraArrayOfStringParameters"].as<std::vector<std::wstring>>());
                        WriteData(JData, OutputPath, OutputNames[J], K);
                        }
                }
                else if (TaskList[I] == DATAMEAN) {
                    jsoncons::wojson Means;
                    Means.insert_or_assign(L"TypeOfData", TypeOfDataIn);
                    for (const auto & Key : TaskInstructions[L"ArrayOfStringParameters"].as<std::vector<std::wstring>>()) {
                        Means.insert_or_assign(Key, jsoncons::json_object_arg);
                        Means[Key].insert_or_assign(L"Means", jsoncons::json_array_arg);
                        Means[Key].insert_or_assign(L"NSamples", jsoncons::json_array_arg);
                        Means[Key][L"Means"].reserve(WorkingFiles.size());
                        Means[Key][L"NSamples"].reserve(WorkingFiles.size());
                    }
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        jsoncons::wojson JData;
                        {
                            std::wstringstream Temp = FileManip::DataInput(InputPath/WorkingFiles[K]);
                            JData = jsoncons::wojson::parse(Temp);
                        }
                        for (const auto & Key : TaskInstructions[L"ArrayOfStringParameters"].as<std::vector<std::wstring>>()) {
                            for (size_t L = 0; L != JData[Key].size(); ++L) {
                                if (JData[Key][L][TypeOfDataIn].size()) {
                                    Means[Key][L"Means"].push_back(Utility::Mean(JData[Key][L][TypeOfDataIn].as<std::vector<double>>()));
                                    Means[Key][L"NSamples"].push_back(JData[Key][L][TypeOfDataIn].size());
                                }
                                else {
                                    Means[Key][L"Means"].push_back(0.);
                                    Means[Key][L"NSamples"].push_back(0);
                                }
                            }
                        }
                    }
                    WriteData(Means, OutputPath, OutputNames[J]);
                }
                else if (TaskList[I] == DIVIDEFASTSLOW) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        if (Verbose) {
                            Output << L"Working on " << WorkingFiles[K]<<L" - "<<ExtraWorkingFiles[K]<<L'\n';
                        }
                        DataContainer Data = ReadData(InputPath/WorkingFiles[K], TypeOfDataIn);
                        std::vector<size_t> Tracks = ReadData1D(TaskInstructions[L"ExtraInputPath"].as<std::wstring>()/ExtraWorkingFiles[K], TaskInstructions[L"ExtraTypeOfDataIn"].as<std::wstring>());
                        auto JData = DivideFastSlow(Data, Tracks);
                        WriteData(JData, OutputPath, OutputNames[J], K);
                    }                    
                }
                else if (TaskList[I] == COORDTOVELMODULE) {
                    for (unsigned long int K = 0; K != WorkingFiles.size(); ++K) {
                        jsoncons::wojson FileTracks;
                        jsoncons::wojson JData;
                        {
                            std::wstringstream Temp = FileManip::DataInput(InputPath/WorkingFiles[K]);
                            JData = jsoncons::wojson::parse(Temp);
                        }
                        for (const auto & Key : TaskInstructions[L"ArrayOfStringParameters"].as<std::vector<std::wstring>>()) {
                            FileTracks.insert_or_assign(Key, jsoncons::json_array_arg);
                            FileTracks[Key].reserve(JData[Key].size());
                            for (size_t L = 0; L != JData[Key].size(); ++L) {
                                DataContainer TrackData;
                                for (int M = 0; M != 3; ++M) {
                                    TrackData[M].reserve(JData[Key][L].size());
                                }
                                for (long unsigned int M = 0ul; M != JData[Key][L].size(); ++M) {
                                    TrackData[0].push_back(JData[Key][L][M][0].as<double>());
                                    TrackData[1].push_back(JData[Key][L][M][1].as<double>());
                                    TrackData[2].push_back(JData[Key][L][M][2].as<double>());
                                }
                                auto FinalData = CoordToVelModule(TrackData, TaskInstructions[L"DoubleParameter"].as<double>());
                                FileTracks[Key].push_back(FinalData);
                            }
                        }
                        WriteData(FileTracks, OutputPath, OutputNames[J], K);
                    }                      
                }
                else {
                    std::wcerr << L"Invalid function ID selected."<<std::endl;
                    return 3;
                }
            }
		}
    }
	return 0;
}