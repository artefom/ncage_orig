class CColor
{
	public:
		CColor() : id(0) {};
		CColor(unsigned char iid) : id(iid) {};
		unsigned char id;	
		//unsigned char id2;	
		//unsigned char id3;	
};
const int World_Bytes_PerPixel = 1;

class TWorld_Base
{
	public:
		static const int bounds = World_Bounds;
        static const int bounds_max = World_Bounds-1;
        static const int matSize = World_Bounds*World_Bounds;
        static const int texSize = World_Bounds*World_Bounds*World_Bytes_PerPixel;
	public:
		TBaseEntity* Cell[bounds][bounds];
	public:
    	Events<TBaseEntity*> events;
    	
        vector<TBaseEntity*> ents;
        vector<TBaseEntity*> thinkers;
        vector<TBaseEntity*> drawers;
        vector<TBaseEntity*> tickers;
        
        TBaseEntity** MouseCell;
        TBaseEntity** PressedEnt;
        //TBaseEntity* dummy;
        
        
		GLuint ProgramObject;
        GLuint texLocation;
        GLuint indLocation;
        GLuint ScrWLocation;
        GLuint ScrHLocation;
        
    	GLuint absgridsizeLocation;
        GLuint TransXLocation;
        GLuint TransYLocation;
        GLuint HalfBoundsLocation;
        
        Vec2i TexturePos1;
        Vec2i TexturePos2;
        Vec2i TextureOverlapPos1;
        Vec2i TextureOverlapPos2;
        float overlap1;
        float overlap2;
        
        GLuint renderTexID;   
        GLuint colorindexTexID; 
		GLuint W_PBO;
        
        Vec2d Position;
        Vec2d Size;
        
        float absgridsize;
        float gridsize;
        float SCALE;
        float SCALE_MIN;
        float SCALE_MAX;
        float grid_min_scale;
        
        Vec2d TRANS;
        int highlighteach;
        
        bool dragging;
        bool hasmouse;
        
        
        bool rendering;
        
        IRGB gridcolor;
        IRGB highlightcolor;
        
        CColor ClearColor;
        
        public: CColor (*pboMemory)[bounds][bounds];
		public:
			
	TWorld_Base()
	{	
	
	
        ents.reserve(1024);
        thinkers.reserve(1024);
        drawers.reserve(1024);
    	tickers.reserve(1024);
		dragging = false;
        hasmouse = false;
        pboMemory = 0;
		for (int i = 0; i != bounds; i++)
		{
			for (int j = 0; j != bounds; j++) Cell[i][j] = 0;
		};
		//dummy = new TBaseEntity();
		/*
		float ColorTable[256][3];
		for (int i=0;i<256;i++)
		{
			// initialize to 0
			ColorTable[i][0] = 0.0f;
			ColorTable[i][1] = 0.0f;
	    	ColorTable[i][2] = 0.0f;
		};
		glColorTable(GL_COLOR_TABLE, GL_RGB, 256, GL_RGB, GL_FLOAT, ColorTable);
		*/
		//glEnable(GL_SHARED_TEXTURE_PALETTE_EXT);
		//glColorTableEXT(GL_SHARED_TEXTURE_PALETTE_EXT,GL_RGBA8,256,GL_RGBA,GL_UNSIGNED_BYTE,ctable);
	};
	
	virtual void Init()
	{

		
		ProgramObject = LoadShader( (char*)("./shader.txt") );
		if (ProgramObject != -1)
		{
			texLocation = glGetUniformLocation( ProgramObject , "texture");
			indLocation = glGetUniformLocation( ProgramObject , "indexes");
		
			ScrWLocation = glGetUniformLocation( ProgramObject , "ScrW");
			ScrHLocation = glGetUniformLocation( ProgramObject , "ScrH");
			absgridsizeLocation = glGetUniformLocation( ProgramObject , "absgridsize" );
			TransXLocation = glGetUniformLocation( ProgramObject , "TransX" );
			TransYLocation = glGetUniformLocation( ProgramObject , "TransY" );
			HalfBoundsLocation = glGetUniformLocation( ProgramObject , "World_Halfbounds" );
		} else cout << "SHADER LOADING FAIL." << endl;
		
		UpdateTextureCoordinates();
		GenBuffers();
	}
	
    void SetSize(Vec2i size)
    {
    	Size.x = size.x;
    	Size.y = size.y;
    };
    
