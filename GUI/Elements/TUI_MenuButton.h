class TUI_MenuButton : public TUI_Button
{
	public:
		TUI_Menu* menu;
		int closedist;
		
		TUI_MenuButton(TUI_ParentLess* parent) : TUI_Button(parent)
		{
			closedist = 40;
			menu = new TUI_Menu();
			menu->Parent = &UI;
			EnableGlobalHooks();
			//btn->Connect_callback(OpenPaintMenu);
		}
		
		virtual void UpdateGlobalPosHelpers()
		{
			TUI_Button::UpdateGlobalPosHelpers();
			
			menu->SetPos( Vec2i(GPos1.x, GPos2.y-1) );
		}
		
		virtual void OnClick()
		{
			menu->SetVisible( !menu->visible );
		}
		
		virtual void OpenContextMenu()
		{
			menu->SetVisible( true );
		}
		
		virtual void CloseContextMenu()
		{
			menu->SetVisible( false );
		}
		
		virtual void OnGlobalMouseDown( MouseRes res )
		{
			if (menu && !menu->HasMouse() && !HasMouse()) CloseContextMenu();
		}
		
		virtual void TestMouseDist(MouseRes res)
		{
			if (!menu) return;
			if ( DistFromBoarder_Square(GPos1,menu->GPos2,MousePos).dist > closedist) CloseContextMenu();
		}
		
};

UI_Hook(HOOK_G_OnMouseDown,TUI_MenuButton,OnGlobalMouseDown);
UI_Hook(HOOK_G_OnMouseMove,TUI_MenuButton,TestMouseDist);
