#pragma once
#include <ctime>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <sstream>
#include <tuple>
#include <fstream>
#include "stringHelper.h"


struct datetime {
	std::string date; // ДД.ММ.ГГ
	std::string time; // ЧЧ:ММ

    // Вспомогательная функция для парсинга и получения компонентов по старшинству
    auto get_components() const {
        int day = 0, month = 0, year = 0;
        int hour = 0, minute = 0;

        // Безопасный разбор строки даты
        std::stringstream ss_date(date);
        char dot1, dot2;
        if (!(ss_date >> day >> dot1 >> month >> dot2 >> year) || dot1 != '.' || dot2 != '.') {
            // Если формат нарушен, возвращаем нулевые значения или выбрасываем исключение
            return std::make_tuple(year, month, day, hour, minute);
        }

        // Безопасный разбор строки времени
        std::stringstream ss_time(time);
        char colon;
        if (!(ss_time >> hour >> colon >> minute) || colon != ':') {
            return std::make_tuple(year, month, day, hour, minute);
        }

        return std::make_tuple(year, month, day, hour, minute);
    }

    static datetime from_string(const std::string& raw_date, const std::string& raw_time) {
        std::stringstream ss_d(raw_date), ss_t(raw_time);
        int d, m, y, h, min;
        char dot1, dot2, colon;

        // Проверяем, что абсолютно все элементы считались корректно
        if ((ss_d >> d >> dot1 >> m >> dot2 >> y) && dot1 == '.' && dot2 == '.' &&
            (ss_t >> h >> colon >> min) && colon == ':') {

            // Если юзер ввел "26", дотягиваем до "2026"
            if (y < 100) {
                y += 2000;
            }

            // Приводим к красивому стандартному виду с ведущими нулями
            std::stringstream formatted_date, formatted_time;
            formatted_date << std::setw(2) << std::setfill('0') << d << "."
                << std::setw(2) << std::setfill('0') << m << "." << y;

            formatted_time << std::setw(2) << std::setfill('0') << h << ":"
                << std::setw(2) << std::setfill('0') << min;

            return { formatted_date.str(), formatted_time.str() };
        }

        // Если данные — мусор, возвращаем пустой или дефолтный объект
        return { "", "" };
    }

    // Оператор равенства (==)
    bool operator==(const datetime& other) const {
        return get_components() == other.get_components();
    }

    // Оператор "меньше" (<)
    bool operator<(const datetime& other) const {
        return get_components() < other.get_components();
    }

    // Оператор "больше" (>)
    bool operator>(const datetime& other) const {
        return get_components() > other.get_components();
    }
};

datetime getCurrentTime();

std::wstring saveUserName();

std::wstring getUserName();

std::wstring getCurrentGreeting(const std::wstring& username);

int compareDatetime(datetime firstDatetime, datetime secondDatetime);