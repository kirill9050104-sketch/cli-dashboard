#pragma once
#include <atomic>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "jsonHelper.h"
#include "startHelper.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

class pomodoroTimer
{
public:
    void startTimer(int minutes);

    void stopTimer();

    void pauseTimer();

    void resumeTimer();

private:
    enum class State {
        STOPPED,
        RUNNING,
        PAUSED
    };

    std::wstring stateToWString(State state);

    // Обязательно делаем переменную атомарной!
    static std::atomic<State> currentState;

    void TimerWorkerFunc(int minutes);

	bool saveTimer(int minutes, int realseconds, const datetime& start, State state);

    bool saveTimer(int minutes, const datetime& start) {
        return saveTimer(minutes, minutes * 60, start, State::STOPPED);
    }

    fs::path timerPath = fs::current_path() / "timers.json";
};

// Инициализация остается почти такой же
inline std::atomic<pomodoroTimer::State> pomodoroTimer::currentState{ pomodoroTimer::State::STOPPED };