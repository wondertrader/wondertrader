#pragma once

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <ostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

#if defined(WIN32) || defined(_WIN64) || defined(__WIN32__)
    #include "direct.h"
    #include <windows.h>
    #define CPPCLI_SEPARATOR_TYPE    "\\"
    #define CPPCLI_SEPARATOR_NO_TYPE "/"
#else
    #include <unistd.h>
    #define CPPCLI_SEPARATOR_TYPE    "/"
    #define CPPCLI_SEPARATOR_NO_TYPE "\\"
#endif

// #define CPPCLI_DEBUG

namespace cppcli {
    static std::mutex _coutMutex;

#ifdef CPPCLI_DEBUG
    template <class... Args>
    void __cppcli_debug_print(const Args &...args)
    {
        std::unique_lock<std::mutex> lock(_coutMutex);
        std::cout << "[CPPCLI_DEBUG] ";
        auto printFunc = [](auto i) { std::cout << i; };
        std::initializer_list<int>{(printFunc(args), 0)...};
        std::cout << std::endl;
    }

#endif
}   // namespace cppcli

#ifdef CPPCLI_DEBUG
    #define CPPCLI_DEBUG_PRINT(...) cppcli::__cppcli_debug_print(__VA_ARGS__)
#endif

namespace cppcli {
    class Option;
    class Rule;

    enum ErrorExitEnum {
        EXIT_PRINT_RULE = 0x00,
        EXIT_PRINT_RULE_HELPDOC = 0x01,
    };
    enum HelpDocEnum {
        USE_DEFAULT_HELPDOC = 0x00,
        USE_UER_DEFINED_HELPDOC = 0x01,
    };

    namespace detail {
        enum ErrorEventType {
            MECESSARY_ERROR = 0x00,
            VALUETYPE_ERROR = 0x01,
            ONEOF_ERROR = 0x02,
            NUMRANGE_ERROR = 0x03,
        };

        enum ValueTypeEnum {
            STRING = 0x00,
            INT = 0x01,
            DOUBLE = 0x02,
        };

        class pathUtil final {
          private:
            static int replace_all(std::string &str, const std::string &pattern, const std::string &newpat);

          public:
            static std::string getFilename(const std::string &filePath);
            static std::string getFilenameWithOutSuffix(const std::string &filePath);
            static std::string getFileSuffix(const std::string &filePath);
            static std::string getFileDir(const std::string &filePath);
        };
        class algoUtil final {
          private:
          public:
            // command params add to map
            static void InitCommandMap(int length, char *strArr[], std::map<std::string, std::string> &stringMap);
            static bool isInt(const std::string &value);
            static bool isDouble(const std::string &value);
            static bool verifyDouble(const std::string &value);
        };


    }   // namespace detail
}   // namespace cppcli

int cppcli::detail::pathUtil::replace_all(std::string &str, const std::string &pattern, const std::string &newpat)
{

    int count = 0;
    const size_t nsize = newpat.size();
    const size_t psize = pattern.size();

    for (size_t pos = str.find(pattern, 0); pos != std::string::npos; pos = str.find(pattern, pos + nsize))
    {
        str.replace(pos, psize, newpat);
        count++;
    }
    return count;
}

std::string cppcli::detail::pathUtil::getFilename(const std::string &filePath)
{
    std::string filePathCopy(filePath);
    replace_all(filePathCopy, CPPCLI_SEPARATOR_NO_TYPE, CPPCLI_SEPARATOR_TYPE);

    assert(std::ifstream(filePathCopy.c_str()).good());
    std::string::size_type pos = filePathCopy.find_last_of(CPPCLI_SEPARATOR_TYPE) + 1;
    return std::move(filePathCopy.substr(pos, filePathCopy.length() - pos));
}

std::string cppcli::detail::pathUtil::getFilenameWithOutSuffix(const std::string &filePath)
{
    std::string filename = getFilename(filePath);
    return std::move(filename.substr(0, filename.rfind(".")));
}

std::string cppcli::detail::pathUtil::getFileSuffix(const std::string &filePath)
{
    std::string filename = getFilename(filePath);
    return std::move(filename.substr(filename.find_last_of('.') + 1));
}

