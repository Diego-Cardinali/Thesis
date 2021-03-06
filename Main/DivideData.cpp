#include "DivideData.h"

std::array<std::vector<double>, 3> DivideData (std::array<std::vector<double>, 3> & Data, const double Parameter, const int Index) {
    std::array<std::vector<double>, 3> GeqData;
    std::array<std::vector<double>, 3> LData;
    GeqData[0].reserve(Data[0].size());
    GeqData[1].reserve(Data[0].size());
    GeqData[2].reserve(Data[0].size());
    LData[0].reserve(Data[0].size());
    LData[1].reserve(Data[0].size());
    LData[2].reserve(Data[0].size());
    for (unsigned long int I = 0; I != Data[0].size(); ++I) {
        if (Data[Index][I] >= Parameter) {
            GeqData[0].push_back(Data[0][I]);
            GeqData[1].push_back(Data[1][I]);
            GeqData[2].push_back(Data[2][I]);
        }
        else {
            LData[0].push_back(Data[0][I]);
            LData[1].push_back(Data[1][I]);
            LData[2].push_back(Data[2][I]);
        }
    }
    Data = GeqData;
    return LData;
}

std::array<std::vector<double>, 3> ReduceInRange (const std::array<std::vector<double>, 3> & Data, const std::array<double, 2> & Parameters, const int Index) {
    std::array<std::vector<double>, 3> ReducedData;
    ReducedData[0].reserve(Data[0].size());
    ReducedData[1].reserve(Data[0].size());
    ReducedData[2].reserve(Data[0].size());
    for (unsigned long int I = 0; I != Data[0].size(); ++I) {
        if (Data[Index][I] >= Parameters[0] and Data[Index][I] <= Parameters[1]) {
            ReducedData[0].push_back(Data[0][I]);
            ReducedData[1].push_back(Data[1][I]);
            ReducedData[2].push_back(Data[2][I]);
        }
    }
    return ReducedData;
}
