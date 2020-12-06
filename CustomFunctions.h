bool barrier = 0;
int barrier_width = 0;
void barrier_create(int width)
{
	TRock* rock;
	const int& bounds = World.bounds;
	barrier_width = width;
	for (int i = 0; i != barrier_width; ++i)
	{
		for (int j = i+1; j != bounds-i; ++j)
		{
			rock = new TRock();
			rock->InitPos(Vec2i(i,j));
			rock->Spawn();
			
			rock = new TRock();
			rock->InitPos(Vec2i(bounds-i-1,bounds-j-1));
			rock->Spawn();
			
			rock = new TRock();
			rock->InitPos(Vec2i(bounds-j-1,i));
			rock->Spawn();
			
			rock = new TRock();
			rock->InitPos(Vec2i(j,bounds-i-1));
			rock->Spawn();
		};
	};
	barrier = true;
};
ConsoleRegisterFunction(barrier_create,"builds barrier");
void barrier_destroy()
{
	if (!barrier) return;
	TBaseEntity* rocks;
	const int& bounds = World.bounds;
	for (int i = 0; i != barrier_width; ++i)
	{
		for (int j = i+1; j != bounds-i; ++j)
		{
			rocks = World.GetCellContent(Vec2i(i,j));
			if (rocks) delete rocks;
			rocks = World.GetCellContent(Vec2i(bounds-i-1,bounds-j-1));
			if (rocks) delete rocks;
			rocks = World.GetCellContent(Vec2i(bounds-j-1,i));
			if (rocks) delete rocks;
			rocks = World.GetCellContent(Vec2i(j,bounds-i-1));
			if (rocks) delete rocks;
		};
	};
	barrier = false;
	barrier_width = 0;
};
ConsoleRegisterFunction(barrier_destroy,"destroys barrier");
