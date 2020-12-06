class TUI_ScrollFrame : public TUI_SimpleFrame
{
	public:
		TUI_Scrollbar* Scrollbar;
		TUI_SimpleFrame* Container;

		TUI_ScrollFrame(TUI_ParentLess* parent) : TUI_SimpleFrame(parent)
		{
		
			Container =  new TUI_SimpleFrame(this);
			Scrollbar =  new TUI_Scrollbar(this);
			Scrollbar->SetSize(4);
			Scrollbar->sevents.Connect( HOOK_OnScroll, this, &TUI_ScrollFrame::OnScroll);
			Scrollbar->SetDelta(1);			
			Container->SetBounds( Vec2i(0,0), Vec2i(Size.x-Scrollbar->Size.x,0) );
			EnableGlobalHooks();
		}
		
		virtual void OnResize()
		{
			Container->SetSize( Vec2i(Size.x-Scrollbar->Size.x,Container->Size.y) );
			Scrollbar->SetDelta( (float)Size.y/Container->Size.y );
			Scrollbar->SetBounds( Vec2i(Size.x - Scrollbar->Size.x,0), Vec2i(Scrollbar->Size.x,Size.y) );
			OnScroll();
		};
		
		virtual void OnScroll()
		{
			if (Size.y >= Container->Size.y) Container->SetPos( Vec2i( 0 , 0) ); else
			Container->SetPos( Vec2i( 0 ,(Size.y-Container->Size.y) * Scrollbar->position) );
			TestMouse(MousePos);
		}
		
		virtual void SetVirtualSize(int size)
		{
			int oldtopdist = Container->Position.y;
			Container->SetSize( Vec2i(Container->Size.x,size) );
			Scrollbar->SetDelta( (float)Size.y/Container->Size.y ); // dont wory its clamped atomaticaly
			Scrollbar->SetPosition((float)oldtopdist/(Size.y-Container->Size.y));
		}
		
		virtual void AutoSize()
		{
			int maxy = Container->GetMaxSize().y;
			SetVirtualSize(maxy + 20);
		};
		
		virtual void OnMouseDown_G(MouseRes res)
		{
			//if ( !IsMouseIn(res.pos) ) return;
			if ( !HasMouse() ) return;
			if (res.btn == 3) Scrollbar->ScrollUp();
			if (res.btn == 4) Scrollbar->ScrollDown();
		}
};

UI_Hook(HOOK_G_OnMouseDown,TUI_ScrollFrame,OnMouseDown_G);
