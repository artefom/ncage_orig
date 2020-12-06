class TUI_MenuPanel : public TUI_Frame
{
	public:
		
		bool autowiwdth;
		bool boarder;
		//Vec2i Padding_top;
		Vec2i Padding_bottom;
		
		TUI_MenuPanel( TUI_ParentLess* parent ) : TUI_Frame(parent)
		{
			autowiwdth = 1;
			boarder = 1;
			
			//Padding_top = 0;
			Padding_bottom = 0;
		};
		
		virtual void UpdateGlobalPosHelpers()
		{
			if (autowiwdth)
			{
				//Position = Vec2i(Padding_top.x,Padding_top.y);
				//Size = Vec2i(Parent->Size.x-Padding_bottom.x-Padding_top.x,Size.y-Padding_top.y-Padding_bottom.y);
				
				Size = Vec2i(Parent->Size.x-Padding_bottom.x,Size.y-Padding_bottom.y);
				
				Update();
				ResizeBuffer(Size);
				OnResize();
			};
			TUI_Frame::UpdateGlobalPosHelpers();
		}
		
		void Init()
		{
			TUI_Button* btn;
			int currentx = 0;
			int maxy = 0;
			for (auto &v : elms) if (v->Size.y > maxy) maxy = v->Size.y;
			SetSize( Vec2i(Size.x,maxy) );
			for (auto &v : elms)
			{
				v->SetBounds( Vec2i(currentx,0), Vec2i(v->Size.x,maxy) );
				currentx+=v->Size.x-1;
				if (v->Size.y > maxy) maxy = v->Size.y;
			};
		}
		
		
		virtual void OnDraw()
		{
			glColor(UI_BG_Medium);
							
			drawrect( 0,Size );
			
			if (boarder) drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
		}
		
};
