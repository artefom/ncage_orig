IRGB UI_BG_Dark = IRGB(15,15,15); // used for backgrounds
IRGB UI_BG_Medium = IRGB(20,37,20); // used for backgrou
IRGB UI_BG_Light = IRGB(20,60,20);
IRGB UI_BG_Bright = IRGB(50,120,50);

IRGB UI_BG_Medium2 = IRGB(5,35,30);

IRGB UI_Outline = IRGB(40,80,40);
IRGB UI_Outline_Shadow = IRGB(40,80,40);
IRGB UI_Outline2 = IRGB(90,190,70);
IRGB UI_Outline2_Shadow = IRGB(60,120,30);
IRGB UI_Text = IRGB(230,230,230);
IRGB UI_Text_Medium = IRGB(120,150,50);
IRGB UI_Text_Dark = IRGB(127,127,127);
IRGB UI_Text_Light = IRGB(255,255,255);

IRGB UI_Red = IRGB(100,120,10);
IRGB UI_Red_Light = IRGB(150,190,30);
IRGB UI_Red_Dark = IRGB(15,20,10);

class TUI_Childless;

void SetGlobalMouseGui(TUI_Childless* gui);

class TUI_ParentLess;
class SEvents;
class UIEvents;

class TUI_Childless
{
	private:
		TUI_Childless** ptp; // pointer to an element of parent's array with contains pointer to this. Made 0 after destructor.
	public:
		Vec2i Position;
		Vec2i Size;
		Vec2i GPos1;
		Vec2i GPos2;
		TUI_ParentLess* Parent;
		
		bool visible; // renderable?
		
		bool dead; // tells if the element needs to be deleted
		
		TUI_Childless** globalhooks; // enable global hooks?
		
		SEvents sevents;
		
		//bool* draw_ref; // if tells if frame is drawn (1 - yes; 0 - needs redraw)
						// refers to parent frame's "draw_ref = new bool;"
		
		static UIEvents events; // UIEvents provide sevents system calls
								// you should also check events that a called
								// not through sevents or events (as OnResize() below)
								// these functionas appear to be here, in 
								// TUI_Childless, to be common, to all UI elements
								
		//static Events<TUI_Childless*> events; // you can switch it, seamlessly (almost)
		
		static TUI_Childless* GlobalMouseGui;
		static TUI_Childless* ClickedElement;
		static TUI_Childless* KeyboardElement;
		
		static list<TUI_ParentLess**> Cleanup_Queue;
		static list<TUI_Childless*> Global_Hooks_Storage;
		
		static list<TUI_Childless*> Unused_Storage;
		
		static bool Cleanup_Global_Hooks_Storage;
		static bool Cleanup_Unused_Storage;
	TUI_Childless(TUI_ParentLess* parent);
	
	TUI_Childless()
	{
		cout << "Created without parent!" << endl;
		Position = 0;
		Parent = 0;
		Size = 0;
		visible = 1;
		dead = 0;
		globalhooks = 0;
		SetPTP(0);
	}
	
	virtual ~TUI_Childless()
	{
		if (globalhooks) 
		{
			*globalhooks = 0;
			Cleanup_Global_Hooks_Storage = true;
		}
		if (ptp) *ptp = 0;
		ptp = 0;
		if (ClickedElement == this) ClickedElement = 0;
		if (GlobalMouseGui == this) GlobalMouseGui = 0;
		if (KeyboardElement == this) KeyboardElement = 0;
	}
	
	inline void SetPTP(TUI_Childless** iptp)
	{
		if (iptp == 0)
		{
			Unused_Storage.push_back(this); ptp = &Unused_Storage.back();
		} else
		ptp = iptp;
	};
		
	virtual Vec2i GetPos()
	{
		return Position;
	}
	
	virtual Vec2i GetSize()
	{
		return Size;
	}
	
	virtual void SetVisible(bool iv);
	
	virtual void SetSize(Vec2i size)
	{
		Size = size;
		UpdateGlobalPosHelpers();
		OnResize();
		sevents.Call(HOOK_OnResize);
	}
	
	virtual void UpdateGlobalPosHelpers()
	{
		GPos1 = GetGlobalPos();
		GPos2 = GPos1+Size;
	}
	
	virtual void SetPos(Vec2i pos)
	{
		Position = pos;
		UpdateGlobalPosHelpers();
	}
	
	virtual void SetBounds( Vec2i pos, Vec2i size)
	{
		Position = pos;
		Size = size;
		UpdateGlobalPosHelpers();
		OnResize();
		sevents.Call(HOOK_OnResize);
	}
	
