#ifndef SCRIPTINTERPRETER_HPP
#define SCRIPTINTERPRETER_HPP

#include <vector>
#include <map>
#include <deque>
#include <memory>
#include "Util/Frame.hpp"
#include "Util/Token.hpp"
#include "Util/FunctionEntry.hpp"

/**
 * This class loads and runs scripts
 *
 * \ingroup Scripting
 */
class Script
{
public:
    typedef std::map<std::string,Value> ContextData;

    typedef std::shared_ptr<Script> Ptr;
    typedef Value (*LibraryFunction)(std::vector<Value>, Script*, const ContextData&);

protected:
    ContextData contextData;
    std::map<std::string,LibraryFunction> libraryFunctions;

private:
	std::string original;
	bool stopping, stopped;
	std::deque<Frame> stackFrames;
	Frame globalFrame;
	std::map<std::string,FunctionEntry> functions; //index to jump to for functions
	std::map<std::string,int> branchTable;
    std::vector<Token> tokens;
    enum ConditionalState
    {
    	LastConditionalExecuted,
    	LastConditionalFailed,
    	NoConditional
    };

    void initBuiltins();

    void reset();

    void locateFunctions(); //runs through tokens and populates functions map, no code is executed

    Value evaluate(std::vector<Token> tkns); //evaluates a string of tokens to a value. Resolves variables and functions

    Value evalEq(std::vector<Token> tkns); //Condenses values and operators into a single value

    Value combine(Token left, Token op, Token right);

    Value& getIdentifier(std::string id);

    bool isVariable(std::string id);

    bool isFunction(std::string name);

    bool isLibraryFunction(std::string name);

    Value executeLibraryFunction(std::string name, std::vector<Value> args);

    Value runTokens(int pos = 0); //this returns the end of the script is reached, or a return token is encountered

public:
	/**
	 * Creates an empty script
	 */
	Script();

	/**
	 * Loads the script from the given file
	 *
	 * \param scr The file to load the script from
	 */
	Script(std::string scr);

	/**
	 * Copies the Script from an existing Script
	 */
    Script(const Script& scr);

	/**
	 * Cleans up all memory
	 */
	virtual ~Script();

	/**
	 * Loads the script from the given file or string.
	 *
	 * \param scr The file to load the script from, or a string containing the script itself
	 */
	void load(std::string scr);

	/**
	 * Runs the script
	 */
	virtual void run();

	/**
	 * Stops the script and returns when the script has finished
	 */
	void stop();

	/**
	 * Returns whether or not the script is actively stopping
	 */
    bool isStopping();

	/**
	 * Tells whether or not the script is currently running
	 */
	bool isRunning();
};

#endif // SCRIPTINTERPRETER_HPP