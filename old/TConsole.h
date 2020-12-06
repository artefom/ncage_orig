const unsigned short TC_ARG_NONE = 0;
const unsigned short TC_ARG_BOOL = 1;
const unsigned short TC_ARG_INT = 2;
const unsigned short TC_ARG_FLOAT = 3;
const unsigned short TC_ARG_CHAR = 4;
const unsigned short TC_ARG_COLOR = 5;

#include <TConsole_variables.h>
#include <TConsole_functions.h>

class TC_Ptr {
public:

    char *name;
    void *ptr;
    unsigned char type; // 0 - variable 1 - function

    TC_Ptr() : name(0), ptr(0), type(0) {};

    TC_Ptr(char *iname, void *iptr, unsigned char itype) : name(iname), ptr(iptr), type(itype) {};
};

class TC_Universal {
public:
    I_TConsole_Var *var;
    TConsole_Callback *cback;

    TC_Universal() {
        var = 0;
        cback = 0;
    };

    TC_Universal(I_TConsole_Var *ivar) {
        var = ivar;
        cback = 0;
    };

    TC_Universal(TConsole_Callback *icback) {
        cback = icback;
        var = 0;
    };
};

class TConsole {
public:

    map <string, TC_Universal> functions;
    vector<const char *> functions_names;
    vector<string *> lines;
    unsigned int max_lines_number = 256;

    TConsole() {
        lines.push_back(new string());
    }


    void ProcessCommandsFromFile(const char *text) {
        string STRING;
        ifstream infile;
        infile.open(text);
        while (!infile.eof()) {
            getline(infile, STRING);
            ProcessCommand(STRING, 0);
        }
        infile.close();
    };

    bool ProcessCommand(string cmd, bool annotations) {
        char command[256];
        command[0] = 0;
        char parameter[256];
        parameter[0] = 0;
        sscanf(cmd.c_str(), "%255s%*[ ]%255[^\n]", command, parameter);
        command[255] = 0;
        parameter[255] = 0;
        map<string, TC_Universal>::iterator it = functions.find(command);
        if (it == functions.end()) return false;
        I_TConsole_Var *v = it->second.var;
        TConsole_Callback *fv = it->second.cback;

        if (v) {
            if ((parameter[0] == 0) && annotations) {
                char txt[64];
                v->snprintf(txt, 64, v->val_ptr);
                (*this) << v->name << " = " << txt;
                v->snprintf(txt, 64, v->def_ptr);
                (*this) << " (default: " << txt << ")" << endl;
            } else v->sscanf(parameter);
            return true;
        }

        if (fv) {
            if (parameter[0] == 0) {
                int iarg = (int) (fv->arg_type);
                if (iarg == TC_ARG_NONE) { fv->sscanf_call(parameter); };
                if (annotations) {

                    switch (iarg) {
                        case TC_ARG_NONE :
                            break;
                        case TC_ARG_INT :
                            (*this) << " int";
                            break;
                        case TC_ARG_FLOAT :
                            (*this) << " float";
                            break;
                        case TC_ARG_CHAR :
                            (*this) << " string";
                            break;
                        case TC_ARG_COLOR :
                            (*this) << " color";
                            break;
                        case TC_ARG_BOOL :
                            (*this) << " bool";
                            break;
                        default:
                            (*this) << " unknown";
                    }
                    if (fv->annotations) (*this) << ' ' << '"' << fv->annotations << '"';
                    if (fv->annotations || iarg != TC_ARG_NONE) (*this) << endl;
                };
            } else {
                fv->sscanf_call(parameter);
            }
            return true;
        }
        return false;
    };

    template<class T>
    T &RegisterVar(T def, const char *name, void (*func)(void) = 0) {
        TConsole_Var <T> *var = new TConsole_Var<T>(def, name, func);
        functions[name] = TC_Universal(var);
        return var->value;
    };

    template<class F>
    void RegisterFunction(F f, const char *name, const char *annotations = 0) {
        TConsole_Callback *func = new TConsole_Callback();
        func->Connect(f);
        func->name = name;
        func->annotations = annotations;
        functions[name] = TC_Universal(func);
    }

    void EnterText(string text) {
        AddText(text);
        flush();
        ProcessCommand(text, true);
    };

    void AddText(string text) {
        size_t found_last = 0;
        size_t found = text.find_first_of('\n');
        while (found != string::npos) {
            *lines.back() += text.substr(found_last, found - found_last);
            lines.push_back(new string());
            found_last = found + 1;
            found = text.find_first_of('\n', found + 1);
        }
        *lines.back() += text.substr(found_last);
        ui_events.Call(&TUI_Childless::Global_Hooks_Storage, HOOK_GC_OnEnterText, text.c_str());
    }

    void flush() {
        AddText("\n");
    };

    template<class T>
    TConsole &operator<<(T in) {
        stringstream ss;
        ss << in;
        string st = ss.str();
        AddText(st);
        return *this;
    }

    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

    typedef CoutType &(*StandardEndLine)(CoutType &);

    TConsole &operator<<(StandardEndLine manip) {
        flush();
        return *this;
    }

    void Init() {
        functions_names.reserve(functions.size());
        for (auto it = functions.begin(); it != functions.end(); ++it) {
            functions_names.push_back(it->first.c_str());
        };
    };

};

char endl(TConsole &os);

TConsole Console;

void testfunction_int(int a) {
    Console << "test function called with int rgument: " << a << endl;
}

void testfunction_float(float a) {
    Console << "test function called with float argument: " << a << endl;
}

void testfunction_string(const char *a) {
    Console << "test function called with string argument: " << a << endl;
}

void testfunction_noargs() {
    Console << "test function called with no args " << endl;
}

int TDebug_initialized = 0;

void Exit() {
    TDebug_initialized = 0;
    glutLeaveMainLoop();
};

#define ConsoleRegisterFunction(a, b)    \
namespace namespace##a{    \
    static char o = (Console.RegisterFunction( &a, #a, b ),0);    \
};

#define ConsoleRegisterFunctionCustomName(a, c, b)    \
namespace namespace##a{    \
    static char o = (Console.RegisterFunction( &a, c, b ),0);    \
};


ConsoleRegisterFunction(testfunction_int, "just a test function");
ConsoleRegisterFunction(testfunction_float, "just a test function");
ConsoleRegisterFunction(testfunction_string, "just a test function");
ConsoleRegisterFunction(testfunction_noargs, 0);
ConsoleRegisterFunctionCustomName(Exit, "exit", 0);

void console_printcontent() {
    cout << "##########Flushing console content: " << endl;
    int i = 0;
    for (auto &v : Console.lines) {
        cout << i << ": " << v << endl;
        ++i;
    };
};

ConsoleRegisterFunction(console_printcontent, "flushing console content");

/*
STATIC_EXECUTE{
	Console.RegisterFunction( &testfunction_int, "function_test_int", "just a test function" );
	Console.RegisterFunction( &testfunction_float, "function_test_float", "just a test function" );
	Console.RegisterFunction( &testfunction_string, "function_test_string", "just a test function" );
	Console.RegisterFunction( &testfunction_noargs, "function_test_noargs", "just a test function" );
}
*/
void Test_valSet() {
    Console << "test_val value changed!" << endl;
}

int &test_val = Console.RegisterVar(314, "test_val", &Test_valSet);

#include <TConsole_scripts.h>