std::string cppcli::detail::pathUtil::getFileDir(const std::string &filePath)
{
    std::string filePathCopy(filePath);
    replace_all(filePathCopy, CPPCLI_SEPARATOR_NO_TYPE, CPPCLI_SEPARATOR_TYPE);
    std::string::size_type pos = filePathCopy.find_last_of(CPPCLI_SEPARATOR_TYPE);
    return std::move(filePathCopy.substr(0, pos));
}



void cppcli::detail::algoUtil::InitCommandMap(int length, char *strArr[], std::map<std::string, std::string> &stringMap)
{

    // command params add to map
    std::string keyTmp;
    std::string valueTmp;
    int keyIndex = -1;

    for (int currentIndex = 1; currentIndex < length; currentIndex++)
    {
        std::string theStr(strArr[currentIndex]);
        if (keyIndex != -1 && theStr.size() > 0 && currentIndex == keyIndex + 1)
        {
            // if theStr is command key, set value as ""
            if(theStr.find_first_of('-') == 0 && theStr.size() > 1 && !isdigit(theStr.at(1)))
            {
                valueTmp = "";
            }
            else
            {   
                valueTmp =  theStr;
            }
            // valueTmp = theStr.find_first_of('-') == 0 && theStr.size() > 1 ? "" : theStr;

            stringMap.insert(std::make_pair(std::move(keyTmp), std::move(valueTmp)));
            keyTmp.clear();
            valueTmp.clear();

            keyIndex = -1;
        }

        if (theStr.find_first_of('-') == 0  && int(std::count(theStr.begin(), theStr.end(), '-')) < theStr.size() && !isdigit(theStr.at(1)) )
        {
            keyIndex = currentIndex;
            keyTmp = std::move(theStr);
        }

        if (currentIndex == length - 1 && keyIndex != -1)
        {

            stringMap.insert(std::make_pair(std::move(keyTmp), std::move("")));
        }
    }
}
bool cppcli::detail::algoUtil::isInt(const std::string &value)
{
    if (value.empty())
    {
        return false;
    }

    int startPos = value.at(0) == '-' ? 1 : 0;
    for (int i = startPos; i < value.size(); i++)
    {
        if (isdigit(value.at(i)) == 0)
            return false;
    }
    return true;
}

bool cppcli::detail::algoUtil::isDouble(const std::string &value)
{
    if (value.empty())
    {
        return false;
    }
    if (value.size() < 3)
        return false;
    std::string tmpValue = value.at(0) == '-' ?  value.substr(0, value.npos): value;
    int numCount = 0;
    for (char const &c : tmpValue)
    {
        if (isdigit(c) != 0)
            numCount++;
    }

    if (numCount == tmpValue.size() - 1 && tmpValue.rfind('.') > 0 && tmpValue.rfind('.') < tmpValue.size() - 1)
    {
        return true;
    }
    return false;
}

bool cppcli::detail::algoUtil::verifyDouble(const std::string &value)
{
    if (isInt(value) || isDouble(value))
        return true;
    return false;
}

/* ################################################################### */
/* ################################################################### */
/* ################################################################### */
/* ################################################################### */
/* ################################################################### */

namespace cppcli {

    class Rule {
      private:
        class detail {
          public:
            struct HelpDocStruct {
                static cppcli::HelpDocEnum _helpDocType;
                static cppcli::Rule *rule;
            };
        };

      private:
        friend class Option;

        std::string _inputValue;
        std::string _shortParam;
        std::string _longParam;
        std::string _helpInfo;
        bool _necessary = false;
        std::vector<std::string> _limitOneVec;
        std::pair<double, double> _limitNumRange;
        cppcli::detail::ValueTypeEnum _valueType = cppcli::detail::ValueTypeEnum::STRING;
        std::string _default = "[EMPTY]";
        std::string _errorInfo;
        bool _existsInMap = false;

      public:
        Rule() = delete;

        Rule(const std::string &, const std::string &) = delete;
        Rule(const std::string &) = delete;

        // Rule& operator=(const cppcli::Rule&) = delete;
        Rule(const std::string &shortParam, const std::string &longParam, const std::string helpInfo)
            : _shortParam(shortParam), _longParam(longParam), _helpInfo(helpInfo),
              _limitNumRange(std::make_pair(double(-1), double(-1))){};

