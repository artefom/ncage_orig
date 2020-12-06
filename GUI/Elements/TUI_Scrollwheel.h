class TUI_Scrollwheel : public TUI_Childless
{
	public:
		float angle;
		bool drag;

		TUI_Scrollwheel(TUI_ParentLess* parent) : TUI_Childless(parent)
		{
			drag = 0;
			angle = 0;
		};
		
		virtual void OnDraw()
		{	
			glColor(UI_BG_Medium);
			drawrect(0,Size);
			int mid = Size.y * 0.5;
			int line;
			
			float d_ang = angle-(int)floor((angle-M_PI_8)/M_PI_4)*M_PI_4;
			glBegin(GL_LINES);
			for ( float ang = d_ang; ang >= -M_PI_2; ang -= M_PI_4 )
			{
				glColor(UI_BG_Bright*cos(ang)+UI_BG_Medium*(1-cos(ang)));
				line = mid-sin(ang)*mid;
				glVertex2f(0,line);
				glVertex2f(Size.x,line);
			};
			glEnd();
			glColor(UI_Outline);
			drawhrect(0,Size-1);
		};
		
		virtual void OnMouseDown(MouseRes res)
		{
			drag=1;
		};
		
		
		virtual void OnMouseUp(MouseRes res)
		{
			drag=0;
		};
		
		virtual void OnMouseMove(MouseRes res)
		{
			if (!drag) return;
			float delta;
			if ( iabs(res.pos.y) > iabs(res.pos.x) ) delta = -(res.pos.y); else
				delta = (res.pos.x)*0.01;
			angle += delta*0.1;
			sevents.Call(HOOK_OnDrag,delta);
			Update();
		};
};

UI_Hook(HOOK_OnMouseDown,TUI_Scrollwheel,OnMouseDown);
UI_Hook(HOOK_OnMouseUp,TUI_Scrollwheel,OnMouseUp);
UI_Hook(HOOK_OnMouseMove,TUI_Scrollwheel,OnMouseMove);
