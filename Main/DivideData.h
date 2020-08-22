#ifndef DIVIDEDATA_H
#define DIVIDEDATA_H

#include <vector>
#include <array>

//Modifies the given argument so that it contains only values for which Data[Index][] >= Parameter
//Returns another object containing only values for which Data[Index][] < Parameter
extern std::array<std::vector<double>, 3> DivideData (std::array<std::vector<double>, 3> & Data, const double Parameter, const int Index);

extern std::array<std::vector<double>, 3> ReduceInRange (const std::array<std::vector<double>, 3> & Data, const std::array<double, 2> & Parameters, const int Index);

#endif