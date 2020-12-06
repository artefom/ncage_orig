
//int& spawner_rate = Console.RegisterVar<int>(20, "spawner_rate");
//int& spawner_food_max = Console.RegisterVar<int>(1000, "spawner_food_max");

default_random_engine generator;
/*
normal_distribution<float> distribution(0,50.0);


void Spawner_Radius_Change();
int& spawner_radius = Console.RegisterVar(50, "spawner_radius", &Spawner_Radius_Change );
void Spawner_Radius_Change()
{
	normal_distribution<float> d1(0,spawner_radius);
  	distribution.param(d1.param());
}
*/

class TSpawner : public TBaseSingleCell
{
	public:		
		//TShared_ptr* ptr;
		MColorID;
		float radius;
		int rate;
		normal_distribution<float> distribution;
		int spawn_type;
		
		TBaseEntity* ent_instance;
		TUI_ParameterBox* pbox;
		
		TSpawner():distribution(0,20.0)
		{
			InitColor(colorID);
			radius = 20;
			rate = 1;
			spawn_type = 0;
			ent_instance = 0;
			pbox = 0;
			//ptr = new TShared_ptr(0);
		}
		
		TSpawner(TSpawner& cpy)
		{
			InitColor(colorID);
			radius = cpy.radius;
			rate = cpy.rate;
			spawn_type = cpy.spawn_type;
			if (cpy.ent_instance) ent_instance = MakeInstance(cpy.ent_instance,spawn_type); else ent_instance = 0;
			pbox = 0;
			OnRadiusChange();
			//ptr = new TShared_ptr(0);
		}
		
		~TSpawner()
		{
			if (ent_instance) delete ent_instance;
		};
		
		virtual void Think()
		{
			for (int i = 0; i != rate; ++i)
			{
				//if (ptr->ptr_count < spawner_food_max) 
				//{
					//float angle = (float)(random()&2047)*0.00306790425;
					//float dist = distribution(generator);
					//Vec2i spos( sin(angle)*dist , cos(angle)*dist );
					Vec2i spos(distribution(generator),distribution(generator));
					spos+=Position;
					spos = World.BoundsClamp_cheap(spos);
					if (spawn_type == 0)
					{
						TBaseEntity* ent = World.GetCellContent(spos);
						if (ent != 0) 
						{
							if (ent != this) delete ent; else
							{delete this; return;};
						};
					} else
					{
						if (ent_instance == 0) return;
						if (World.GetCellContent(spos) == 0)
						{
							TBaseSingleCell* ent;
							if (spawn_type == 2/*bot type id in instancing.cpp*/) ent = new TBot(); else 
							ent = (TBaseSingleCell*)MakeInstance(ent_instance,spawn_type);
							ent->InitPos(spos);
							ent->Spawn();
						};
					};
				//}
			}
		}
		
		virtual void OnSpawn()
		{
			Register(World.thinkers);	
		};
		
		virtual void OnSpawnTypeChange()
		{
			if (ent_instance) {ent_instance->ClearParameters(); delete ent_instance;};
			ent_instance = 0;
			ent_instance = Instance.CreateByID(spawn_type);
			if (pbox)
			{
				pbox->ClearParameters();
				if (ent_instance) ent_instance->RegisterParameters(pbox);
			};
		};
		
		virtual void OnRadiusChange()
		{
			normal_distribution<float> d1(0,radius);
			distribution.param(d1.param());
		};
		
		virtual void RegisterParameters( TUI_ParameterBox* parbox )
		{
			parbox->AddParameter(&radius,"radius",this, &TSpawner::OnRadiusChange);
			parbox->AddParameter(&rate,"rate");
			TUI_ContextButton* cbtn = (TUI_ContextButton*)
			parbox->AddParameter(&spawn_type,"spawner type",this, &TSpawner::OnSpawnTypeChange, PARAM_CONTEXTBUTTON);
			cbtn->AddContext("Erase",0);
			Instance.FillContext_SingleCell(cbtn);
			cbtn->InitContext(spawn_type);
			pbox = (TUI_ParameterBox*)parbox->AddParameter((char*)(0),"ent params",PARAM_PARAM);
			if (spawn_type && ent_instance)
			{
				ent_instance->RegisterParameters(pbox);
			};
		};
		
		virtual void ClearParameters()
		{
			if (ent_instance) ent_instance->ClearParameters();;
			if (pbox) pbox->ClearParameters();
			pbox = 0;
		};
		
};
DefaultColor(TSpawner,0,255,0);
