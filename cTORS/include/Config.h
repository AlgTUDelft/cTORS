/** \file Config.h
 * Describes the Config class
 */
#pragma once
#ifndef CONFIG_H
#define CONFIG_H
//!\cond SYS_HEADER
#include <filesystem>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <iostream>
//!\endcond
#include "Exceptions.h"
namespace fs = std::filesystem;
using namespace std;
/** a json object, from nlohmann::json */
using json = nlohmann::json;

/**
 * The Config class describes the configuration for the Engine
 */
class Config
{
private:
	static const string configFileString;
	map<string, bool> businessRules;
	map<string, bool> actionRules;
	map<string, json> actionParams;

	void ImportBusinessRules(const json& j);
	void ImportActionRules(const json& j);
public:
	/** Construct a default Config object */
	Config() = default;
	/** Construct a Config object based on the given json file */
	Config(const string& path);
	/** Returns true iff the BusinessRule defined by the name is active */
	bool IsBusinessRuleActive(const string& name) const;
	/** Returns true iff the ActionGenerator defined by the name is active */
	bool IsGeneratorActive(const string& name) const;
	/** Get the parameters for the ActionGenerator defined by the name */
	const json GetActionParameters(const string& name) const;
};

#endif