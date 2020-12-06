class TUI_NRenderFrame : public TUI_SimpleFrame
{
	public:
	//TNNetwork* network;
	TBaseEntity* monitor_ent;
	int monitor_id;
	vector<TBaseEntity*>* array;
	bool drag;
	bool simulating;
	//TNeuron* dragner;
	Vec2d dragner_oldpos1;
	Vec2d dragner_oldpos2;
    float absgridsize;
    float gridsize;
    float SCALE;
    float SCALE_MIN;
    float SCALE_MAX;
    float grid_min_scale;
        
    Vec2d TRANS;
    int highlighteach;

    IRGB gridcolor;
    IRGB highlightcolor;
    
    float radius;
    float scale;
    
	TUI_NRenderFrame(TUI_ParentLess* parent) : TUI_SimpleFrame(parent)
	{
		EnableGlobalHooks();
		//network = 0;
		monitor_id = 0;
		monitor_ent = 0;
		drag = 0;
		//dragner = 0;
		simulating = 0;
	    gridsize = 16;
        highlighteach=4;
        gridcolor = IRGB(100,230,50);
        highlightcolor = IRGB(100,230,50);
        grid_min_scale = 16;
        SCALE = 0.5;
        SCALE_MIN = 0.01; SCALE_MAX = 5; SCALE = 1;
        absgridsize = gridsize*SCALE;
        
        //radius = TNeuron::size;
        scale = 10;
	}
	/*
	void SetNetwork(TBaseEntity* ent, TNNetwork* net)
	{
		monitor_ent = ent;
		monitor_id = monitor_ent->ID;
		array = monitor_ent->ID_array;
		network = net;
		int i = 0;
		//cout << "number of inputs: " << network->inputs.size() << endl;
		//for (auto & it : network->inputs )
		//{
		//	++i;
		//	cout << i << ": (" << it->Position.x << "; " << it->Position.y << ")" << endl;	
		//};
	};
	*/
	virtual void OnDraw()
	{
    	float localgridsize;
        //Size = Vector2d(ScrW,ScrH);
        Vec2d pos,size;
        pos = 0;
        size = Size;
        glColor(UI_BG_Medium);
        drawrect(0,Size);
        glBegin(GL_LINES);
            
            localgridsize = gridsize;
                while(localgridsize*SCALE < grid_min_scale) {
                    localgridsize*=2;
                };
            glColor(gridcolor, Clamp(SCALE*0.0625,0,0.5));
            drawGrid(&pos,&size,&TRANS,SCALE,localgridsize);
            
                localgridsize = gridsize*16;
                while(localgridsize*SCALE < grid_min_scale) {
                    localgridsize*=2;
                };
            glColor(gridcolor, Clamp(SCALE*0.25,0,0.5));
            drawGrid(&pos,&size,&TRANS,SCALE,localgridsize);
            
            localgridsize = gridsize*256;
                while(localgridsize*SCALE < grid_min_scale) {
                    localgridsize*=2;
                };
            glColor( gridcolor, Clamp(SCALE,0,0.5) );
            drawGrid(&pos,&size,&TRANS,SCALE,localgridsize);
            
            
            glColor(IRGBA(130,200,60,255));
            if (TRANS.x > 0 & TRANS.x < ScrW) {glVertex2f(TRANS.x,Position.y); glVertex2f(TRANS.x,Size.y);};
            if (TRANS.y > 0 & TRANS.y < ScrH) {glVertex2f(Position.x,TRANS.y); glVertex2f(Size.x,TRANS.y);};
        glEnd();
        
        if (!IsValid()) return;
        
        if (simulating)	Simulate();
		/*
        if (dragner) 
		{
			dragner_oldpos2 = dragner_oldpos1;
			dragner_oldpos1 = dragner->Position;
			dragner->Position = WorldToLocal(MousePos-GPos1);
			dragner->Velocity = dragner->Position-(dragner_oldpos2+dragner_oldpos1)*0.5;
			Update();
		};
		*/
        float localscale = SCALE * scale;
		glPushMatrix();
		glTranslate(TRANS);
		glScale(localscale);
    	float val;
    	glColor4f(1,1,1,0.5);
    	/*
    	for (TNeuron* ptr = network->neurons.begin, *end = network->neurons.capacity_end; ptr != end; ++ptr)
    	{
    		//for (auto &v : it->connections)
    		//{
    		//	glColor3f(-v.weight,0.2,v.weight);
    		//	drawarrow(it->Position,v.connection->Position,radius*0.6);	
    		//};
    	};
    	
    	glColor4f(0.9,0.2,0.1,1);
    	for (TNeuron** ptr = network->inputs.begin, **end = network->inputs.end; ptr != end; ++ptr)
    		fcircle((*ptr)->Position,radius*1.3);	
    	glColor4f(0.1,0.2,0.9,1);
    	for (TNeuron** ptr = network->outputs.begin, **end = network->outputs.end; ptr != end; ++ptr)
    		fcircle((*ptr)->Position,radius*1.3);	
    	
    	glColor4f(0,0,0,1);
    	for (TNeuron* ptr = network->neurons.begin, *end = network->neurons.capacity_end; ptr != end; ++ptr)
    	{
    		val = ptr->GetValue();
    		glColor3f(val*0.8+0.1,val*0.8+0.1,val*0.8+0.1);
    		fcircle(ptr->Position,radius);
    	};
    	glPopMatrix();
    	//cout << "drawn!" << endl;
    	return;
    	*/
	};
	
	~TUI_NRenderFrame()
	{
	}
	
	void PreCleanup()
	{
		if (!array) return;
		if ((array->size() <= monitor_id) || (array->at(monitor_id) != monitor_ent))
		{
			monitor_id = 0;
			array = 0;
			monitor_ent = 0;
			//network = 0;
		};
	};
	
	bool IsValid()
	{
		PreCleanup();
		//if (network == 0 || array == 0) {cout << "invalid!" << endl; return false;};
		//cout << "valid!" << endl;
		return true;
	};
	void PostCleanup()
	{
		if (monitor_ent != 0) monitor_id = monitor_ent->ID;
	};
	
	void OnMouseDown(MouseRes res)
	{
		/*
		vector<TNeuron*> result;
		switch (res.btn) {
			case GLUT_LEFT_BUTTON : FindInCircle(result,WorldToLocal(res.pos),radius);
				if (result.size() != 0){dragner = result[0]; dragner_oldpos1 = dragner->Position; dragner_oldpos2 = dragner_oldpos1;
					Update();};
				break;
			case GLUT_DRAG_BUTTON : drag = true; return;
			case 3	: RESCALE(1.1,res.pos); return;
			case 4	: RESCALE(0.909090909090909091,res.pos); return;
			default : break;
		};
		*/
	}
	
	Vec2d WorldToLocal(Vec2d mpos)
	{
		return (mpos-TRANS)/(SCALE*scale);
	};
	
	/*
	void FindInCircle(vector<TNeuron*>& result, Vec2d point, float radius)
	{
		
		TNeuron* ptr = network->neurons.begin;
    	TNeuron* end = network->neurons.end;
    	for(ptr;ptr!=end;++ptr)
    	{
    		if ((ptr->Position.x < point.x+radius) && (ptr->Position.x > point.x-radius) &&
    			(ptr->Position.y < point.y+radius) && (ptr->Position.y > point.y-radius))
    			if ((ptr->Position-point).Length() < radius) result.push_back(ptr);
    	};
	};
	*/
	
	void OnMouseUp(MouseRes res)
	{
		/*
		switch (res.btn) {
			case GLUT_DRAG_BUTTON : drag = false; break;
			case GLUT_LEFT_BUTTON : dragner = 0; break;
		};
		*/
	}
		
	void OnMouseMove(MouseRes res)
	{
		if (drag)
		{
			TRANS+=res.pos;
			Update();
		};
	}
		
    void RESCALE(float a, Vec2i pos)
    {
        Vec2d mpos;
        mpos = pos-TRANS;
        Move( mpos*(1-Scale(a) ));
    };
    
    
    float Scale(float a) // returns clamped a (if Scale is maxed and a > 0 returns 0}
    {
		float new_SCALE = SCALE*a;
		Update();
		if (new_SCALE > SCALE_MAX)
		{
			new_SCALE = SCALE_MAX/SCALE;
			SCALE = SCALE_MAX;
			absgridsize = SCALE*gridsize;
			return new_SCALE;	
		} else if (new_SCALE < SCALE_MIN)
		{
			new_SCALE = SCALE_MIN/SCALE;
			SCALE = SCALE_MIN;
			absgridsize = SCALE*gridsize;
			return new_SCALE;	
		};
		SCALE = new_SCALE;
    	absgridsize = SCALE*gridsize;
    	return a;
    }
    
    void Move(Vec2d delta)
    {
    	SetTrans(TRANS+delta);
    }
    
    void SetTrans(Vec2d pos)
    {
    	TRANS = pos;
    	sevents.Call(HOOK_OnDrag);
    }
    
    void SetSimulate( bool sim )
    {
    	simulating = sim;
    };
    
    void Simulate()
    {
    	/*
    	if (!network || monitor_ent == dummy) return;
    	float mass = 100;
    	float scale = 0.7;
    	float timescale = 1;
    	TNeuron* ptr=0,*end=0;
    	TNerCon* cptr=0,*cend=0;
    	ptr = network->neurons;
    	end = network->neurons+network->neurons_size;
    	
    	for(ptr;ptr!=end;++ptr)
    	{
    		cptr = &ptr->connections[0];
    		cend = &ptr->connections[0]+ptr->connections.size();
    		for(cptr; cptr != cend; ++cptr)
    		{
    			TNeuron& ner1 = *ptr;
    			TNeuron& ner2 = *cptr->connection;
    			float distance = (ner2.Position-ner1.Position).Length();
        		if (distance != 0)
        		{
	    			Vec2d normal = (ner2.Position-ner1.Position)/distance;
	    			distance*=scale;
	    			float force = -distance-normal.Dot(ner2.Velocity-ner1.Velocity)*10;
	    			ner1.Velocity -= normal*((force/mass)*timescale);
	    			ner2.Velocity += normal*((force/mass)*timescale);
    			};
    		};
    		TNeuron* ptr2 = network->neurons;
    		TNeuron* end2 = network->neurons+network->neurons_size;
    		for(ptr2; ptr2 != end2; ++ptr2)
    		{
    			TNeuron& ner1 = *ptr;
    			TNeuron& ner2 = *ptr2;
        		float distance = (ner2.Position-ner1.Position).Length();
        		if (distance != 0)
        		{
	    			Vec2d normal = (ner2.Position-ner1.Position)/distance;
	    			distance*=scale;
	    			float force = 1/distance;
	    			if (force > 10) force = 10;
	    			if (force < -10) force = -10;
	    			ner1.Velocity -= normal*((force/mass)*timescale);
	    			ner2.Velocity += normal*((force/mass)*timescale);
    			};
    		};
    	};
    	ptr = network->neurons;
    	end = network->neurons+network->neurons_size;
    	for(ptr;ptr!=end;++ptr) {ptr->Velocity*=0.99; ptr->Position += ptr->Velocity*timescale;};
    	Update();
    	*/
    };
};

