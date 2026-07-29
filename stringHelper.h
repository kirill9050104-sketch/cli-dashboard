#pragma once
#include <string>
#include <sstream> // Позволяет работать со строковыми потоками


std::string toUtf8(const std::wstring& wstr);

std::wstring toWstring(const std::string& s);

std::wstring convNumToTwo_digitW(const std::wstring& number);

std::string convNumToTwo_digitA(const std::string& number);