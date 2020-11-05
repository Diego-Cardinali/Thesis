#include "DivideFastSlow.h"

jsoncons::wojson GetFastSlowLimits (const std::array<std::vector<double>, 3> & Data, const std::vector<size_t> & Lengths) {
    jsoncons::wojson Divided;
    Divided.insert_or_assign(L"Slow", jsoncons::json_array_arg);
    Divided.insert_or_assign(L"Fast", jsoncons::json_array_arg);
    size_t Cumul = 0;
    for (size_t I = !static_cast<bool>(Lengths[0]); I != Lengths.size(); ++I) {
        jsoncons::wojson Track;
        Track.insert_or_assign(L"Start", jsoncons::json_array_arg);
        Track.insert_or_assign(L"Stop", jsoncons::json_array_arg);
        Track.insert_or_assign(L"Length", Lengths[I]);
        Track[L"Start"].push_back(Data[0][Cumul]);
        Track[L"Start"].push_back(Data[1][Cumul]);
        Track[L"Start"].push_back(Data[2][Cumul]);
        Cumul += Lengths[I];
        Track[L"Stop"].push_back(Data[0][Cumul-1]);
        Track[L"Stop"].push_back(Data[1][Cumul-1]);
        Track[L"Stop"].push_back(Data[2][Cumul-1]);
        Divided[I%2?L"Slow":L"Fast"].push_back(Track);
    }
    return Divided;
}

jsoncons::wojson DivideFastSlow (const std::array<std::vector<double>, 3> & Data, const std::vector<size_t> & Lengths) {
    jsoncons::wojson Divided;
    Divided.insert_or_assign(L"Slow", jsoncons::json_array_arg);
    Divided.insert_or_assign(L"Fast", jsoncons::json_array_arg);
    size_t Cumul = 0;
    for (size_t I = !static_cast<bool>(Lengths[0]); I != Lengths.size(); ++I) {
        jsoncons::wojson Track(jsoncons::json_array_arg);
        Track.reserve(Lengths[I]);
        for (size_t J = Cumul; J != Cumul+Lengths[I]; ++J) {
            jsoncons::wojson Point(jsoncons::json_array_arg);
            Point.push_back(Data[0][J]);
            Point.push_back(Data[1][J]);
            Point.push_back(Data[2][J]);
            Track.push_back(Point);
        }
        Cumul += Lengths[I];
        Divided[I%2?L"Slow":L"Fast"].push_back(Track);
    }
    return Divided;
}
