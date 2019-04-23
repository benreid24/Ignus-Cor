#include "Shared/Scripts/Script Interpreter.hpp"
#include "Shared/Scripts/Script Manager.hpp"
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
	try {
	    return libraryFunctions[name](args);
	}
	catch (out_of_range e) {
		throw runtime_error("Not enough arguments in call to "+name);
	}
}

namespace {
    Value print(vector<Value> args) {
        for (unsigned int i = 0; i<args.size(); ++i) {
            if (args.at(i).type==Value::Integer)
                cout << args.at(i).iValue;
            else
                cout << args.at(i).sValue;
        }
        cout << endl;
        return Value();
    }

    map<string,Script::LibraryFunction> builtins = {
        {"print", &print}
    };
}
