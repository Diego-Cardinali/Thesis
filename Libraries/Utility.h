#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>

namespace Utility {
    //Returns true if 'Y' or 'y', false if 'N' or 'n', otherwise the loop repeats.
    //In case of invalid input, verbose will inform user and require new input,
    //non verbose will only require new input.
    extern bool CheckYN (const bool Verbose = true);

    //Computes the module of a set of n coordinates, treated as a vector.
    extern double Module (const std::vector <double> & Vector);

    //Computes the n-dimensional Euclidean distance of 2 sets of coordinates.
    //If the vectors are not compatible (equal dimension) the value is -1.
    extern double Distance (const std::vector <double> & Vector1, const std::vector <double> & Vector2);

    //Computes the unweighted mean of the elements of a vector.
    extern double Mean (const std::vector <double> & Vector);

    //Returns a normalized version of a vector. Takes any type, but only returns double.
    template <class C> std::vector<double> NormalizeVector (const std::vector<C> & Vector);
}

//Returns a normalized version of a vector. Takes any type, but only returns double.
//Will return empty vector if Module is 0.
template <class C> std::vector<double> Utility::NormalizeVector (const std::vector<C> & Vector) {
    double Module = Utility::Module(std::vector <double>(Vector.begin(), Vector.end()));
    std::vector <double> Normalized;
    if (Module != 0) {
        Normalized.reserve(Vector.size());
        for (auto Element : Vector) {
            Normalized.push_back(static_cast<double>(Element)/Module);
        }
    }
    return Normalized;
}
#endif