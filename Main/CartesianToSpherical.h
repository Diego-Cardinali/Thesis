#ifndef CARTTOSPH_H
#define CARTTOSPH_H

#include <vector>
#include <array>

#include <boost/geometry.hpp>

/*********************************************************
*      This code converts cartesian data (of the type    *
*      TypeOfData) into spherical coordinates in the     *
*      form (Azimuth, Inclination, Radius).              *
*********************************************************/

extern std::array<std::vector<double>, 3> CartesianToSpherical (const std::array<std::vector<double>, 3> & ToConvert);
#endif