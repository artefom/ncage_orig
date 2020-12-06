class TUI_Label : public TUI_Childless
{
	public:
		char* text;
		
		int textheight;
		int textwidth;
		char orientation;
		int textpos;
		
	TUI_Label(TUI_ParentLess* parent ) : TUI_Childless(parent)
	{
		textwidth = 0;
		textpos = 0;
		orientation = 0;
		textheight = MainFont_Height+12;
	}
	
	virtual void SetText( char* itxt)
	{
		text = itxt;
		textwidth = GetTextWidth(text);
		SetOrientation(	orientation );
	};
	
	virtual void OnDraw()
	{
		glColor(UI_Text_Dark);
		drawtext(textpos, Size.y*0.5 + MainFont_Height*0.5, text);
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
		SetOrientation(	orientation );
	};
	
	virtual bool TestMouse( Vec2i mpos )
	{
		return false;	
	};

};
