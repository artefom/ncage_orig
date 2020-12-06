class TUI_Button_Colorable : public TUI_Button
{
	public:
		IRGB* color_normal;
		IRGB* color_active;
		IRGB* color_pressed;
		IRGB* color_text_normal;
		IRGB* color_text_pressed;
		
	TUI_Button_Colorable(TUI_ParentLess* parent) : TUI_Button(parent)
	{
		color_normal = &UI_Red;
		color_active = &UI_Red_Light;
		color_pressed = &UI_Red_Dark;
		color_text_normal = &UI_Text;
		color_text_pressed = &UI_Text_Dark;
	}
		
	virtual void OnDraw()
	{
		IRGB textcolor;
		if (presents) 
		{
			if (hold) glColor(UI_Red_Dark); else glColor(UI_Red_Light);
		} else glColor(UI_Red);
		drawrect(0,Size);
		if (boarder) drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
		
		if (hold && presents) glColor(UI_Text_Dark); else glColor(UI_Text);
		drawtext(textpos, Size.y*0.5 + MainFont_Height*0.5, caption);
		
	}
};

class TUI_WFrame : public TUI_Frame
{
	public:
		
	char* caption;
	char bstyle;
	
	TUI_WFrame(TUI_ParentLess* parent) : TUI_Frame(parent)
	{
		caption = "Window";	
		bstyle = 0;
	}
		
	virtual void SetWindowSize(Vec2i size)
	{
		Parent->SetSize(size);
	}
	
	virtual void SetWindowPos(Vec2i pos)
	{
		Parent->SetPos(pos);	
	}

};

class TUI_Window : public TUI_SimpleFrame
{
	public:
	Vec2i Padding_top;
	Vec2i Padding_bottom;
	Vec2i Size_min;
	Vec2i mouse_offset;
	bool dragging;
	char scalling;
	int cursor_type;

	TUI_Button_Colorable* CloseButton;
	TUI_WFrame* Frame;
	
	TUI_Window(TUI_ParentLess* parent) : TUI_SimpleFrame(parent)
	{
		dragging = false;
		scalling = false;
		//EnableBuffering();
		CloseButton = new TUI_Button_Colorable(this);
		CloseButton->SetCaption("X");
		CloseButton->sevents.Connect(HOOK_OnClick, this, & TUI_Window::OnExit );
		LoadFrame(new TUI_WFrame(this));
		
		cursor_type = GLUT_CURSOR_LEFT_ARROW;
	}
	
	TUI_Window() : TUI_SimpleFrame(&UI)
	{
		dragging = false;
		scalling = false;
		//EnableBuffering();
		CloseButton = new TUI_Button_Colorable(this);
		CloseButton->SetCaption("X");
		CloseButton->sevents.Connect( HOOK_OnClick, this, & TUI_Window::OnExit );
		
		cursor_type = GLUT_CURSOR_LEFT_ARROW;
	}
	
	virtual void LoadFrame( TUI_WFrame* frame )
	{
		Frame = frame;
		UpdateBoatderStyle();
		SetBounds(Frame->Position-Padding_bottom, Frame->Size+Padding_bottom+Padding_top);
	};
	
	virtual void OnFrameSizeChange()
	{
		SetBounds(Frame->Position-Padding_bottom, Frame->Size+Padding_bottom+Padding_top);
	}
	
	void OnResize()
	{
		CloseButton->SetSize( Vec2i(Padding_top.y*1.5,Padding_top.y*0.8) );
		CloseButton->SetPos( Vec2i(GetSize().x - CloseButton->GetSize().x - Padding_bottom.x, 0) );
		Frame->SetBounds( Padding_top, Size-Padding_bottom-Padding_top );
	}
	
	
	virtual void OnDraw()
	{
		glColor(UI_BG_Medium,0.5);
		drawrect(0,Size);
		drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
		drawbevel(UI_Outline,UI_Outline_Shadow,Frame->GetPos()-1, Frame->GetPos()+Frame->GetSize()+1 );
		glColor(UI_Text,1);
		drawtext(Padding_top.x,Padding_bottom.y+MainFont_Height,Frame->caption,Size.x-Padding_top.x);
	}
	
	virtual char TestDrags(Vec2i pos)
	{
		if (!Frame->bstyle) return 0;
		char result = 0;
		
		if (Frame->bstyle & 1)
		{
			if (pos.x < Padding_top.x*2) result = result | 1;
			if (pos.x > GetSize().x-Padding_bottom.x*2) result = result | 4;
		};
		if (Frame->bstyle & 2)
		{
    		if (pos.y < Padding_bottom.y*2) result = result | 2;
    		if (pos.y > GetSize().y-Padding_bottom.y*2) result = result | 8;
    		if ( (result == 2) && (pos.y >= Padding_bottom.y) ) result = 0;
    	}
    	return result;
	}
	
    virtual void OnMouseDown(MouseRes res)
    {
		scalling = TestDrags(res.pos);
    	if (scalling) 
    	{
    		Frame->StartScaling();
			mouse_offset = GPos2-MousePos;
		} else 
		{
			if (res.pos.y < Padding_top.y) dragging = true;
		}
    }
    
