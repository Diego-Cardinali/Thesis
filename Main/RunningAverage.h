#ifndef RUNNINGAVERAGE_H
#define RUNNINGAVERAGE_H

#include <jsoncons/json.hpp>
#include <vector>
#include <array>
#include <iterator>

#include "Utility.h"

//Computes the running average of each element in the vector, taking Radius element
//BEFORE and Radius element after.
extern std::vector<double> RunningAverage (const std::vector<double> & Data, const size_t Radius);

#endif