	virtual bool IsMouseIn(Vec2i mpos)
	{
		if ((Size.x == 0) || (Size.y == 0)) return 0;
		return (mpos.x > GPos1.x) && (mpos.x < (GPos2.x))
					&& (mpos.y > GPos1.y) && (mpos.y < (GPos2.y));
	}
	
	virtual bool TestMouse(Vec2i mpos)
    {
    	if ( !IsMouseIn(mpos) || dead) return false;
		SetGlobalMouseGui( this );
    	return true;
    }
    
    virtual bool HasMouse();
    
    virtual void Update();
    
	virtual Vec2i GetGlobalPos();
	

	virtual void Draw()
	{
		glPushMatrix();
		glTranslate( GetPos() );
		OnDraw();
		glPopMatrix();
	}
	
	virtual void OnDraw()
	{
		
	}
	
	virtual void Tick()
	{
		
	}
	
	virtual int GetCursor()
	{
		return GLUT_CURSOR_LEFT_ARROW;	
	};
	
	virtual void OnResize()
	{

	}
	
	virtual void Suicide();

	virtual void EnableGlobalHooks()
	{
		Global_Hooks_Storage.push_back(this);	
		globalhooks = &Global_Hooks_Storage.back();
	}
	
	/*
	virtual void DisableGlobalHooks() // unsupported because cleanup global hooks storage is done only on deletes
	{
		globalhooks = false;
		Cleanup_Global_Hooks_Storage = true;
	}
	*/
	
	static void CleanupUnusedStorage()
	{
		auto it = Unused_Storage.begin();
		while (it != Unused_Storage.end())
		{
			if (*it == 0) it = Unused_Storage.erase(it); else ++it;	
		};
	};
	static void CleanupGlobalHooksStorage() // only called on elements remove
	{
		if (!Cleanup_Global_Hooks_Storage) return;
		Cleanup_Global_Hooks_Storage = 0;
		auto it = Global_Hooks_Storage.begin();
		while (it != Global_Hooks_Storage.end())
		{
			if ( !(*it) )
			{
				//cout << "CLEANED GLOBAL HOOK! " << endl;
				it = Global_Hooks_Storage.erase(it);
			} else it++;
		}
	}
	
	static void SetKeyboardElement( TUI_Childless* elm );
	
};

TUI_Childless* TUI_Childless::GlobalMouseGui = 0;
TUI_Childless* TUI_Childless::ClickedElement = 0;
TUI_Childless* TUI_Childless::KeyboardElement = 0;
list<TUI_ParentLess**> TUI_Childless::Cleanup_Queue;

list<TUI_Childless*> TUI_Childless::Global_Hooks_Storage;
list<TUI_Childless*> TUI_Childless::Unused_Storage;
bool TUI_Childless::Cleanup_Global_Hooks_Storage = 0;
bool TUI_Childless::Cleanup_Unused_Storage = 0;



bool cur_update = 1;
void UpdateMouseCursor()
{
	cur_update = 1;
}

#include <UIEvents.h>


UIEvents TUI_Childless::events; // you can switch it, seamlessly (almost)
UIEvents &ui_events = TUI_Childless::events; // UIEvents provide sevents system calls

//Events<TUI_Childless*> TUI_Childless::events;
//Events<TUI_Childless*> &ui_events = TUI_Childless::events;

void CursorUpdate()
{
	cur_update = 0;
	if (TUI_Childless::ClickedElement != 0)
	{
		glutSetCursor(TUI_Childless::ClickedElement->GetCursor());
		return;
	}
	
	if (TUI_Childless::GlobalMouseGui != 0) glutSetCursor(TUI_Childless::GlobalMouseGui->GetCursor());
	else glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
};

void SetGlobalMouseGui(TUI_Childless* ui)
{
	if (TUI_Childless::GlobalMouseGui != ui)
	{
		if (TUI_Childless::GlobalMouseGui != 0) ui_events.Call(TUI_Childless::GlobalMouseGui,HOOK_OnMouseExit);
		ui_events.Call(ui,HOOK_OnMouseEnter);
		TUI_Childless::GlobalMouseGui = ui;
		CursorUpdate();
	};
};


//#define UI_Hook(name,class,method) STATIC_EXECUTE{ ui_events.Connect(name,static_cast<class*>(0),&class::method); };
#define UI_Hook(name,class,method) namespace UI##name##class##method { static const bool &o = (ui_events.Connect(name,static_cast<class*>(0),&class::method),false); }

UI_Hook(HOOK_Draw,TUI_Childless,Draw);

//############################# INCLUDE USER INTERFACE #####################