        Rule(const std::string &shortParam, const std::string &longParam, const std::string helpInfo, bool necessary)
            : _shortParam(shortParam), _longParam(longParam), _helpInfo(helpInfo), _necessary(necessary),
              _limitNumRange(std::make_pair(double(-1), double(-1))){};

        Rule *limitInt();   /// valid after setting input type
        Rule *limitDouble();
        Rule *asHelpParam();
        bool exists();

        template <class T, class = typename std::enable_if<std::is_same<T, std::string>::value>::type>
        const std::string get()
        {
            return _inputValue;
        }

        template <class T, class = typename std::enable_if<std::is_same<T, int>::value>::type>
        int get()
        {
            return std::stoi(_inputValue);
        }

        template <class T, class = typename std::enable_if<std::is_same<T, double>::value>::type>
        double get()
        {
            return std::stod(_inputValue);
        }

        template <class... Args>
        Rule *limitOneOf(Args... args)
        {
            std::ostringstream oss;

            auto addToVec = [this, &oss](auto i) {
                oss << i;
                _limitOneVec.push_back(std::move(oss.str()));
                oss.str("");
            };
            std::initializer_list<int>{(addToVec(args), 0)...};
            return this;
        }

        template <class T,
                  class = typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, float>::value ||
                                                  std::is_same<T, double>::value>::type>
        Rule *limitNumRange(T min, T max)
        {
            _limitNumRange = std::make_pair(double(min), double(max));
            return this;
        }

        template <class T>
        Rule *setDefault(const T &defaultValue)
        {
            std::ostringstream oss;
            oss << defaultValue;
            _default = oss.str();
            return this;
        }

      private:
        const std::string getError(cppcli::detail::ErrorEventType errorEventType);

        std::string buildHelpInfoLine();

#ifdef CPPCLI_DEBUG
        std::string debugInfo() const;
#endif
    };

}   // namespace cppcli

cppcli::HelpDocEnum cppcli::Rule::detail::HelpDocStruct::_helpDocType = cppcli::HelpDocEnum::USE_DEFAULT_HELPDOC;
cppcli::Rule *cppcli::Rule::detail::HelpDocStruct::rule = nullptr;

cppcli::Rule *cppcli::Rule::limitInt()
{
    _valueType = cppcli::detail::ValueTypeEnum::INT;
    return this;
}

cppcli::Rule *cppcli::Rule::limitDouble()
{
    _valueType = cppcli::detail::ValueTypeEnum::DOUBLE;
    return this;
}

cppcli::Rule *cppcli::Rule::asHelpParam()
{
    if (_necessary == true)
    {
        _necessary = false;
    }
    cppcli::Rule::detail::HelpDocStruct::rule = this;
    return this;
}

bool cppcli::Rule::exists() { return _existsInMap; }

const std::string cppcli::Rule::getError(cppcli::detail::ErrorEventType errorEventType)
{
    std::ostringstream oss;

    oss << "[";
    switch (errorEventType)
    {
    case cppcli::detail::ErrorEventType::MECESSARY_ERROR: {
        if (_longParam.empty())
            oss << _shortParam;
        else
            oss << _shortParam << std::move(" | ") << _longParam;
        break;
    }
    case cppcli::detail::ErrorEventType::VALUETYPE_ERROR: {
        if (_valueType == cppcli::detail::ValueTypeEnum::DOUBLE)
            oss << std::move(" NUMBER (DOUBLE) ");
        else if (_valueType == cppcli::detail::ValueTypeEnum::INT)
            oss << std::move(" NUMBER (INT) ");
        break;
    }
    case cppcli::detail::ErrorEventType::ONEOF_ERROR: {
        for (int i = 0; i < _limitOneVec.size(); i++)
        {
            if (i == (_limitOneVec.size() - 1))
            {
                oss << _limitOneVec.at(i);
                break;
            }
            oss << _limitOneVec.at(i) << std::move(" ");
        }

        break;
    }
    case cppcli::detail::ErrorEventType::NUMRANGE_ERROR: {
        oss << _limitNumRange.first << std::move("(MIN), ") << _limitNumRange.second << std::move("(MAX)");
        break;
    }
    }
    oss << "]";
    return std::move(oss.str());
}

