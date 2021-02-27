#include "Config.h"

const string Config::configFileString = "config.json";


Config::Config(string folderName) {
	try {
		ifstream fileInput(fs::path(folderName) / fs::path(configFileString));
		if (!fileInput.good())
			throw InvalidConfigException("The specified file '" + folderName + "' does not exist");
		json j;
		fileInput >> j;
		importBusinessRules(j["business_rules"]);
		importActionRules(j["actions"]);
	}
	catch (exception& e) {
		cout << "Error in loading config: " << e.what() << "\n";
		throw e;
	}
}

void Config::importBusinessRules(const json& j) {
	for (auto& jit : j["rules"].items()) {
		auto& name = jit.key();
		auto& value = jit.value();
		businessRules[name] = value.at("on").get<bool>();
	}
}

void Config::importActionRules(const json& j) {
	for (auto& jit : j.items()) {
		auto& name = jit.key();
		auto& value = jit.value();
		actionRules[name] = value.at("on").get<bool>();
		actionParams[name] = value.at("parameters");
	}
}

bool Config::IsBusinessRuleActive(string name) const { 
	auto it = businessRules.find(name);
	if(it == businessRules.end()) return true;
	return it->second;
}

bool Config::IsGeneratorActive(string name) const {
	auto it = actionRules.find(name);
	if(it == actionRules.end()) return true;
	return it->second;
}

const json Config::GetActionParameters(string name) const {
	auto it = actionParams.find(name);
	if(it == actionParams.end()) return json();
	return it->second;
}
