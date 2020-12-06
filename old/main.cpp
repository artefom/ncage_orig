#include <random>

#include <glutfunc.h>

#include <typeinfo>

#include <MultiDelegate.h>

#include <Delegate.h>

#include <Callback.h>

#include <SpecializedEvents.h>

#include <Entities/TBaseEntity.h>

#define Debugging

#define error(x) cout << __FILE__ << ":" << __LINE__ << "	" << x << endl;

Vec2i MousePos;

enum
{
	HOOK_Think,
	HOOK_Tick,
	HOOK_Draw,
	HOOK_OnResize,
	HOOK_OnMouseEnter,
	HOOK_OnMouseExit,
	HOOK_OnGainFocus,
	HOOK_OnLooseFocus,
	HOOK_OnEnter,
	HOOK_OnScroll,
	
	HOOK_OnMouseDown,
	HOOK_OnMouseUp,
	HOOK_OnKeyDown,
	HOOK_OnKeyUp,
	HOOK_OnKeySpecialDown,
	HOOK_OnKeySpecialUp,
	HOOK_OnKeyboardExit,
	HOOK_OnKeyboardEnter,
	HOOK_OnClick,
	HOOK_OnDoubleClick,
	HOOK_OnMouseMove,
	HOOK_OnDrag,
	HOOK_OnChange,
	
	HOOK_G_Think,
	HOOK_G_OnMouseMove,
	HOOK_G_OnMouseDown,
	HOOK_G_OnMouseUp,
	HOOK_G_OnPreWorldCleanup,
	HOOK_G_OnPostWorldCleanup,
	HOOK_GC_OnEnterText,
	HOOK_GC_OnMove,
	HOOK_WorldTranslate
};

enum
{
	PARAM_VALUEBOX,
	PARAM_CONTEXTBUTTON,
	PARAM_IRGB,
	PARAM_PARAM
};

const char* GetHookNameByID(int ID)
{
	switch (ID)
	{
	case HOOK_Think : return "HOOK_Think"; break;
	case HOOK_Tick : return "HOOK_Tick"; break;
	case HOOK_Draw : return "HOOK_Draw"; break;
	case HOOK_OnResize : return "HOOK_OnResize"; break;
	case HOOK_OnMouseEnter : return "HOOK_OnMouseEnter"; break;
	case HOOK_OnMouseExit : return "HOOK_OnMouseExit"; break;
	case HOOK_OnGainFocus : return "HOOK_OnGainFocus"; break;
	case HOOK_OnLooseFocus : return "HOOK_OnLooseFocus"; break;
	case HOOK_OnMouseDown : return "HOOK_OnMouseDown"; break;
	case HOOK_OnMouseUp : return "HOOK_OnMouseUp"; break;
	case HOOK_OnKeyDown : return "HOOK_OnKeyDown"; break;
	case HOOK_OnKeyUp : return "HOOK_OnKeyUp"; break;
	case HOOK_OnKeySpecialDown : return "HOOK_OnKeySpecialDown"; break;
	case HOOK_OnKeySpecialUp : return "HOOK_OnKeySpecialUp"; break;
	case HOOK_OnKeyboardExit : return "HOOK_OnKeyboardExit"; break;
	case HOOK_OnKeyboardEnter : return "HOOK_OnKeyboardEnter"; break;
	case HOOK_OnClick : return "HOOK_OnClick"; break;
	case HOOK_OnDoubleClick : return "HOOK_9OnDoubleClick"; break;
	case HOOK_OnMouseMove : return "HOOK_OnMouseMove"; break;
	case HOOK_OnDrag : return "HOOK_OnDrag"; break;
	case HOOK_G_Think : return "HOOK_G_Think"; break;
	case HOOK_G_OnMouseMove : return "HOOK_G_OnMouseMove"; break;
	case HOOK_G_OnMouseDown : return "HOOK_G_OnMouseDown"; break;
	case HOOK_G_OnMouseUp : return "HOOK_G_OnMouseUp"; break;
	case HOOK_G_OnPreWorldCleanup : return "HOOK_G_OnPreWorldCleanup"; break;
	case HOOK_G_OnPostWorldCleanup : return "HOOK_G_OnPostWorldCleanup"; break;
	case HOOK_GC_OnEnterText : return "HOOK_GC_OnEnterText"; break;
	case HOOK_GC_OnMove : return "HOOK_GC_OnMove"; break;
	case HOOK_WorldTranslate : return "HOOK_WorldTranslate"; break;
	case HOOK_OnEnter : return "HOOK_OnEnter"; break;
	case HOOK_OnScroll : return "HOOK_OnScroll"; break;
	default : return 0;
	};
};


