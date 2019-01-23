#include <iostream>
#include <cstring>
#include <exception>

namespace utf8 {

    constexpr uint8_t DIFF_BYTES = 0;
    constexpr uint8_t ALL_1_BYTE = 1;
    constexpr uint8_t ALL_2_BYTE = 2;
    constexpr uint8_t ALL_3_BYTE = 3;
    constexpr uint8_t ALL_4_BYTE = 4;

    constexpr char EMPTY = '\xFF';

    struct proxy_code_point
    {
        static constexpr int MODE = 4;
        using pchar = char*;
        pchar code_unit[MODE];
        proxy_code_point(char* cstr)
        {
            int i = 0;
            while (*cstr != '\0' && i < MODE) {
                code_unit[i] = cstr;
                ++cstr;
                ++i;
            }

            std::cout << i << std::endl;
            //for (int i = 0; i < MODE && *cstr != '\0'; ++cstr, ++i) code_unit[i] = cstr;
            //code_unit[0] = cstr;
            //code_unit[1] = &cstr[1];
        }
        proxy_code_point& operator=(const char* cstr)
        {
            for (int i = 0; i < MODE && *cstr != '\0'; ++cstr, ++i) *code_unit[i] = *cstr;
            return *this;
        }

        proxy_code_point& operator=(const proxy_code_point& pcp)
        {
            for (int i = 0; i < MODE && *pcp.code_unit[i] != EMPTY; ++i) *code_unit[i] = *pcp.code_unit[i];
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const proxy_code_point& rhs)
        {
            os.write(*rhs.code_unit, MODE);
            return os;
        }

        char& operator[](int key)
        {
            return *code_unit[key];
        }
    };

    struct virtual_string_data
    {
        char* data;
        std::size_t sz;
        std::size_t len;
        virtual_string_data() : data(nullptr) { }
        virtual ~virtual_string_data() { delete[] data; }

        virtual proxy_code_point operator[](int key) = 0;
    };

    template< int MODE >
    struct base_string_data : public virtual_string_data
    {

        base_string_data() : virtual_string_data() { }
        base_string_data(std::size_t length) { len = length; sz = len * MODE; data = new char[sz]; }

        virtual proxy_code_point operator[](int key)
        {
            proxy_code_point cpc(&data[key * MODE]);
            for (int i = MODE; i < 4; ++i) cpc[i] = EMPTY;
            return &data[key * MODE];
        }
    };

    struct base_string
    {
        virtual_string_data* pdata;
        base_string() : pdata(nullptr) { }
        base_string(const char* cstr) //: pdata(new base_string_data< 2 >(strlen(cstr))) 
        {
            // TODO do it correctly work with empty strings ""
            int len = 0;
            const char* s = cstr;
            //const char* const cs = s;
            int all_bytes = process_codepoint(s);
            //std::cout << all_bytes << std::endl;
            //throw std::exception("wtd");
            if (!all_bytes) throw std::runtime_error("utf8: error code point");
            s += all_bytes;
            len++;
            while (*s != '\0') {
                len++;
                int bytes = process_codepoint(s);
                s += bytes;
            }

            // TODO make it constexpr
            //if (!all_bytes) pdata = new base_string_data< UTF8_DIFF_BYTES >(len);
            if (all_bytes == 1) pdata = new base_string_data< ALL_1_BYTE >(len);
            else if (all_bytes == 2) pdata = new base_string_data< ALL_2_BYTE >(len);
            else if (all_bytes == 3) pdata = new base_string_data< ALL_3_BYTE >(len);
            else if (all_bytes == 4) pdata = new base_string_data< ALL_4_BYTE >(len);
        }
        ~base_string() { delete pdata; }

        static constexpr bool is_first1_byte(const char* iter) { return (*iter >= '\x00' && *iter <= '\x7F'); }
        static constexpr bool is_first2_byte(const char* iter) { return (*iter >= '\xC2' && *iter <= '\xDF'); }
        static constexpr bool is_first3_byte(const char* iter) { return (*iter >= '\xE0' && *iter <= '\xEF'); }
        static constexpr bool is_first4_byte(const char* iter) { return (*iter >= '\xF0' && *iter <= '\xF4'); }
        static constexpr bool is_contus_byte(const char* iter) { return (*iter >= '\x80' && *iter <= '\xBF'); }
        static constexpr int process_codepoint(const char* iter)
        {
            if (is_first1_byte(iter)) return 1;
            if (is_first2_byte(iter) && is_contus_byte(iter + 1)) return 2;
            if (is_first3_byte(iter) && is_contus_byte(iter + 1) && is_contus_byte(iter + 2)) return 3;
            if (is_first4_byte(iter) && is_contus_byte(iter + 1) && is_contus_byte(iter + 2) && is_contus_byte(iter + 3)) return 4;
            return 0;
        }
    };

    struct string
    {
        base_string str;
        string(const char* cstr) : str(cstr) { for (int i = 0; i < str.pdata->sz - 1; ++cstr, ++i) str.pdata->data[i] = *cstr; }

        friend std::ostream& operator<<(std::ostream& os, const string& rhs)
        {
            os.write(rhs.str.pdata->data, rhs.str.pdata->sz);
            return os;
        }

        proxy_code_point operator[](int k)
        {
            return str.pdata->operator[](k);
        }

        std::size_t size() const
        {
            return str.pdata->sz;
        }

        std::size_t length() const
        {
            return str.pdata->len;
        }
    };


}
