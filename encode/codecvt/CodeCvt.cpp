#include "CodeCvt.h"


std::optional<std::string> CodeCvt::WstrToStr(const std::wstring &Src, UINT CodePage) noexcept {
    if (Src.empty())
        return std::nullopt;
    int byteSize = WideCharToMultiByte(CodePage,
                                       NULL,
                                       Src.c_str(),
                                       (int) Src.length(),
                                       nullptr,
                                       0,
                                       nullptr,
                                       nullptr
    );
    if (byteSize <= 0)
        return std::nullopt;
    std::string szResult(byteSize, 0);
    WideCharToMultiByte(CodePage,
                        NULL,
                        Src.c_str(),
                        (int) Src.length(),
                        &szResult.at(0),
                        byteSize,
                        nullptr,
                        nullptr
    );
    return szResult;
}

std::optional<std::wstring> CodeCvt::StrToWstr(const std::string &Src, UINT CodePage) noexcept {
    if (Src.empty())
        return std::nullopt;
    // 以字符为单位
    int charSize = MultiByteToWideChar(CodePage,
                                       NULL,
                                       Src.c_str(),
                                       (int) Src.length(),
                                       nullptr,
                                       0
    );
    if (charSize <= 0)
        return std::nullopt;
    std::wstring wszResult(charSize, 0);
    MultiByteToWideChar(CodePage,
                        NULL,
                        Src.c_str(),
                        (int) Src.length(),
                        &wszResult.at(0),
                        charSize
    );
    return wszResult;
}

std::unique_ptr<char[]> CodeCvt::WstrToStr(wchar_t *Src, UINT CodePage) noexcept {
    int byteSize = WideCharToMultiByte(CodePage,
                                       NULL,
                                       Src,
                                       -1,
                                       nullptr,
                                       0,
                                       nullptr,
                                       nullptr
    );
    if (byteSize <= 0)
        return {};
    std::unique_ptr<char[]> Dst = std::make_unique<char[]>(byteSize);
    WideCharToMultiByte(CodePage,
                        NULL,
                        Src,
                        -1,
                        Dst.get(),
                        byteSize,
                        nullptr,
                        nullptr
    );
    return Dst;
}

std::unique_ptr<wchar_t[]> CodeCvt::StrToWstr(char *Src, UINT CodePage) noexcept {
    // 以字符为单位
    int charSize = MultiByteToWideChar(CodePage,
                                       NULL,
                                       Src,
                                       -1,
                                       nullptr,
                                       0
    );
    if (charSize <= 0)
        return {};
    std::unique_ptr<wchar_t[]> Dst = std::make_unique<wchar_t[]>(charSize);
    MultiByteToWideChar(CodePage,
                        NULL,
                        Src,
                        -1,
                        Dst.get(),
                        charSize
    );
    return Dst;
}