void SetUpRender(int w, int h);


bool Simulating = 1;
bool ui_visible = 1;
bool world_visible = 1;

void SetSim(bool is)
{
	Simulating = is;
};

int gametime = 0;
int CurTime() {return glutGet(GLUT_ELAPSED_TIME);};
int GameTime() {return gametime;};

#include <GUI/Base/TUI_Childless.h>
#include <TWorld.h>
#include <CustomFunctions.h>

void KeySpecialDown( int key, int x, int y);
void KeySpecialUp( int key, int x, int y);
void KeyDown(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);

void MouseDown(int btn, int state, int x, int y);
void MouseMove(int x, int y);
void OnMouseDown(int btn, int x, int y);
void OnMouseUp(int btn, int x, int y);

int numpressedkeys = 40;
int pressedkeys [40];

/* GLUT callback Handlers */


void SetUpRender(int w, int h)
{
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glViewport(0, 0, w, h);
    glOrtho (0, w, h, 0, -1, 1);
    glTranslatef (0.375, 0.375, 0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}
static void 
resize(int w, int h)
{
    if (h == 0) h = 1;
    ScrW = w;
    ScrH = h;
    
    World.SetSize( Vec2i(w,h) );
    if (ui_visible) UI.SetSize( Vec2i(w,h) );
    
	SetUpRender(w,h);
}

int nexttick = 0;

int nextdisplay = 0;
int lastdisplay = 0;
int delta;
int tdelta;

bool drawn = 0;

TUI_FPSMark* FPSMark;

static void 
display(void)
{
	if (drawn) return;
	
    delta = CurTime()-lastdisplay;
    lastdisplay = CurTime();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   	 
   	TUI_ParentLess::CleanupAll();
   	 
    if(world_visible){
	    //World.Think();
	    World.Draw(delta);
	};
	World.Think();
    FPSMark->UpdateFPS();
    ++gametime;
    if (ui_visible) 
	{
		ui_events.SingleCall(	&TUI_Childless::Global_Hooks_Storage,HOOK_G_Think );
		UI.Draw();
		Debug.Draw();
	};
    drawn = 1;

};
 
TUI_Window* Console_Global = 0;
static void 
idle(void)
{
    while (CurTime() < nextdisplay) 
	{
		//tdelta = CurTime()-tdelta;
		//events.Call(HOOK_Think,tdelta);
		Sleep(1);
		//tdelta = CurTime();
	}
	
	if ( CurTime() > nexttick )
	{
		nexttick = CurTime()+200;
		if(world_visible) World.Tick();
		if (TUI_Childless::KeyboardElement != 0)
		{
			if (ui_visible) TUI_Childless::KeyboardElement->Tick();
		}
		if (TUI_Childless::ClickedElement && TUI_Childless::ClickedElement->dead) TUI_Childless::ClickedElement = 0;
		if (TUI_Childless::GlobalMouseGui && TUI_Childless::GlobalMouseGui->dead) TUI_Childless::GlobalMouseGui = 0;
		if (TUI_Childless::KeyboardElement&& TUI_Childless::KeyboardElement->dead) TUI_Childless::KeyboardElement = 0;
		if (Console_Global && Console_Global->dead) Console_Global = 0;
		TUI_ParentLess::CleanupGlobalHooksStorage();
		TUI_ParentLess::CleanupUnusedStorage();
	}
			
	nextdisplay = CurTime()+16;
	drawn = 0;
    glutPostRedisplay();
    
}

/* Program entry point */
int 
main(int argc, char *argv[])
{
	TDebug_initialized = 1234567890;  
	srand( time(0) );
	fr_randomize();
   	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_RGBA);

    int scr_height = GetSystemMetrics(SM_CYSCREEN);
    int scr_width = GetSystemMetrics(SM_CXSCREEN);

	glutInitWindowPosition(scr_width*0.5-400,scr_height*0.5-300);
	glutInitWindowSize(800,600);
	glutCreateWindow("gluttest");
	glewInit();

	//cout << "GL_ARB_imaging: " << (unsigned int)GLEW_ARB_imaging << endl;
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	//cout << "OpenGL extensions: " << glGetString(GL_EXTENSIONS) << endl;
	//int NumberOfExtensions;
	//glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
	//for(int i=0; i<NumberOfExtensions; i++) {
	//	cout << i << ": " << glGetStringi(GL_EXTENSIONS, i) << endl;
	//};

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glDisable(GL_DEPTH_TEST);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
	glutSpecialFunc(KeySpecialDown);
 	glutSpecialUpFunc(KeySpecialUp);
 	
 	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	
	glutMouseFunc(MouseDown);
	glutPassiveMotionFunc(MouseMove);
	glutMotionFunc(MouseMove);
    
// ################ GEN BUFFERS

// ################ START PROGRAMM

    glClearColor(0,0,0,0);
    
    for (int i = 0; i != 0; ++i)
    {
    	int size = 25;
    	char* name = new char[size];
    	for (int i = 5; i != size; ++i) name[i] = (random()%10)+'0';
    	name[0] = 't';
    	name[1] = 'e';
    	name[2] = 's';
    	name[3] = 't';
    	name[4] = '_';
    	name[size-1] = 0;
    	Console.RegisterVar( random() , name);	
    }
    Console.Init();
    World.Init();
    
    
    /*
	TUI_WFrame* Window = UI_CreateWindow( new TUI_WFrame(&UI) );
		Window->SetWindowSize( Vec2i(400,200) );
	TUI_Button* btn = new TUI_Button(Window);
		btn->SetPos( Vec2i(10,10) );
		btn->SetCaption( (char*)("Some Button") );
		btn->AutoSize();
		//btn->Connect(Window, &TUI_Window::TestFunc, 200);
		btn->Connect_callback( PrintSomething, (char*)("Some button printed somethig") );
	*/
	
	TUI_Button* btn;
	TUI_MenuButton* mbtn;
	TUI_Menu* menu;
	
	FPSMark = new TUI_FPSMark(&UI);
	
	TUI_MenuPanel* panel = new TUI_MenuPanel(&UI);
	panel->Padding_bottom.x = 100;

	mbtn = new TUI_MenuButton(panel);
	mbtn->SetCaption((char*)"File");
	mbtn->AutoSize();
	menu = mbtn->menu;
		btn = menu->AddButton((char*)"Open");
		btn = menu->AddButton((char*)"Save");
		btn = menu->AddButton((char*)"...");
	menu->Init();
	
	mbtn = new TUI_MenuButton(panel);
	mbtn->SetCaption((char*)"Window");
	mbtn->AutoSize();
	menu = mbtn->menu;
		btn = menu->AddButton((char*)"Paint Menu");
		TWF_PaintMenu* ref;
		btn->scallbacks.Connect(HOOK_OnClick, OpenWindow<TWF_PaintMenu> );
		btn = menu->AddButton((char*)"Graph View");
		btn->scallbacks.Connect(HOOK_OnClick, OpenWindow<TWF_GraphView> );
		
		btn = menu->AddButton((char*)"Console");
		btn->scallbacks.Connect(HOOK_OnClick, OpenWindow<TWF_Console> );
		
	menu->Init();
	panel->Init();
	FPSMark->SetSize( Vec2i(101,panel->Size.y) );
	
	
	Console.ProcessCommandsFromFile("parameters.cfg");
    glutMainLoop();
    return EXIT_SUCCESS;
}

