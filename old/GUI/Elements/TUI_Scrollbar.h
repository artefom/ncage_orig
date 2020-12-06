class TUI_Scrollbar : public TUI_Childless
{
	public:
		bool hold; // is mouse held?
		bool boarder; // draw boarder?
		bool vertical; // vertical?
		float delta; // how much scroller is changed on scroll, size of scroller
		float position;// position of scroller
		int space; // amount of free space above and under scroller
		int scrollerSize; // size of scroller
		int M_dev; // values the mouse deviation when dragging
		int OldSize;
		
	TUI_Scrollbar(TUI_ParentLess* parent) : TUI_Childless( parent )
	{
		hold = 0;
		boarder = 1;
		vertical = 1;
		SetDelta(1);
		position = 0;
		OldSize = 0;
	}
	
	virtual ~TUI_Scrollbar()
	{

	}
	
	virtual void OnResize()
	{
		scrollerSize = Size.y*delta;
		space = Size.y-scrollerSize;
	}
	
	virtual void SetDelta(float d)
	{
		delta = Clamp(d,0,1);
		scrollerSize = Size.y*delta;
		
		OnResize();
	}
	
	virtual void OnDraw()
	{
		glColor(UI_BG_Medium);
		drawrect(0,Size);
		glColor(UI_BG_Light);
		
		int top = space*position;
		int btm = top+scrollerSize;
		drawrect(Vec2i(0,top),Vec2i(Size.x,btm) );
		//if (boarder)
		//{
			//drawbevel(UI_Outline,UI_Outline_Shadow,Vec2i(0,top),Vec2i(Size.x,btm));
			//drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
			//glColor(UI_BG_Dark);
			//drawhrect(0,Size-1);
		//};

		//if (vertical)
		//{
		//}
	}
	
	
	virtual void OnMouseDown(MouseRes res)
	{
		if (res.btn == 3) ScrollUp(); else
		if (res.btn == 4) ScrollDown(); else
			
		if (res.btn == GLUT_LEFT_BUTTON)
		{
			M_dev = MousePos.y-space*position;
			hold = true;
		};
	}
	
	virtual void OnMouseUp(MouseRes res)
	{
		hold = false;
	}

	virtual void OnMouseMove(MouseRes res)
	{
		if (!hold) return;
		SetPosition( (float)(MousePos.y-M_dev)/space );
	}
	
	virtual void SetPosition(float pos)
	{
		pos = Clamp( pos,0,1);
		if (pos == position) return;
		position = pos;
		
		sevents.Call(HOOK_OnScroll);
		
		Update();
	}
	
	virtual void ScrollUp()
	{
		SetPosition(position-delta/space*Size.y*0.33);	
	};
	
	virtual void ScrollDown()
	{
		SetPosition(position+delta/space*Size.y*0.33);
	};

};

UI_Hook(HOOK_OnMouseDown,TUI_Scrollbar,OnMouseDown);
UI_Hook(HOOK_OnMouseUp,TUI_Scrollbar,OnMouseUp);
UI_Hook(HOOK_OnMouseMove,TUI_Scrollbar,OnMouseMove);
