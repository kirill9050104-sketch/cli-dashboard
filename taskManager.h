#pragma once
#include <ctime>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "stringHelper.h"
#include "jsonHelper.h"
#include "startHelper.h"

class taskManager {
public:
	enum class TCStatus /*task completion status*/ {
		UNKNOWN,     // неизвестен
		IN_PROGRESS, // в процессе
		COMPLETED,   // выполнено
		OVERDUE      // просрочено
	};

	enum class TCPriority : int/*task completion priority*/ {
		UNKNOWN = 0, // неизвестен
		LOW = 1,	 // низкий
		MEDIUM = 2,  // средний
		HIGH = 3     // высокий
	};

	fs::path taskPath = fs::current_path() / "tasks.json";

	void addTask(const std::wstring& task);

	void showCurrentTasks();

	bool changeStatus(int id, taskManager::TCStatus status);

	bool changePriority(int id, taskManager::TCPriority priority);

	bool changeDueDate(int id, const datetime& dueDate);

	taskManager::TCPriority WStringToTCP(const std::wstring& status);

	taskManager::TCStatus WStringToTCS(const std::wstring& status);
private:
	std::wstring TCSToWString(taskManager::TCStatus status);

	std::wstring TCPToWString(taskManager::TCPriority priority);
};