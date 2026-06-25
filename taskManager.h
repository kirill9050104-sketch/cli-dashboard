#pragma once
#include <ctime>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "stringHelper.h"
#include "jsonHelper.h"

class taskManager {
public:
	fs::path taskPath = fs::current_path() / "tasks.json";

	void addTask(std::wstring task);

	void showCurrentTasks();

	bool changeStatus(int id, int status /*0-не выполненено, 1-выполнено*/);
};