Vec2i Console_pos = 100;
Vec2i Console_size = Vec2i(150,200);
bool ctoggle = false;
void Console_Toggle()
{
	if (Console_Global && !Console_Global->dead) 
	{
		Console_pos = Console_Global->Position;
		Console_size = Console_Global->Size;
		Console_Global->Suicide(); Console_Global = 0; 
		return;
	};
	Console_Global = (TUI_Window*)(UI_CreateWindow<TWF_Console>()->Parent);
	TUI_Childless::SetKeyboardElement(((TWF_Console*)(Console_Global->Frame))->edit);
	Console_Global->SetBounds(Console_pos,Console_size);
};

void KeySpecialDown( int key, int x, int y)
{
	if ((key == 192) && (ctoggle == false)) {ctoggle = true; Console_Toggle();};
	if (key == 5) ui_visible = !ui_visible;
	if (key == 6) world_visible = !world_visible;
	if (ui_visible) ui_events.Call(TUI_Childless::KeyboardElement,HOOK_OnKeySpecialDown, key );
	if (!TUI_Childless::KeyboardElement || TUI_Childless::KeyboardElement->dead) Console_scripts.OnKeyDown(key);
}

void KeySpecialUp( int key, int x, int y)
{
	if (key == 192) ctoggle = false;
	if (ui_visible) ui_events.Call(TUI_Childless::KeyboardElement,HOOK_OnKeySpecialUp, key );
	if (!TUI_Childless::KeyboardElement) Console_scripts.OnKeyUp(key);
}

