// Спецификация для одного аргумента.

class IUIContainer 
{ 
public: 
    virtual void Call(int ID, list<TUI_Childless*>* a, IArguments* ) = 0;
    virtual void Call(int ID, TUI_Childless* b, IArguments* i_args ) = 0;
    virtual void Call(int ID, list<TUI_Childless*>* a, IArguments*, typename list<TUI_Childless*>::iterator& it) = 0;
    
    virtual void Call(int ID, vector<TUI_Childless*>* a, IArguments* ) = 0;
    virtual void Call(int ID, vector<TUI_Childless*>* a, IArguments*, typename vector<TUI_Childless*>::iterator& it) = 0;
    
    
    virtual void SingleCall(int ID, list<TUI_Childless*>* a, IArguments* ) = 0;
    virtual void SingleCall(int ID, TUI_Childless* b, IArguments* i_args ) = 0;
    virtual void SingleCall(int ID, list<TUI_Childless*>* a, IArguments*, typename list<TUI_Childless*>::iterator& it) = 0;
    
    virtual void SingleCall(int ID, vector<TUI_Childless*>* a, IArguments* ) = 0;
    virtual void SingleCall(int ID, vector<TUI_Childless*>* a, IArguments*, typename vector<TUI_Childless*>::iterator& it) = 0;
};
template< class T, class M> class UIContainer : public IUIContainer {};

// Здесь будет храниться информация о событии

//  Спецификация без аргументов.
template<class T>
  class UIContainer<T, void (T::*)(void)> : public IUIContainer
{
    typedef void (T::*M)(void);
    
    public: 
    T* m_class; M m_method;

    UIContainer( T* c, M m ) : m_class( c ), m_method( m ) {}
	
    void Call(int ID, list<TUI_Childless*>* a, IArguments* i_args )
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) { (c->*m_method)(); c->sevents.Call(ID); };
        }
    }
    
    void Call(int ID, list<TUI_Childless*>* a, IArguments* i_args, typename list<TUI_Childless*>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) { (c->*m_method)(); c->sevents.Call(ID); };
        }
    }
    
    void Call(int ID, vector<TUI_Childless*>* a, IArguments* i_args )
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) { (c->*m_method)(); c->sevents.Call(ID); };
        }
    }
    
    void Call(int ID, vector<TUI_Childless*>* a, IArguments* i_args, typename vector<TUI_Childless*>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) { (c->*m_method)(); c->sevents.Call(ID); };
        }
    }
    
    void Call(int ID, TUI_Childless* b, IArguments* i_args ) 
    {
        T* c = dynamic_cast<T*>(b);
        if (c) {(c->*m_method)(); c->sevents.Call(ID);};
    }
//##############################################################################  

    void SingleCall(int ID, list<TUI_Childless*>* a, IArguments* i_args )
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) (c->*m_method)();
        }
    }
    
    void SingleCall(int ID, list<TUI_Childless*>* a, IArguments* i_args, typename list<TUI_Childless*>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) (c->*m_method)();
        }
    }
    
    void SingleCall(int ID, vector<TUI_Childless*>* a, IArguments* i_args )
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) (c->*m_method)();
        }
    }
    
    void SingleCall(int ID, vector<TUI_Childless*>* a, IArguments* i_args, typename vector<TUI_Childless*>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
           	T* c = dynamic_cast<T*>(*it);
            if (c) (c->*m_method)();
        }
    }
    
    void SingleCall(int ID, TUI_Childless* b, IArguments* i_args ) 
    {
        T* c = dynamic_cast<T*>(b);
        if (c) (c->*m_method)();
    }
    
};

// Спецификация для одного аргумента.

template< class T, class A1 >
  class UIContainer< T, void (T::*)(A1) > : public IUIContainer
{  
    public: 
        
    typedef void (T::*M)(A1);
    typedef Arguments<A1> A;
    public: UIContainer( T* c, M m ) : m_class( c ), m_method( m ) {}
    private: T* m_class; M m_method;
    
    void Call(int ID, list<TUI_Childless*>* a, IArguments* i_args)
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) {(c->*m_method)(a->arg1);  c->sevents.Call(ID,a->arg1);};
        }
    }
    
    void Call(int ID, list<TUI_Childless*>* a, IArguments* i_args, typename list<TUI_Childless*>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) {(c->*m_method)(a->arg1); c->sevents.Call(ID,a->arg1);};
        }
    }
    
    void Call(int ID, vector<TUI_Childless*>* a, IArguments* i_args)
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) {(c->*m_method)(a->arg1); c->sevents.Call(ID,a->arg1);};
        }
    }
    
    void Call(int ID, vector<TUI_Childless*>* a, IArguments* i_args, typename vector<TUI_Childless*>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) {(c->*m_method)(a->arg1); c->sevents.Call(ID,a->arg1);};
        }
    }
    
    void Call(int ID, TUI_Childless* b, IArguments* i_args ) 
    {
        T* c = dynamic_cast<T*>(b);
        A* a = dynamic_cast< A* >( i_args );
        if (c && a) {(c->*m_method)(a->arg1); c->sevents.Call(ID,a->arg1);};
    }
