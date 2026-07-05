//#include "commandParser.h"
//
//void commandParser::addComand(std::wstring command) {
//	commands.push_back(command);
//}
//
//command commandParser::parseCommand(std::wstring text)
//{
//	for (auto& commandName : commands) {
//		std::wstring trimCommand = trim(text);
//		if (trimCommand.find(commandName) == 0) {
//			std::wstring command_name = commandName;
//			int start = command_name.size();
//			int end = commandName.size() - command_name.size();
//			std::wstring command_body = trimCommand.substr(start, end);
//			command result = { command_name, command_body };
//			return result;
//		}
//	}
//	return command();
//}
//
//std::wstring commandParser::trim(std::wstring str)
//{
//	int start = str.find_first_not_of(L"\t\n\r ");
//	int end = str.find_last_not_of(L"\t\n\r ");
//	std::wstring result = str.substr(start, end - start);
//	if (result.empty()) {
//		return L"";
//	}
//	return result;
//}
