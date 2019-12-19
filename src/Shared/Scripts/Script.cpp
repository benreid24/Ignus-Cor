#include "Shared/Scripts/Script.hpp"
#include "Shared/Scripts/ScriptManager.hpp"
#include "Shared/Scripts/Util/Parser.hpp"
#include <SFML/System.hpp>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <cmath>
using namespace std;
using namespace sf;

namespace {
const string ERROR_FILE_PREFIX = " in file ";

string intToString(double i)
{
       stringstream ss;
       ss << i;
       return ss.str();
}

double stringToInt(const string& s) {
    stringstream ss(s);
    double d;
    ss >> d;
    return d;
}

void raiseError(string error, const Token* token = nullptr) {
    if (token) {
        error += " on line ";
        error += intToString(token->line);
        error += ERROR_FILE_PREFIX;
        error += token->file;
    }
    throw runtime_error(error);
}
}

Script::Script() : isDryRun(false)
{
	reset();
	initBuiltins();
}

Script::Script(const string&scr) : Script()
{
    load(scr);
}

Script::Script(const Script& scr) : Script() {
    contextData = scr.contextData;
    libraryFunctions = scr.libraryFunctions;
    tokens = scr.tokens;
    functions = scr.functions;
    branchTable = scr.branchTable;
}

Script::~Script()
{
	//dtor
}

void Script::reset()
{
	stopping = false;
	stopped = true;
	globalFrame.locals.clear();
	stackFrames.clear();
}

void Script::locateFunctions()
{
	enum
	{
		Reading,
		CheckingParen,
		ScanningArgType,
		ScanningArgName,
		NextArgTransition
	}state = Reading;
	string name;
	FunctionEntry func;
	ArgumentEntry arg;

	for (unsigned int i = 0; i<tokens.size(); ++i)
	{
		switch (state)
		{
		case Reading:
			if (tokens.at(i).type==Token::Identifier)
			{
				if (i>0)
				{
					if (tokens.at(i-1).type==Token::DataType)
					{
						state = CheckingParen;
						name = tokens.at(i).data;
					}
				}
			}
			break;

		case CheckingParen:
			state = Reading;
			if (tokens.at(i).type==Token::Operator && tokens.at(i).data=="(")
			{
				if (tokens.at(i+1).type==Token::DataType)
					state = ScanningArgType;
				else if (tokens.at(i+1).type==Token::Operator && tokens.at(i+1).data==")")
					goto addFunc;
			}
			break;

		case ScanningArgType:
            if (tokens.at(i).type==Token::DataType)
			{
				state = ScanningArgName;
				arg.type = Value::String;
				if (tokens.at(i).data=="int")
					arg.type = Value::Integer;
			}
			else
				raiseError("Was expecting a data type", &tokens.at(i));
			break;

		case ScanningArgName:
			if (tokens.at(i).type==Token::Identifier)
			{
				state = NextArgTransition;
				arg.name = tokens.at(i).data;
				func.arguments.push_back(arg);
			}
			else
				raiseError("Was expecting an identifier next", &tokens.at(i));
			break;

		case NextArgTransition:
			if (tokens.at(i).type==Token::ArgDelim)
                state = ScanningArgType;
			else if (tokens.at(i).type==Token::Operator && tokens.at(i).data==")")
			{
				addFunc:
				state = Reading;
				func.address = i+2; //i+1 is the {, so i+2 is the first token in the function
				functions.insert(make_pair(name,func));
				func.arguments.clear();
			}
			else
				raiseError("Was expecting ',' or ')'", &tokens.at(i));
		}
	}
}

void Script::load(const string&str)
{
	Parser parser(str);
	tokens = parser.getTokens();
	original = parser.getScript();
	reset();
	locateFunctions();
	for (unsigned int i = 0; i<tokens.size(); ++i)
	{
		if (tokens.at(i).type==Token::Label)
		{
			if (branchTable.find(tokens.at(i).data)!=branchTable.end())
				raiseError("Branch label '"+tokens.at(i).data+"' duplicate encountered", &tokens.at(i));
			branchTable[tokens.at(i).data] = i;
		}
	}
}

