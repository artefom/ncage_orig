class I_TConsole_Var
{ 
	public:
		
		const char* name;
		void* val_ptr;
		void* def_ptr;
		
		void (*func)(void);
		
		//virtual void Set( void* val ) = 0;
		
		virtual void sscanf( const char* text ) = 0;
		
		virtual void snprintf( char* ptr, int n, void* val ) = 0;
};

template<class T> class TConsole_Var : public I_TConsole_Var // node of console variables array
{
	public:
			
	T value;
	T def;
	
	TConsole_Var( T idef, const char* iname, void (*ifunc)(void))
	{
		def = idef;
		value = def;
		name = iname;
		func = ifunc;
		
		val_ptr = &value;
		def_ptr = &def;
	}
	
	void snprintf( char* ptr, int n, void* val );
	void sscanf( const char* text );
	
	/*
	void sscanf( char* text )
	{
		std::sscanf(text,"%d",&value);
		if (func != 0) func();
	}
	
	void snprintf( char* ptr, int n, void* val )
	{
		std::snprintf(ptr,n,"undefined print format, printing int: %d", *(T*)(val) );
	}
	*/

};


#define RegisterPrintType( type, print )	\
	template<> void TConsole_Var<type>::sscanf( const char* text ){std::sscanf(text,print,&value); if (func != 0) func();};	\
	template<> void TConsole_Var<type>::snprintf( char* ptr, int n, void* val ){std::snprintf(ptr,n,print, *(type*)(val) );}

RegisterPrintType(float, "%g" );
RegisterPrintType(double, "%g" );
RegisterPrintType(int, "%d" );
RegisterPrintType(long int, "%ld" );
RegisterPrintType(unsigned int, "%u" );
RegisterPrintType(bool, "%d" );



template<> void TConsole_Var<IRGB>::sscanf( const char* text )
{
	unsigned short r = def.r,g = def.g,b = def.b;
	std::sscanf(text,"%hu %hu %hu",&r,&g,&b);
	value.r = r;
	value.g = g;
	value.b = b;
	if (func != 0) func();
};

template<> void TConsole_Var<IRGB>::snprintf( char* ptr, int n, void* val )
{
	unsigned short r = ((IRGB*)val)->r;
	unsigned short g = ((IRGB*)val)->g;
	unsigned short b = ((IRGB*)val)->b;
	std::snprintf(ptr,n,"%hu %hu %hu", r,g,b );
}