    void GenBuffers()
    {
    	//cout << "not here1" << endl;
    	CColor* data = new CColor[matSize];
		CColor* ptr = (CColor*)data;
		for (int i = 0; i != matSize; i++)
		{
			*ptr = ClearColor;
			ptr++;
		};
		
		
		glGenTextures(1, &renderTexID);
		//glUniform1i(texLocation, renderTexID);
		cout << "renderTexID = " << renderTexID << endl;
		
		glBindTexture(GL_TEXTURE_2D, renderTexID);
	
    	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
    	//cout << "not here2" << endl;
    	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, bounds, bounds, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
		//cout << "not here3" << endl;
		glGenBuffersARB(1,&W_PBO); 
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, W_PBO);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, texSize, data, GL_STATIC_DRAW );
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);	
		//cout << "not here4" << endl;
		delete[] data;

		glGenTextures(1, &colorindexTexID);
		glBindTexture(GL_TEXTURE_1D, colorindexTexID);
		glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, ColorIndexes);
    	glBindTexture(GL_TEXTURE_1D, 0);
    };
    
    
    void Move(Vec2d delta)
    {
    	SetTrans(TRANS+delta);
    	UpdateTextureCoordinates();
    }
    
    void SetTrans(Vec2d pos)
    {
    	TRANS = pos;
    	events.Call(HOOK_WorldTranslate);
    	ui_events.Call(HOOK_WorldTranslate);
    }
    
    void SetScale(float sc)
    {
    	SCALE = sc;
    	if (SCALE > SCALE_MAX) 
		{
			SCALE = SCALE_MAX; 
		}else
    	if (SCALE < SCALE_MIN) 
		{
    		SCALE = SCALE_MIN;
		}
    	absgridsize = SCALE*gridsize;
    }
    
    float Scale(float a) // returns clamped a (if Scale is maxed and a > 0 returns 0}
    {
		float new_SCALE = SCALE*a;
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
    void RESCALE(float a)
    {
        Vec2d mpos;
        mpos = MousePos-TRANS-Position;
        Move( mpos*(1-Scale(a) ));
    };

    void UpdateTextureCoordinates()
    {
    	Vec2i tSize = Vec2i(bounds,bounds)*absgridsize*0.5;
        TexturePos1 = TRANS-tSize;
    	TexturePos2 = TRANS+tSize;		
    	
    	tSize = tSize*(overlap2-overlap1);
    	TextureOverlapPos1 = TRANS-tSize;
    	TextureOverlapPos2 = TRANS+tSize;
    }
    	
	void PutPixel(Vec2i pos, CColor color)
	{
		(*pboMemory)[pos.x][pos.y] = color; 
	}
	
	void PutPixel(CColor* pix, CColor color)
	{
		*pix = color; 
	}
	
	CColor* GetPixel(Vec2i pos)
	{
		return &(*pboMemory)[pos.x][pos.y];
	}
	
	CColor ReadPixel(Vec2i pos)
	{
		return (*pboMemory)[pos.x][pos.y];
	}
	
	void drawtex(Vec2i TexturePos1, Vec2i TexturePos2)
	{
		glBegin( GL_QUADS );
			glTexCoord2f(overlap1,overlap1); 
				glVertex2f(TexturePos1.x,TexturePos1.y);
			glTexCoord2f(overlap1,overlap2); 
				glVertex2f(TexturePos2.x,TexturePos1.y);
			glTexCoord2f(overlap2,overlap2); 
				glVertex2f(TexturePos2.x,TexturePos2.y);
			glTexCoord2f(overlap2,overlap1); 
				glVertex2f(TexturePos1.x,TexturePos2.y);
		glEnd();		
	}
    
    Vec2i ScreenToCell(Vec2d pos)
    {
    	Vec2i Cell = pos-TRANS;
    	Cell.x = fast_floor(Cell.x / absgridsize + bounds*0.5 ) % bounds;
		Cell.y = fast_floor(Cell.y / absgridsize + bounds*0.5 ) % bounds;
		if (Cell.x < 0) Cell.x += bounds;
		if (Cell.y < 0) Cell.y += bounds;
    	return Cell;
    }
    
    Vec2i ScreenToCell_Unclamped(Vec2d pos)
    {
    	Vec2i Cell = pos-TRANS;
    	Cell.x = fast_floor( Cell.x / absgridsize + bounds*0.5 );
		Cell.y = fast_floor( Cell.y / absgridsize + bounds*0.5 );
    	return Cell;
    }
    
    Vec2i BoundsClamp(Vec2i pos)
    {
		while (pos.x < 0) pos.x += bounds;
		while (pos.y < 0) pos.y += bounds;
		while (pos.x >= bounds) pos.x -=bounds;
		while (pos.y >= bounds) pos.y -=bounds;
		return pos; 
	};
    
    inline Vec2i BoundsClamp_cheap(Vec2i pos)
    {
		if (pos.x < 0) pos.x += bounds; else {if (pos.x >= bounds) pos.x -= bounds;};
		if (pos.y < 0) pos.y += bounds; else {if (pos.y >= bounds) pos.y -= bounds;};
    	return pos;
    }
    
    virtual void Cleanup() = 0;
    
    void Render()
	{
	glBindTexture(GL_TEXTURE_2D, renderTexID);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, W_PBO);
	
	//pboMemory = ( IRGB (*)[World_Bounds][World_Bounds] ) glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY);
	pboMemory = ( CColor (*)[World_Bounds][World_Bounds] )
	glMapBufferRange(GL_PIXEL_UNPACK_BUFFER_ARB , 0 , texSize , GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	//glMapBufferRange(GL_PIXEL_UNPACK_BUFFER_ARB , 0 , texSize , GL_WRITE_ONLY );	
	
	    //for (auto it = ents.begin(); it != ents.end(); it++)
	    //{
	    //    (*it)->Draw_PBO();
	    //}
        
	glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bounds, bounds, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bounds, bounds, 0, GL_RGB,GL_UNSIGNED_BYTE, 0);
	
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);	                               
						
    glUseProgram(ProgramObject);
    
	glUniform1i(ScrWLocation, Size.x);
    glUniform1i(ScrHLocation, Size.y);
    glUniform1i(HalfBoundsLocation,bounds*0.5);
	glUniform1f(absgridsizeLocation, absgridsize);
	glUniform1f(TransXLocation, TRANS.x );
	glUniform1f(TransYLocation, TRANS.y );          
	glUniform1i(texLocation, 0);
	glUniform1i(indLocation, 1);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, colorindexTexID);
	
	//glBindTexture(GL_TEXTURE_1D, 0);
	//glDisable(GL_TEXTURE_1D);
	//glUniform1i(texLocation, renderTexID );
	//glUniform1i(indLocation, colorindexTexID);
		
		//glBindTexture( GL_TEXTURE_2D, renderTexID );	
		//glColor4f(1,1,1,0.5);
		//drawtex(TextureOverlapPos1,TextureOverlapPos2);
		//glColor4f(1,1,1,1);
		
		glBegin( GL_QUADS );
				glVertex2f(TexturePos1.x,TexturePos1.y);
				glVertex2f(TexturePos2.x,TexturePos1.y); 
				glVertex2f(TexturePos2.x,TexturePos2.y);
				glVertex2f(TexturePos1.x,TexturePos2.y);
		glEnd();	
	
	glUseProgram(0);
	
	glActiveTextureARB(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, 0);
	glDisable(GL_TEXTURE_1D);
	
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glBindTexture(GL_TEXTURE_1D, 0);
	//glDisable( GL_TEXTURE_2D );
	
	//glDisable( GL_TEXTURE_1D );
	
	//glActiveTextureARB(GL_TEXTURE0);
