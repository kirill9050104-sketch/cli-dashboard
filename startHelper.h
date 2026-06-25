#pragma once
#include <ctime>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include "stringHelper.h"


struct datetime {
	std::string date; // ДД.ММ.ГГ
	std::string time; // ЧЧ:ММ
};

datetime getCurrentTime();

std::wstring saveUserName();

std::wstring getUserName();

std::wstring getCurrentGreeting(const std::wstring& username);