#include "Base58.h"
#include <vector>

char const *Base58::get_alphabet() {
    static char constexpr map[] = {"123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"};
    return &map[0];
}

signed char const *Base58::get_inverse() {
    static signed char constexpr base58DecodeMap[256] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1, -1, -1, -1, -1,
            -1, 9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
            22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
            -1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
            47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    return &base58DecodeMap[0];
}

std::string Base58::base58_encode(const std::string &Input) {
    // Skip & count leading zeroes.
    auto begin = Input.data();
    auto end = Input.data() + Input.size();
    int zeroes = 0;
    int length = 0;
    while (end - begin > 0 && *begin == 0) {
        ++begin;
        zeroes++;
    }
    // Allocate enough space in big-endian base58 representation.
    int size = (end - begin) * 138 / 100 + 1; // log(256) / log(58), rounded up.
    std::vector<unsigned char> b58(size);
    // Process the bytes.
    while ((end - begin) > 0) {
        int carry = (std::uint8_t) *begin;
        int i = 0;
        // Apply "b58 = b58 * 256 + ch".
        for (auto it = b58.rbegin(); (carry != 0 || i < length) && (it != b58.rend()); it++, i++) {
            carry += 256 * (*it);
            *it = carry % 58;
            carry /= 58;
        }
        length = i;
        ++begin;
    }
    // Skip leading zeroes in base58 result.
    auto it = b58.begin() + (size - length);
    while (it != b58.end() && *it == 0)
        it++;
    // Translate the result into a string.
    std::string str;
    str.reserve(zeroes + (b58.end() - it));
    str.assign(zeroes, '1');

    auto const encodeMap = get_alphabet();
    while (it != b58.end())
        str += encodeMap[*(it++)];
    return str;
}

std::string Base58::base58_decode(const std::string &Input) {
    auto begin = Input.data();
    // Skip leading spaces.
    while (*begin && isspace(*begin))
        begin++;
    // Skip and count leading '1's.
    int zeroes = 0;
    int length = 0;
    while (*begin == '1') {
        zeroes++;
        begin++;
    }
    // Allocate enough space in big-endian base256 representation.
    std::vector<std::uint8_t> b256(strlen(begin) * 733 / 1000 + 1); // log(58) / log(256), rounded up.
    auto const decodeMap = get_inverse();
    std::string str;
    // Process the characters.
    while (*begin && !isspace(*begin)) {
        // Decode base58 character
        int carry = (std::uint8_t) decodeMap[*begin];
        if (carry == -1)  // Invalid b58 character
            return str;
        int i = 0;
        for (auto it = b256.rbegin(); (carry != 0 || i < length) && (it != b256.rend()); ++it, ++i) {
            carry += 58 * (*it);
            *it = carry % 256;
            carry /= 256;
        }
        length = i;
        begin++;
    }
    // Skip trailing spaces.
    while (isspace(*begin))
        begin++;
    if (*begin != 0)
        return str;
    // Skip leading zeroes in b256.
    auto it = b256.begin();
    while (it != b256.end() && *it == 0)
        it++;
    // Copy result into output vector.
    while (it != b256.end())
        str.push_back((char) *(it++));
    return str;
}



