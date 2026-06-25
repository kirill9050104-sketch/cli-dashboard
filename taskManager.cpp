#include "taskManager.h"

void taskManager::addTask(std::wstring task) {
    json taskObj = { {"content", toUtf8(task)}, {"status", 0}};
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
        int status = item.value("status", 0);

		std::wstring content = toWstring(contentStr);

        std::wcout << count << L". " << content;
        if (status == 1) {
            std::wcout << L" ✅";
        }
        std::wcout << std::endl;
		count++;
	}
	std::wcout << L"---------------------------------------------------" << std::endl;
}

bool taskManager::changeStatus(int id, int status /*0-не выполненено, 1-выполнено*/)
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
        tasks.at(id - 1)["status"] = status;
    }
    else {
		std::wcout << L"Неверный идентификатор задачи." << std::endl;
		return false;
    }

    saveToJson(taskPath, tasks);

    return true;
}