//##########################################################################

    void SingleCall(int ID, list<TUI_Childless*>* a, IArguments* i_args)
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }
    
    void SingleCall(int ID, list<TUI_Childless*>* a, IArguments* i_args, typename list<TUI_Childless*>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }
    
    void SingleCall(int ID, vector<TUI_Childless*>* a, IArguments* i_args)
    {
        for (auto it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }
    
    void SingleCall(int ID, vector<TUI_Childless*>* a, IArguments* i_args, typename vector<TUI_Childless*>::iterator& it)
    {
        for (it = a->begin(); it != a->end(); it++)
        {
            T* c = dynamic_cast<T*>(*it);
            A* a = dynamic_cast< A* >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }
    
    void SingleCall(int ID, TUI_Childless* b, IArguments* i_args ) 
    {
        T* c = dynamic_cast<T*>(b);
        A* a = dynamic_cast< A* >( i_args );
        if (c && a) (c->*m_method)(a->arg1);
    }
};

class UIEvents {
    public:
        vector<TUI_Childless*>* array;
        typename vector<TUI_Childless*>::iterator it;
        vector< vector<IUIContainer*>* > UIContainers;
        
        UIEvents() 
		{
			array = 0;
		};
        ~UIEvents() // DANGEROUS!
        {	
        	//cout << "ERAZING EVENTS! " << endl;
        	for ( auto &v : UIContainers )
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
            while(UIContainers.size() <= ID) UIContainers.push_back(0);
			if (UIContainers[ID] == 0) UIContainers[ID] = new vector<IUIContainer*>;
			UIContainers[ID]->push_back( new UIContainer< T, U >( i_class, i_method ) );
        }
        
        // массовый вызов событий
        
        void Call(int ID) // один аргумент
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	v->Call(ID, array, (Arguments<>*)(0) );
            }
            
            //cout << "Calling " << GetHookNameByID(ID) << endl;
        };
        
        void Call( int ID, typename vector<TUI_Childless*>::iterator it ) // ИТЕРАЦИЯ!
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	v->Call(ID, array, (Arguments<>*)(0), it);
            }
            
            //cout << "Calling " << GetHookNameByID(ID) << endl;
        };
        
        template< class T1 > void Call(int ID, T1 i_arg1 ) // два аргумента
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	Arguments< T1 > args (i_arg1);
            	v->Call(array, &args );
            }
            
            //cout << "Calling " << GetHookNameByID(ID) << endl;
        };
        
        // Вызов события для частного объекта

        void Call(TUI_Childless* b, int ID) // один аргумент
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
			for (auto &v : *UIContainers[ID] )
			{
				v->Call(ID, b, (Arguments<>*)(0) );
			}
			
			//cout << "Calling " << GetHookNameByID(ID) << endl;
        };
        
        template< class T1 > void Call(TUI_Childless* b, int ID, T1 i_arg1 ) // два аргумента
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
			for (auto &v : *UIContainers[ID] )
			{
				Arguments< T1 > args (i_arg1);
            	v->Call(ID, b, &args );
			}
			
			//cout << "Calling " << GetHookNameByID(ID) << endl;
        };
        
        
// ############################ Для особого массива:
        
        // массовый вызов событий
        
        void Call(list<TUI_Childless*>* ar, int ID) // один аргумент
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	v->Call(ID, ar, (Arguments<>*)(0) );
            }
            
            //cout << "Calling " << GetHookNameByID(ID) << endl;
        };
        
        template< class T1 > void Call(list<TUI_Childless*>* ar, int ID, T1 i_arg1 ) // два аргумента
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	Arguments< T1 > args (i_arg1);
            	v->Call(ID, ar, &args );
            }
            
            //cout << "Calling " << GetHookNameByID(ID) << endl;
        };
        
//################################# Single Calls!

        void SingleCall(int ID) // один аргумент
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	v->SingleCall(ID, array, (Arguments<>*)(0) );
            }
            
            //cout << "Single Calling " << GetHookNameByID(ID) << endl;
        };
        
        void SingleCall( int ID, typename vector<TUI_Childless*>::iterator it ) // ИТЕРАЦИЯ!
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	v->SingleCall(ID, array, (Arguments<>*)(0), it);
            }
            
            //cout << "Single Calling " << GetHookNameByID(ID) << endl;
        };
        
        template< class T1 > void SingleCall(int ID, T1 i_arg1 ) // два аргумента
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	Arguments< T1 > args (i_arg1);
            	v->SingleCall(array, &args );
            }
            
            //cout << "Single Calling " << GetHookNameByID(ID) << endl;
        };
        
        // Вызов события для частного объекта

        void SingleCall(TUI_Childless* b, int ID) // один аргумент
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
			for (auto &v : *UIContainers[ID] )
			{
				v->SingleCall(ID, b, (Arguments<>*)(0) );
			}
			
			//cout << "Single Calling " << GetHookNameByID(ID) << endl;
        };
        
        template< class T1 > void SingleCall(TUI_Childless* b, int ID, T1 i_arg1 ) // два аргумента
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
			for (auto &v : *UIContainers[ID] )
			{
				Arguments< T1 > args (i_arg1);
            	v->SingleCall(ID, b, &args );
			}
			
			//cout << "Single Calling " << GetHookNameByID(ID) << endl;
        };
        
        
// ############################ Для особого массива:
        
        // массовый вызов событий
        
        void SingleCall(list<TUI_Childless*>* ar, int ID) // один аргумент
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	v->SingleCall(ID, ar, (Arguments<>*)(0) );
            }
            
            //cout << "Single Calling " << GetHookNameByID(ID) << endl;
        };
        
        template< class T1 > void SingleCall(list<TUI_Childless*>* ar, int ID, T1 i_arg1 ) // два аргумента
        {
            if ( (ID >= UIContainers.size()) || (UIContainers[ID] == 0) ) return;
            for (auto &v : *UIContainers[ID] )
            {
            	Arguments< T1 > args (i_arg1);
            	v->SingleCall(ID, ar, &args );
            }
            
            //cout << "Single Calling " << GetHookNameByID(ID) << endl;
        };
        
};
