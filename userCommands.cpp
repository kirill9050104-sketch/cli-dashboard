#include "userCommands.h"


void userCommands::chekCommand()
{
	std::wstring input;
	std::getline(std::wcin, input);
	PCommand command = comPars.parseCommand(input);
	std::wstring commandBody = command.commandBody;
	std::wstring commandName = command.commandName;

	if (commandName == L"task.add") {
		std::wstring task = commandBody;
		taskManag.addTask(task);
		return;
	}

	if (commandName == L"task.display") {
		taskManag.showCurrentTasks();
		return;
	}

	if (commandName == L"pomodoro.start") {
		pomodoro.startTimer(25);
		return;
	}

	if (commandName == L"pomodoro.stop") {
		std::wcout << L"Таймер остановлен." << std::endl;
		pomodoro.stopTimer();
		return;
	}

	if (commandName == L"pomodoro.resume") {
		std::wcout << L"Таймер продолжаеться." << std::endl;
		pomodoro.resumeTimer();
		return;
	}

	if (commandName == L"pomodoro.pause") {
		std::wcout << L"Таймер приостановлен." << std::endl;
		pomodoro.pauseTimer();
		return;
	}

	if (commandName == L"task.status.set") {
		taskStatusSet(commandBody);
		return;
	}

	if (commandName == L"task.priority.set") {
		taskPrioritySet(commandBody);
		return;
	}

	if (commandName == L"task.due_date.set") {
		taskDueDateSet(commandBody);
		return;
	}

	std::wcout << L"Ошибка! Еще не зделана реализация этой команды." << std::endl;
}

void userCommands::taskStatusSet(std::wstring arguments) {
	std::wstringstream stream(arguments);
	int id = -1;
	std::wstring statusStr = L"";
	std::wstring garbage;

	stream >> id >> statusStr;

	if (stream.fail()) {
		std::wcout << L"Ошибка! Номер задачи должен быть числом, а статус текстом." << std::endl;
		return;
	}

	if (stream >> garbage) {
		std::wcout << L"Ошибка! Обнаружены лишние аргументы в конце команды." << std::endl;
		return;
	}

	taskManager::TCStatus status = taskManag.WStringToTCS(statusStr);

	if (id <= 0 || (status == taskManager::TCStatus::UNKNOWN)) {
		std::wcout << L"Ошибка! аргументы введены не верно." << std::endl;
		return;
	}

	if (!taskManag.changeStatus(id, status)) {
		std::wcout << L"Ошибка! Не удалось изменить статус задачи. Проверьте номер задачи." << std::endl;
		return;
	}

	std::wcout << L"Статус успешно установлен на " << statusStr << L" у задачи под номером " << id << std::endl;
}

void userCommands::taskPrioritySet(std::wstring arguments)
{
	std::wstringstream stream(arguments);
	int id = -1;
	std::wstring priorityStr = L"";
	std::wstring garbage;

	stream >> id >> priorityStr;

	if (stream.fail()) {
		std::wcout << L"Ошибка! Номер задачи должен быть числом, а приоритет текстом." << std::endl;
		return;
	}

	if (stream >> garbage) {
		std::wcout << L"Ошибка! Обнаружены лишние аргументы в конце команды." << std::endl;
		return;
	}

	taskManager::TCPriority priority = taskManag.WStringToTCP(priorityStr);
	
	if (id <= 0 || (priority == taskManager::TCPriority::UNKNOWN)) {
		std::wcout << L"Ошибка! аргументы введены не верно." << std::endl;
		return;
	}

	if (!taskManag.changePriority(id, priority)) {
		std::wcout << L"Ошибка! Не удалось изменить приоритет задачи. Проверьте номер задачи." << std::endl;
		return;
	}

	std::wcout << L"Приоритет успешно установлен на " << priorityStr << L" у задачи под номером " << id << std::endl;
}

void userCommands::taskDueDateSet(std::wstring arguments)
{
	std::wstringstream stream(arguments);
	int id = -1;
	std::wstring dateStr = L"";
	std::wstring timeStr = L"";
	std::wstring garbage;

	if (!(stream >> id >> dateStr >> timeStr)) {
		std::wcout << L"Ошибка! Номер задачи должен быть числом, а дата и время указаны через пробел." << std::endl;
		return;
	}

	if (stream >> garbage) {
		std::wcout << L"Ошибка! Обнаружены лишние аргументы в конце команды." << std::endl;
		return;
	}

	datetime dueDate = datetime::from_string(toUtf8(dateStr), toUtf8(timeStr));

	if (id <= 0 || dueDate.date == "") {
		std::wcout << L"Ошибка! Аргументы введены неверно или формат даты нарушен." << std::endl;
		return;
	}
	if (dueDate < getCurrentTime()) {
		std::wcout << L"Ошибка! Дата не может быть раньше сегодняшнего дня." << std::endl;
		return;
	}
	if (!taskManag.changeDueDate(id, dueDate)) {
		std::wcout << L"Ошибка! Не удалось изменить срок выполнения задачи. Проверьте номер задачи." << std::endl;
		return;
	}

	std::wcout << L"Срок выполнения успешно установлен на " << dateStr << L" | " << timeStr << L" у задачи под номером " << id << std::endl;
}