Value Script::combine(Token left, Token op, Token right)
{
	Value ret;
	ret.type = Value::String;
	if (left.type==Token::IntVal)
		ret.type = Value::Integer;

	if (op.data=="+")
	{
		ret.iValue = left.value+right.value;
        ret.sValue = left.data+right.data;
        if (left.type==Token::IntVal && right.type==Token::StrVal)
		{
			ret.sValue = intToString(left.value)+right.data.c_str();
			ret.type = Value::String;
		}
		if (left.type==Token::StrVal && right.type==Token::IntVal)
		{
			ret.sValue = left.data+intToString(right.value);
			ret.type = Value::String;
		}
		return ret;
	}
	else if (op.data=="!=")
	{
		ret.type = Value::Integer;
		ret.iValue = int(left.value!=right.value);
		if (right.type==Token::StrVal && left.type==Token::StrVal)
        ret.iValue = int(left.data!=right.data);
        else if (left.type==Token::IntVal && right.type==Token::StrVal)
            ret.iValue = int(left.value!=stringToInt(right.data));
		else if (left.type==Token::StrVal && right.type==Token::IntVal)
			ret.iValue = int(left.data!=intToString(right.value));
		return ret;
	}
	else if (op.data=="==")
	{
		ret.type = Value::Integer;
		ret.iValue = int(left.data==right.data);
		if (right.type==Token::IntVal && left.type==Token::IntVal)
			ret.iValue = int(abs(left.value-right.value)<0.0001);
        else if (left.type==Token::IntVal && right.type==Token::StrVal)
            ret.iValue = int(left.value==atoi(right.data.c_str()) && atoi(right.data.c_str())!=0);
		else if (left.type==Token::StrVal && right.type==Token::IntVal)
			ret.iValue = int(left.data==intToString(right.value) && atoi(left.data.c_str())!=0);
		return ret;
	}

	if (right.type!=Token::IntVal)
		raiseError("Was expecting integer", &right);
	if (left.type!=Token::IntVal)
		raiseError("Was expecting integer", &left);

	if (op.data=="-")
		ret.iValue = left.value-right.value;
	else if (op.data=="*")
		ret.iValue = left.value*right.value;
	else if (op.data=="/")
		ret.iValue = left.value/right.value;
	else if (op.data=="%")
		ret.iValue = int(left.value)%int(right.value);
	else if (op.data=="<")
		ret.iValue = left.value<right.value;
	else if (op.data==">")
		ret.iValue = left.value>right.value;
	else if (op.data=="<=")
		ret.iValue = left.value<=right.value;
	else if (op.data==">=")
		ret.iValue = left.value>=right.value;
	else if (op.data=="&&")
		ret.iValue = int(int(left.value+0.5)!=0 && int(right.value+0.5)!=0);
	else if (op.data=="||")
		ret.iValue = int(int(left.value+0.5)!=0 || int(right.value+0.5)!=0);
	else
		raiseError("Illegal operator '"+op.data+"' encountered", &op);

	return ret;
}

