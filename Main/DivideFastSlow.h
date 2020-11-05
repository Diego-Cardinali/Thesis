#ifndef DIVIDEFASTSLOW_H
#define DIVIDEFASTSLOW_H

#include <vector>
#include <array>
#include <jsoncons/json.hpp>

extern jsoncons::wojson GetFastSlowLimits (const std::array<std::vector<double>, 3> &, const std::vector<size_t> &);
extern jsoncons::wojson DivideFastSlow (const std::array<std::vector<double>, 3> &, const std::vector<size_t> &);

#endif