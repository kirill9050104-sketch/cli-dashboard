#pragma once
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

static int lastErr;

json readJson(fs::path pathToJson);

bool saveToJson(fs::path pathToJson, json jsonToSave);

bool addToJson(fs::path pathToJson, json jsonToAdd);
