#include "Displacement.h"
std::vector<double> ComputeDisplacement (const std::array<std::vector<double>, 3> & Data) {
    std::vector<double> Displacement;
    Displacement.reserve(Data[0].size()-1);
    const std::vector<double> Origin = {Data[0][0], Data[1][0], Data[2][0]};
    std::vector<double> Point = {Data[0][1], Data[1][1], Data[2][1]};
    for (size_t I = 2; I != Data[0].size(); ++I) {
        Displacement.push_back(Utility::Distance(Origin, Point));
        Point[0] = Data[0][I];
        Point[1] = Data[1][I];
        Point[2] = Data[2][I];
    }
    Displacement.push_back(Utility::Distance(Origin, Point));
    return Displacement;
}