std::string cppcli::Rule::buildHelpInfoLine()
{
    std::ostringstream oss;

    int commandsDis = 28;
    int helpInfoDis = 36;
    int necessaryDis = 20;
    int defaultStrDis = 20;
    int theDis = 2;

    oss << std::setw(commandsDis) << std::left << (_longParam.empty() ? _shortParam : _shortParam + " | " + _longParam);

    int writeTime = _helpInfo.size() % helpInfoDis - theDis == 0 ? int(_helpInfo.size() / (helpInfoDis - theDis))
                                                                 : int((_helpInfo.size() / (helpInfoDis - theDis))) + 1;
    std::string necessaryOutStr = _necessary ? "true" : "false";
    std::string defaultValueOutStr = _default == "[EMPTY]" ? _default : "=" + _default;
 
    if (writeTime == 1)
    {
        oss << std::setw(helpInfoDis) << std::left << _helpInfo;
        oss << std::setw(necessaryDis) << std::left << "MUST-ENTER[" + necessaryOutStr + "]";
        oss << std::setw(defaultStrDis) << std::left << "DEFAULT->" + _default;
        oss << std::endl;
        return std::move(oss.str());
    }
    int pos = 0;
    for (int i = 0; i < writeTime; i++)
    {
        if (i == 0)
        {
            oss << std::setw(helpInfoDis) << std::setw(helpInfoDis) << _helpInfo.substr(pos, helpInfoDis - theDis);
            oss << std::setw(necessaryDis) << std::left << "MUST-ENTER[" + necessaryOutStr + "]";
            oss << std::setw(defaultStrDis) << std::left << "DEFAULT->" + _default;
            oss << std::endl;
            pos += helpInfoDis - theDis;
        }
        else
        {
            oss << std::setw(commandsDis + 4) << std::left << "";
            oss << _helpInfo.substr(pos, helpInfoDis - theDis);
            oss << std::endl;
            pos += helpInfoDis - theDis;
        }
    }

    return std::move(oss.str());
}

#ifdef CPPCLI_DEBUG
std::string cppcli::Rule::debugInfo() const
{

    std::ostringstream oss;

    if (_longParam.empty())
    {
        oss << "command params --> " << _shortParam << std::endl;
    }
    else
    {
        oss << "command params --> " << _shortParam << "|" << _longParam << std::endl;
    }

    oss << "[CPPCLI_DEBUG]     input value = " << _inputValue << std::endl;
    oss << "[CPPCLI_DEBUG]     necessary = " << _necessary << std::endl;
    oss << "[CPPCLI_DEBUG]     valueType = " << _valueType << std::endl;
    oss << "[CPPCLI_DEBUG]     default = " << _default << std::endl;
    oss << "[CPPCLI_DEBUG]     exist = " << _existsInMap << std::endl;

    oss << "[CPPCLI_DEBUG]     limitOneVec = (";
    for (int i = 0; i < _limitOneVec.size(); i++)
    {
        if (i == _limitOneVec.size() - 1)
        {
            oss << _limitOneVec.at(i);
            break;
        }
        oss << _limitOneVec.at(i) << ", ";
    }
    oss << "), size=" << _limitOneVec.size() << std::endl;

    oss << "[CPPCLI_DEBUG]     limitNumRange = (" << _limitNumRange.first << " " << _limitNumRange.second << ")";

    return oss.str();
}
#endif

/* =================================================================================*/
/* =================================================================================*/
/* ===============================    Option    =====================================*/
/* =================================================================================*/
/* =================================================================================*/

namespace cppcli {

    class Option {
      private:
        class detail {
            detail() = delete;
            detail(const detail &) = delete;
            friend class cppcli::Option;
            static int necessaryVerify(Option &opt);
            static int valueTypeVerify(Option &opt);
            static int numRangeVerify(Option &opt);
            static int oneOfVerify(Option &opt);
        };

