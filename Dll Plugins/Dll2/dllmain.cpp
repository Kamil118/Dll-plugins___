// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "plugin_interface.h"
#include <iostream>
#include <string>
#include <vector>

class test_plugin : public plugin_base {
    void execute() override {
        std::cout << "this is first plugin of first dll" << std::endl;
    }
};

class test_plugin2 : public plugin_base {
    void execute() override {
        std::cout << "this is second plugin of first dll" << std::endl;
    }
};

class test_plugin_constructor : public plugin_constructor {
    plugin_base* operator()(void) override { return new test_plugin(); };
    std::string get_class_name() override { return "plugin 1_A"; };
};

class test_plugin_constructor2 : public plugin_constructor {
    plugin_base* operator()(void) override { return new test_plugin2(); };
    std::string get_class_name() override { return "plugin 2_A"; };
};

__declspec(dllexport) std::vector<plugin_constructor*> __cdecl get_constructors(void) {
    std::vector<plugin_constructor*> ret;
    ret.push_back(new test_plugin_constructor());
    ret.push_back(new test_plugin_constructor2());
    return ret;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

