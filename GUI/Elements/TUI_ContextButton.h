class TUI_MenuContext_Simple : public TUI_SimpleFrame
{
	public:
		int textwidth_max;
		int textwidth_min;
		list<TUI_Childless*> &buttons;
		vector<TUI_Button*> btns;
		int chosen_id;
		TUI_MenuContext_Simple(TUI_ParentLess* parent) : TUI_SimpleFrame( parent ), buttons(elms)
		{
			textwidth_max = 0;
			textwidth_min = 10;
			chosen_id = -1;
		};
		
		virtual void Init()
		{
			textwidth_max = 0;
			TUI_Button* btn;
			for (auto &v : elms)
			{
				if (!v->dead)
				{
					btn = static_cast<TUI_Button*>(v);
					if (btn->textwidth > textwidth_max) textwidth_max = btn->textwidth;
				};
			};
			int curheight = 1;
			textwidth_max += 12;
			if (textwidth_max < textwidth_min) textwidth_max = textwidth_min;
			for (auto &v : elms)
			{
				if (!v->dead)
				{
					btn = static_cast<TUI_Button*>(v);
					btn->SetOrientation(-1);
					btn->SetPos( Vec2i(1,curheight) );
					btn->SetSize( Vec2i(textwidth_max-2, MainFont_Height+12) );
					btn->boarder = 0;
					curheight += MainFont_Height+11;
				};
			};
			SetSize( Vec2i(textwidth_max,curheight+2) );
		}
		
		virtual void AddButton(string text)
		{
			TUI_Button* btn = new TUI_Button(this);
			btn->SetCaption( text );
			btn->sevents.Connect(HOOK_OnClick, this, &TUI_MenuContext_Simple::SetContext);
			btn->SetArgument((int)(buttons.size()-1));
			btns.push_back(btn);
			Init();
		};
		
		virtual int ChoseID(int ID)
		{
			if ( (chosen_id >= 0) && (chosen_id < btns.size()) ) btns[chosen_id]->light = 0;
			chosen_id = ID;
			chosen_id = clamp(ID,-1,btns.size()-1);
			if (chosen_id != -1) btns[chosen_id]->light = 1;
			return chosen_id;
		};
		
		virtual const char* ChosenText()
		{
			if (chosen_id == -1) return 0;
			return btns[chosen_id]->caption.c_str();
		};
		
		virtual int ChosenID()
		{
			return chosen_id;
		};
		
		virtual void SetContext(int ID)
		{
			sevents.Call(HOOK_OnClick,ID);
		};
		
		virtual void OnDraw()
		{
			//glColor(UI_BG_Dark);
			//drawrect( 0,Size );
			drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
		}
};

class TUI_ContextButton : public TUI_Button
{
	public:
		
		vector< char* > Context;
		vector< int > IDs;
		TUI_MenuContext_Simple* menu;
		int c_context;
		int closedist;
		bool contextcaption;
		TBitmapTex arrow_up;
		TBitmapTex arrow_down;
		Vec2i markpos;
		
		TUI_ContextButton(TUI_ParentLess* parent) : TUI_Button( parent )
		{
			EnableGlobalHooks();
			c_context = 0;
			closedist = 40;	
			menu = 0;
			contextcaption = 1;
			arrow_up.LoadTexture("arrow_up");
			arrow_down.LoadTexture("arrow_down");
		}
		
		~TUI_ContextButton()
		{
			KillContextMenu();
		}
				
		virtual void SetOrientation(char orient)
		{
			TUI_Button::SetOrientation(orient);
			if ((orientation != -1) && (textpos < arrow_down.Size.x+8)) textpos = arrow_down.Size.x+8; return;
		}
	
		virtual int AddContext( char* caption )
		{
			Context.push_back( caption );
			if (IDs.size() != 0) IDs.push_back(IDs.back()+1); else IDs.push_back(0);
			return IDs.back();
		}
		
		virtual int AddContext( char* caption, int ID )
		{
			Context.push_back( caption );
			IDs.push_back(ID);
			return ID;
		}
		
		virtual void InitContext( int ID )
		{
			c_context = ID;
			if (contextcaption) SetCaption( Context[c_context] );
		};
		
		virtual void SetContext( int ID )
		{
			c_context = ID;
			if (contextcaption) SetCaption( Context[c_context] );
			sevents.Call(HOOK_OnChange);
		};
		
		virtual void OnResize()
		{
			TUI_Button::OnResize();
			markpos.x = 5;
			if (orientation == -1) markpos.x = Size.x - arrow_down.Size.x - 5;
			markpos.y = (Size.y-arrow_down.Size.y)*0.5;
		};
		
		virtual void OnDraw()
		{
			TUI_Button::OnDraw();
			if (toggle)
			{
				glColor(UI_BG_Light);
				drawrect(markpos,arrow_up);
			}
			else
			{
				glColor(UI_BG_Bright);
				drawrect(markpos,arrow_down);
			};
		}
		
		virtual void OpenContextMenu()
		{
			if (menu) return;
			if (Context.size() == 0) return;
			toggle = 1;
			if (Context.size() == 0) return;
			menu = new TUI_MenuContext_Simple(&UI);
			
			TUI_Button* btn;
			
			for (int i = 0; i != Context.size(); i++)
			{
				btn = new TUI_Button(menu);
				btn->SetCaption( Context[i] );
				btn->sevents.Connect(HOOK_OnClick, this, &TUI_ContextButton::OnContextClick);
				btn->SetArgument( IDs[i] );
			};
			menu->SetPos( GPos1 + Vec2i(0,Size.y-1) );
			menu->textwidth_min = Size.x;
			menu->Init();
		}
		
		virtual void KillContextMenu()
		{
			if (!menu) return;
			toggle = 0;
			menu->Suicide();
			menu = 0;
			Update();
		}
		
		virtual void UpdateGlobalPosHelpers()
		{
			TUI_Button::UpdateGlobalPosHelpers();
			if (menu) menu->SetPos(GPos1+Vec2i(0,Size.y-1));
		}
		
		virtual void AutoSize()
		{
			SetSize( Vec2i(textwidth + 12 + 4 + arrow_down.Size.x, MainFont_Height+12) );	
		}
		
		virtual void OnContextClick( int ID )
		{
			SetContext( ID );
			if (args) delete args;
			args = new Arguments<int>( ID );
			sevents.Call(HOOK_OnClick,args);
		}
		
		virtual void OnClick()
		{
			if (menu) {KillContextMenu();} else OpenContextMenu();
		}
		
		virtual void OnGlobalMouseDown( MouseRes res )
		{
			if (menu && !menu->HasMouse() && !HasMouse()) KillContextMenu();
		}
		
		virtual void TestMouseDist(MouseRes res)
		{
			if (!menu) return;
			if ( DistFromBoarder_Square(GPos1,menu->GPos2,MousePos).dist > closedist) KillContextMenu();
		}
		
};

UI_Hook(HOOK_G_OnMouseDown,TUI_ContextButton,OnGlobalMouseDown);
UI_Hook(HOOK_G_OnMouseMove,TUI_ContextButton,TestMouseDist);
