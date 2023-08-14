#ifndef LIBSUPERCPP4WIN_CODECVT_H
#define LIBSUPERCPP4WIN_CODECVT_H

#include <string>
#include <memory>
#include <optional>
#include <Windows.h>


namespace CodeCvt {
    //used for string
    std::optional<std::string> WstrToStr(const std::wstring &Src, UINT CodePage = CP_ACP);
    std::optional<std::wstring> StrToWstr(const std::string &Src, UINT CodePage = CP_ACP);

    //used for char *
    std::unique_ptr<char[]> WstrToStr(wchar_t *Src, UINT CodePage = CP_ACP);
    std::unique_ptr<wchar_t[]> StrToWstr(char *Src, UINT CodePage = CP_ACP);
}
#endif //LIBSUPERCPP4WIN_CODECVT_H
