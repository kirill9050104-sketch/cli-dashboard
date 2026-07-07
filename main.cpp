#include <iostream>
#include <locale>
#include <fcntl.h>    // Для _setmode
#include <io.h>       // Для _setmode
#include <Windows.h>
#include <vector>

#include "stringHelper.h"
#include "taskManager.h"
#include "startHelper.h"
#include "pomodoroTimer.h"

int main() {
	std::setlocale(LC_ALL, "Russian");

	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	SetConsoleTitleW(L"Cli Dashboard");

	std::wstring username = getUserName();
	std::wstring greeting = getCurrentGreeting(username);
	std::wcout << greeting << std::endl;
	taskManager taskManag;
	pomodoroTimer pomodoro;
	datetime currentTimestemp = getCurrentTime();
	std::wcout << L"Сегодня " << toWstring(currentTimestemp.date) << std::endl;
	std::wcout << L"Текущее время: " << toWstring(currentTimestemp.time) << std::endl;
	std::wcout << std::endl;

	taskManag.showCurrentTasks();
	std::wcout << std::endl;

	std::wcout << L"Для добавления задачи напишите '/task.add [текст]'." << std::endl;
	std::wcout << L"Для изменения статуса задачи напишите '/task.status.set [номер] [статус]('inProgress' - в процессе, 'completed' - выполнено')." << std::endl;
	std::wcout << L"Для изменения приоритета задачи напишите '/task.priority.set [номер] [приоритет]('low' - низкий, 'medium' - средний, 'high' - высокий')." << std::endl;
	std::wcout << L"Для изменения срока выполнения задачи напишите '/task.due_date.set [номер] [дата](формат: ДД.ММ.ГГ) [время](формат: ЧЧ:ММ)." << std::endl;
	std::wcout << L"Для просмотра текущих задач напишите '/task.display'." << std::endl;
	std::wcout << L"Для запуска помодоро таймера на 25 мин напишите '/pomodoro.start'." << std::endl;
	std::wcout << L"Для приостановки помодоро таймера напишите '/pomodoro.pause'." << std::endl;
	std::wcout << L"Для полной остановки помодоро таймера напишите '/pomodoro.stop'." << std::endl;
	std::wcout << L"Для продолжения помодоро таймера напишите '/pomodoro.resume'." << std::endl;
	std::wcout << std::endl;

	while (true) {
		std::wstring input;
		std::getline(std::wcin, input);
		if (input.find(L"/task.add ") != std::wstring::npos) {
			std::wstring task = input.substr(10, input.size() - 10);
			taskManag.addTask(task);
			std::wcout << L"Задача '" << task << L"' добавлена в список дел." << std::endl;
		}
		if (input.find(L"/task.display") != std::wstring::npos) {
			taskManag.showCurrentTasks();
		}
		if (input.find(L"/pomodoro.start") != std::wstring::npos) {
			pomodoro.startTimer(25);
		}
		if (input.find(L"/pomodoro.stop") != std::wstring::npos) {
			std::wcout << L"Таймер остановлен." << std::endl;
			pomodoro.stopTimer();
		}
		if (input.find(L"/pomodoro.resume") != std::wstring::npos) {
			std::wcout << L"Таймер продолжаеться." << std::endl;
			pomodoro.resumeTimer();
		}
		if (input.find(L"/pomodoro.pause") != std::wstring::npos) {
			std::wcout << L"Таймер приостановлен." << std::endl;
			pomodoro.pauseTimer();
		}
		if (input.find(L"/task.status.set ") != std::wstring::npos) {
			std::wstring arguments = input.substr(17);
			std::wstringstream stream(arguments);
			int id = -1;
			std::wstring statusStr = L"";
			stream >> id >> statusStr;
			taskManager::TCStatus status = taskManag.WStringToTCS(statusStr);
			if (stream.fail()) {
				std::wcout << L"Ошибка! Номер задачи должен быть числом, а статус текстом." << std::endl;
			}
			else if (id <= 0 || (status == taskManager::TCStatus::UNKNOWN)) {
				std::wcout << L"Ошибка! аргументы введены не верно." << std::endl;
			}
			else {
				if (taskManag.changeStatus(id, status)) {
					std::wcout << L"Статус успешно установлен на " << statusStr << L" у задачи под номером " << id << std::endl;
				}
				else {
					std::wcout << L"Ошибка! Не удалось изменить статус задачи. Проверьте номер задачи." << std::endl;
				}
			}
		}
		if (input.find(L"/task.priority.set ") != std::wstring::npos) {
			std::wstring arguments = input.substr(19);
			std::wstringstream stream(arguments);
			int id = -1;
			std::wstring priorityStr = L"";
			stream >> id >> priorityStr;
			taskManager::TCPriority priority = taskManag.WStringToTCP(priorityStr);
			if (stream.fail()) {
				std::wcout << L"Ошибка! Номер задачи должен быть числом, а приоритет текстом." << std::endl;
			}
			else if (id <= 0 || (priority == taskManager::TCPriority::UNKNOWN)) {
				std::wcout << L"Ошибка! аргументы введены не верно." << std::endl;
			}
			else {
				if (taskManag.changePriority(id, priority)) {
					std::wcout << L"Приоритет успешно установлен на " << priorityStr << L" у задачи под номером " << id << std::endl;
				}
				else {
					std::wcout << L"Ошибка! Не удалось изменить приоритет задачи. Проверьте номер задачи." << std::endl;
				}
			}
		}
		if (input.find(L"/task.due_date.set ") != std::wstring::npos) {
			std::wstring arguments = input.substr(19);
			std::wstringstream stream(arguments);
			int id = -1;
			std::wstring dateStr = L"";
			std::wstring timeStr = L"";
			// 1. Попытка считать ровно 3 аргумента
			std::wstring garbage;
			if (!(stream >> id >> dateStr >> timeStr)) {
				std::wcout << L"Ошибка! Номер задачи должен быть числом, а дата и время указаны через пробел." << std::endl;
			}
			// 2. Проверка на "хвосты" в потоке
			else if (stream >> garbage) {
				std::wcout << L"Ошибка! Обнаружены лишние аргументы в конце команды." << std::endl;
			}
			else {
				// Парсим только после того, как убедились, что строки извлечены корректно
				datetime dueDate = datetime::from_string(toUtf8(dateStr), toUtf8(timeStr));

				// 3. Бизнес-валидация данных (с учетом "" из твоего from_string)
				if (id <= 0 || dueDate.date == "") {
					std::wcout << L"Ошибка! Аргументы введены неверно или формат даты нарушен." << std::endl;
				}
				else if (dueDate < getCurrentTime()) {
					std::wcout << L"Ошибка! Дата не может быть раньше сегодняшнего дня." << std::endl;
				}
				else {
					if (taskManag.changeDueDate(id, dueDate)) {
						std::wcout << L"Срок выполнения успешно установлен на " << dateStr << L" | " << timeStr << L" у задачи под номером " << id << std::endl;
					}
					else {
						std::wcout << L"Ошибка! Не удалось изменить срок выполнения задачи. Проверьте номер задачи." << std::endl;
					}
				}
			}
		}
	}
}