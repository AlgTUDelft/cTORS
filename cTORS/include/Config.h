#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <iostream>
#include "Exceptions.h"
namespace fs = std::filesystem;
using namespace std;
using json = nlohmann::json;

class Config
{
private:
	static const string configFileString;
	map<string, bool> businessRules;
	map<string, bool> actionRules;
	map<string, json> actionParams;

	void importBusinessRules(const json& j);
	void importActionRules(const json& j);
public:
	Config() = default;
	Config(string path);
	bool IsBusinessRuleActive(string name) const;
	bool IsGeneratorActive(string name) const;
	const json GetActionParameters(string name) const;
};

