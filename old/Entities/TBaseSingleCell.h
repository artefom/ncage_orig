class TBaseSingleCell_Cleanup_dummy : public TBaseEntity
{
	public:

};

class TBaseSingleCell : public TBaseEntity
{
    protected:
        Vec2i Position;
        TBaseEntity** oldcell;
        CColor* oldpixel;
        //Vec2i OldPos;
    public:
    
    CColor Color;
    
    TBaseSingleCell()
    {
    	Position = 0;
    	oldcell = 0;
	};
	
    ~TBaseSingleCell()
    {
     	if (World.GetCellContent(Position) == this)
    	{
	    	World.PutPixel(Position,World.ClearColor);
    	};
    	World.Clear(Position); 
    }
    
    virtual void Draw_PBO()
	{
	    //if (World.GetCellContent(OldPos) == 0)
	    //	World.PutPixel(OldPos,World.ClearColor);
	    //World.PutPixel(Position,Color);
	    //OldPos = Position;
	}
	
	virtual bool Spawn()
	{
		if ( (oldcell == 0) || (oldpixel == 0) ) { cout << "TBaseSingleCell: Spawn fail!" << endl; return false;};
    	World.SetCell(Position,this);
    	World.PutPixel(Position,Color);
    	OnSpawn();
    	return true;
	};
	
	void InitColor( unsigned char colorID )
	{
		Color = colorID;
	};
    
	void InitPos(Vec2i pos)
	{
		Position = World.BoundsClamp(pos);
		oldpixel = World.GetPixel(Position);
		oldcell = World.GetCell(Position);
		World.SetCell(oldcell,this);
		World.PutPixel(oldpixel,Color);
	};
	
	bool SetPos(Vec2i pos)
	{
		pos = World.BoundsClamp(pos);
		if ( World.GetCellContent(pos) ) return false;
		
		World.SetCell(oldcell,0);
		World.PutPixel(oldpixel,World.ClearColor);
		Position = pos;
		oldpixel = World.GetPixel(Position);
		oldcell = World.GetCell(Position);
		World.SetCell(oldcell,this);
		World.PutPixel(oldpixel,Color);
		return true;
	}
	
    Vec2i GetPos()
    {
    	return Position;
    }
};
