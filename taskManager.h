#pragma once
#include <ctime>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
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

	enum class TCPriority /*task completion priority*/ {
		UNKNOWN, // неизвестен
		LOW,	 // низкий
		MEDIUM,  // средний
		HIGH     // высокий
	};

	fs::path taskPath = fs::current_path() / "tasks.json";

	void addTask(std::wstring task);

	void showCurrentTasks();

	bool changeStatus(int id, taskManager::TCStatus status);

	bool changePriority(int id, taskManager::TCPriority priority);

	bool changeDueDate(int id, datetime dueDate);

	taskManager::TCPriority WStringToTCP(std::wstring status);

	taskManager::TCStatus WStringToTCS(std::wstring status);
private:
	std::wstring TCSToWString(taskManager::TCStatus status);

	std::wstring TCPToWString(taskManager::TCPriority priority);
};