#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H

#include <vector>
#include <array>
#include <string>
#include <jsoncons/json.hpp>

#include "../Libraries/Utility.h"

extern void ComputeDisplacement (jsoncons::wojson &);
extern jsoncons::wojson DisplacementOverTime (const std::array<std::vector<double>, 3> &, const std::vector<size_t> &);
extern void MeanDisplacement (jsoncons::wojson &, const std::vector<std::wstring> &, const std::vector<std::wstring> &);

#endif