UI_Hook(HOOK_G_OnPreWorldCleanup,TUI_NRenderFrame,PreCleanup);
UI_Hook(HOOK_G_OnPostWorldCleanup,TUI_NRenderFrame,PostCleanup);
UI_Hook(HOOK_OnMouseDown,TUI_NRenderFrame,OnMouseDown);
UI_Hook(HOOK_OnMouseUp,TUI_NRenderFrame,OnMouseUp);
UI_Hook(HOOK_OnMouseMove,TUI_NRenderFrame,OnMouseMove);

class TWF_NRender : public TUI_WFrame
{
	public:
		
		TUI_NRenderFrame* RenderFrame;
		TUI_MenuPanel* Panel;
		
		TUI_Button* rbtn;
		TUI_Button* sbtn;
		bool relaxing;
		bool simulating;
				
		TWF_NRender(TUI_ParentLess* parent) : TUI_WFrame(parent)
		{
			relaxing = 0;
			simulating = 0;
			caption = "NView";
			bstyle = 3;
			RenderFrame = new TUI_NRenderFrame(this);
			
			
			Panel = new TUI_MenuPanel(this);
			//panel->Padding_bottom.x = 100;
				//TUI_Button* btn;
				sbtn = new TUI_Button(Panel);
				sbtn->SetCaption("Simulate");
				sbtn->AutoSize();
				sbtn->sevents.Connect(HOOK_OnClick,this,&TWF_NRender::ToggleSimulate);
				//btn->Connect( RenderFrame, & TUI_NRenderFrame::Propagate );
			
			Panel->Init();
		}
		
		virtual void ToggleRelax()
		{
			relaxing = !relaxing;
			rbtn->toggle = relaxing;
		}
		
		virtual void ToggleSimulate()
		{
			simulating = !simulating;
			sbtn->toggle = simulating;
			RenderFrame->SetSimulate(simulating);
		}
		
		
		virtual void OnDraw()
		{
			TUI_WFrame::OnDraw();
		}
		
		
		virtual void OnResize()
		{
			RenderFrame->SetBounds(0, Vec2i(Size.x,Size.y-Panel->Size.y ) );
			Panel->SetPos( Vec2i(0,RenderFrame->Size.y) );
		};
};

