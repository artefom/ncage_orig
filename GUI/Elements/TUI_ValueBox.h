class TUI_ValueBox : public TUI_SimpleFrame
{
	public:
		
		TUI_Scrollwheel* scroller;
		TUI_Edit* edit;
		int btn_width;
		float value;
		float dmult;
		char text[8];
		bool sign;
		bool round;
		TUI_ValueBox( TUI_ParentLess* parent ) : TUI_SimpleFrame( parent )
		{
			btn_width = 10;
			scroller = new TUI_Scrollwheel(this);
			scroller->sevents.Connect(HOOK_OnDrag,this,&TUI_ValueBox::OnDrag);
			scroller->sevents.Connect(HOOK_OnMouseDown,this,&TUI_ValueBox::StartDrag);
			edit = new TUI_Edit(this);
			//edit->dtext_x = 3;
			value = 0;
			dmult = 1;
			sign = 1;
			round = 0;
			edit->sevents.Connect(HOOK_OnLooseFocus,this,&TUI_ValueBox::UpdateEdit);
			edit->sevents.Connect(HOOK_OnEnter,this,&TUI_ValueBox::OnEnter);
			UpdateEdit();
		};
		
		virtual void OnResize()
		{
			scroller->SetBounds(
				Vec2i(Size.x-btn_width,0),
				Vec2i(btn_width,Size.y) 
			);
			edit->SetBounds(
				Vec2i(0,0),
				Vec2i(Size.x-btn_width,Size.y) 
			);
		};
		
		virtual void Increase()
		{
			if (value == 0) value = 1;
			value *= 1.05;
			UpdateEdit();
		};
		
		virtual void Decrease()
		{
			value /= 1.05;
			UpdateEdit();
		};
		
		virtual void UpdateEdit()
		{
			if (round)snprintf(text,8,"%.0f",value); else snprintf(text,8,"%.3f",value);
			edit->SetText(text);
			sevents.Call(HOOK_OnEnter);
		};
		
		virtual void OnEnter()
		{
			sscanf(edit->GetText().c_str(),"%g",&value);
			edit->KeyboardExit();
			UpdateEdit();
		};
		
		virtual void StartDrag(MouseRes res)
		{
			dmult = sqrt(value)*0.05;
			if (dmult < 0.05) dmult = 0.05;
		};
		
		virtual void OnDrag(float delta)
		{
			value += delta*dmult;
			if (!sign && (value < 0)) value = 0;
			UpdateEdit();
		};
		
		virtual void SetValue(float val)
		{
			value =	val;
			if (round)snprintf(text,8,"%.0f",value); else snprintf(text,8,"%.3f",value);
			if (!sign && (value < 0)) value = 0;
			edit->SetText(text);
		};
};
