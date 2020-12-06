const int Instance_SingleCellCount = 6;
class TInstance
{
	public:
	vector<string> names;
	TInstance()
	{
		names.push_back("");
		names.push_back("Rock");
		names.push_back("Bot");
		names.push_back("Food");
		names.push_back("Poison");
		names.push_back("Spawner");
		names.push_back("Seed");
	};
	TBaseEntity* CreateByID(int ID);
	TBaseEntity* MakeInstance_orig(TBaseEntity* inst, int ID);
	TBaseEntity* CreateByName( string name );
	void FillContext_SingleCell(TUI_ContextButton* cbtn);
	void FillInstance_SingleCell( TBaseSingleCell* instances[] );
};
TInstance Instance;

#define MakeInstance(x, y) ((__file2__=__FILE__,__line2__=__LINE__) && 0 ? NULL : Instance.MakeInstance_orig(x,y))
