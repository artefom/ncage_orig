	TBaseEntity* TInstance::CreateByID(int ID)
	{
		switch (ID)
		{
			case 0: return 0;
			case 1: return new TRock();
			case 2: return new TBot();
			case 3: return new TFood();
			case 4: return new TPoison();
			case 5: return new TSpawner();
			case 6: return new TSeed();
			default: return 0;
		};
	};
	
	TBaseEntity* TInstance::MakeInstance_orig(TBaseEntity* inst, int ID)
	{	
		TBaseEntity* cent = 0;
		#define copy_const( class )  new class(*static_cast<class*>(inst)) 
		switch (ID)
		{
			case 0 : cent = 0; break;
			case 1 : cent = copy_const(TRock); break;
			case 2 : cent = copy_const(TBot); break;
			case 3 : cent = copy_const(TFood); break;
			case 4 : cent = copy_const(TPoison); break;
			case 5 : cent = copy_const(TSpawner); break;
			case 6 : cent = copy_const(TSeed); break;
			default: cent = 0; break;
		};
		#undef copy_const
		if (cent)
		{
			cent->ID = -1;
			cent->ID_array = 0;	
		};
		return cent;
	};
	TBaseEntity* TInstance::CreateByName( string name )
	{
		
	};
	
	void TInstance::FillContext_SingleCell(TUI_ContextButton* cbtn)
	{
		for (int i = 1; i != names.size(); ++i)
		{
			cbtn->AddContext((char*)names[i].c_str(),i);
		};
	};
	
	void TInstance::FillInstance_SingleCell( TBaseSingleCell* instances[] )
	{
		instances[0] = 0;
		instances[1] = new TRock();
		instances[2] = new TBot();
		instances[3] = new TFood();
		instances[4] = new TPoison();
		instances[5] = new TSpawner();
		instances[6] = new TSeed();
	};
