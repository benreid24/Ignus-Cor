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
    typedef Value (*LibraryFunction)(const std::vector<Value>&, Script*, const ContextData&);

protected:
    ContextData contextData;
    std::map<std::string,LibraryFunction> libraryFunctions;

private:
    Value runResult;
	std::string original;
	bool stopping, stopped, isDryRun;
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

    Value& getIdentifier(const std::string& id, const Token* info = nullptr);

    bool isVariable(const std::string& id);

    bool isFunction(const std::string& name);

    bool isLibraryFunction(const std::string& name);

    Value executeLibraryFunction(const std::string& name, const std::vector<Value>& args);

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
	Script(const std::string& scr);

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
	void load(const std::string& scr);

	/**
	 * Runs the script
	 */
	void run();

	/**
	 * Runs the script, but does not modify any external state. Returns error message if any
	 */
    std::string dryRun();

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

	/**
	 * Returns the result of running the script
	 */
    Value result();

    /**
     * Reduces an error message from dryRun to just the error and line
     */
    static std::string minimizeError(std::string error);
};

#endif // SCRIPTINTERPRETER_HPP
