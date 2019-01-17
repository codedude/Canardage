
#include "utils.hpp"

#include <string>
#include <cstring>
#include <istream>
#include <iostream>


static void *(*const volatile memset_ptr)(void *, int, size_t) = memset;
void Utils::memset_s(void *ptr, int v, size_t n)
{
    (memset_ptr)(ptr, v, n);
}
/*
    Secure getline from cin with limit (ASCII only)
    Clear cin and return iostate
*/
int Utils::getline_s(char* s, std::streamsize n)
{
    std::fill(s, s+n+1, 0);
    std::cin.getline(s, n);
    int r = std::cin.rdstate();
    std::cin.clear();

    /* non-ASCII chars forbidden */
    int i = 0;
    while(s[i]) {
        if(s[i++] < 0) {
            r |= std::istream::badbit;
            break;
        }
    }

    /* Clear cin if too long input */
    if(r) {
        if(r & std::istream::failbit) {
            std::cin.ignore(2147483647, '\n');
            std::cin.clear();
        }
        std::fill(s, s+n+1, 0);
    }

    return r;
}

