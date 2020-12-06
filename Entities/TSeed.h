class TSeed : public TBaseSingleCell
{
	public:
		int nextdup;
		static int dup_delta;
		static int dup_variation;
		MColorID;		
		TSeed()
		{
			flags = ENT_EDIBLE;
			nextdup = GameTime()+dup_delta+(random1()-0.5)*dup_variation;
			InitColor(colorID);
		};
		~TSeed()
		{

		};
		
		virtual void Tick()
		{
			if (GameTime() < nextdup) return;
			Vec2i nvec;
			nvec.x = (random()&7)+1;
			nvec.y = (random()&7)+1;
			if (random()&1 == 1) nvec.x = -nvec.x;
			if (random()&1 == 1) nvec.y = -nvec.y;
			nvec+=Position;
			if (!World.GetCellContent( World.BoundsClamp_cheap(nvec) ))
			{
			TSeed* seed = new TSeed();
			seed->InitPos(nvec);
			seed->Spawn();
			};
			
			nextdup = GameTime()+dup_delta+(random1()-0.5)*dup_variation;
		};
};
DefaultColor(TSeed,150,150,100);
int TSeed::dup_delta = 6000;
int TSeed::dup_variation = 2000;
