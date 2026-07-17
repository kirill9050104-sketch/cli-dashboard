#include "commandParser.h"

commandParser::commandParser()
{
	json commands = readJson(commandsPath);
	if (commands.empty() && commands.is_array()) {
		std::wcout << L"Ошибка! Не вышло загрузить команды." << std::endl;
		return;
	}
	for (const auto& command : commands) {
		ACommand result;

		result.name = toWstring(command.value("name", ""));
		result.description = toWstring(command.value("description", ""));
		result.typeUse = toWstring(command.value("typeUse", ""));
		result.usageExample = toWstring(command.value("usageExample", ""));

		addComand(result);
	}
}

void commandParser::printCommands()
{
	std::wcout << L"_______________________Список команд_______________________" << std::endl;
	std::wcout << std::endl;

	for (const auto& command : commands) {
		std::wcout << L"------------Команда " << command.name << L"-----------" << std::endl;
		std::wcout << L"Описание: " << command.description << std::endl;
		std::wcout << L"Вид формления: " << command.typeUse << std::endl;
		std::wcout << L"Пример использования: " << command.usageExample << std::endl;
		std::wcout << L"--------------------------------------------------------" << std::endl;
		std::wcout << std::endl;
	}
	std::wcout << L"___________________________________________________________" << std::endl;
}

void commandParser::addComand(const ACommand& command) {
	commands.push_back(command);
}

void commandParser::addComands(const std::vector<ACommand>& addCommands) {
	for (const auto& command : addCommands) {
		commands.push_back(command);
	}
}

ACommand commandParser::getCommand(int id)
{
	return commands[id];
}

PCommand commandParser::parseCommand(const std::wstring& text)
{

	std::wcout << L"Логи обработки команды" << std::endl;

	std::wstring trimCommand = trim(text);

	if (trimCommand.empty()) {
		std::wcout << L"Ошибка! Команда не может быть пустой." << std::endl;
		return PCommand();
	}

	if (trimCommand[0] != L'/') {
		std::wcout << L"Ошибка! Команда должна начинатся со знака '/'." << std::endl;
		return PCommand();
	}

	trimCommand = trimCommand.substr(1);
	std::wcout << L"Чистая команда: " << trimCommand << std::endl;
	for (auto& command : commands) {
		if (trimCommand.find(command.name) == 0) {
			std::wstring command_name = command.name;
			std::wcout << L"Имя команды: " << command_name << std::endl;
			size_t start = command_name.size() + 1;
			std::wstring command_body = L"";
			if (trimCommand.size() > start) {
				command_body = trimCommand.substr(start);
			}
			std::wcout << L"Тело команды: " << command_body << std::endl;
			PCommand result = { command_name, command_body };
			return result;
		}
	}

	std::wcout << L"Ошибка! Команда не найдена." << std::endl;

	return PCommand();
}

std::wstring commandParser::trim(const std::wstring& str)
{
	if (str.empty()) {
		return L"";
	}
	size_t start = str.find_first_not_of(L"\t\n\r ");
	size_t end = str.find_last_not_of(L"\t\n\r ");
	std::wstring result = str.substr(start);
	if (result.empty()) {
		return L"";
	}
	return result;
}
