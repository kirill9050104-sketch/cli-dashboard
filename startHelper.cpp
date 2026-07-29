#include <ctime>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <chrono>
#include "stringHelper.h"
#include "startHelper.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

//fs::path userSettingsPath = fs::current_path() / "userSettings.json";

datetime getCurrentTime() {
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	char bufferDate[80];
	char bufferTime[80];

	datetime result;

	strftime(bufferDate, sizeof(bufferDate), "%d.%m.%Y", &ltm);
	std::string date = std::string(bufferDate);

	strftime(bufferTime, sizeof(bufferTime), "%H:%M", &ltm);
	std::string time = std::string(bufferTime);

    result = datetime::from_string(date, time);

	return result;
}

fs::path getUserSettingsPath() {
    return fs::current_path() / "userSettings.json";
}

std::wstring saveUserName() {
    std::wcout << L"Приветствуем, у вас не сохранено ваше имя." << std::endl;
    std::wcout << L"Введите своё имя (как к Вам обращаться): ";
    std::wstring username;
    std::getline(std::wcin, username);
    std::wcout << L"Запись вашего имени в файл 'userSettings.json'..." << std::endl;

    json config = json::object();
    fs::path userSettingsPath = getUserSettingsPath(); // Исправлено

    // 1. Проверяем, существует ли файл и не пустой ли он
    if (fs::exists(userSettingsPath) && fs::file_size(userSettingsPath) > 0) {
        std::ifstream rf(userSettingsPath);
        if (rf.is_open()) {
            try {
                config = json::parse(rf);
            }
            catch (const json::parse_error& e) {
                config = json::object();
            }
            rf.close();
        }
    }

    // 2. Записываем имя
    config["name"] = toUtf8(username);

    // 3. Сохраняем обновленный JSON обратно в файл
    std::ofstream wf(userSettingsPath);
    if (wf.is_open()) {
        wf << config.dump(4);
        wf.close();
        std::wcout << L"Имя пользователя успешно обновлено в файле!" << std::endl;
    }
    else {
        std::wcout << L"Не удалось открыть файл для записи." << std::endl;
    }
    return username;
}

std::wstring getUserName() {
    fs::path userSettingsPath = getUserSettingsPath(); // ИСПРАВЛЕНО: теперь переменная видна!

    // Если файла нет или он пустой, сразу запрашиваем имя у пользователя
    if (!fs::exists(userSettingsPath) || fs::file_size(userSettingsPath) == 0) {
        return saveUserName();
    }

    json config = json::object();
    std::ifstream rf(userSettingsPath);

    if (rf.is_open()) {
        try {
            config = json::parse(rf);
        }
        catch (const json::parse_error& e) {
            rf.close();
            return saveUserName(); // Если файл поврежден, перезаписываем
        }
        rf.close();
    }

    // 2. Читаем значение из ключа "name" (ИСПРАВЛЕН комментарий и логика)
    // Если ключа "name" внутри файла не оказалось, запрашиваем ввод имени
    if (!config.contains("name")) {
        return saveUserName();
    }

    std::string name = config.value("name", "DefaultName");
    return toWstring(name);
}

int getCurrentHour() {
    // 1. Получаем текущую точку времени
    auto now = std::chrono::system_clock::now();

    // 2. Конвертируем в привычный time_t
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);

    // 3. Безопасно получаем локальное время
    std::tm localTime;

    localtime_s(&localTime, &time_now);

    // 4. Возвращаем час
    return localTime.tm_hour;
}

std::wstring getCurrentGreeting(const std::wstring& username) {
    int hour = getCurrentHour();
    std::wstring greeting;

    if (hour >= 5 && hour <= 11) {
        greeting = L"Доброе утречко, " + username + L"! Пора покорять новые вершины!!!";
    }
    else if (hour >= 12 && hour <= 16) {
        greeting = L"Добрый день, " + username + L"! Готовы к продуктивной работе?";
    }
    else if (hour >= 17 && hour <= 22) {
        greeting = L"Добрый вечер, " + username + L"! Самое время завершить начатое.";
    }
    else { // Срабатывает для часов: 23, 0, 1, 2, 3, 4
        greeting = L"Привет, " + username + L"! Не спится? Значит пора поработать в тишине.\n*Главное о сне тоже не забывайте!!!";
    }

    return greeting;
}
