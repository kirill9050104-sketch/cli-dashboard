#include "taskManager.h"

std::wstring taskManager::TCSToWString(taskManager::TCStatus status)
{
    static const std::unordered_map<taskManager::TCStatus, std::wstring> mapper = {
        { taskManager::TCStatus::UNKNOWN, L"unknown" },
        { taskManager::TCStatus::IN_PROGRESS, L"inProgress" },
        { taskManager::TCStatus::COMPLETED, L"completed" },
        { taskManager::TCStatus::OVERDUE, L"overdue" }
    };

    // Строка 2: Ищем переданный enum в нашей таблице
    auto it = mapper.find(status);

    // Строка 3: Если нашли — возвращаем строку, иначе кидаем исключение
    if (it != mapper.end()) {
        return it->second;
    }

    throw std::invalid_argument("Unknown TC state");
}

taskManager::TCStatus taskManager::WStringToTCS(std::wstring status)
{
    static const std::unordered_map<std::wstring, taskManager::TCStatus> mapper = {
        { L"unknown", taskManager::TCStatus::UNKNOWN },
        { L"inProgress", taskManager::TCStatus::IN_PROGRESS },
        { L"completed", taskManager::TCStatus::COMPLETED },
        { L"overdue", taskManager::TCStatus::OVERDUE }
    };

    // Строка 2: Ищем переданный enum в нашей таблице
    auto it = mapper.find(status);

    // Строка 3: Если нашли — возвращаем строку, иначе кидаем исключение
    if (it != mapper.end()) {
        return it->second;
    }

    return taskManager::TCStatus::UNKNOWN;
}

void taskManager::addTask(std::wstring task) {
    json taskObj = { {"content", toUtf8(task)}, {"status", toUtf8(TCSToWString(taskManager::TCStatus::UNKNOWN))}};
    addToJson(taskPath, taskObj);
}

void taskManager::showCurrentTasks() {
	json tasks = readJson(taskPath);

	// ИСПРАВЛЕНО: Если код ошибки критический — выходим
    if (lastErr > 0 && lastErr != 3) { 
        std::wcout << L"Ошибка синхронизации задач, проверьте файл 'tasks.json'." << std::endl;
        return;
    }

    // ИСПРАВЛЕНО: Если массив пустой (неважно, файла не было или в файле просто [])
    if (tasks.empty()) {
        std::wcout << L"Список задач пуст." << std::endl;
        return;
    }

	std::wcout << L"------------Список запланированных задач------------" << std::endl;

	int count = 1;
	for (auto& item : tasks)
	{
		std::string contentStr = item.value("content", "неизвесная задача");
        taskManager::TCStatus status = WStringToTCS(toWstring(item.value("status", "unknown")));

		std::wstring content = toWstring(contentStr);

        std::wcout << count << L". " << content << L" || Статус: ";
        switch (status) {
        case taskManager::TCStatus::COMPLETED:
            std::wcout << L"Выполнено ✅";
            break;
        case taskManager::TCStatus::UNKNOWN:
            std::wcout << L"Неизвестен";
            break;
        case taskManager::TCStatus::IN_PROGRESS:
            std::wcout << L"В процессе...";
            break;
        case taskManager::TCStatus::OVERDUE:
            std::wcout << L"Просрочено 🚳❌";
            break;
        }
        std::wcout << std::endl;
		count++;
	}
	std::wcout << L"---------------------------------------------------" << std::endl;
}

bool taskManager::changeStatus(int id, taskManager::TCStatus status /*0-не выполненено, 1-выполнено*/)
{
    json tasks = readJson(taskPath);

    // ИСПРАВЛЕНО: Если код ошибки критический — выходим
    if (lastErr > 0 && lastErr != 3) {
        std::wcout << L"Ошибка синхронизации задач, проверьте файл 'tasks.json'." << std::endl;
        return false;
    }

    // ИСПРАВЛЕНО: Если массив пустой (неважно, файла не было или в файле просто [])
    if (tasks.empty()) {
        std::wcout << L"Список задач пуст." << std::endl;
        return false;
    }

    if (id > 0 && id <= tasks.size()) {
        std::string statusStr = toUtf8(TCSToWString(status));
        tasks.at(id - 1)["status"] = statusStr;
    }
    else {
		std::wcout << L"Неверный идентификатор задачи." << std::endl;
		return false;
    }

    saveToJson(taskPath, tasks);

    return true;
}