Value Script::evalEq(vector<Token> tkns)
{
    //1. recursively find and solve parenthesis
    //2. find and apply unary operators and check to see if the first value is negated
    //3. go left to right combining values with operators following pemdas

    if (tkns.size()==0)
		return Value();

    for (unsigned int i = 0; i<tkns.size(); ++i)
	{
		if (tkns.at(i).type==Token::Operator && tkns.at(i).data=="(")
		{
			unsigned int start = i+1;
			unsigned int end = 0;
			int parenCount = 1;
			vector<Token> subEq;

			while (parenCount>0)
			{
				++i;
				if (tkns.at(i).type==Token::Operator && tkns.at(i).data=="(")
					parenCount++;
				if (tkns.at(i).type==Token::Operator && tkns.at(i).data==")")
					parenCount--;
				if (i>=tkns.size())
					raiseError("Unexpected end of equation", &tkns[tkns.size()-1]);
			}
			end = i-1;
			subEq.reserve(end-start+1);
			for (unsigned int i = start; i<=end; ++i)
				subEq.push_back(tkns.at(i));

			Token t;
			t = evalEq(subEq);
			tkns.erase(tkns.begin()+start,tkns.begin()+end+2);
			t.line = tkns[start].line;
			t.file = tkns[start].file;

			tkns[start-1] = t;
			i = start-1;
		}
	}

	if (tkns[0].type==Token::Operator && tkns[0].data=="-")
	{
		tkns[1].value = -tkns[1].value;
		tkns.erase(tkns.begin());
	}
	for (unsigned int i = 0; i<tkns.size(); ++i)
	{
		if (tkns.at(i).type==Token::Operator && tkns.at(i).data=="!")
		{
            if (tkns.at(i+1).type==Token::IntVal)
				tkns.at(i).value = !bool(int(tkns.at(i).value));
			else if (tkns.at(i+1).type==Token::StrVal)
			{
				tkns.at(i+1).type = Token::IntVal;
                tkns.at(i+1).value = double(tkns.at(i+1).data.size()!=0);
			}
			else
				raiseError("Illegal operand type", &tkns.at(i+1));
		}
	}

	string ops[] = {"*/%", //times, divide, modulo
					"+-", //plus, minus
					">=<==!=", //greater (equal), less (equal), equal, not equal
					"&&||"}; //and, or

	for (int i = 0; i<4; ++i)
	{
		for (unsigned int j = 0; j<tkns.size(); ++j)
		{
			if (ops[i].find(tkns.at(j).data)!=string::npos && tkns.at(j).type==Token::Operator)
			{
			    if (j<=0 || j>=tkns.size()-1)
                    raiseError("Invalid number of operands for operator "+tkns.at(j).data, &tkns.at(j));
				tkns.at(j-1) = combine(tkns.at(j-1),tkns.at(j),tkns.at(j+1));
				tkns.erase(tkns.begin()+j+1);
				tkns.erase(tkns.begin()+j);
				j--;
			}
		}
	}

	if (tkns.size()==1)
	{
		Value r;
		if (tkns[0].type==Token::IntVal)
		{
			r.type = Value::Integer;
			r.iValue = tkns[0].value;
		}
		else
		{
			r.type = Value::String;
			r.sValue = tkns[0].data;
		}
		return r;
	}
	raiseError("An error occurred solving the equation", &tkns[0]);
	return Value(); //unreachable but makes the compiler happy
}

Value Script::evaluate(vector<Token> tkns)
{
	//1. search and replace variables
	//2. search and run functions
	//3. Call evalEq

	for (unsigned int i = 0; i<tkns.size(); ++i)
	{
		if (tkns.at(i).type==Token::Identifier)
		{
			if (isVariable(tkns.at(i).data))
                tkns.at(i) = getIdentifier(tkns.at(i).data, &tkns.at(i));
			else if (!isFunction(tkns.at(i).data))
                raiseError("Unknown identifier '"+tkns.at(i).data+"' encountered", &tkns.at(i));
		}
	}

	enum
	{
		Searching,
		ReadingArgEq
	}state = Searching;
	string name;
	vector<Token> argEq;
	vector<Value> args;
	int parenCount = 0;
	unsigned int start = 0;

	for (unsigned int i = 0; i<tkns.size(); ++i)
	{
		switch (state)
		{
		case Searching:
			if (tkns.at(i).type==Token::Identifier)
			{
				name = tkns.at(i).data;
				state = ReadingArgEq;
				parenCount = 1;
				start = i;
				++i;
                if (tkns.at(i).type != Token::Operator || tkns.at(i).data != "(")
                    raiseError("Unexpected token "+Token::toString(tkns.at(i).type), &tkns.at(i));
				argEq.clear();
				args.clear();
			}
			break;

		case ReadingArgEq:
			if (tkns.at(i).type==Token::Operator && tkns.at(i).data=="(")
			{
				argEq.push_back(tkns.at(i));
				parenCount++;
			}
			else if (tkns.at(i).type==Token::Operator && tkns.at(i).data==")")
			{
				parenCount--;
				if (parenCount==0)
				{
					if (argEq.size()>0)
						args.push_back(evaluate(argEq));
					if (isLibraryFunction(name))
						tkns[start] = executeLibraryFunction(name,args);
					else
					{
						int pc = functions[name].address;
						Frame f;
						for (unsigned int j = 0; j<args.size(); ++j)
						{
							if (j>=functions[name].arguments.size())
								raiseError("Too many arguments passed to function '"+name+"'", &tkns[start]);
							else if (args[j].type!=functions[name].arguments[j].type)
								raiseError("Type mismatch in call to '"+name+"'", &tkns[start]);
							f.locals[functions[name].arguments[j].name] = args[j];
						}
						stackFrames.push_back(f);
						tkns[start] = runTokens(pc);
						stackFrames.pop_back();
					}
					tkns.erase(tkns.begin()+start+1,tkns.begin()+i+1);
					state = Searching;
					i -= 2;
				}
				else
					argEq.push_back(tkns.at(i));
			}
			else if (tkns.at(i).type==Token::ArgDelim)
			{
			    if (parenCount == 1) {
                    args.push_back(evaluate(argEq));
                    argEq.clear();
			    }
			    else
                    argEq.push_back(tkns.at(i));
			}
			else
				argEq.push_back(tkns.at(i));
			break;
		}
	}

	return evalEq(tkns);
}

