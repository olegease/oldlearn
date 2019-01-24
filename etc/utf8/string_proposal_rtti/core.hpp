#pragma once

#include "../func.hpp"
#include <cstdint>
#include <iostream>



namespace utf8 {
    constexpr uint8_t DIFF_BYTES = 0; // not implemented yet
    constexpr uint8_t ALL_1_BYTE = 1;
    constexpr uint8_t ALL_2_BYTE = 2;
    constexpr uint8_t ALL_3_BYTE = 3;
    constexpr uint8_t ALL_4_BYTE = 4;

    constexpr char EMPTY = '\xFF';
    char EMPTY_CHAR = EMPTY;
    char* const EMPTY_POINTER = &EMPTY_CHAR;
}

