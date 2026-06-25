#include <string>
#include <Windows.h>

std::wstring toWstring(std::string s)
{
    if (s.empty())
        return L"";
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string toUtf8(const std::wstring& wstr)
{
    if (wstr.empty())
        return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring convNumToTwo_digitW(std::wstring number) {
    if (number.size() == 1) {
        number = L'0' + number;
    }
    return number;
}

std::string convNumToTwo_digitA(std::string number) {
    if (number.size() == 1) {
        number = '0' + number;
    }
    return number;
}
