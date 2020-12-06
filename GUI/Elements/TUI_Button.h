class TUI_Button : public TUI_Childless
{
	public:
		bool hold;
		bool presents;
		bool boarder;
		bool toggle;
		bool light;
		
		string caption;
		int textwidth;
		char orientation;
		int textpos;
		
		IArguments* args;
		SCallbacks scallbacks;
		TBaseEntity** detector;
		
		
	TUI_Button(TUI_ParentLess* parent) : TUI_Childless( parent )
	{
		presents = 0;
		hold = 0;
		textwidth = 0;
		orientation = 0;
		boarder = 1;
		args = 0;
		caption.clear();
		toggle = 0;
		detector = 0;
		light = 0;
		
		SetOrientation(0);
	}
	
	virtual ~TUI_Button()
	{
		if (args) delete args;
	}
	
	virtual void OnDraw()
	{
			IRGB textcolor;
			IRGB bgcolor;
			
			if (toggle)
			{
				textcolor = UI_Text_Dark;
				bgcolor = UI_BG_Dark;
			}
			else
			{
				if (presents)
				{
					if (hold)
					{
						textcolor = UI_Text_Dark;
						bgcolor = UI_BG_Dark;
					} 
					else
					{
						textcolor = UI_Text_Light;
						bgcolor = UI_BG_Light;
					}
				} 
				else
				{
					if (light)
					{
						textcolor = UI_Text_Light;
						bgcolor = UI_BG_Light;
					}
					else
					{
						textcolor = UI_Text;
						bgcolor = UI_BG_Medium;
					};
				};
			};
			
			glColor(bgcolor);
			drawrect(0,Size);
						
			if (boarder) drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
			glColor(textcolor);
			drawtext(textpos, Size.y*0.5 + MainFont_Height*0.5, caption,Size.x-textpos);
	}
	
	virtual void SetOrientation(char orient)
	{
		orientation = orient;
		if (orientation == 0) {textpos = Size.x*0.5 - textwidth*0.5; return;};
		if (orientation == 1) {textpos = Size.x - textwidth-4; return; };
		if (orientation == -1) textpos = 4;
	}
	
	virtual void OnResize()
	{
		SetOrientation(orientation);
	}
	
	virtual void SetCaption(char* text)
	{
		caption = text;
		textwidth = GetTextWidth(text);
		Update();
	}
	
	virtual void SetCaption(string text)
	{
		caption = text;
		textwidth = GetTextWidth(caption.c_str());
		Update();
	}
	
	virtual void AutoSize()
	{
		SetSize( Vec2i(textwidth + 12, MainFont_Height+12) );	
	}
	
	virtual void StretchToBoard()
	{
		SetSize( Vec2i(Parent->Size.x - Position.x*2, MainFont_Height+12) );	
	}
	
	virtual void OnMouseDown(MouseRes res)
	{
		if (res.btn != GLUT_LEFT_BUTTON) return;
		hold = true;
		Update();
	}
	
	virtual void OnMouseUp(MouseRes res)
	{
		if (res.btn != GLUT_LEFT_BUTTON) return;
		if (hold && presents)
		{
			hold = false;
			Update();
			OnClick();
		} else {hold = false; Update();};
	}
	
	virtual void OnMouseEnter()
	{
		presents = 1;
		Update();
	}
	
	virtual void OnMouseExit()
	{
		presents = 0;
		Update();
	}
	
	
	virtual void OnClick()
	{
		CheckValid();
		
		sevents.Call(HOOK_OnClick,args);
		scallbacks.Call(HOOK_OnClick,args);
	}

	template<class T> void SetArgument( T arg )
	{
		args = new Arguments<T>(arg);
	};
	
	virtual void CheckValid()
	{
		if (detector == 0) return;
	};
	
};
//UI_Hook(HOOK_G_OnPreWorldCleanup,TUI_Button,CheckValid);

UI_Hook(HOOK_OnMouseDown,TUI_Button,OnMouseDown);
UI_Hook(HOOK_OnMouseUp,TUI_Button,OnMouseUp);
UI_Hook(HOOK_OnMouseEnter,TUI_Button,OnMouseEnter);
UI_Hook(HOOK_OnMouseExit,TUI_Button,OnMouseExit);

