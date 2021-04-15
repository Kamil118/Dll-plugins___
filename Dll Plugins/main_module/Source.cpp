#define MAINMODULE
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "plugin_interface.h"

using namespace std;

vector<string> explode(string s, const char * const delimiter) {
	vector<string> ret;
	size_t last_position = 0;
	size_t position = s.find(delimiter, 0);
	if (position == string::npos) {
		ret.push_back(s);
		return ret;
	}
	while (position != string::npos) {
		ret.push_back(s.substr(last_position, position - last_position));
		last_position = position;
		position = s.find(delimiter, position +strlen(delimiter));
	}
	ret.push_back(s.substr(last_position + strlen(delimiter), s.length() - last_position + strlen(delimiter)));
	return ret;
}

class PluginManager {
	
private:
	struct plugin_data {
		plugin_base* ptr;
		string name;
		HINSTANCE source;
	};
	vector<plugin_data> plugin_instances;
public:
	
	void load_dll(string name) {
		//name = ".\" + name;
		HINSTANCE source = LoadLibraryA((LPCSTR)name.c_str());
		if (source != NULL) {
			auto func = (std::vector<plugin_constructor*> (__cdecl*)(void))GetProcAddress(source, "?get_constructors@@YA?AV?$vector@PAVplugin_constructor@@V?$allocator@PAVplugin_constructor@@@std@@@std@@XZ");//what the hell is this name
			if (func != NULL) {
				std::vector<plugin_constructor*> convec = func();
				for (auto p : convec) {
					plugin_data np;
					np.source = source;
					np.ptr = p->operator()();
					np.name = p->get_class_name();
					plugin_instances.push_back(np);
					delete p;
				}
			}
			else {
				cout << "could not find get_constuctors() in the " << name << endl;
				cout << GetLastError() << endl;
			}
		}
		else {
			cout << "Could not load " + name << endl;
			cout << GetLastError() << endl;
		}
	}
	void call_all(void) {
		for (plugin_data p : plugin_instances) {
			cout << p.name << endl << "From: " << p.source << endl;
			p.ptr->execute();
			cout << "\n=================\n";
		}
	}
	
};

int main() {
	SetErrorMode(0xFFFFFFFF);
	PluginManager pm;
	string line = "";
	getline(cin, line);
	vector<string> tokens = explode(line, " ");
	while (tokens.size() < 1 || tokens[0] != "exit") {
		if (tokens.size() != 0) {
			if (tokens[0] == "load") {
				if (tokens.size() == 2) {
					pm.load_dll(tokens[1]);
				}
				else {
					cout << "\n No file provided\n";
				}
			}
			if (tokens[0] == "do") {
				pm.call_all();
			}
		}
		getline(cin, line);
		tokens = explode(line," ");
	}
}