bool Script::isFunction(const string& name)
{
	return (isLibraryFunction(name) || functions.find(name)!=functions.end());
}

bool Script::isVariable(const string& id) {
    for (auto j = stackFrames.rbegin(); j!=stackFrames.rend(); ++j) {
        if (j->locals.find(id)!=j->locals.end())
            return true;
    }
    if (globalFrame.locals.find(id)!=globalFrame.locals.end())
        return true;
    return false;
}

Value& Script::getIdentifier(const string& id, const Token* info) {
    for (auto j = stackFrames.rbegin(); j!=stackFrames.rend(); ++j) {
        if (j->locals.find(id)!=j->locals.end())
            return j->locals[id];
    }
    if (globalFrame.locals.find(id)!=globalFrame.locals.end())
        return globalFrame.locals[id];
    if (stackFrames.size()>0) {
        stackFrames.back().locals[id] = Value();
        return stackFrames.back().locals[id];
    }
    raiseError("Undeclared identifier '"+id+"'", info);
    static Value poop; return poop; //compiler man
}

Value Script::runTokens(int pos)
{
	vector<Token> tkns;
	Value test;
	string name;
	int parenCount = 0;
	int blockCount = 0; //terminate if blockcount gets to -1 (for loops)
	ConditionalState conditionalState = NoConditional;
	int tokensTillResetCond = 0;
	bool execCond = false, resetCondState = true;

	for (unsigned int i = pos; i<tokens.size(); ++i)
	{
		if (stopping)
			throw runtime_error("Script killed");

		switch (tokens.at(i).type)
		{
		case Token::DataType: //variable declaration or function definition
		    if (i+2 >= tokens.size())
                raiseError("Unexpected end of input", &tokens.at(i));

            if (tokens.at(i+1).type!=Token::Identifier)
				raiseError("Was expecting identifier after data type", &tokens.at(i));

			name = tokens.at(i+1).data;
			i = i+2;
            if (tokens.at(i).type==Token::Assignment || tokens.at(i).type==Token::LineDelim) //variable declaration
			{
				if (stackFrames.empty())
				{
					if (globalFrame.locals.find(name)!=globalFrame.locals.end())
						raiseError("Redeclared identifier "+name, &tokens.at(i));
					globalFrame.locals[name] = Value();
					globalFrame.locals[name].type = Value::Integer;
				}
				else
				{
                    if (stackFrames.back().locals.find(name)!=stackFrames.back().locals.end())
                        raiseError("Redeclared identifier "+name, &tokens.at(i));
					stackFrames.back().locals[name] = Value();
					stackFrames.back().locals[name].type = Value::Integer;
				}

				if (tokens.at(i).type==Token::Assignment)
				{
					tkns.clear();
					while (tokens.at(i).type!=Token::LineDelim)
					{
						++i;
						if (i >= tokens.size())
                            raiseError("Unexpected end of input", &tokens.at(i-1));
						tkns.push_back(tokens.at(i));
					}
					tkns.erase(tkns.begin()+tkns.size()-1); //last one will be ';'
					if (tkns.size() == 0)
                        raiseError("Empty expression in assignment", &tokens.at(i-1));
					if (stackFrames.empty())
						globalFrame.locals[name] = evaluate(tkns);
					else
						stackFrames.back().locals[name] = evaluate(tkns);
				}
			}
			else //function definition, skip
			{
				while (tokens.at(i).type!=Token::BlockOpen)
					++i;
				parenCount = 1;
				while (parenCount>0)
				{
					++i;
					if (tokens.at(i).type==Token::BlockOpen)
						parenCount++;
					if (tokens.at(i).type==Token::BlockClose)
                        parenCount--;
				}
			}
			if (tokensTillResetCond == 0)
                conditionalState = NoConditional;
			break;

		case Token::Branch:
			if (tokens.at(i+1).type!=Token::Identifier)
				raiseError("Expecting identifier after branch", &tokens.at(i));
			if (branchTable.find(tokens.at(i+1).data)==branchTable.end())
				raiseError("Branch to unknown identifier encountered", &tokens.at(i));
			i = branchTable[tokens.at(i+1).data];

			if (tokensTillResetCond == 0)
                conditionalState = NoConditional;
			break;

		case Token::Elif:
			if (conditionalState==NoConditional)
				raiseError("'elif' encountered without preceding 'if'", &tokens.at(i));
			resetCondState = false; //to allow failure to fall through a chain of elif's
			//fall through to check condition

		case Token::Conditional:
			if (tokens.at(i).type==Token::Conditional)
				resetCondState = true; //we want to update the state

            if (tokens.at(i+1).type!=Token::Operator || tokens.at(i+1).data!="(")
				raiseError("Was expecting '(' after 'if'", &tokens.at(i));
			tkns.clear();
			parenCount = 1;
			i = i+2;
			while (parenCount>0)
			{
                if (tokens.at(i).type==Token::Operator && tokens.at(i).data=="(")
					parenCount++;
				if (tokens.at(i).type==Token::Operator && tokens.at(i).data==")")
					parenCount--;
				tkns.push_back(tokens.at(i));
				++i;
			}
			i--; //so i points to the ')', that way i+1 means the same thing for both if and else
			tkns.erase(tkns.begin()+tkns.size()-1); //last one will be ')'
			test = evaluate(tkns);
			execCond = test.nonZero();
			if (!resetCondState) //elif
				execCond = execCond && conditionalState!=LastConditionalExecuted;
			//fall through to execute or skip the conditional code

		case Token::Else:
			if (tokens.at(i).type==Token::Else)
			{
				execCond = conditionalState==LastConditionalFailed; //check condition in case of else
				resetCondState = true;
			}
			if (execCond)
			{
				conditionalState = LastConditionalExecuted;
				if (tokens.at(i+1).type==Token::BlockOpen)
				{
				    stackFrames.push_back(Frame());
					Value ret = runTokens(i+2);
					stackFrames.pop_back();
					if (!(ret.type==Value::Void && ret.iValue==0 && ret.sValue=="noret"))
                        return ret;
					parenCount = 1;
					++i;
					while (parenCount>0)
					{
						++i;
						if (tokens.at(i).type==Token::BlockOpen)
							parenCount++;
						if (tokens.at(i).type==Token::BlockClose)
							parenCount--;
					}
				}
				else
                    tokensTillResetCond = 2;
			}
			else
			{
				if (tokens.at(i+1).type==Token::BlockOpen)
				{
					parenCount = 1;
					i = i+2;
					while (parenCount>0)
					{
						if (tokens.at(i).type==Token::BlockOpen)
							parenCount++;
						if (tokens.at(i).type==Token::BlockClose)
							parenCount--;
						++i;
					}
					i--;
				}
				else
				{
				    tokensTillResetCond = 2;
					while (tokens.at(i).type!=Token::LineDelim)
						++i;
				}
				if (resetCondState)
					conditionalState = LastConditionalFailed;
			}
			break;

		case Token::Loop:
            if (tokens.at(i+1).type!=Token::Operator || tokens.at(i+1).data!="(")
				raiseError("Was expecting '(' after 'while'", &tokens.at(i));

			i += 2;
			parenCount = 1;
			tkns.clear();
			while (parenCount>0)
			{
				tkns.push_back(tokens.at(i));
				++i;
				if (tokens.at(i).type==Token::Operator && tokens.at(i).data=="(")
					parenCount++;
				if (tokens.at(i).type==Token::Operator && tokens.at(i).data==")")
					parenCount--;
			}

			if (tokens.at(i+1).type!=Token::BlockOpen)
				raiseError("Was expecting '{' after loop", &tokens.at(i));

			i += 2;
			test = evaluate(tkns);
			while ((test.type==Value::Integer && int(test.iValue+0.01)!=0) || (test.type==Value::String && test.sValue.size()>0))
			{
				if (stopping)
                    throw runtime_error("Script killed");

                stackFrames.push_back(Frame());
				Value ret = runTokens(i);
				stackFrames.pop_back();
				if (!(ret.type==Value::Void && ret.iValue==0 && ret.sValue=="noret"))
                    return ret;
				test = evaluate(tkns);
			}

			parenCount = 1;
			while (parenCount>0)
			{
				if (tokens.at(i).type==Token::BlockClose)
					parenCount--;
				if (tokens.at(i).type==Token::BlockOpen)
					parenCount++;
				++i;
			}
			i--;
			if (tokensTillResetCond == 0)
                conditionalState = NoConditional;
			break;

		case Token::Return:
			++i;
			tkns.clear();
			while (tokens.at(i).type!=Token::LineDelim)
			{
                tkns.push_back(tokens.at(i));
                ++i;
			}
			return evaluate(tkns);

		case Token::BlockOpen:
            blockCount++;
            conditionalState = NoConditional;
            break;

		case Token::BlockClose:
			blockCount--;
			if (blockCount<0)
                goto voidRet;
			conditionalState = NoConditional;
			break;

		case Token::LineDelim:
		    if (tokensTillResetCond == 0)
                conditionalState = NoConditional;
			break;

		case Token::Label:
			conditionalState = NoConditional;
			break;

		case Token::Identifier:
			name = tokens.at(i).data;
			tkns.clear();
			if (i+1 >= tokens.size())
                raiseError("Stray identifier", &tokens.at(i));
            ++i;

			while (tokens.at(i).type!=Token::LineDelim)
			{
				tkns.push_back(tokens.at(i));
				++i;
				if (i >= tokens.size())
                    raiseError("Unexpected end of expression", &tokens.at(i-1));
			}
			if (tkns[0].type == Token::Assignment) {
                tkns.erase(tkns.begin());
                if (tkns.size() == 0)
                    raiseError("Empty expression in assignment", &tokens.at(i-1));
                getIdentifier(name, &tokens.at(i)) = evaluate(tkns);
			}
			else
                evaluate(tkns);

			if (tokensTillResetCond == 0)
                conditionalState = NoConditional;
			break;

        default:
            raiseError("Unexpected token "+Token::toString(tokens.at(i).type)+" encountered", &tokens.at(i));
		}

		if (tokensTillResetCond > 0)
            tokensTillResetCond -= 1;
	}

	voidRet:
	return Value();
}

