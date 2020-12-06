class TUI_SimpleFrame : public TUI_ParentLess
{
	public:
		
		
	TUI_SimpleFrame( TUI_ParentLess* parent ) : TUI_ParentLess(parent)
	{
	};
	
	TUI_SimpleFrame() : TUI_ParentLess()
	{
	};
		 
	virtual bool TestMouse(Vec2i mpos)
    {
    	if ( !IsMouseIn(mpos) ) return false;
    	    	
    	for (auto v = elms.rbegin(); v != elms.rend(); v++)
    	{
				if ( (*v)->TestMouse(mpos) ) return true;	
    	}
    	
    	
    	if (!Parent) { 	
			SetGlobalMouseGui( 0 );
			return false; 
		};
		
		SetGlobalMouseGui( this );
    	
    	return true;
    }
		
	virtual Vec2i GetGlobalPos()
	{
		if (!Parent) return Position;
		return Parent->GetGlobalPos()+Position;
	};

	virtual bool HasMouse()
	{
    	if (!GlobalMouseGui) return false;
		if (GlobalMouseGui == this) return true;
		TUI_Childless* parent = GlobalMouseGui;
		while(parent)
		{
			if (parent == this) return true;
			parent = parent->Parent;
		};
		return false;
	};
    
    virtual void Draw()
    {
		glPushMatrix();
		glTranslate( GetPos() );
			OnDraw();
			for (auto &it : elms) it->Draw();
		glPopMatrix();
    };
    
	virtual void Update()
	{
		if (Parent) Parent->Update();
	}
	
	virtual void SetupViewport()
	{	
		Parent->SetupViewport();
		glTranslate( Position );
	};

};

