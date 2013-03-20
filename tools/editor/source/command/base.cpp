#include <sstream>
#include <string>

#include "command/base.h"

std::string Trim(const std::string& str)
{
    std::string::size_type a = str.find_first_not_of(' ');
    std::string::size_type b = str.find_last_not_of(' ');
    return str.substr(a == std::string::npos ? 0 : a, b == std::string::npos ? str.length() - 1 : b - a + 1);
}

Command::~Command()
{
    
}
    
bool Command::IsArgumentSet(const std::string& argument)
{
    ArgumentMap::iterator it = _Arguments.find(argument);
    
    if (it == _Arguments.end())
        return false;
    
    return true;
}
    
unsigned long Command::GetArgumentCount(const std::string& argument)
{
    ArgumentMap::iterator it = _Arguments.find(argument);
    
    if (it == _Arguments.end())
        return 0;

    return it->second.size();
}

std::string Command::GetArgument(const std::string& argument, unsigned long index)
{
    ArgumentMap::iterator it = _Arguments.find(argument);
    
    if (it == _Arguments.end())
        return "";
    
    std::vector<std::string>& values = it->second;
    
    if (index < values.size())
        return values[index];
    
    return "";
}

void Command::SetArguments(const std::string& arguments)
{
    enum State
    {
        kStateArg,
        kStateTag
    };
    
    State state = kStateTag;
    
    std::vector<std::string> argList;
    std::string tag = "";
    std::ostringstream arg;
    
    for (int i=0; i<arguments.length(); i++)
    {
        char c = arguments.at(i);
        switch (state)
        {
        case kStateArg:
                if (c == '-')
                {
                    state = kStateTag;
                    tag = arguments.at(i+1);
                    i++;
                }
            break;
        case kStateTag:
                if (c == ',')
                {
                    argList.push_back(Trim(arg.str()));
                    arg.str("");
                }
                else if (c == '-'  && (i == 0 || arguments[i-1] == ' ') && (i == arguments.length() || (arguments[i+1] > '0' && arguments[i+1] > '9')))
                {
                    if (arg.str().length())
                    {
                        argList.push_back(Trim(arg.str()));
                        arg.str("");
                    }

                    _Arguments[tag] = argList;
                    argList.clear();

                    state = kStateArg;
                    i--;
                }
                else
                {
                    arg << c;
                }
            break;
        }
    }
    
    if (arg.str().length() > 0)
    {
        argList.push_back(Trim(arg.str()));
    }
    
    _Arguments[tag] = argList;
}

bool Command::CanUndo()
{
    return false;
}

bool Command::Undo()
{
    return false;
}
