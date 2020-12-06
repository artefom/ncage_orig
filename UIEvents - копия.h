// Спецификация для одного аргумента.

class IUIContainer {
public:
    virtual void Call(list<TUI_Childless *> *a, IArguments *) = 0;

    virtual void Call(TUI_Childless *b, IArguments *i_args) = 0;

    virtual void Call(list<TUI_Childless *> *a, IArguments *, typename list<TUI_Childless *>::iterator &it) = 0;

    virtual void Call(vector<TUI_Childless *> *a, IArguments *) = 0;

    virtual void Call(vector<TUI_Childless *> *a, IArguments *, typename vector<TUI_Childless *>::iterator &it) = 0;
};

template<class B, class T, class M>
class UIContainer : public IUIContainer {
};

// Здесь будет храниться информация о событии

//  Спецификация без аргументов.
template<class B, class T>
class UIContainer<B, T, void (T::*)(void)> : public IUIContainer {
    typedef void (T::*M)(void);

public:
    T *m_class;
    M m_method;

    UIContainer(T *c, M m) : m_class(c), m_method(m) {}

    void Call(list <B> *a, IArguments *i_args) {
        for (auto it = a->begin(); it != a->end(); it++) {
            T *c = dynamic_cast<T *>(*it);
            if (c) { (c->*m_method)(); };
        }
    }

    void Call(list <B> *a, IArguments *i_args, typename list<B>::iterator &it) {
        for (it = a->begin(); it != a->end(); it++) {
            T *c = dynamic_cast<T *>(*it);
            if (c) { (c->*m_method)(); };
        }
    }

    void Call(vector <B> *a, IArguments *i_args) {
        for (auto it = a->begin(); it != a->end(); it++) {
            T *c = dynamic_cast<T *>(*it);
            if (c) { (c->*m_method)(); };
        }
    }

    void Call(vector <B> *a, IArguments *i_args, typename vector<B>::iterator &it) {
        for (it = a->begin(); it != a->end(); it++) {
            T *c = dynamic_cast<T *>(*it);
            if (c) { (c->*m_method)(); };
        }
    }

    void Call(B b, IArguments *i_args) {
        T *c = dynamic_cast<T *>(b);
        if (c) (c->*m_method)();
    }

};

// Спецификация для одного аргумента.

template<class B, class T, class A1>
class UIContainer<B, T, void (T::*)(A1)> : public IUIContainer {
public:

    typedef void (T::*M)(A1);

    typedef Arguments <A1> A;
public:
    UIContainer(T *c, M m) : m_class(c), m_method(m) {}

private:
    T *m_class;
    M m_method;

    void Call(list <B> *a, IArguments *i_args) {
        for (auto it = a->begin(); it != a->end(); it++) {
            T *c = dynamic_cast<T *>(*it);
            A *a = dynamic_cast< A * >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }

    void Call(list <B> *a, IArguments *i_args, typename list<B>::iterator &it) {
        for (it = a->begin(); it != a->end(); it++) {
            T *c = dynamic_cast<T *>(*it);
            A *a = dynamic_cast< A * >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }

    void Call(vector <B> *a, IArguments *i_args) {
        for (auto it = a->begin(); it != a->end(); it++) {
            T *c = dynamic_cast<T *>(*it);
            A *a = dynamic_cast< A * >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }

    void Call(vector <B> *a, IArguments *i_args, typename vector<B>::iterator &it) {
        for (it = a->begin(); it != a->end(); it++) {
            T *c = dynamic_cast<T *>(*it);
            A *a = dynamic_cast< A * >( i_args );
            if (c && a) (c->*m_method)(a->arg1);
        }
    }

    void Call(B b, IArguments *i_args) {
        T *c = dynamic_cast<T *>(b);
        A *a = dynamic_cast< A * >( i_args );
        if (c && a) (c->*m_method)(a->arg1);
    }

};

class UIEvents {
public:
    vector<TUI_Childless *> *array;
    typename vector<TUI_Childless *>::iterator it;
    vector<vector < IUIContainer * >* >
    UIContainers;

    UIEvents() {
        array = 0;
    };

    ~UIEvents() // DANGEROUS!
    {
        //cout << "ERAZING EVENTS! " << endl;
        for (auto &v : UIContainers) {
            if (v) {

                for (auto &k : *v) {
                    delete k;
                };
                delete v;
            }
        };

    }


    // присоединение класса и его метода
    template<class T, class U>
    void Connect(int ID, T *i_class, U i_method) {
        while (UIContainers.size() <= ID) UIContainers.push_back(0);
        if (UIContainers[ID] == 0) UIContainers[ID] = new vector<IUIContainer *>;
        UIContainers[ID]->push_back(new UIContainer<TUI_Childless *, T, U>(i_class, i_method));
    }

    // массовый вызов событий

    void Call(int ID) // один аргумент
    {
        if ((ID >= UIContainers.size()) || (UIContainers[ID] == 0)) return;
        for (auto &v : *UIContainers[ID]) {
            v->Call(array, (Arguments<> * )(0));
        }
    };

    void Call(int ID, typename vector<TUI_Childless *>::iterator it) // ИТЕРАЦИЯ!
    {
        if ((ID >= UIContainers.size()) || (UIContainers[ID] == 0)) return;
        for (auto &v : *UIContainers[ID]) {
            v->Call(array, (Arguments<> * )(0), it);
        }
    };

    template<class T1>
    void Call(int ID, T1 i_arg1) // два аргумента
    {
        if ((ID >= UIContainers.size()) || (UIContainers[ID] == 0)) return;
        for (auto &v : *UIContainers[ID]) {
            Arguments <T1> args(i_arg1);
            v->Call(array, &args);
        }
    };

    // Вызов события для частного объекта

    void Call(TUI_Childless *b, int ID) // один аргумент
    {
        if ((ID >= UIContainers.size()) || (UIContainers[ID] == 0)) return;
        for (auto &v : *UIContainers[ID]) {
            v->Call(b, (Arguments<> * )(0));
        }
    };

    template<class T1>
    void Call(TUI_Childless *b, int ID, T1 i_arg1) // два аргумента
    {
        if ((ID >= UIContainers.size()) || (UIContainers[ID] == 0)) return;
        for (auto &v : *UIContainers[ID]) {
            Arguments <T1> args(i_arg1);
            v->Call(b, &args);
        }
    };


// ############################ Для особого массива:

    // массовый вызов событий

    void Call(list<TUI_Childless *> *ar, int ID) // один аргумент
    {
        if ((ID >= UIContainers.size()) || (UIContainers[ID] == 0)) return;
        for (auto &v : *UIContainers[ID]) {
            v->Call(ar, (Arguments<> * )(0));
        }
    };

    template<class T1>
    void Call(list<TUI_Childless *> *ar, int ID, T1 i_arg1) // два аргумента
    {
        if ((ID >= UIContainers.size()) || (UIContainers[ID] == 0)) return;
        for (auto &v : *UIContainers[ID]) {
            Arguments <T1> args(i_arg1);
            v->Call(ar, &args);
        }
    };
};

UIEvents TUI_Childless::events;
UIEvents &ui_events = TUI_Childless::events;
