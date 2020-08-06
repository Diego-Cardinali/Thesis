#ifndef VARIATIONS_H
#define VARIATIONS_H

#include <vector>
#include <array>

extern std::array<std::vector<double>, 3> VariationsOnDeltaT (const std::array<std::vector<double>, 3>&, const double);
extern std::array<std::vector<double>, 3> Variations (const std::array<std::vector<double>, 3>&);
#endif