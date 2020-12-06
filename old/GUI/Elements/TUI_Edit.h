class TUI_Edit : public TUI_Childless
{
	private:
		string text;
		Vec2i textpos;
		vector<int> textposes;
		int cursor_pos;
		int selcursor_pos;
		bool cursor_visible;
		bool active;
		char counter;
		bool changed;
		bool selecting;
		bool hold;
		
		string context_text_save;
		TUI_MenuContext_Simple* context_menu;
		int context_id;
	public:
	TUI_Edit(TUI_ParentLess* parent) : TUI_Childless(parent)
	{
		textpos = Vec2i(5,5+MainFont_Height);
		cursor_pos = 0;
		selcursor_pos = 0;
		cursor_visible = 0;
		active = 0;
		counter = 0;
		changed = 1;
		selecting = 0;
		context_id = -1;
		hold = 0;
		context_menu=0;
	};
	
	~TUI_Edit()
	{
		if (context_menu) context_menu->Suicide();
	};
	
	virtual void OnDraw()
	{
		glColor(UI_BG_Medium);
		drawrect(0,Size);
		if (!active) // draw if inactive
		{
			glColor(UI_Text);
			drawtext(textpos,text);
			drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
			return;
		};
		int curpos = textpos.x;
		selcursor_pos = clamp(selcursor_pos,0,text.size());
		cursor_pos = clamp(cursor_pos,0,text.size());
		if (changed || (textposes.size() != text.size()) ) // Update text poses
		{
			textposes.clear();
			textposes.shrink_to_fit();
			textposes.reserve(text.size()+1);
			for (int i = 0; i <= text.size(); ++i) {textposes.push_back(curpos); curpos += glutBitmapWidth(MainFont, text[i]); };
			changed = false;
		};
		if (IsSelected()) // Draw selection
		{
			glColor(UI_BG_Bright);
			drawrect( Vec2i(textposes[selcursor_pos],0) , Vec2i(textposes[cursor_pos],Size.y) );
		}
		if (cursor_visible == true) // draw cursor lines
		{
			glColor(UI_Text);
			glBegin(GL_LINES);
			glVertex2f(textposes[cursor_pos],0);
			glVertex2f(textposes[cursor_pos],Size.y);
			glEnd();
		};
		glColor(UI_Text);
		drawtext(textpos,text);
		drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
	};
	
	virtual void KeyboardExit()
	{
		active = 0;
		cursor_visible = false;
		counter = 0;
		sevents.Call(HOOK_OnLooseFocus);
		Update();
	};
	
	virtual void KeyboardEnter()
	{
		active = true;
		cursor_visible = true;
		counter = 0;
		sevents.Call(HOOK_OnGainFocus);
		Update();
	}
	
	virtual void Tick()
	{
			if ((active) && (++counter >= 3)) 
			{cursor_visible = !cursor_visible; counter = 0; Update(); };
	}
	
	virtual void OnMouseDown()
	{
		SetKeyboardElement(this);
		SetCursor( ScreenToCursor(MousePos) );
		selecting = true;
		hold = true;
	};
	
	
	virtual void OnMouseUp()
	{
		selecting = false;
		hold = false;
	};
	
	virtual void OnMouseMove()
	{
		if (hold) SetCursor(ScreenToCursor(MousePos));
	};
	
	virtual void OnDoubleClick()
	{

	};
	
	virtual int ScreenToCursor( Vec2i pos )
	{
		if (text.size() == 0) return 0;
		int curpos = GPos1.x+textpos.x+glutBitmapWidth(MainFont, text[0]);
		int ch = 0;
		while(curpos<pos.x)
		{
			++ch;
			curpos+=glutBitmapWidth(MainFont, text[ch]);
			if (ch == text.size()) break;
		};
		if ((ch < text.size()) && (pos.x>curpos-glutBitmapWidth(MainFont, text[ch])*0.5)) ch += 1;
		return ch;
	};
	
	virtual void SetCursor(int pos)
	{
		if (pos < 0) cursor_pos = 0; else 
		if (pos > text.size()) cursor_pos = text.size(); else
		cursor_pos = pos;
		GetAsyncKeyState(0x10);
		if ((!selecting) && !GetAsyncKeyState(0x10)) selcursor_pos = cursor_pos;
		cursor_visible = true;
		counter = 0;
		Update();
	};
	
	virtual void SetSelectionCursor(int pos)
	{
		if (pos < 0) selcursor_pos = 0; else
		if (pos > text.size()) selcursor_pos = text.size(); else
		selcursor_pos = pos;	
	};
	virtual void OnKeyDown( unsigned char key )
	{
		if (isprint(key))
		{
			clearSelection();
			insert(cursor_pos, key);
			return;
		};
		if ( key == 13 ) {sevents.Call(HOOK_OnEnter);};
		if ( key == 8 ) if (IsSelected()) clearSelection(); else erase(cursor_pos-1);		
	};
	
	virtual void OnKeySpecialDown( int key )
	{
		if (key == 100) SetCursor(cursor_pos-1);  // left
		if (key == 102) SetCursor(cursor_pos+1);// right
		if ( key == 101 ) SetContext(context_id-1); // up
		if ( key == 103 ) SetContext(context_id+1); // down
		Update();
	}
	
	void clearSelection()
	{
		if (!IsSelected()) return;
		int first = min(cursor_pos,selcursor_pos);
		int len = max(cursor_pos,selcursor_pos)-first;
		erase(first,len);
	};
	
	inline void insert(int pos, char ch)
	{
		if (!isprint(ch) || (pos < 0) || (pos > text.size())) return;
		if (cursor_pos >= pos) cursor_pos+=1;
		if (selcursor_pos >= pos) selcursor_pos+=1;
		text.insert(text.begin()+pos,ch);
		OnTextChange();
	};
	
	inline void push_back(char ch)
	{
		if (cursor_pos == text.size()) cursor_pos+=1;
		if (selcursor_pos == text.size()) selcursor_pos+=1;
		text += ch;
		OnTextChange();
	};
	
	inline void clear()
	{
		cursor_pos = 0;
		selcursor_pos = 0;
		text.clear();
		OnTextChange();
	};
	
	inline void erase( int pos )
	{
		if ((pos < 0) || (pos >= text.size())) return;
		if (cursor_pos < pos) cursor_pos-=1;
		if (selcursor_pos < pos) selcursor_pos-=1;
		text.erase(text.begin()+pos);
		OnTextChange();
	};
	
	inline void erase( int pos, int len )
	{
		if ((pos < 0) || (pos >= text.size()) || (len < 0)) return;
		if (cursor_pos > pos)
			if (cursor_pos < pos+len) cursor_pos = pos; else cursor_pos -= len;
		if (selcursor_pos > pos)
			if (selcursor_pos < pos+len) selcursor_pos = pos; else selcursor_pos -= len;
		text.erase(pos,len);
		OnTextChange();
	};
	
	inline const string& GetText()
	{
		return text;
	};
	
	inline void SetText(string itext)
	{
		text = itext;
		if (cursor_pos < text.size()) cursor_pos = text.size();
		if (selcursor_pos < text.size()) selcursor_pos = text.size();
		OnTextChange();
	};
	
	bool IsSelected()
	{
		return (selcursor_pos!=cursor_pos);
	};
	
	virtual void SetContext(int id)
	{
		if (context_menu == 0) {context_id = -1; return;};
		context_id = context_menu->ChoseID(id);
		const char* txt = context_menu->ChosenText();
		if (txt == 0)
		{
			if (context_text_save.size() != 0)
			{
				text = context_text_save;
				SetCursor(text.size());
			};
		}else
		{
			if (context_text_save.size() == 0) context_text_save = text;
			text = txt;
			SetCursor(text.size());
			changed = true;
		};
	};
	
	virtual void AddContext(string text)
	{
		if (context_menu == 0)
		{
			context_menu = new TUI_MenuContext_Simple(&UI);
			context_menu->sevents.Connect(HOOK_OnClick,this,&TUI_Edit::SetContext);
			context_menu->SetPos(GPos1+Vec2i(0,Size.y-1));
		};
		context_menu->AddButton(text);
	};
	
	virtual void ClearContext()
	{
		if (context_menu != 0) context_menu->Suicide();
		context_menu = 0;
		context_id=-1;
		context_text_save.clear();
	};
	
	virtual void UpdateGlobalPosHelpers()
	{
		TUI_Childless::UpdateGlobalPosHelpers();
		if (context_menu) context_menu->SetPos(GPos1+Vec2i(0,Size.y-1));
	}
	
	virtual void OnTextChange()
	{
		context_text_save.clear();
		changed = true;
		Update();
		sevents.Call(HOOK_OnChange);
	};
};

UI_Hook(HOOK_OnKeyboardExit,TUI_Edit,KeyboardExit);
UI_Hook(HOOK_OnKeyboardEnter,TUI_Edit,KeyboardEnter);

UI_Hook(HOOK_OnMouseDown,TUI_Edit,OnMouseDown);
UI_Hook(HOOK_OnMouseUp,TUI_Edit,OnMouseUp);
UI_Hook(HOOK_OnMouseMove,TUI_Edit,OnMouseMove);
UI_Hook(HOOK_OnDoubleClick,TUI_Edit,OnDoubleClick);

UI_Hook(HOOK_OnKeyDown,TUI_Edit,OnKeyDown);
UI_Hook(HOOK_OnKeySpecialDown,TUI_Edit,OnKeySpecialDown);