    virtual void OnMouseUp(MouseRes res)
    {
    	if (dragging | scalling) glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    	dragging = false;
    	scalling = false;
    	Frame->StopScaling();
    }
	
	virtual void OnMouseMove(MouseRes res)
	{
		UpdateMouseCursor();
		if (dragging) {SetPos(GetPos() + res.pos ); return;};
		if (scalling)
		{
			Vec2i pos = MousePos-Size+mouse_offset;
			if ( !(scalling & 1) ) pos.x = GPos1.x;
			if ( !(scalling & 2) ) pos.y = GPos1.y;		
			if (pos.x > (GPos2.x-Size_min.x) ) pos.x = GPos2.x-Size_min.x;
			if (pos.y > (GPos2.y-Size_min.y) ) pos.y = GPos2.y-Size_min.y;
			
			Vec2i size = MousePos-pos+mouse_offset;
			if ( !(scalling & 4) ) size.x = (GPos2.x-pos.x);
			if ( !(scalling & 8) ) size.y = (GPos2.y-pos.y);
			if (size.x < Size_min.x) size.x = Size_min.x;
			if (size.y < Size_min.y) size.y = Size_min.y;
			
			mouse_offset += GPos1;
			SetBounds(pos,size);
			mouse_offset -= GPos1;
			return;
		}
		char drag_test = TestDrags(MousePos-GPos1);
					
					
		if ( ((drag_test & 3) == 3) || ((drag_test & 12) == 12) ) { cursor_type = GLUT_CURSOR_BOTTOM_RIGHT_CORNER; return;};
		if ( ((drag_test & 6) == 6) || ((drag_test & 9) == 9) ) { cursor_type = GLUT_CURSOR_BOTTOM_LEFT_CORNER; return;};
		
		if (drag_test & 10) {cursor_type = GLUT_CURSOR_UP_DOWN; return;};
		if (drag_test & 5) { cursor_type = GLUT_CURSOR_LEFT_RIGHT; return;};
		cursor_type = GLUT_CURSOR_LEFT_ARROW;
	}
	
	virtual int GetCursor()
	{
		return cursor_type;
	}
	
	virtual void OnMouseExit()
	{
		if (!(dragging | scalling) ) UpdateMouseCursor();
	};
	
	virtual void OnExit()
	{
		Suicide();
	}
	
	virtual void UpdateBoatderStyle()
	{
		Padding_top = Vec2i(3,24);
		Padding_bottom = 3;
		if (Frame->bstyle & 1)
		{
			Padding_top.x = 6;
			Padding_bottom.x = 6;
		}
		
		if (Frame->bstyle & 2) Padding_bottom.y = 6;
		Size_min = Vec2i( Padding_top.y*1.5+Padding_top.x+Padding_bottom.x, Padding_top.y+Padding_bottom.y );
	};
	
	virtual void SetBoarderStyle( char bs )
	{
		Frame->bstyle = bs;
		Padding_top = Vec2i(3,24);
		Padding_bottom = 3;
		if (bs & 1)
		{
			Padding_top.x = 6;
			Padding_bottom.x = 6;
		}
		
		if (bs & 2) Padding_bottom.y = 6;
		Size_min = Vec2i( Padding_top.y*1.5+Padding_top.x+Padding_bottom.x, Padding_top.y+Padding_bottom.y );
		OnResize();
		//Size_min = Vec2i( Padding_top.y*1.5+Padding_top.x+Padding_bottom.x, Padding_top.y+Padding_bottom.y );
		//Padding_top = Vec2i(6,24);
		//Padding_bottom = 6;
	};
	
	virtual void SetCaption( char* caption )
	{
		caption = caption;
	};

};

UI_Hook(HOOK_OnMouseDown,TUI_Window,OnMouseDown);
UI_Hook(HOOK_OnMouseUp,TUI_Window,OnMouseUp);
UI_Hook(HOOK_OnMouseMove,TUI_Window,OnMouseMove);

UI_Hook(HOOK_OnMouseExit,TUI_Window,OnMouseExit);

Vec2i NextWindowPos = 50;

template<class T> T* UI_CreateWindow()
{
	TUI_Window* Window = new TUI_Window;
	Window->LoadFrame( new T(Window) );
	Window->SetPos(NextWindowPos);
	NextWindowPos+=50;
	if (NextWindowPos.x > 600) NextWindowPos = 50;
	return reinterpret_cast<T*>(Window->Frame);	
}

template<class T, class A1> T* UI_CreateWindow(A1 arg)
{;
	TUI_Window* Window = new TUI_Window;
	Window->LoadFrame( new T(Window,arg) );
	Window->SetPos(NextWindowPos);
	NextWindowPos+=50;
	if (NextWindowPos.x > 600) NextWindowPos = 50;
	return reinterpret_cast<T*>(Window->Frame);
}

template<class T> void OpenWindow()
{
	UI_CreateWindow<T>();
}