      public:
        Option(int argc, char *argv[]);
        Option(const cppcli::Option &) = delete;
        Option operator=(const cppcli::Option &) = delete;
        cppcli::Rule *operator()(const std::string &shortParam, const std::string &longParam,
                                 const std::string helpInfo);
        cppcli::Rule *operator()(const std::string &shortParam, const std::string &longParam,
                                 const std::string helpInfo, bool necessary);
        ~Option();
        void parse();
        bool exists(const std::string shortParam);
        bool exists(const cppcli::Rule *rule);





#ifdef CPPCLI_DEBUG
        void printCommandMap();
#endif

        const std::string getWorkPath();
        const std::string getExecPath();

      private:
        cppcli::ErrorExitEnum _exitType = cppcli::ErrorExitEnum::EXIT_PRINT_RULE;
        std::map<std::string, std::string> _commandMap;
        std::vector<cppcli::Rule *> _ruleVec;

        std::string _workPath;   // exe path
        std::string _execPath;   // exec command path

      private:
        void rulesGainInputValue();
        std::string getInputValue(const cppcli::Rule &rule);
        std::string buildHelpDoc();
        void printHelpDoc();
        bool mapExists(const cppcli::Rule *rule);
        void pathInit(int argc, char *argv[]);

        void errorExitFunc(const std::string errorInfo, int index, cppcli::ErrorExitEnum exitType,
                           cppcli::detail::ErrorEventType eventType);

        template <class T, class = typename std::enable_if<std::is_same<T, std::string>::value>::type>
        std::string get(const std::string shortParam)
        {
            for (cppcli::Rule *rule : _ruleVec)
            {
                if (rule->_shortParam == shortParam)
                {
                    return rule->get<std::string>();
                }
            }
            std::cout << "error: don't set where short-param = " << shortParam << std::endl;
            std::exit(-1);
        }

        template <class T, class = typename std::enable_if<std::is_same<T, int>::value>::type>
        int get(const std::string shortParam)
        {
            for (cppcli::Rule *rule : _ruleVec)
            {
                if (rule->_shortParam == shortParam)
                {
                    return rule->get<int>();
                }
            }
            std::cout << "error: don't set where short-param = " << shortParam << std::endl;
            std::exit(-1);
        }

        template <class T, class = typename std::enable_if<std::is_same<T, double>::value>::type>
        double get(const std::string shortParam)
        {
            for (cppcli::Rule *rule : _ruleVec)
            {
                if (rule->_shortParam == shortParam)
                {
                    return rule->get<double>();
                }
            }
            std::cout << "error: don't set where short-param = " << shortParam << std::endl;
            std::exit(-1);
        }

        template <class T, class = typename std::enable_if<std::is_same<T, std::string>::value>::type>
        std::string get(cppcli::Rule *rule)
        {
            return rule->get<std::string>();
        }

        template <class T, class = typename std::enable_if<std::is_same<T, int>::value>::type>
        int get(cppcli::Rule *rule)
        {
            return rule->get<int>();
        }

        template <class T, class = typename std::enable_if<std::is_same<T, double>::value>::type>
        double get(cppcli::Rule *rule)
        {
            return rule->get<double>();
        }

    };

}   // namespace cppcli

int cppcli::Option::Option::detail::necessaryVerify(Option &opt)
{
    cppcli::Rule *rule = nullptr;
    for (int index = 0; index < opt._ruleVec.size(); index ++)
    {
        rule = opt._ruleVec.at(index);
        if (rule->_necessary && !opt.mapExists(rule))
        {
#ifdef CPPCLI_DEBUG
            CPPCLI_DEBUG_PRINT("failed in necessaryVerify, fail rule in following");
            CPPCLI_DEBUG_PRINT(rule->debugInfo(), "\n");
#endif
            return index;
        }
    }
    return -1;
};

