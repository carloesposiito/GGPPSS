#pragma once

//////////////////////////////////////////////////////
//                                                  //
//                 BASIC LIBRARIES                  //
//                                                  //
//////////////////////////////////////////////////////

#include <Arduino.h>
#include <Wire.h>

//////////////////////////////////////////////////////
//                                                  //
//                   CONFIGURATION                  //
//                                                  //
//////////////////////////////////////////////////////

#define DEBUG true

//////////////////////////////////////////////////////
//                                                  //
//                    STRUCTURES                    //
//                                                  //
//////////////////////////////////////////////////////

/// @brief Holds navigation data.
struct NAVIGATION_DATA
{
    /// @brief Describes if data are valid.
    bool isValid = true;

    /// @brief Next direction description.
    String nextDirection = "";

    /// @brief Next direction distance.
    String nextDirectionDistance = "";

    /// @brief Arrival time.
    String arrivalTime = "";

    /// @brief Left time and distance.
    String leftData = "";

    /// @brief Inequality operator to compare two NAVIGATION_DATA objects.
    bool operator!=(const NAVIGATION_DATA &other) const
    {
        return nextDirection != other.nextDirection ||
               nextDirectionDistance != other.nextDirectionDistance ||
               arrivalTime != other.arrivalTime ||
               leftData != other.leftData;
    }
};