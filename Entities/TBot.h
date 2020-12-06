default_random_engine bot_generator;
normal_distribution<float> bot_distribution(0,0.5);

int bot_alloc = 0;

class TBot;
extern TBot& TBotSet_default;

class TBot : public TBaseSingleCell
{
	public:
	MColorID;
	bool driving;
	
	Vec2d Velocity;
	
	TBot()
	{
		InitColor(colorID);
	};
	
	virtual void OnSpawn()
	{
		Register(World.thinkers);
	};
	
	void Think()
	{

	};
	
	virtual void Drive()
	{
		driving = !driving;
	};
	
	virtual void OpenNView()
	{
		//TWF_NRender* Window = UI_CreateWindow<TWF_NRender>();
		//Window->RenderFrame->SetNetwork(this,&network);
		//Window->SetWindowSize( Vec2i(400,200) );
	};
	
	virtual void TestBreed()
	{

	};
	
};

//Hook(HOOK_OnClick,TBot,OpenNView);
DefaultColor(TBot,255,50,100);

void debug_spawnbot()
{
	TBot* bot = new TBot();
	bot->InitPos(512);
	bot->Spawn();
};
ConsoleRegisterFunction( debug_spawnbot, "spawns bot" );

