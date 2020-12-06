class TPaintTool_Base
{
	public:
		
		Delegate FillCell;
		
	virtual void StartPaint(Vec2i mpos){};
	virtual void Paint(Vec2i mpos){};
	virtual void EndPaint(Vec2i mpos){};
	virtual void ThinkPaint(Vec2i mpos){};
	virtual void RegisterParameters( TUI_ParameterBox* parbox ) {};
	template< class T, class U > void Connect( T* i_class, U i_method )
	{
		FillCell.Connect(i_class, i_method);
	}
};

class TPaintTool_Pen : public TPaintTool_Base
{
	public:
		Vec2i oldpos;

		int width;
		float interval_percent;
		int interval;
		int interval_counter;
		
	TPaintTool_Pen()
	{
		width = 0;
		interval_percent = 0.5;
		interval = 0;
		interval_counter = 0;
	};
		
	virtual void StartPaint(Vec2i mpos)
	{
		GetAsyncKeyState(0x10);
		if (!GetAsyncKeyState(0x10)) oldpos = mpos;
		Paint(mpos);
	}
	
	void inline DrawPixel(int x, int y, IArguments* args)
	{
		static_cast<Arguments<Vec2i>*>(args)->arg1.x = x;
		static_cast<Arguments<Vec2i>*>(args)->arg1.y = y;
		FillCell(args);
	};
	
	virtual void PaintPoint( IArguments* args, int radius )
	{
		int x0 = static_cast<Arguments<Vec2i>*>(args)->arg1.x;
		int y0 = static_cast<Arguments<Vec2i>*>(args)->arg1.y;
		
	    int x = radius;
	    int y = 0;
	    int xChange = 1 - (radius << 1);
	    int yChange = 0;
	    int radiusError = 0;

	    while (x >= y)
	    {
	    	for (int i = x0 - x; i <= x0 + x; i++)
	        {
	            DrawPixel(i, y0 + y,args);
	            DrawPixel(i, y0 - y,args);
	        }
	        for (int i = x0 - y; i <= x0 + y; i++)
	        {
	            DrawPixel(i, y0 + x,args);
	            DrawPixel(i, y0 - x,args);
	        }
	
	        y++;
	        radiusError += yChange;
	        yChange += 2;
	        if (((radiusError << 1) + xChange) > 0)
	        {
	            x--;
	            radiusError += xChange;
	            xChange += 2;
	        }
	    }

		static_cast<Arguments<Vec2i>*>(args)->arg1.x = x0;
		static_cast<Arguments<Vec2i>*>(args)->arg1.y = y0;
	};
	virtual void Paint(Vec2i mpos)
	{	
		if (!FillCell.Connected()) return;

	    const int deltaX = abs(mpos.x - oldpos.x);
	    const int deltaY = abs(mpos.y - oldpos.y);
	    const int signX = oldpos.x < mpos.x ? 1 : -1;
	    const int signY = oldpos.y < mpos.y ? 1 : -1;
	    //
	    int error = deltaX - deltaY;
	    //
	    Arguments<Vec2i> args( mpos );
	    PaintPoint(&args,width);
	    
	    while(oldpos.x != mpos.x || oldpos.y != mpos.y) {
	    	++interval_counter;
	    	if (interval_counter >= interval)
	    	{
	    		static_cast<Arguments<Vec2i>*>(&args)->arg1 = oldpos;
	        	PaintPoint(&args,width);
	        	interval_counter = 0;
	    	};
	        const int error2 = error * 2;
	        //
	        if(error2 > -deltaY) {
	            error -= deltaY;
	            oldpos.x += signX;
	        }
	        if(error2 < deltaX) {
	            error += deltaX;
	            oldpos.y += signY;
	        }
	    };
	    oldpos = mpos;
	}
	
	virtual void EndPaint(Vec2i mpos)
	{
		
	}
	
	virtual void OnWidthChange()
	{
		interval = width*interval_percent;
		interval_counter = 0;
	};
	
	virtual void RegisterParameters( TUI_ParameterBox* parbox )
	{
		if (parbox == 0) return;
		parbox->AddParameter(&width,"width",this, &TPaintTool_Pen::OnWidthChange);
	};

};

class TPaintTool_Spray : public TPaintTool_Base
{
	public:
		float spray_radius;
		float spray_rate;
		default_random_engine generator;
		normal_distribution<float> distribution;
		Vec2i oldpos;
		int counter; // variable for inside use
		int count;	// how much paint loops dont paint
		
	TPaintTool_Spray()
	{
		spray_rate = 1000;
		spray_radius = 20;
		counter = 0;
		count = 0;
		normal_distribution<float> d1(0,spray_radius);
  		distribution.param(d1.param());
	};
	
	virtual void StartPaint(Vec2i mpos)
	{
		oldpos = mpos;
		Paint(mpos);
	}
	
	virtual void Paint(Vec2i mpos)
	{	

	};
	
	virtual void EndPaint(Vec2i mpos){}
	
	virtual void PaintPoint( IArguments* args , int rate)
	{
		Vec2i mpos = static_cast<Arguments<Vec2i>*>(args)->arg1;
		for (int i = 0; i != rate; ++i)
		{
			static_cast<Arguments<Vec2i>*>(args)->arg1 = mpos+Vec2i(distribution(generator),distribution(generator));
	    	FillCell(args);
		}	
		static_cast<Arguments<Vec2i>*>(args)->arg1 = mpos;
	};
	
	virtual void ThinkPaint(Vec2i mpos)
	{
			if (!FillCell.Connected()) return;
			
		    const int deltaX = abs(mpos.x - oldpos.x);
		    const int deltaY = abs(mpos.y - oldpos.y);
		    const int signX = oldpos.x < mpos.x ? 1 : -1;
		    const int signY = oldpos.y < mpos.y ? 1 : -1;
		    //
		    int error = deltaX - deltaY;		    
		    float rate;
			if (oldpos == mpos) rate = spray_rate; else rate = (float)spray_rate/(oldpos-mpos).Length();
			if (rate < 1) count = 1.0f/rate; else count = 0;
			IArguments* args = new Arguments<Vec2i>( oldpos );
			
		    while(oldpos.x != mpos.x || oldpos.y != mpos.y) {
		    	static_cast<Arguments<Vec2i>*>(args)->arg1 = oldpos;
		    	if (count == 0) PaintPoint(args,rate); else
		    		if (counter < count) counter++; else { PaintPoint(args,1); counter = 0; };
		        const int error2 = error * 2;
		        //
		        if(error2 > -deltaY) {
		            error -= deltaY;
		            oldpos.x += signX;
		        }
		        if(error2 < deltaX) {
		            error += deltaX;
		            oldpos.y += signY;
		        }
		    };
		    oldpos = mpos;
		    static_cast<Arguments<Vec2i>*>(args)->arg1 = oldpos;
			if (count == 0) PaintPoint(args,rate); else
				if (counter < count) counter++; else { PaintPoint(args,1); counter = 0; };
			delete args;
	};
	
	virtual void OnRadiusChange()
	{
		normal_distribution<float> d1(0,spray_radius);
  		distribution.param(d1.param());
	};

	virtual void RegisterParameters( TUI_ParameterBox* parbox )
	{
		if (parbox == 0) return;
		parbox->AddParameter(&spray_radius,"radius",this,&TPaintTool_Spray::OnRadiusChange);
		parbox->AddParameter(&spray_rate,"rate");
	};
	
};

