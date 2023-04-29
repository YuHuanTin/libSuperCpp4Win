// src from: https://github.com/bitcoin/bitcoin/blob/e9262ea32a6e1d364fb7974844fadc36f931f8c6/src/base58.cpp

#ifndef LIBSUPERCPP4WIN_BASE58_H
#define LIBSUPERCPP4WIN_BASE58_H

#include <string>

class Base58 {
private:
    inline static
    char const *get_alphabet();

    inline static
    signed char const*get_inverse();

public:
    static std::string base58_encode(const std::string &Input);
    static std::string base58_decode(const std::string &Input);
};


#endif //LIBSUPERCPP4WIN_BASE58_H
