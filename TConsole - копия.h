class TConsole_IContainter{ public: virtual void Call( IArguments* ) = 0; };
template< class F > class TConsole_Container : public TConsole_IContainter {};

template<>
class TConsole_Container< void (*)(void) > : public TConsole_IContainter // без аргументов
{
	public:
			
	typedef void (*M)(void); 
	M f;
	
	TConsole_Container( M func, unsigned char& arg_type )
	{
		f = func;
		arg_type = 0;
	}
	
	virtual void Call( IArguments* args )
	{
		f();
	};
};

template< class A1 >
class TConsole_Container< void (*)(A1) > : public TConsole_IContainter // один аргумент
{
	public:
			
	typedef void (*M)(A1); 
	typedef Arguments<A1> A;
	M f;
	
	TConsole_Container( M func, unsigned char& arg_type )
	{
		f = func;
		
		if ( typeid(A1) == typeid(int) ) {arg_type = 1; return;};
		if ( typeid(A1) == typeid(float) ) {arg_type = 2; return;};
		if ( typeid(A1) == typeid(char*) ) {arg_type = 4; return;};
	}
	
	virtual void Call( IArguments* i_args )
	{
		A* args = dynamic_cast<A*>(i_args);
		f( args->arg1 );
	};
};


class TConsole_Callback
{
	private:
		TConsole_IContainter* func_container;
	public:
		char* name;
		unsigned char arg_type;
		
		TConsole_Callback()
		{
			name = 0;
			arg_type = 0;
			func_container = 0;
		}
		
		~TConsole_Callback()
		{
			if (name) delete[] name;
			if (func_container) delete func_container;
		}
		template< class F > void Connect( F f )
		{
			if (func_container) delete func_container;
			func_container = new TConsole_Container< F >( f, arg_type );
			
			cout << "registering func with arguments type: " << (unsigned int)arg_type << endl;
		}
		
		virtual void operator()( IArguments* )
		{
			func_container->Call( (Arguments<>*)(0) );
		}
		
		template< class T1 > void operator()( T1 i_arg1 )
		{
			
			Arguments< T1 > args( i_arg1 );
			func_container->Call( &args );
		}
};

vector< TConsole_Callback > console_functions;


void testfunction_int( int a )
{
	cout << "test function called with int rgument: " << a << endl;
}

void testfunction_float( float a )
{
	cout << "test function called with float argument: " << a << endl;
}

void testfunction_string( char* a )
{
	cout << "test function called with string argument: " << a << endl;
}

void testfunction_noargs( )
{
	cout << "test function called with no args " << endl;
}

int getnum( char* text )
{
	int output = 0;
	while ( (*text != 0) && !( ( (*text) >= '0' ) && ( (*text) <= '9' ) ) ) ++text;
	while ( (*text != 0) && ( ( (*text) >= '0' ) && ( (*text) <= '9' ) ) )
	{
		output = output*10+(*text)-'0';
		++text;	
	}
	return output;
}

bool syntax_analyze( char* text, int& func_id, int& arg )
{	
	unsigned int j = 0;
	for (unsigned int i = 0; i != console_functions.size(); ++i)
	{
		for (j = 0; (console_functions[i].name[j] != 0) && 
			(console_functions[i].name[j] == text[j]); ++j);
		if ( (console_functions[i].name[j] == 0) && ( isspace(text[j]) || (text[j] == 0) ) )
		{
			func_id = i;
			goto end;	
		}
	}
	return false;
	end:;
	
	int ai = 0;
	float fi = 0;
	char* ci = 0;
	
	cout << "found function! " << endl;
	cout << "console_functions[func_id].arg_type = " << (unsigned int)console_functions[func_id].arg_type << endl;
	switch( console_functions[func_id].arg_type )
	{
		case 1 : sscanf(text+j,"%d",&ai); 
					cout << "calling!" << endl;
					console_functions[func_id](ai); 
					break;
		case 2 : sscanf(text+j,"%f",&fi); 
					console_functions[func_id](fi); 
					break;
		case 4 : sscanf(text+j,"%s",&ci); 
					if ( (ci != 0) && (*ci != 0) ) console_functions[func_id](ci); 
					break;
		default: console_functions[func_id](0); break;
	}

	return true;

};

class TConsole
{
	public:
		
	TConsole()
	{
		console_function_register( &testfunction_int, "function_test_int" );
		console_function_register( &testfunction_float, "function_test_float" );
		console_function_register( &testfunction_string, "function_test_string" );
		console_function_register( &testfunction_noargs, "function_test_noargs" );
	}
	
	void EnterText( char* text )
	{
		ProcessCommand( text );
	};
	
	void ProcessCommand( char* command )
	{
		int id, arg;
		syntax_analyze( command, id, arg );
		
		//if (
	}
};

TConsole Console;
