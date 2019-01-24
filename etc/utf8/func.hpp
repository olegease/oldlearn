#pragma once

namespace utf8
{
    // declaration
    constexpr int process_codepoint(const char* iter);

    // 1 line declaration and definition
    constexpr bool is_first1_byte(const char* iter) { return (*iter >= '\x00' && *iter <= '\x7F'); }
    constexpr bool is_first2_byte(const char* iter) { return (*iter >= '\xC2' && *iter <= '\xDF'); }
    constexpr bool is_first3_byte(const char* iter) { return (*iter >= '\xE0' && *iter <= '\xEF'); }
    constexpr bool is_first4_byte(const char* iter) { return (*iter >= '\xF0' && *iter <= '\xF4'); }
    constexpr bool is_contus_byte(const char* iter) { return (*iter >= '\x80' && *iter <= '\xBF'); }

    // definition
    constexpr int process_codepoint(const char* iter)
    {
        if (is_first1_byte(iter)) return 1;
        if (is_first2_byte(iter) && is_contus_byte(iter + 1)) return 2;
        if (is_first3_byte(iter) && is_contus_byte(iter + 1) && is_contus_byte(iter + 2)) return 3;
        if (is_first4_byte(iter) && is_contus_byte(iter + 1) && is_contus_byte(iter + 2) && is_contus_byte(iter + 3)) return 4;
        return 0;
    }
}

