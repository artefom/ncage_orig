//  Контейнер для хранения до 2-х аргументов.
//  Контейнер для хранения указателя на метод.
class IContainer_Func{ public: virtual void Call( IArguments* ) = 0; };
template<class M > class Container_Func : public IContainer_Func {};

//  Специализация для метода без аргументов.
template<>
class Container_Func< void (*)(void) > : public IContainer_Func
{
  typedef void (*M)(void);
  public: Container_Func( M m ) : m_method( m ) {}
  private: M m_method;
  public: void Call( IArguments* i_args )
  {
    (*m_method)();
  }
};



//  Специализация для метода с одним аргументом.
template< class A1 >
  class Container_Func< void (*)(A1) > : public IContainer_Func
{
  typedef void (*M)(A1);
  typedef Arguments<A1> A;
  public: Container_Func( M m ) : m_method( m ) {}
  private: M m_method;
  public: void Call( IArguments* i_args )
  {
    A* a = dynamic_cast< A* >( i_args );
    assert( a );
    if( a ) (*m_method)( a->arg1 );
  }
};

class Callback
{
public:

  Callback() : m_Container_Func( 0 ) {}
  ~Callback() { if( m_Container_Func ) delete m_Container_Func; }

  template< class F> void Connect( F func )
  {
    if( m_Container_Func ) delete m_Container_Func;

    m_Container_Func = new Container_Func< F >( func );
  }

  void operator()()
  {
    m_Container_Func->Call( (Arguments<>*)(0) );
  }

  template< class T1 > void operator()( T1 i_arg1 )
  {
    m_Container_Func->Call( & Arguments< T1 >( i_arg1 ) );
  }

  template< class T1, class T2 > void operator()( T1 i_arg1, T2 i_arg2 )
  {
    m_Container_Func->Call( & Arguments< T1, T2 >( i_arg1, i_arg2 ) );
  }
  
  void operator()( IArguments* args )
  {
    m_Container_Func->Call( args );
  }
  
  bool Connected()
  {
	return m_Container_Func;
  }

private:
  IContainer_Func* m_Container_Func;
};

class SCallbacks
{
	public:
		
	map<int, vector< Callback > > Callbacks;
	
	template< class T > void Connect( int hook_id, T func )
	{
		vector<Callback>& lst = Callbacks[hook_id];
		lst.push_back( Callback() );
		lst.back().Connect( func );
	}
	
	void Call( int ID )
	{
		vector<Callback>& lst = Callbacks[ID];
		for ( auto &v : lst )
		{
			v();
		}
	}
	
	template<class T> void Call( int ID, T arg )
	{
		vector<Callback>& lst = Callbacks[ID];
		for ( auto &v : lst )
		{
			v(arg);
		}
	}
	
	void Clear()
	{
		Callbacks.clear();
	};
	
};