/* Normal texture draw (instead shader draw)
	glEnable( GL_TEXTURE_2D );
		
		glBindTexture( GL_TEXTURE_2D, renderTexID );	
		glColor4f(1,1,1,0.5);
		drawtex(TextureOverlapPos1,TextureOverlapPos2);
		glColor4f(1,1,1,1);
		
		glBegin( GL_QUADS );
			glTexCoord2f(0,0); 
				glVertex2f(TexturePos1.x,TexturePos1.y);
			glTexCoord2f(0,1); 
				glVertex2f(TexturePos2.x,TexturePos1.y);
			glTexCoord2f(1,1); 
				glVertex2f(TexturePos2.x,TexturePos2.y);
			glTexCoord2f(1,0); 
				glVertex2f(TexturePos1.x,TexturePos2.y);
		glEnd();	
	glDisable( GL_TEXTURE_2D );
*/
	};
    
    void Draw(int delta)
    {
    	glColor3f(0,0,0);
    	drawrect(0,Size);

    	Render();
    	
    	float localgridsize;
        Size = Vector2d(ScrW,ScrH);
        
        glBegin(GL_LINES);
            
            localgridsize = gridsize;
                while(localgridsize*SCALE < grid_min_scale) {
                    localgridsize*=2;
                };
            glColor(gridcolor, Clamp(SCALE*0.0625,0,0.5));
            drawGrid(&Position,&Size,&TRANS,SCALE,localgridsize);
            
                localgridsize = gridsize*16;
                while(localgridsize*SCALE < grid_min_scale) {
                    localgridsize*=2;
                };
            glColor(gridcolor, Clamp(SCALE*0.25,0,0.5));
            drawGrid(&Position,&Size,&TRANS,SCALE,localgridsize);
            
            localgridsize = gridsize*256;
                while(localgridsize*SCALE < grid_min_scale) {
                    localgridsize*=2;
                };
            glColor( gridcolor, Clamp(SCALE,0,0.5) );
            drawGrid(&Position,&Size,&TRANS,SCALE,localgridsize);
            
            
            glColor(IRGB(60,130,160));
            if (TRANS.x > 0 & TRANS.x < ScrW) {glVertex2f(TRANS.x,Position.y); glVertex2f(TRANS.x,Size.y);};
            if (TRANS.y > 0 & TRANS.y < ScrH) {glVertex2f(Position.x,TRANS.y); glVertex2f(Size.x,TRANS.y);};
        glEnd();
        
        glColor3f(0.63,0.5,0.23);
        drawhrect(TexturePos1,TexturePos2);
        glColor3f(0.1,0.1,0.1);
		drawhrect(TextureOverlapPos1,TextureOverlapPos2);
    };

};