void KeyDown(unsigned char key, int x, int y) 
{
    if (ui_visible) ui_events.Call(TUI_Childless::KeyboardElement,HOOK_OnKeyDown, key );
	KeySpecialDown(toVirtual(key),x,y);
    //HID.KeyDown(key);
}

void KeyUp(unsigned char key, int x, int y) 
{
	if (ui_visible) ui_events.Call(TUI_Childless::KeyboardElement,HOOK_OnKeyUp, key );
	KeySpecialUp(toVirtual(key),x,y);
};

void MouseDown(int btn, int state, int x, int y) {
    bool bstate;
    if (state == GLUT_DOWN) {bstate = true; OnMouseDown(btn,x,y); Console_scripts.OnKeyDown(btn);};
    if (state == GLUT_UP) {bstate = false; OnMouseUp(btn,x,y);} Console_scripts.OnKeyUp(btn);;
    if (btn == GLUT_LEFT_BUTTON) {MouseStates[0] = bstate;};
    if (btn == GLUT_RIGHT_BUTTON){MouseStates[1] = bstate;};
    if (btn == GLUT_MIDDLE_BUTTON){MouseStates[3] = bstate;};
}

void MouseMove(int x, int y){
	//UI.OnMouseMove
	
	if (ui_visible)
	{
		MouseRes res(0,x-MousePos.x,y-MousePos.y);
		
		if (cur_update) CursorUpdate();
		
		ui_events.Call(TUI_Childless::GlobalMouseGui,HOOK_OnMouseMove,res );
		if (TUI_Childless::ClickedElement != TUI_Childless::GlobalMouseGui)
			ui_events.Call(TUI_Childless::ClickedElement,HOOK_OnMouseMove,res );
	
	    ui_events.SingleCall(	&TUI_Childless::Global_Hooks_Storage,HOOK_G_OnMouseMove,res);
		if (!UI.TestMouse(MousePos) && ((TUI_Childless::ClickedElement == 0) || TUI_Childless::ClickedElement->dead)) World.TestMouse(MousePos); 
			else {World.SetMouseCell(0); World.hasmouse = false;};
	};
    World.OnMouseMove(Vector2d(x-MousePos.x,y-MousePos.y));
    MousePos.x = x;
    MousePos.y = y;
};

void OnMouseDown(int btn, int x, int y)
{
	if (ui_visible){
		MouseRes res(btn,x,y);
		UI.ClickedElement = TUI_Childless::GlobalMouseGui;
		if (TUI_Childless::GlobalMouseGui && !TUI_Childless::GlobalMouseGui->dead)
		{
			MouseRes localres(btn,x-TUI_Childless::GlobalMouseGui->GPos1.x,y-TUI_Childless::GlobalMouseGui->GPos1.y);
			ui_events.Call(TUI_Childless::GlobalMouseGui,HOOK_OnMouseDown,localres);
		};
		ui_events.SingleCall(	&TUI_Childless::Global_Hooks_Storage,HOOK_G_OnMouseDown,res);
	};
	World.OnMouseDown(x, y, btn);
};

int lastclick;
void OnMouseUp(int btn, int x, int y)
{
	if (ui_visible){
		MouseRes res(btn,x,y);
		
		if (TUI_Childless::ClickedElement != TUI_Childless::KeyboardElement)
			TUI_Childless::SetKeyboardElement(0);
		
		if (TUI_Childless::ClickedElement && !TUI_Childless::ClickedElement->dead)
		{
			MouseRes localres(btn,x-TUI_Childless::ClickedElement->GPos1.x,y-TUI_Childless::ClickedElement->GPos1.y);
			ui_events.Call(TUI_Childless::ClickedElement,HOOK_OnMouseUp,localres );
			ui_events.Call(TUI_Childless::ClickedElement,HOOK_OnClick,localres );
			if ( (CurTime()-lastclick) < 300 ) ui_events.Call(TUI_Childless::ClickedElement,HOOK_OnDoubleClick,localres );
		}
		ui_events.SingleCall(	&TUI_Childless::Global_Hooks_Storage,HOOK_G_OnMouseUp,res);
		TUI_Childless::ClickedElement = 0;
		lastclick = CurTime();
	};
	World.OnMouseUp(x,y, btn);
};

