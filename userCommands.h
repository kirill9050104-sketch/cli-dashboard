#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>

#include "stringHelper.h"
#include "taskManager.h"
#include "startHelper.h"
#include "pomodoroTimer.h"
#include "commandParser.h"

class userCommands {
public:
	void chekCommand();

private:
	commandParser comPars;

	pomodoroTimer pomodoro;

	taskManager taskManag;


	void taskStatusSet(std::wstring arguments);

	void taskPrioritySet(std::wstring arguments);

	void taskDueDateSet(std::wstring arguments);
};