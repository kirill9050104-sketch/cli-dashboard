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
	enum class TCStatus /*task completion status*/ {
		IN_PROGRESS, // в процессе
		UNKNOWN,     // статус неизвестен
		COMPLETED,   // выполнено
		OVERDUE      // просроцено
	};

	fs::path taskPath = fs::current_path() / "tasks.json";

	void addTask(std::wstring task);

	void showCurrentTasks();

	bool changeStatus(int id, taskManager::TCStatus status /*0-не выполненено, 1-выполнено*/);

	TCStatus WStringToTCS(std::wstring status);
private:
	std::wstring TCSToWString(taskManager::TCStatus status);
};