int cppcli::Option::Option::detail::valueTypeVerify(Option &opt)
{
    cppcli::Rule *rule = nullptr;
    for (int index = 0; index < opt._ruleVec.size(); index ++)
    {
        rule = opt._ruleVec.at(index);
        if (rule->_valueType == cppcli::detail::ValueTypeEnum::STRING || !opt.mapExists(rule))
        {
            continue;
        }

        if (rule->_valueType == cppcli::detail::ValueTypeEnum::INT &&
            !cppcli::detail::algoUtil::isInt(rule->_inputValue))
        {
#ifdef CPPCLI_DEBUG
            CPPCLI_DEBUG_PRINT("failed in valueTypeVerify, fail rule in following");
            CPPCLI_DEBUG_PRINT(rule->debugInfo(), "\n");
#endif
           
            return index;
        }

        if (rule->_valueType == cppcli::detail::ValueTypeEnum::DOUBLE &&
            !cppcli::detail::algoUtil::verifyDouble(rule->_inputValue))
        {
#ifdef CPPCLI_DEBUG
            CPPCLI_DEBUG_PRINT("failed in valueTypeVerify, fail rule in following");
            CPPCLI_DEBUG_PRINT(rule->debugInfo(), "\n");
#endif
            return index;
        }

    }
    return -1;
}

int cppcli::Option::Option::detail::numRangeVerify(Option &opt)
{
    cppcli::Rule *rule = nullptr;
    for (int index = 0; index < opt._ruleVec.size(); index ++)
    {
        rule = opt._ruleVec.at(index);
        if (rule->_valueType == cppcli::detail::ValueTypeEnum::STRING || !opt.mapExists(rule))
        {
            continue;
        }

        // no set it
        if (rule->_limitNumRange.first == -1 && rule->_limitNumRange.second == -1)
        {
            continue;
        }

        if(rule->_inputValue.empty() || !cppcli::detail::algoUtil::verifyDouble(rule->_inputValue))
        {
#ifdef CPPCLI_DEBUG
            CPPCLI_DEBUG_PRINT("failed in numRangeVerify, fail rule in following");
            CPPCLI_DEBUG_PRINT(rule->debugInfo(), "\n");
#endif
            return index;
        }

        if (std::stod(rule->_inputValue) < rule->_limitNumRange.first ||
            std::stod(rule->_inputValue) > rule->_limitNumRange.second)
        {
#ifdef CPPCLI_DEBUG
            CPPCLI_DEBUG_PRINT("failed in numRangeVerify, fail rule in following");
            CPPCLI_DEBUG_PRINT(rule->debugInfo(), "\n");
#endif
            return index;
        }
    }
    return -1;
}

int cppcli::Option::Option::detail::oneOfVerify(Option &opt)
{

    cppcli::Rule *rule = nullptr;
    for (int index = 0; index < opt._ruleVec.size(); index ++)
    {
        rule = opt._ruleVec.at(index);
        if (rule->_limitOneVec.size() == 0 || !opt.mapExists(rule))
        {
            continue;
        }

        if (std::find(rule->_limitOneVec.begin(), rule->_limitOneVec.end(), rule->_inputValue) ==
            rule->_limitOneVec.end())
        {

#ifdef CPPCLI_DEBUG
            CPPCLI_DEBUG_PRINT("failed in oneOfVerify, fail rule in following");
            CPPCLI_DEBUG_PRINT(rule->debugInfo(), "\n");
#endif

            return index;
        }
    }
    return -1;
}

#ifdef CPPCLI_DEBUG
void cppcli::Option::Option::printCommandMap()
{
    CPPCLI_DEBUG_PRINT("-- commandMap, size = ", _commandMap.size());
    for (const std::pair<std::string, std::string> &pr : _commandMap)
    {
        CPPCLI_DEBUG_PRINT("    ", pr.first, "=", pr.second);
    }
    CPPCLI_DEBUG_PRINT("-- end commandMap");
}
#endif

void cppcli::Option::errorExitFunc(const std::string errorInfo, int index, cppcli::ErrorExitEnum exitType,
                                   cppcli::detail::ErrorEventType eventType)
{

    cppcli::Rule rule = *_ruleVec.at(index);

    // std::unique_lock<std::mutex> lock(cppcli::_coutMutex);
    std::ostringstream oss;
    if (eventType != cppcli::detail::ErrorEventType::MECESSARY_ERROR)
        oss << ", where command param = [" << rule._shortParam << "]";
    if (cppcli::Rule::detail::HelpDocStruct::rule != nullptr)
    {
        oss << std::endl << "Use [" << cppcli::Rule::detail::HelpDocStruct::rule->_shortParam << "] gain help doc";
    }

    std::cout << errorInfo << rule.getError(eventType) << oss.str() << std::endl;
    if (exitType == cppcli::EXIT_PRINT_RULE_HELPDOC)
        std::cout << buildHelpDoc();

    std::exit(0);
}

