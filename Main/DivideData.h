#ifndef DIVIDEDATA_H
#define DIVIDEDATA_H

#include <vector>
#include <array>

//Modifies the given argument so that it contains only values for which Data[Index][] >= Parameter
//Returns another object containing only values for which Data[Index][] < Parameter
extern std::array<std::vector<double>, 3> DivideData (std::array<std::vector<double>, 3> & Data, const double Parameter, const int Index);

#endif