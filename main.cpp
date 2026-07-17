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
#include "commandParser.h"
#include "userCommands.h"

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
	commandParser comPars;
	userCommands uC;
	datetime currentTimestemp = getCurrentTime();
	std::wcout << L"Сегодня " << toWstring(currentTimestemp.date) << std::endl;
	std::wcout << L"Текущее время: " << toWstring(currentTimestemp.time) << std::endl;
	std::wcout << std::endl;

	taskManag.showCurrentTasks();
	std::wcout << std::endl;

	comPars.printCommands();
	std::wcout << std::endl;

	while (true) {
		uC.chekCommand();
	}
}