cppcli::Option::Option(int argc, char *argv[])
{
    // init work path and exec path
    pathInit(argc, argv);

#ifdef CPPCLI_DEBUG
    CPPCLI_DEBUG_PRINT("---------------- argc argv start");
    auto beStr = [&]() -> const std::string {
        std::ostringstream oss;
        for (int i = 0; i < argc; i++)
            oss << argv[i] << "  ";
        return oss.str();
    };
    CPPCLI_DEBUG_PRINT("argc = ", argc, " || argv = ", beStr());
#endif

    _ruleVec.reserve(64);

    // command params save to map
    cppcli::detail::algoUtil::InitCommandMap(argc, argv, _commandMap);
#ifdef CPPCLI_DEBUG
    CPPCLI_DEBUG_PRINT("---------------- argv map start");
    printCommandMap();
#endif
}

cppcli::Option::~Option()
{
    for (cppcli::Rule *rule : _ruleVec)
    {
        if (rule != nullptr)
        {
            delete (rule);
        }
    }
    _ruleVec.clear();
}


cppcli::Rule *cppcli::Option::operator()(const std::string &shortParam, const std::string &longParam,
                                         const std::string helpInfo)
{
    if (shortParam.find("-") == shortParam.npos)
    {
        std::cout << "short-param must contains \"-\" " << std::endl;
        std::exit(-1);
    }
    if (!longParam.empty() && longParam.find("-") == longParam.npos)
    {
        std::cout << "long-param must empty or contains \"-\" " << std::endl;
        std::exit(-1);
    }


    _ruleVec.push_back(new cppcli::Rule(shortParam, longParam, helpInfo));
    return _ruleVec.back();
}

cppcli::Rule *cppcli::Option::operator()(const std::string &shortParam, const std::string &longParam,
                                         const std::string helpInfo, bool necessary)
{
    if (shortParam.find("-") == shortParam.npos)
    {
        std::cout << "short-param must contains \"-\" " << std::endl;
        std::exit(-1);
    }
    if (!longParam.empty() && longParam.find("-") == longParam.npos)
    {
        std::cout << "long-param must empty or contains \"-\" " << std::endl;
        std::exit(-1);
    }

    _ruleVec.push_back(new cppcli::Rule(shortParam, longParam, helpInfo, necessary));
    return _ruleVec.back();
}

void cppcli::Option::pathInit(int argc, char *argv[])
{

    char execBuf[1024];
    char workBuf[1024];
#if defined(WIN32) || defined(_WIN64) || defined(__WIN32__)
    _getcwd(execBuf, sizeof(execBuf));
    GetModuleFileName(NULL, workBuf, sizeof(workBuf));
#else
    auto none1 = getcwd(execBuf, sizeof(execBuf));
    auto none2 = readlink("/proc/self/exe", workBuf, sizeof(workBuf));
#endif
    _execPath = execBuf;
    _workPath = std::move(cppcli::detail::pathUtil::getFileDir(workBuf));

#ifdef CPPCLI_DEBUG
    CPPCLI_DEBUG_PRINT("execPath = ", _execPath, ", workPath = ", _workPath);
#endif
}

std::string cppcli::Option::getInputValue(const cppcli::Rule &rule)
{

    std::string inputValue;
    if (_commandMap.find(rule._shortParam) != _commandMap.end())
    {
        inputValue = _commandMap[rule._shortParam];
    }
    if (_commandMap.find(rule._longParam) != _commandMap.end())
    {
        inputValue = _commandMap[rule._longParam];
    }

    return inputValue;
}

void cppcli::Option::rulesGainInputValue()
{
    std::string inputValue;

    for (cppcli::Rule *rule : _ruleVec)
    {
        if (!mapExists(rule))
            continue;
            
        rule->_existsInMap = true;
        inputValue = getInputValue(*rule);
        
        if (!inputValue.empty())
        {
            rule->_inputValue = inputValue;
            
            continue;
        }
        if(inputValue.empty() && rule->_default != "[EMPTY]")
        {
            rule->_inputValue = rule->_default;
        }
    }
}

