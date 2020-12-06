
// FLAGS
unsigned int ENT_EDIBLE = 1;
unsigned int ENT_POISON = 2;
unsigned int ENT_ALIVE = 4;

class TBaseEntity;

TBaseEntity* dummy = 0;
class TUI_ParameterBox;
class TBaseEntity // base entity
{
	public:
		unsigned int flags;
		int ID; // ID is not static! Can be changed only on Cleanup??? If not, check everything (TBot and NView)
		vector<TBaseEntity*>* ID_array;
		
	TBaseEntity()
	{		
		ID = -1;
		flags = 0;
	}
		
	inline virtual ~TBaseEntity()
	{
		if (ID != -1) ID_array->at(ID) = dummy;
	}
	
	virtual void Think()
	{
		
	};

	
	virtual void Draw_PBO()
	{
		
	};
	
	
	virtual void InitDraw()
	{
		
	};
	
	virtual void Tick()
	{
		
	};
	
	virtual bool Spawn()
	{
		OnSpawn();
		return true;
	};
	
	virtual void OnSpawn()
	{
		
	};
	
	virtual void RegisterParameters( TUI_ParameterBox* parbox )
	{
		
	};
	
	virtual void ClearParameters()
	{
		
	};

	void Register(vector<TBaseEntity*>& array)
	{
		ID_array = &array;
		ID = array.size();
		array.push_back(this);
	};
};
