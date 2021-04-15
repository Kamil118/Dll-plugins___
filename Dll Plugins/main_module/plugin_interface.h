#pragma once
#include <vector>
#include <string>

class plugin_base {
public:
	virtual void execute() = 0;
};

class plugin_constructor {
public:
	virtual plugin_base* operator()(void) = 0;
	virtual std::string get_class_name() { return "unspecified"; };
};

// make sure to implement __declspec(dllexport) std::vector<plugin_constructor*> __cdecl get_constructors(void)
