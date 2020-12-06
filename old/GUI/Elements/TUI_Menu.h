class TUI_Menu : public TUI_SimpleFrame
{
	public:
		int textwidth_max;
		int textwidth_min;
		int closedist;
		
		
		TUI_Menu(TUI_ParentLess* parent) : TUI_SimpleFrame( parent )
		{
			textwidth_max = 0;
			textwidth_min = 10;
			closedist = 0;
		};
		
		TUI_Menu() : TUI_SimpleFrame()
		{
			textwidth_max = 0;
			textwidth_min = 10;
			closedist = 0;
			visible = 0;
		};
		
		virtual void Init()
		{
			int maxx = 0;
			int cury = 0;
			for (auto &v : elms)
				if (v->Size.x > maxx) maxx = v->Size.x;

			for (auto &v : elms)
			{
				v->SetBounds( Vec2i(0,cury),Vec2i(maxx,v->Size.y) );
				cury += v->Size.y-1;
			}
			
			SetSize(Vec2i(maxx,cury));
		}
		
		virtual TUI_Button* AddButton(char* caption)
		{
			TUI_Button* btn = new TUI_Button(this);
			btn->SetCaption(caption);
			btn->AutoSize();
			Init();
			return btn;
		}

		
		virtual void OnGlobalMouseUp( MouseRes res )
		{
			Suicide();
		}
		
		
		virtual void EnableCloseOnPick()
		{
			EnableGlobalHooks();
		}
		
		virtual void OnDraw()
		{
			//glColor(UI_BG_Dark);
			//drawrect( 0,Size );
			drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
		}
};

UI_Hook(HOOK_G_OnMouseUp,TUI_Menu,OnGlobalMouseUp);


