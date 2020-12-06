class TUI_ParentLess : public TUI_Childless
{
	protected:
		list<TUI_Childless*> elms; // can have child elements, used as base UI (see bottom)
	public:
		
	
	TUI_ParentLess(TUI_ParentLess* parent) : TUI_Childless(parent)
	{

	}
	
	TUI_ParentLess()
	{
	}
	
	virtual ~TUI_ParentLess() 
	{
		for (auto v = elms.begin(); v != elms.end(); v++) delete *v;
	}
	
	virtual bool TestMouse(Vec2i mpos)
    {
    	if ( !IsMouseIn(mpos) || dead ) return false;
    	    	
    	for (auto v = elms.rbegin(); v != elms.rend(); v++)
    	{
			if ( (*v)->TestMouse(mpos) ) return true;	
    	}
		SetGlobalMouseGui( 0 );
		return false; 
    }
	
	template<class T> T* add(T obj)
	{
		elms.push_back(obj);
		OnAdd();
		return &elms.back();
	}
	
	virtual void OnAdd()
	{
		
	};

	virtual void Draw()
	{
		glPushMatrix();
		glTranslate( GetPos() );
			OnDraw();
			for (auto &it : elms) it->Draw();
		glPopMatrix();
	}
	
	virtual void Cleanup()
	{
		auto v = elms.begin();
		while (v != elms.end())
		{
			if (*v == 0)
			{
				v = elms.erase(v);
			} else
			{
				if ((*v)->dead)
				{
					delete *v;
					v = elms.erase(v);	
				} else 
				{
					if ( !(*v)->visible ) 
					{
						(*v)->SetPTP(0);
						v = elms.erase(v);
					}	
					else v++;
				};
			};
		}
	}
	
	virtual void SetupViewport()
	{	
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		SetUpRender(ScrW, ScrH);
	};
	

	static void CleanupAll()
	{
		if (Cleanup_Queue.size() == 0) return;
		for ( auto &v : Cleanup_Queue)
		{
			if (*v != 0) (*v)->Cleanup();
		}
		Cleanup_Queue.clear();
	}
	
	virtual void UpdateGlobalPosHelpers()
	{
		TUI_Childless::UpdateGlobalPosHelpers();
		for ( auto &v : elms)
		{
			v->UpdateGlobalPosHelpers();
		}
	}

	virtual Vec2i GetGlobalPos()
	{
		return Position;
	};

	virtual bool HasMouse()
	{
		return IsMouseIn(MousePos);
	};
    
	virtual void Update()
	{

	}
	
	virtual Vec2i GetMaxSize()
	{
		Vec2i result = 0;
		for (auto &it : elms)
		{
			if ((it->Position.y + it->Size.y) > result.y) result.y = (it->Position.y + it->Size.y);
			if ((it->Position.y + it->Size.y) > result.x) result.x = (it->Position.y + it->Size.y);
		};
		return result;
	};    
};

	TUI_Childless::TUI_Childless(TUI_ParentLess* parent)
	{
		Position = 0;
		Parent = parent;
		Size = 0;
		visible = 1;
		dead = 0;
		globalhooks = 0;
		ptp = Parent->add(this);
	}
	
Vec2i TUI_Childless::GetGlobalPos()
{
	return Parent->GetGlobalPos()+Position;
};

bool TUI_Childless::HasMouse()
{
    if (!GlobalMouseGui) return false;
	if (GlobalMouseGui == this) return true;
	TUI_Childless* parent = GlobalMouseGui;
	while(parent)
	{
		if (parent == this) return true;
		parent = parent->Parent;
	};
	return false;
};
    
void TUI_Childless::Update()
{
	Parent->Update();
}
    
void TUI_Childless::Suicide() // After this class has unpredicted behaviour
{
	dead = true;
	sevents.Clear();
	if (Parent->ptp) Cleanup_Queue.push_back((TUI_ParentLess**)(Parent->ptp)); else cout << "TUI_ParentLess: PTP IZ 0!" << endl;
}

void TUI_Childless::SetVisible(bool iv)
{
	if (!visible && iv)
	{
		if (ptp) *ptp = 0; else cout << "PTP IZ 0 on ebabling visible!" << endl;
		SetPTP(Parent->add(this));
	}
	if (visible && !iv)
	{
		if (Parent->ptp) Cleanup_Queue.push_back((TUI_ParentLess**)(Parent->ptp)); 
		else cout << "TUI_ParentLess: PTP IZ 0 on disabling visible!" << endl;
	};
	visible = iv;
}

	void TUI_Childless::SetKeyboardElement( TUI_Childless* elm )
	{
		ui_events.Call(TUI_Childless::KeyboardElement,HOOK_OnKeyboardExit);
		KeyboardElement = elm;
		ui_events.Call(TUI_Childless::KeyboardElement,HOOK_OnKeyboardEnter);
	}
	
TUI_ParentLess UI; // Here is it. Global UI that main deals with



