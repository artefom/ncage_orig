const int World_Bounds = 1024;

//unsigned char (*pboMemory)[World_Bounds][World_Bounds][World_Bytes_PerPixel];

IRGBA ColorIndexes[256];
int last_color_index = 1;
unsigned char ReserveColor( IRGBA col )
{
	if (last_color_index >= 256) return 0;
	ColorIndexes[last_color_index] = col;
	++last_color_index;
	return last_color_index-1;
}
#define MColorID static unsigned char colorID; static int type_id
#define DefaultColor(class,r,g,b)	\
unsigned char class::colorID = ReserveColor( IRGBA(r,g,b,255) );

#include <thread>

#include <TWorld_Base.h>

#define GLUT_DRAG_BUTTON GLUT_MIDDLE_BUTTON
class TWorld : public TWorld_Base
{
	//private:
	//	Events<TBaseEntity*> events;
    public:
        
    TWorld()
	{

	};        
	
	virtual void Init()
	{
		dummy = new TBaseEntity();
	    gridsize = 16;
        highlighteach=4;
        ColorIndexes[0] = IRGBA(50,50,50,255);

        gridcolor = IRGB(200,200,200);
        highlightcolor = IRGB(200,200,200);
        grid_min_scale = 16;
        
        SCALE = 0.5;
        SCALE_MIN = 0.01; SCALE_MAX = 5; SCALE = 1;
        
        SetTrans(Vector2d(100,100));
        absgridsize = gridsize*SCALE;
        overlap1 = -0.1;
        overlap2 = 1.1;
        total_zeros = 0;
        TWorld_Base::Init();
        
        glutSetColor(255,200, 100, 200);
        
        Render();
	}
	
	
	virtual bool IsInBounds( Vec2i vec )
	{
		if ((vec.x < 0) || (vec.y < 0) || (vec.x > bounds_max) || (vec.y > bounds_max)) return false;
		return true;
	};
	
	inline void Checkbounds( Vec2i cell )
	{
		if ((cell.x < 0) || (cell.x >= bounds)) {cout << "World out of bounds!" << endl; exit(0);};
    	if ((cell.y < 0) || (cell.y >= bounds)) {cout << "World out of bounds!" << endl; exit(0);};	
	}
	
	inline TBaseEntity** GetCell( Vec2i cell )
    {
    	//Checkbounds(cell);
    	return &Cell[cell.x][cell.y];
    }
    
    inline TBaseEntity* GetCellContent( Vec2i cell )
    {
    	//Checkbounds(cell);
    	return	Cell[cell.x][cell.y];
    }
    
    inline void SetCell(Vec2i cell, TBaseEntity* ent)
    {
    	//Checkbounds(cell);
    	Cell[cell.x][cell.y] = ent;
    }
    
    inline void SetCell(TBaseEntity** cell, TBaseEntity* ent)
    {
    	//Checkbounds(cell);
    	*cell = ent;
    }
    
    
    void Think();
    
    virtual void Tick()
    {
    	if (Simulating == 0) return;
    	
    	Cleanup();
    }
    
    
    inline void Clear(Vec2i pos)
    {
    	Cell[pos.x][pos.y] = 0;
	}
	
