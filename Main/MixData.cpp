#include "MixData.h"

std::array<std::vector<double>, 3> MixData (const std::vector<double> & ColumnZero, const std::vector<double> & ColumnOne, const std::vector<double> & ColumnTwo) {
    std::array<std::vector<double>, 3> MixedData;
    if (ColumnZero.size() != ColumnOne.size() or ColumnZero.size() != ColumnTwo.size() or ColumnOne.size() != ColumnTwo.size()) {
        std::wcerr << L"Error, sizes of the arguments do not match!\n"<<std::endl;
        return MixedData;
    }
    MixedData[0].reserve(ColumnZero.size());
    MixedData[1].reserve(ColumnZero.size());
    MixedData[2].reserve(ColumnZero.size());
    for (unsigned long int I = 0; I != ColumnZero.size(); ++I) {
        MixedData[0].push_back(ColumnZero[I]);
        MixedData[1].push_back(ColumnOne[I]);
        MixedData[2].push_back(ColumnTwo[I]);
    }
    return MixedData;
}