void Script::run()
{
	if (tokens.size()==0)
		return;

	reset();
	stopped = false;
	stopping = false;
	try
	{
		runResult = runTokens(0);
	}
	catch (const out_of_range& e)
	{
		cout << "Parenthesis or bracket mismatch somewhere in file "+tokens.at(0).file << endl;
	}
	catch (const runtime_error& e)
	{
		cout << e.what() << endl;
		cout << original << endl;
	}
	stopped = true;
}

string Script::dryRun()
{
	if (tokens.size()==0)
		return "";

	reset();
	stopped = false;
	stopping = false;
	isDryRun = true;
	try
	{
		runTokens(0);
	}
	catch (const out_of_range& e)
	{
		return ("Parenthesis or bracket mismatch somewhere in file "+tokens.at(0).file);
	}
	catch (const runtime_error& e)
	{
		return e.what();
	}
	stopped = true;
	isDryRun = false;
	return "";
}

void Script::stop()
{
	stopping = true;
	Clock timer;
	while (!stopped && timer.getElapsedTime().asMilliseconds()<300)
		sleep(milliseconds(30));
}

bool Script::isRunning()
{
	return !stopped;
}

bool Script::isStopping() {
    return stopping;
}

Value Script::result() {
    return runResult;
}

string Script::minimizeError(string error) {
    auto i = error.find(ERROR_FILE_PREFIX);
    if (i != string::npos)
        error.erase(i);
    return error;
}
