#pragma once
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

static int lastErr;

json readJson(const fs::path& pathToJson);

bool saveToJson(const fs::path& pathToJson, const json& jsonToSave);

bool addToJson(const fs::path& pathToJson, const json& jsonToAdd);
