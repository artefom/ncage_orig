//  Контейнер для хранения до 2-х аргументов.
//  Контейнер для хранения указателя на метод.
class IContainer_Single{ public: virtual void Call( IArguments* ) = 0; };
template< class T, class M > class Container_Single : public IContainer_Single {};

//  Специализация для метода без аргументов.
template< class T >
  class Container_Single< T, void (T::*)(void) > : public IContainer_Single
{
  typedef void (T::*M)(void);
  public: Container_Single( T* c, M m ) : m_class( c ), m_method( m ) {}
  private: T* m_class; M m_method;
  public: void Call( IArguments* i_args )
  {
    (m_class->*m_method)();
  }
};

//  Специализация для метода с одним аргументом.
template< class T, class A1 >
  class Container_Single< T, void (T::*)(A1) > : public IContainer_Single
{
  typedef void (T::*M)(A1);
  typedef Arguments<A1> A;
  public: Container_Single( T* c, M m ) : m_class( c ), m_method( m ) {}
  private: T* m_class; M m_method;
  public: void Call( IArguments* i_args )
  {
    A* a = dynamic_cast< A* >( i_args );
    assert( a );
    if( a ) (static_cast<T*>(m_class)->*m_method)( a->arg1 );
  }
};

//  Специализация для метода с двумя аргументами
template< class T, class A1, class A2 >
  class Container_Single< T, void (T::*)(A1,A2) > : public IContainer_Single
{
  typedef void (T::*M)(A1,A2);
  typedef Arguments<A1,A2> A;
  public: Container_Single( T* c, M m ) : m_class( c ), m_method( m ) {}
  private: T* m_class; M m_method;
  public: void Call( IArguments* i_args )
  {
    A* a = dynamic_cast< A* >( i_args );
    assert( a );
    if( a ) (m_class->*m_method)( a->arg1, a->arg2 );
  }
};

//  Собственно делегат.
class Delegate
{
public:

  Delegate() : m_Container_Single( 0 ) {}
  ~Delegate() { if( m_Container_Single ) delete m_Container_Single; }

  template< class T, class U > void Connect( T* i_class, U i_method )
  {
    if( m_Container_Single ) delete m_Container_Single;
    m_Container_Single = new Container_Single< T, U >( i_class, i_method );
  }

  void operator()()
  {
    m_Container_Single->Call( (Arguments<>*)(0) );
  }

  template< class T1 > void operator()( T1 i_arg1 )
  {
  	Arguments< T1 >a ( i_arg1 ) ;
    m_Container_Single->Call( &a );
  }

  template< class T1, class T2 > void operator()( T1 i_arg1, T2 i_arg2 )
  {
    m_Container_Single->Call( & Arguments< T1, T2 >( i_arg1, i_arg2 ) );
  }
  
  void operator()( IArguments* args )
  {
    m_Container_Single->Call( args );
  }
  
  bool Connected()
  {
	return m_Container_Single;
  }

private:
  IContainer_Single* m_Container_Single;
};

template<> void Delegate::operator()<IArguments*>(IArguments* i_arg1)
{
	m_Container_Single->Call( i_arg1 );
};
