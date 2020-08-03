#include <vector>
#include <array>

#include <boost/geometry.hpp> //Header only, no linking necessary

/*********************************************************
*        This code converts cartesian data (of the type  *
*        TypeOfData) into spherical coordinates in the   *
*        form (Azimuth, Inclination, Radius).            *
*********************************************************/

namespace BG = boost::geometry;
typedef BG::model::point<double, 3, BG::cs::cartesian> CartesianPoint;
typedef BG::model::point<double, 3, BG::cs::spherical<BG::radian>> SphericalPoint;

std::array <std::vector<double>, 3> CartesianToSpherical (const std::array<std::vector<double>, 3> & ToConvert)  {
    std::array <std::vector<double>, 3> Spherical;
    for (int I = 0; I != 3; ++I) {
        Spherical[I].reserve(ToConvert[I].size());
    }
    CartesianPoint CP = {0,0,0};
    SphericalPoint SP = {0,0,0};
    for (unsigned long int I = 0; I != ToConvert[0].size(); ++I) {
        BG::set<0>(CP, ToConvert[0][I]);
        BG::set<1>(CP, ToConvert[1][I]);
        BG::set<2>(CP, ToConvert[2][I]);
        BG::transform(CP, SP);
        Spherical[0].push_back(BG::get<0>(SP));
        Spherical[1].push_back(BG::get<1>(SP));
        Spherical[2].push_back(BG::get<2>(SP));
    }
    return Spherical;
}