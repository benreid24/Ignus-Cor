#include "Shared/Scripts/Script.hpp"
#include "Shared/Scripts/ScriptManager.hpp"
#include "Shared/Properties.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include <cmath>
using namespace std;
using namespace sf;

namespace {
extern map<string,Script::LibraryFunction> builtins;
}

void Script::initBuiltins() {
    libraryFunctions = builtins;
}

bool Script::isLibraryFunction(string name) {
	return libraryFunctions.find(name) != libraryFunctions.end();
}

Value Script::executeLibraryFunction(string name, vector<Value> args) {
    if (isDryRun)
        return Value("0");
	try {
	    return libraryFunctions[name](args, this, contextData);
	}
	catch (const out_of_range& e) {
		throw runtime_error("Not enough arguments in call to "+name);
	}
}

namespace {
Value print(vector<Value> args, Script*, const Script::ContextData&) {
    for (unsigned int i = 0; i<args.size(); ++i) {
        if (args.at(i).type==Value::Integer)
            cout << args.at(i).iValue;
        else
            cout << args.at(i).sValue;
    }
    cout << endl;
    return Value();
}

//For runScript, make copy first then run that

map<string,Script::LibraryFunction> builtins = {
    {"print", &print}
};
}
