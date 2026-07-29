#include "jsonHelper.h"

bool saveToJson(const fs::path& pathToJson, const json& jsonToSave) {
    std::ofstream wf(pathToJson);

    if (wf.is_open())
    {
        wf << jsonToSave.dump(4);
        wf.close();
    }
    else {
        return false;
    }
    return true;
}

bool addToJson(const fs::path& pathToJson, const std::string& key, const json& jsonToAdd)
{
    json newJson = readJson(pathToJson);

    if (lastErr == 1) {
        std::wcout << L"Ошибка синхронизации json, проверьте файл по пути " << pathToJson << std::endl;
        return false;
    }

    if (key.empty()) {
        if (!newJson.is_array()) newJson = json::array();
        newJson.push_back(jsonToAdd);
    }
    else {
        if (!newJson.is_object()) newJson = json::object();
        if (!newJson.contains(key) || !newJson[key].is_array()) {
            newJson[key] = json::array();
        }
        newJson[key].push_back(jsonToAdd);
    }

    return saveToJson(pathToJson, newJson);
}

json readJson(const fs::path& pathToJson) {
    lastErr = -1;
    json tasks = json::array();

    // 1. Проверяем существование файла
    if (!fs::exists(pathToJson) || fs::file_size(pathToJson) == 0)
    {
        lastErr = 0; // Файл отсутствует или пуст на диске
        return tasks;
    }

    // 2. Открываем файл (работает и с wstring-путями в C++17)
    std::ifstream file(pathToJson);
    if (!file.is_open())
    {
        lastErr = 1; // Не удалось открыть файл (например, занят процессом)
        std::wcout << L"Не удалось открыть файл (например, занят процессом)" << std::endl;
        return tasks;
    }

    try
    {
        // Безопасный парсинг (не упадет, если в файле "битый" текст)
        json data = json::parse(file, nullptr, true);
        file.close(); // Закрываем сразу после чтения

        if (!data.is_array())
        {
            lastErr = 2; // Структура нарушена (в файле объект {}, а не массив [])
            return data;
        }

        if (data.empty())
        {
            lastErr = 3; // Массив валиден, но пуст
            return tasks;
        }

        lastErr = -1; // Ошибок нет, всё успешно прочитано
        return data;
    }
    catch (const json::parse_error& e) // Ловим специфичную ошибку JSON
    {
        
        lastErr = 4; // Ошибка синтаксиса JSON (битый файл)
        std::wcout << L"Ошибка синтаксиса JSON (битый файл)" << std::endl;
        if (file.is_open()) file.close();
        return tasks;
    }
    catch (const std::exception& e)
    {
        lastErr = 1; // Общая системная ошибка чтения
        if (file.is_open()) file.close();
        return tasks;
    }
}