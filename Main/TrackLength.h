#ifndef TRACKLENGTH_H
#define TRACKLENGTH_H

#include <iterator>

//Container referred to by Out is filled with size_t representing the number of consecutive
//elements of the container referred to by Begin-End which are respectively higher and
//lower of Threshold. All even positions in Out are segments for which the data is greater
//or equal to Threshold, if the data starts with a portion  that is lower the first element
//will be a 0. The return value is the size of Out.
template<typename InIt, typename OutIt, typename T> 
size_t TrackLength( InIt Begin, InIt End, OutIt Out, T Threshold ) {
    size_t CntIns {};
    size_t n {};
    if ( Begin != End ) {
        if ( *Begin < Threshold ) {
            //If starting data is Low
            *Out++ = {};
            ++CntIns;
        }
        else {
            ++Begin;
            ++n;
        }
        while ( Begin != End ) {
            if ( CntIns % 2 ) {
                // Low
                if ( *Begin < Threshold ) {
                    ++n;
                }
                else {
                    *Out++ = n;
                    ++CntIns;
                    n = 1;
                }
            }
            else {
                // High
                if ( *Begin >= Threshold ) {
                    ++n;
                }
                else {
                    *Out++ = n;
                    ++CntIns;
                    n = 1;
                }
            }
            ++Begin;
        }
        *Out++ = n;
    }
    return CntIns;
}

#endif