#include <thread>
#include <Windows.h>
#include <unordered_map>
#include <stdexcept>
#include "stringHelper.h"
#include "pomodoroTimer.h"

std::wstring pomodoroTimer::stateToWString(State state)
{
	static const std::unordered_map<State, std::wstring> mapper = {
		{ State::PAUSED, L"paused" },
		{ State::RUNNING, L"running" },
		{ State::STOPPED, L"stopped" }
	};

	// Строка 2: Ищем переданный enum в нашей таблице
	auto it = mapper.find(state);

	// Строка 3: Если нашли — возвращаем строку, иначе кидаем исключение
	if (it != mapper.end()) {
		return it->second;
	}

	throw std::invalid_argument("Unknown timer state");
}

void pomodoroTimer::TimerWorkerFunc(int minutes)
{
	std::wcout << L"Помодоро таймер запущен на " << minutes << L" минут." << std::endl;
	datetime startTimeStamp = getCurrentTime();
	pomodoroTimer::currentState = pomodoroTimer::State::RUNNING;
	int realseconds = 0;
	for (int seconds = minutes * 60; seconds >= 0; seconds--)
	{
		realseconds = seconds;
		if (currentState.load() == pomodoroTimer::State::RUNNING) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::wstring minutesWstr = convNumToTwo_digitW(std::to_wstring(seconds / 60));
			std::wstring secondsWStr = convNumToTwo_digitW(std::to_wstring(seconds % 60));
			std::wstring title = L"Фокус: " + minutesWstr + L":" + secondsWStr + L" | Dashboard";
			SetConsoleTitleW(title.c_str());
		}

		while (currentState.load() == pomodoroTimer::State::PAUSED) {
			// Ждем. Чтобы не нагружать процессор на 100%, добавляем микро-паузу
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (currentState.load() == pomodoroTimer::State::STOPPED) {
			break;
		}
	}
	SetConsoleTitleW(L"Cli Dashboard");
	currentState = pomodoroTimer::State::STOPPED;
	if (realseconds == 0) {
		MessageBoxW(NULL, L"Фокусировка завершена. Пора немного передохнуть!", L"Pomodoro", MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
	}
	if (realseconds > 0) {
		MessageBoxW(NULL, L"Фокусировка была прервана.", L"Pomodoro", MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
	}
	saveTimer(minutes, realseconds, startTimeStamp, currentState);
}

void pomodoroTimer::startTimer(int minutes)
{
	std::thread timerThread([this, minutes]() {
		this->TimerWorkerFunc(minutes);
		});
	timerThread.detach();
}

void pomodoroTimer::stopTimer()
{
	pomodoroTimer::currentState = pomodoroTimer::State::STOPPED;
}

void pomodoroTimer::pauseTimer()
{
	pomodoroTimer::currentState = pomodoroTimer::State::PAUSED;
}

void pomodoroTimer::resumeTimer()
{
	pomodoroTimer::currentState = pomodoroTimer::State::RUNNING;
}

bool pomodoroTimer::saveTimer(int minutes, int realseconds, datetime start, pomodoroTimer::State state) {
	realseconds = minutes * 60 - realseconds;
	int realminutes = realseconds / 60;
	realseconds = realseconds % 60;
	std::string minutesStr = convNumToTwo_digitA(std::to_string(minutes)) + ":00";
	std::string realminutesStr = convNumToTwo_digitA(std::to_string(realminutes));
	std::string realsecondsStr = convNumToTwo_digitA(std::to_string(realseconds));
	json timerObj = {
		{"start", start.date + " || " + start.time},
		{"time", realminutesStr + ":" + realsecondsStr + " / " + minutesStr},
		{"state", toUtf8(stateToWString(state))}
	};
	return addToJson(timerPath, timerObj);
}