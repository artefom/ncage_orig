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
		
		if ( typeid(A1) == typeid(int) ) {arg_type = TC_ARG_INT; return;};
		if ( typeid(A1) == typeid(float) ) {arg_type = TC_ARG_FLOAT; return;};
		if ( typeid(A1) == typeid(const char*) ) {arg_type = TC_ARG_CHAR; return;};
		if ( typeid(A1) == typeid(bool) ) {arg_type = TC_ARG_BOOL; return;};
		if ( typeid(A1) == typeid(IRGB) ) {arg_type = TC_ARG_COLOR; return;};
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
		const char* name;
		const char* annotations;
		unsigned char arg_type;
		
		TConsole_Callback()
		{
			name = 0;
			arg_type = 0;
			func_container = 0;
			annotations = 0;
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
		
		virtual bool sscanf_call( const char* text )
		{
			switch (arg_type)
			{
				case TC_ARG_INT :
					int ival; 
					std::sscanf(text,"%d",&ival); 
					operator()(ival);
					return true; break;
				case TC_ARG_FLOAT : 
					float fval; 
					std::sscanf(text,"%f",&fval); 
					operator()(fval);
					return true; break;
				case TC_ARG_CHAR : 
					operator()(text);
					return true; break;
				case TC_ARG_BOOL :
					std::sscanf(text,"%d",&ival);
					bool bval; bval = ival;
					operator()(bval);
					return true; break;
				case TC_ARG_NONE :
					operator()(0);
					return true; break;
				default:
					
					return false;
			}
			
		}
};
