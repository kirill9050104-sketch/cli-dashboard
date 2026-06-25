#include <iostream>
#include <locale>
#include <fcntl.h>    // Для _setmode
#include <io.h>       // Для _setmode
#include <Windows.h>

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
	std::wcout << L"Для изменения статуса задачи напишите '/task.status.set [номер] [статус](0-в процессе, 1-выполнена)'." << std::endl;
	std::wcout << L"Для просмотра текущих задач напишите '/task.display'." << std::endl;
	std::wcout << L"Для запуска помодоро таймера на 25 мин напишите '/pomodoro.start'." << std::endl;
	std::wcout << L"Для приостановки помодоро таймера напишите '/pomodoro.pause'." << std::endl;
	std::wcout << L"Для полной остановки помодоро таймера напишите '/pomodoro.stop'." << std::endl;
	std::wcout << L"Для продолжения помодоро таймера напишите '/pomodoro.resume'.\n" << std::endl;

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
			int status = -1;
			stream >> id >> status;
			if (stream.fail()) {
				std::wcout << L"Ошибка! Номер задачи и статус должны быть числами." << std::endl;
			}
			else if (id <= 0 || (status != 0 && status != 1)) {
				std::wcout << L"Ошибка! аргументы введены не верно." << std::endl;
			}
			else {
				taskManag.changeStatus(id, status);
				std::wcout << L"Статус успешно установлен на " << status << L" у задачи под номером " << id << std::endl;
			}
		}
	}
}