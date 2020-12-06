// Аргументы 

struct NIL {};

class IArguments { public: virtual ~IArguments() {} };
template< class T1 = NIL, class T2 = NIL >
  class Arguments : public IArguments
{
  public: Arguments() {}
  public: Arguments( T1 i_arg1 ) : arg1( i_arg1 ) {}
  public: Arguments( T1 i_arg1, T2 i_arg2 ) : arg1( i_arg1 ), arg2( i_arg2 ) {}
  public: T1 arg1; T2 arg2;
};

// Базовый класс контейнеров для последующей шаблонной инициализации

template <class Base> class IContainer 
{ 
public: 
    virtual void Call(list<Base>* a, IArguments* ) = 0;
    virtual void Call( Base b, IArguments* i_args ) = 0;
    virtual void Call(list<Base>* a, IArguments*, typename list<Base>::iterator& it) = 0;
    
    virtual void Call(vector<Base>* a, IArguments* ) = 0;
    virtual void Call(vector<Base>* a, IArguments*, typename vector<Base>::iterator& it) = 0;
};
template< class B, class T, class M> class Container : public IContainer<B> {};

// Здесь будет храниться информация о событии

//  Спецификация без аргументов.
template< class B , class T>
  class Container< B, T, void (T::*)(void)> : public IContainer<B>
{
    typedef void (T::*M)(void);
    
    public: 
    T* m_class; M m_method;

    Container( T* c, M m ) : m_class( c ), m_method( m ) {}
	
    void Call(list<B>* a, IArguments* i_args )
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) { (c->*m_method)(); };
        }
    }
    
    void Call(list<B>* a, IArguments* i_args, typename list<B>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) { (c->*m_method)(); };
        }
    }
    
    void Call(vector<B>* a, IArguments* i_args )
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) { (c->*m_method)(); };
        }
    }
    
    void Call(vector<B>* a, IArguments* i_args, typename vector<B>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) { (c->*m_method)(); };
        }
    }
    
    void Call( B b, IArguments* i_args ) 
    {
        T* c = dynamic_cast<T*>(b);
        if (c) (c->*m_method)();
    }
        
};

// Спецификация для одного аргумента.

template< class B, class T, class A1 >
  class Container< B, T, void (T::*)(A1) > : public IContainer<B>
{  
    public: 
        
    typedef void (T::*M)(A1);
    typedef Arguments<A1> A;
    public: Container( T* c, M m ) : m_class( c ), m_method( m ) {}
    private: T* m_class; M m_method;
    
    void Call(list<B>* a, IArguments* i_args)
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }
    
    void Call(list<B>* a, IArguments* i_args, typename list<B>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }
    
    void Call(vector<B>* a, IArguments* i_args)
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }
    
    void Call(vector<B>* a, IArguments* i_args, typename vector<B>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }
    
    void Call( B b, IArguments* i_args ) 
    {
        T* c = dynamic_cast<T*>(b);
        A* a = dynamic_cast< A* >( i_args );
        if (c && a) (c->*m_method)(a->arg1);
    }
    
};


// событие
template<class Base> class Events {
    public:
        list<Base>* array;
        typename list<Base>::iterator it;
        vector< vector<IContainer<Base>*>* > Containers;
        
        Events() 
		{
			array = 0;
		};
        ~Events() // DANGEROUS!
        {	
        	//cout << "ERAZING EVENTS! " << endl;
        	for ( auto &v : Containers )
        	{
        		if (v)
        		{
        			
        			for (auto &k : *v)
        			{
        				delete k;		
        			};
        			delete v;
        		}
        	};
        	
        }
        
        
        // присоединение класса и его метода 
        template< class T, class U > void Connect( int ID, T* i_class, U i_method )
        {
            while(Containers.size() <= ID) Containers.push_back(0);
			if (Containers[ID] == 0) Containers[ID] = new vector<IContainer<Base>*>;
			Containers[ID]->push_back( new Container< Base, T, U >( i_class, i_method ) );
        }
        
        // массовый вызов событий
        
        void Call(int ID) // один аргумент
        {
            if ( (ID >= Containers.size()) || (Containers[ID] == 0) ) return;
            for (auto &v : *Containers[ID] )
            {
            	v->Call(array, (Arguments<>*)(0) );
            }
        };
        
        void Call( int ID, typename list<Base>::iterator it ) // ИТЕРАЦИЯ!
        {
            if ( (ID >= Containers.size()) || (Containers[ID] == 0) ) return;
            for (auto &v : *Containers[ID] )
            {
            	v->Call(array, (Arguments<>*)(0), it);
            }
        };
        
        template< class T1 > void Call(int ID, T1 i_arg1 ) // два аргумента
        {
            if ( (ID >= Containers.size()) || (Containers[ID] == 0) ) return;
            for (auto &v : *Containers[ID] )
            {
            	Arguments< T1 > args (i_arg1);
            	v->Call(array, &args );
            }
        };
        
        // Вызов события для частного объекта

        void Call(Base b, int ID) // один аргумент
        {
            if ( (ID >= Containers.size()) || (Containers[ID] == 0) ) return;
			for (auto &v : *Containers[ID] )
			{
				v->Call(b, (Arguments<>*)(0) );
			}
        };
        
        template< class T1 > void Call(Base b, int ID, T1 i_arg1 ) // два аргумента
        {
            if ( (ID >= Containers.size()) || (Containers[ID] == 0) ) return;
			for (auto &v : *Containers[ID] )
			{
				Arguments< T1 > args (i_arg1);
            	v->Call(b, &args );
			}
        };
        
        
// ############################ Для особого массива:
        
        // массовый вызов событий
        
        void Call(list<Base>* ar, int ID) // один аргумент
        {
            if ( (ID >= Containers.size()) || (Containers[ID] == 0) ) return;
            for (auto &v : *Containers[ID] )
            {
            	v->Call(ar, (Arguments<>*)(0) );
            }
        };
        
        template< class T1 > void Call(list<Base>* ar, int ID, T1 i_arg1 ) // два аргумента
        {
            if ( (ID >= Containers.size()) || (Containers[ID] == 0) ) return;
            for (auto &v : *Containers[ID] )
            {
            	Arguments< T1 > args (i_arg1);
            	v->Call(ar, &args );
            }
        };
        
};