	int total_zeros;
	void Cleanup()
    {
    	//events.Call(HOOK_G_OnPreWorldCleanup);
    	ui_events.SingleCall(&TUI_Childless::Global_Hooks_Storage,HOOK_G_OnPreWorldCleanup);
    	int size_before = thinkers.size();
    	TBaseEntity** ptr = &thinkers[0];
    	TBaseEntity** end = &thinkers[0]+thinkers.size();
    	while( ptr != end )
		{
			if (ptr == 0)
			{
				cout << "PTR IZ 0!" << endl;
				return;	
			};
			if (*ptr == dummy)
			{
				*ptr = thinkers.back();
				(*ptr)->ID = ptr-&thinkers[0];
				thinkers.pop_back();
				--end;
			} else
			
			if (*ptr == 0)
			{
				++total_zeros;
				cout << "cleaned 0! total: " << total_zeros << endl;
				
				*ptr = thinkers.back();
				if (*ptr) (*ptr)->ID = ptr-&thinkers[0];
				thinkers.pop_back();
				--end;
			} else ++ptr;
		};
		
		//events.Call(HOOK_G_OnPostWorldCleanup);
    	//ui_events.Call(HOOK_G_OnPostWorldCleanup);
		//cout << "current size:		" << thinkers.size() << "/		" << thinkers.capacity() << " Cleaned	" << size_before-thinkers.size() << " thinkers" << endl;
    }
    
         
    void OnMouseDown(int x, int y, int btn)
	{
    	if (!hasmouse) return;
    	
        if ( btn == GLUT_DRAG_BUTTON ) 
		{
			dragging = true;
			return;
		}
		if (btn == GLUT_LEFT_BUTTON)
		{
			if ( (MouseCell != 0) && (*MouseCell != 0) )
			{
				PressedEnt = MouseCell;
				events.Call(*MouseCell,HOOK_OnMouseDown);
			};
			return;	
		}
		
	    if (btn == 3) {RESCALE(1.1); return;};
        if (btn == 4) {RESCALE(0.9); return;};
		
    };
    void OnMouseUp(int x,int y, int btn)
	{
    	if ( (MouseCell != 0) && (*MouseCell != 0) )
    	{
    		if ( PressedEnt && (PressedEnt == MouseCell) )
    		{	
				events.Call(*PressedEnt,HOOK_OnClick);
			};
			events.Call(*MouseCell,HOOK_OnMouseUp);
		}
		else
		{
			if (PressedEnt)
			{
				events.Call(*PressedEnt,HOOK_OnMouseUp);	
			}
		}
		PressedEnt = 0;
			
        if (btn == GLUT_DRAG_BUTTON) dragging = false;    
    };
    
    void OnMouseMove(Vec2i delta){
        if (!dragging) return;
        Move(delta);
    };
    
    void TestMouse(Vec2i mpos)
    {
    	hasmouse = true;
    	TBaseEntity** Cell = GetCell( ScreenToCell(MousePos) );
		SetMouseCell(Cell);
    }

	void SetMouseCell(TBaseEntity **Cell) // im sure *Cell != 0
	{
		if ( (MouseCell != Cell) )
    	{
    		if ( (MouseCell != 0) && (*MouseCell != 0) )
    		{
    			events.Call(*MouseCell,HOOK_OnMouseExit);
    		}
    		if ( (Cell != 0) && (*Cell != 0) ) {
    			events.Call(*Cell,HOOK_OnMouseEnter);
    		}
    		MouseCell = Cell;
		}
	}	    
};

TWorld World;

Events<TBaseEntity*> &events = World.events;
int begin_size;
void thinkers_seg(TBaseEntity** begin, TBaseEntity** end)
{
	for (TBaseEntity** ptr = begin; ptr != end; ++ptr)
	{
		if (World.thinkers.capacity() != begin_size) {cout << "Rellocated on think" << endl; return;};
		if (*ptr == 0) cout << "ptr iz 0! " << endl; else (*ptr)->Think();	
	};
};
		
void printthread()
{
	cout << "threading!" << endl;	
};
void TWorld::Think()
{
	if (Simulating == 0) return;
  	//ui_events.Call(	&TUI_Childless::Global_Hooks_Storage,HOOK_G_Think);  
  	
	
  	begin_size = thinkers.capacity();
	auto part1 = &thinkers[0];
	auto part2 = &thinkers[0]+(int)(thinkers.size()*0.25);
	auto part3 = &thinkers[0]+(int)(thinkers.size()*0.5);
	auto part4 = &thinkers[0]+(int)(thinkers.size()*0.75);
	auto part5 = &thinkers[0]+(int)(thinkers.size()*1.00);
	
	thinkers_seg(part1,part5); return;
	thread thread1(thinkers_seg,part1,part2);
	thread thread2(thinkers_seg,part2,part3);
	thread thread3(thinkers_seg,part3,part4);
	thread thread4(thinkers_seg,part4,part5);
	
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
}
#define Hook(name,class,method) namespace name##class##method { static const bool &o = (events.Connect(name,static_cast<class*>(0),&class::method),false); }

#include <Include.h>

