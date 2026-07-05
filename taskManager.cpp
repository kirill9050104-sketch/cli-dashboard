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

    return L"unknown";
}

taskManager::TCPriority taskManager::WStringToTCP(std::wstring status)
{
    static const std::unordered_map<std::wstring, taskManager::TCPriority> mapper = {
        { L"unknown", taskManager::TCPriority::UNKNOWN },
        { L"inProgress", taskManager::TCPriority::HIGH },
        { L"completed", taskManager::TCPriority::MEDIUM },
        { L"overdue", taskManager::TCPriority::LOW }
    };

    // Строка 2: Ищем переданный enum в нашей таблице
    auto it = mapper.find(status);

    // Строка 3: Если нашли — возвращаем строку, иначе кидаем UNKNOWN
    if (it != mapper.end()) {
        return it->second;
    }

    return taskManager::TCPriority::UNKNOWN;
}

std::wstring taskManager::TCPToWString(taskManager::TCPriority priority)
{
    static const std::unordered_map<taskManager::TCPriority, std::wstring> mapper = {
        { taskManager::TCPriority::UNKNOWN, L"unknown" },
        { taskManager::TCPriority::HIGH, L"high" },
        { taskManager::TCPriority::MEDIUM, L"medium" },
        { taskManager::TCPriority::LOW, L"low" }
    };

    // Строка 2: Ищем переданный enum в нашей таблице
    auto it = mapper.find(priority);

    // Строка 3: Если нашли — возвращаем строку, иначе кидаем исключение
    if (it != mapper.end()) {
        return it->second;
    }

    return L"unknown";
}

taskManager::TCStatus taskManager::WStringToTCS(std::wstring priority)
{
    static const std::unordered_map<std::wstring, taskManager::TCStatus> mapper = {
        { L"unknown", taskManager::TCStatus::UNKNOWN },
        { L"inProgress", taskManager::TCStatus::IN_PROGRESS },
        { L"completed", taskManager::TCStatus::COMPLETED },
        { L"overdue", taskManager::TCStatus::OVERDUE }
    };

    // Строка 2: Ищем переданный enum в нашей таблице
    auto it = mapper.find(priority);

    // Строка 3: Если нашли — возвращаем строку, иначе кидаем UNKNOWN
    if (it != mapper.end()) {
        return it->second;
    }

    return taskManager::TCStatus::UNKNOWN;
}

void taskManager::addTask(std::wstring task) {
	std::string taskStr = toUtf8(task);
    std::string statusStr = toUtf8(TCSToWString(taskManager::TCStatus::IN_PROGRESS));
    std::string priorityStr = toUtf8(TCPToWString(taskManager::TCPriority::LOW));
    json taskObj = { {"content", taskStr}, {"status", statusStr}, {"priority", priorityStr}};
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
        taskManager::TCPriority priority = WStringToTCP(toWstring(item.value("priority", "unknown")));
        std::string dueDateStr = item.value("dueDate", "00.00.00");
        std::string dueTimeStr = item.value("dueTime", "00:00");
        datetime dueDate = datetime::from_string(dueDateStr, dueTimeStr);

		std::wstring content = toWstring(contentStr);

        datetime currentTime = getCurrentTime();
        if (currentTime > dueDate) {
            changeStatus(count, TCStatus::OVERDUE);
            status = TCStatus::OVERDUE;
        }

        std::wcout << count << L". " << content << L" || Статус: ";
        switch (status) {
        case taskManager::TCStatus::COMPLETED:
            std::wcout << L"Выполнено [✓]";
            break;
        case taskManager::TCStatus::UNKNOWN:
            std::wcout << L"Не задан [-]";
            break;
        case taskManager::TCStatus::IN_PROGRESS:
            std::wcout << L"В процессе...";
            break;
        case taskManager::TCStatus::OVERDUE:
            std::wcout << L"Просрочено [X]";
            break;
        }

        std::wcout << L" || Приоритет: ";
        switch (priority) {
            case taskManager::TCPriority::HIGH:
            std::wcout << L"Высокий";
            break;
        case taskManager::TCPriority::UNKNOWN:
            std::wcout << L"Не задан [-]";
            break;
        case taskManager::TCPriority::MEDIUM:
            std::wcout << L"Средний";
            break;
        case taskManager::TCPriority::LOW:
            std::wcout << L"Низкий";
            break;
        }

        if (dueDateStr != "00.00.00") {
            std::wcout << L" || Срок выполнения: " << toWstring(dueDateStr) << L" | " << toWstring(dueTimeStr);
        } else {
            std::wcout << L" || Срок выполнения: Не задан [-]";
        }

        std::wcout << std::endl;
		count++;
	}
	std::wcout << L"----------------------------------------------------" << std::endl;
}

bool taskManager::changeStatus(int id, taskManager::TCStatus status)
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

bool taskManager::changePriority(int id, taskManager::TCPriority priority)
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
        std::string priorityStr = toUtf8(TCPToWString(priority));
        tasks.at(id - 1)["priority"] = priorityStr;
    }
    else {
        std::wcout << L"Неверный идентификатор задачи." << std::endl;
        return false;
    }

    saveToJson(taskPath, tasks);

    return true;
}

bool taskManager::changeDueDate(int id, datetime dueDate)
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
        std::string dueDateStr = dueDate.date;
        tasks.at(id - 1)["dueDate"] = dueDateStr;
        std::string dueTimeStr = dueDate.time;
        tasks.at(id - 1)["dueTime"] = dueTimeStr;
    }
    else {
        std::wcout << L"Неверный идентификатор задачи." << std::endl;
        return false;
    }

    saveToJson(taskPath, tasks);

    return true;
}
