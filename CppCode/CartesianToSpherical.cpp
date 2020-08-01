#include <iostream>
#include <jsoncons/json.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <array>
#include <iterator>
#include <iomanip>
#include <cmath>

#include "[IO]Pipeline.h"
#include "FileManip.h"
#include "ExploreDirectory.h"  //requires Boost
#include "Utility.h"

#include <boost/geometry.hpp> //Header only, no linking necessary

namespace BG = boost::geometry;
typedef BG::model::point<double, 3, BG::cs::cartesian> CartesianPoint;
typedef BG::model::point<double, 3, BG::cs::spherical<BG::radian>> SphericalPoint;

std::array <std::vector<double>, 3> ToSpherical (const std::array<std::vector<double>, 3> & ToConvert)  {
    std::array <std::vector<double>, 3> Spherical;
    for (int I = 0; I != 3; ++I) {
        Spherical[I].reserve(ToConvert[I].size());
    }
    CartesianPoint CP = {0,0,0};
    SphericalPoint SP = {0,0,0};
    for (unsigned long int I = 0; I != ToConvert[0].size(); ++I) {
        BG::set<0>(CP, ToConvert[0][I]);
        BG::set<1>(CP, ToConvert[1][I]);
        BG::set<2>(CP, ToConvert[2][I]);
        BG::transform(CP, SP);
        Spherical[0].push_back(BG::get<0>(SP));
        Spherical[1].push_back(BG::get<1>(SP));
        Spherical[2].push_back(BG::get<2>(SP));
    }
    return Spherical;
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

        //Process instructions
        jsoncons::wojson Instructions;
        Instructions = jsoncons::wojson::parse(Input);
        std::filesystem::path InputPath = Instructions[L"InputPath"].as<std::wstring>();
        std::vector<std::wstring> InputNames = Instructions[L"InputNames"].as<std::vector<std::wstring>>();
        std::filesystem::path OutputPath = Instructions[L"OutputPath"].as<std::wstring>();
        std::vector<std::wstring> OutputNames = Instructions[L"OutputNames"].as<std::vector<std::wstring>>();
        std::wstring TypeOfData = Instructions[L"TypeOfData"].as<std::wstring>();

        if (InputNames.size() != OutputNames.size()) {
            std::wcerr << L"Error: Number of input and output names do not match."<<std::endl;
            return 1;
        }
        for (unsigned long int I = 0; I != InputNames.size(); ++I) {
            std::vector<std::filesystem::path> WorkingFiles = Explore::ExploreDirectoryByName(InputNames[I], InputPath);
            for (unsigned long int J = 0; J != WorkingFiles.size(); ++J) {
                std::array <std::vector <double>, 3> Data;
                {
                    jsoncons::wojson JData;
                    {
                        std::wstringstream Temp = FileManip::DataInput(InputPath/WorkingFiles[J]);
                        JData = jsoncons::wojson::parse(Temp);
                    }
                    //Put data in vectors
                    for (int K = 0; K != 3; ++K) {
                        Data[I].reserve(JData[L"List"].size());
                    }
                    for (long unsigned int K = 0ul; K != JData[L"List"].size(); ++K) {
                        Data[0].push_back(JData[L"List"][I][TypeOfData][0].as<double>());
                        Data[1].push_back(JData[L"List"][I][TypeOfData][1].as<double>());
                        Data[2].push_back(JData[L"List"][I][TypeOfData][2].as<double>());
                    }
                }
                //Change coordinates
                Data = ToSpherical(Data);
                {
                    jsoncons::wojson JData;
                    JData.insert_or_assign(L"List", jsoncons::json_array_arg);
                    for (unsigned long int K = 0; K != Data[0].size(); ++K) {
                        jsoncons::wojson Coordinates;
                        Coordinates.insert_or_assign(L"Coordinates", jsoncons::json_array_arg);
                        Coordinates[L"Coordinates"].push_back(Data[0][K]);
                        Coordinates[L"Coordinates"].push_back(Data[1][K]);
                        Coordinates[L"Coordinates"].push_back(Data[2][K]);
                        JData[L"List"].push_back(Coordinates);
                    }
                    //Save Data
                    std::wstring Number;
                    {
                        std::wstringstream Temp;
                        Temp << std::setw(3) << std::setfill(L'0') << J+1;
                        Number = Temp.str();
                    }
                    std::filesystem::path Output =  OutputPath/std::wstring(OutputNames[I]+L"_"+Number+L".json");
                    std::wofstream OutStream (Output);
                    OutStream << JData;
                    OutStream.close();
                }
            }
        }
    }
    return 0;
}