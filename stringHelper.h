#pragma once
#include <string>
#include <sstream> // Позволяет работать со строковыми потоками


std::string toUtf8(const std::wstring& wstr);

std::wstring toWstring(std::string s);

std::wstring convNumToTwo_digitW(std::wstring number);

std::string convNumToTwo_digitA(std::string number);