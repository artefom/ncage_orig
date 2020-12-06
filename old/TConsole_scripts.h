//GLUT_LEFT_BUTTON;
//GLUT_RIGHT_BUTTON;
//GLUT_MIDDLE_BUTTON;

class TConsole_scripts
{
	public:
	map<string,unsigned char> key_names;
	map<unsigned char, string> commands;
	map<unsigned char, string> commands_up;
	TConsole_scripts()
	{
		key_names["mouse1"] = 0;
		key_names["mouse2"] = 1;
		key_names["mouse3"] = 2;
		key_names["space"] = 32;
		key_names["arrowleft"] = 37;
		key_names["arrowup"] = 38;
		key_names["arrowright"] = 39;
		key_names["arrowdown"] = 40;
	};
	
	void OnKeyDown(int key)
	{
		auto it = commands.find(key);
		if (it != commands.end()) Console.ProcessCommand((it->second).c_str(),false);
	};
	
	void OnKeyUp(int key)
	{
		auto it = commands_up.find(key);
		if (it != commands_up.end()) Console.ProcessCommand((it->second).c_str(),false);
	};
	
	void Bind(unsigned char key,string command)
	{
		if (key == 255) return;
		commands[key] = command;
		for (int i = 0; i < command.size(); ++i)
			if (command[i] == '+')
			{
				command[i] = '-';
				size_t end = command.find_first_of (';', i);
				commands_up[key] = command.substr(i, end);
			};
	};
	
	unsigned char GetVirtualKeyByName(string name)
	{
		if (name.size() == 1) return toVirtual(name[0]);
		auto it = key_names.find(name);
		if (it == key_names.end()) return -1;
		return it->second;
	};
};
TConsole_scripts Console_scripts;

void console_bind(const char* str)
{
	int size = 0;
	for (const char* i = str; *i != 0; ++i) ++size;
	char buf[size+1];
	for (int i = 0; i != size; ++i) buf[i] = str[i];
	buf[size] = 0;
	char* key = 0;
	char* command = 0;
	char* ptr = buf;
	unsigned char qotes_id = 0;
	for (ptr; *ptr != 0; ++ptr)
	{
		if (*ptr == '"') 
		{
			switch (qotes_id)
			{
				case 0 : key = ptr+1; break;
				case 1 : *ptr = 0; break;
				case 2 : command = ptr+1; break;
				case 3 : *ptr = 0; break;
				default: break;
			};
			qotes_id+=1;
		} else
		if (!isspace(*ptr))
		{
			if (qotes_id == 2) {command = ptr; qotes_id = 4; break;};
			if (qotes_id == 0) {
				key = ptr; qotes_id = 2; 
				while((*ptr) && (!isspace(*ptr))) ++ptr; 
				if (*ptr == 0) break; else *ptr = 0;
			};
		};
	};
	if (command == 0) return;
	Console_scripts.Bind(Console_scripts.GetVirtualKeyByName(string(key)),string(command));
};

ConsoleRegisterFunctionCustomName(console_bind,"bind","bind \"key\" \"command\"");

void console_print(const char* str)
{
	Console << str << endl;	
};
ConsoleRegisterFunctionCustomName(console_print,"print","print text");
