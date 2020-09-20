#include "DivideFastSlow.h"

jsoncons::wojson DivideFastSlow (const std::array<std::vector<double>, 3> & Coordinates, const std::vector<size_t> & Lengths) {
    jsoncons::wojson Divided;
    Divided.insert_or_assign(L"Slow", jsoncons::json_array_arg);
    Divided.insert_or_assign(L"Fast", jsoncons::json_array_arg);
    size_t Cumul = 0;
    for (size_t I = !static_cast<bool>(Lengths[0]); I != Lengths.size(); ++I) {
        std::cerr<<"Before "<<Cumul<<std::endl;
        jsoncons::wojson Track;
        Track.insert_or_assign(L"Start", jsoncons::json_array_arg);
        Track.insert_or_assign(L"Stop", jsoncons::json_array_arg);
        Track.insert_or_assign(L"Length", Lengths[I]);
        Track[L"Start"].push_back(Coordinates[0][Cumul]);
        Track[L"Start"].push_back(Coordinates[1][Cumul]);
        Track[L"Start"].push_back(Coordinates[2][Cumul]);
        Cumul += Lengths[I];
        std::cerr<<"After "<<Cumul<<std::endl;
        Track[L"Stop"].push_back(Coordinates[0][Cumul-1]);
        Track[L"Stop"].push_back(Coordinates[1][Cumul-1]);
        Track[L"Stop"].push_back(Coordinates[2][Cumul-1]);
        std::cerr<<"Way after"<<std::endl;
        Divided[I%2?L"Slow":L"Fast"].push_back(Track);
    }
    return Divided;
}