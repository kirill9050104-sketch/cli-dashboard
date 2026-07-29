#pragma once
#include <vector>
#include <iostream>
#include "stringHelper.h"
#include "jsonHelper.h"

struct PCommand /*parse command*/ {
	std::wstring commandName;
	std::wstring commandBody;
};

struct ACommand /*add command*/ {
	std::wstring name;
	std::wstring description;
	std::wstring typeUse;
	std::wstring usageExample;
};

class commandParser
{
public:
	commandParser();

	void printCommands();

	void addComand(const ACommand& command);

	void addComands(const std::vector<ACommand>& addCommands);

	ACommand getCommand(int id);

	PCommand parseCommand(const std::wstring& text);
private:
	std::vector<ACommand> commands;

	std::wstring trim(const std::wstring& str);

	fs::path commandsPath = fs::current_path() / "commands.json";
};