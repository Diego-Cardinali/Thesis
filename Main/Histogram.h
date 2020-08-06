#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <filesystem>
#include <array>
#include <iterator>
#include <map>
#include <algorithm>
#include <iomanip>

#include <jsoncons/json.hpp>

//#include "FileManip.h"
//#include "ExploreDirectory.h"  //requires Boost
//#include "Utility.h"

extern std::map <double, size_t> MakeHistogram (const std::vector<double>&);
extern std::vector<size_t> MakeBins (const std::map <double, size_t>&, const size_t);
extern std::vector<double> MakeProbabilities (std::vector<size_t>);
extern jsoncons::wojson Histograms (const std::array<std::vector<double>, 3>&, const size_t);

#endif