bool cppcli::Option::mapExists(const cppcli::Rule *rule)
{
    if (rule != nullptr)
    {
        return _commandMap.find(rule->_shortParam) != _commandMap.end() ||
               _commandMap.find(rule->_longParam) != _commandMap.end();
    }
    return false;
}

bool cppcli::Option::exists(const cppcli::Rule *rule)
{
#ifdef CPPCLI_DEBUG
    CPPCLI_DEBUG_PRINT("---------------- exist rule");
    CPPCLI_DEBUG_PRINT(rule->debugInfo());
#endif
    return mapExists(rule);
}

bool cppcli::Option::exists(const std::string shortParam)
{

    for (cppcli::Rule *rule : _ruleVec)
    {
        if (rule->_shortParam == shortParam)
        {
#ifdef CPPCLI_DEBUG
            CPPCLI_DEBUG_PRINT("---------------- exist rule");
            CPPCLI_DEBUG_PRINT(rule->debugInfo());
#endif
            return mapExists(rule);
        }
    }
    return false;
}

std::string cppcli::Option::buildHelpDoc()
{
    std::ostringstream oss;
    oss << "options:" << std::endl;
    for (cppcli::Rule *rule : _ruleVec)
    {
        oss << rule->buildHelpInfoLine();
    }
    return oss.str();
}

void cppcli::Option::printHelpDoc()
{
#ifdef CPPCLI_DEBUG
    if (nullptr == cppcli::Rule::detail::HelpDocStruct::rule)
    {
        CPPCLI_DEBUG_PRINT("warning: you don't set help param\n");
    }
#endif

    if (!mapExists(cppcli::Rule::detail::HelpDocStruct::rule))
    {
        return;
    }

    std::cout << buildHelpDoc();
    std::exit(0);
}

const std::string cppcli::Option::getWorkPath() { return _workPath; }
const std::string cppcli::Option::getExecPath() { return _execPath; }

void cppcli::Option::parse()
{
    // rules save Unique correspondence input value
    rulesGainInputValue();

#ifdef CPPCLI_DEBUG
    CPPCLI_DEBUG_PRINT("---------------- rules vector start");
    for (int i = 0; i < _ruleVec.size(); i++)
    {
        CPPCLI_DEBUG_PRINT("vec index = ", i, "  ", _ruleVec[i]->debugInfo());
    }

#endif

    // check param exists of help , if exists print it
    printHelpDoc();

    int necessaryResult = Option::detail::necessaryVerify(*this);
    int valueTypeResult = Option::detail::valueTypeVerify(*this);
    int oneOfResult = Option::detail::oneOfVerify(*this);
    int numRangeResult = Option::detail::numRangeVerify(*this);

#ifdef CPPCLI_DEBUG
    CPPCLI_DEBUG_PRINT("---------------- verify result");
    CPPCLI_DEBUG_PRINT("necessaryResult=", necessaryResult, ", valueTypeResult=", valueTypeResult,
                       ",oneOfResult=", oneOfResult, ", numRangeResult", numRangeResult);

#endif

    if (necessaryResult > -1)
    {
        errorExitFunc("Must enter this param: ", necessaryResult, _exitType,
                      cppcli::detail::ErrorEventType::MECESSARY_ERROR);
    }

    if (valueTypeResult > -1)
    {
        errorExitFunc("Please enter the correct type: ", valueTypeResult, _exitType,
                      cppcli::detail::ErrorEventType::VALUETYPE_ERROR);
    }

    if (oneOfResult > -1)
    {
        errorExitFunc("Must be one of these values: ", oneOfResult, _exitType,
                      cppcli::detail::ErrorEventType::ONEOF_ERROR);
    }

    if (numRangeResult > -1)
    {
        errorExitFunc("Must be within this range: ", numRangeResult, _exitType,
                      cppcli::detail::ErrorEventType::NUMRANGE_ERROR);
    }

#ifdef CPPCLI_DEBUG
    CPPCLI_DEBUG_PRINT("---------------- parse result");
    CPPCLI_DEBUG_PRINT(">>>>>>>>>   PASS   <<<<<<<<<<");
#endif
}
