class TUI_Window;

class TUI_Frame : public TUI_SimpleFrame
{
	public:
		GLuint texture;
		Vec2i texture_size;
		Vec2d texture_coords;
		GLuint fbo;
		bool update;
		bool scaling;
	TUI_Frame( TUI_ParentLess* parent ) : TUI_SimpleFrame(parent)
	{
		scaling = 0;
		update = 1;
		texture = 0;
		texture_coords = 1;
		texture_size = 1;
		fbo = 0;		
	}
	
	virtual ~TUI_Frame()
	{
		// Free the memory;
		if (texture) glDeleteTextures( 1, &texture );
		if (fbo) glDeleteFramebuffers(1, &fbo);
		//if ( IsBuffering() ) delete draw_ref;
	};

	virtual void Draw()
	{
		if (update)
		{
			update = 0;
			ScreenShot();
		}
		
		glPushMatrix();
		glTranslate( GetPos() );
		glColor4f(1,1,1,1);
		glEnable( GL_TEXTURE_2D );
		
			glBindTexture( GL_TEXTURE_2D, texture );
				
			glBegin( GL_QUADS );
				glTexCoord2d(0.0,texture_coords.y); glVertex2d(0,0);
				glTexCoord2d(texture_coords.x,texture_coords.y); glVertex2d(Size.x,0);
				glTexCoord2d(texture_coords.x,0.0); glVertex2d(Size.x,Size.y);
				glTexCoord2d(0.0,0.0); glVertex2d(0,Size.y);
			glEnd();
				
		glDisable( GL_TEXTURE_2D );

		glPopMatrix();
	}
	
	
	virtual void SetSize(Vec2i size)
	{
		Size = size;
		UpdateGlobalPosHelpers();
		if (!scaling) ResizeBuffer(size); else UpdateTextureCoords();
		OnResize();
	}
	
	virtual void SetBounds( Vec2i pos, Vec2i size)
	{
		Position = pos;
		Size = size;
		UpdateGlobalPosHelpers();
		if (!scaling) ResizeBuffer(size); else UpdateTextureCoords();
		OnResize();
	}

	virtual void SetupViewport()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glMatrixMode (GL_PROJECTION);
	    glLoadIdentity();
		glViewport(0, 0, Size.x, Size.y);
	    glOrtho (0, Size.x, Size.y, 0, -1, 1);
	    glTranslatef (0.375, 0.375, 0);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();
	};
	
	virtual void ResizeBuffer(Vec2i isize)
	{
		if (texture) glDeleteTextures( 1, &texture );
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
		//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, isize.x, isize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		texture_size = isize;
		if (fbo == 0)
		{
			glGenFramebuffers(1, &fbo);
		};
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0); //Only need to do this once.
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		UpdateTextureCoords();
		Update();
	
	}
	
	virtual void UpdateTextureCoords()
	{
		texture_coords.x = (float)Size.x/texture_size.x;
		texture_coords.y = (float)Size.y/texture_size.y;
		if ((texture_coords.x > 1) || (texture_coords.y > 1)) ResizeBuffer(Size*1.5);
		update = 1;
	};
	
	virtual void StartScaling()
	{
		scaling = true;
		ResizeBuffer(Size*1.5);
	};
	
	virtual void StopScaling()
	{
		scaling = false;
		ResizeBuffer(Size);
	};

	virtual void Update()
	{
		update = 1;
		Parent->Update();
	}
	
	virtual void ScreenShot()
	{
		//CreateBuffer();				
		//##############DRAWING
		
		SetupViewport();
			OnDraw();
			for (auto &it : elms) it->Draw();	
		Parent->SetupViewport();
	}
	
	virtual void OnDraw()
	{
		glColor(UI_BG_Dark);
		drawrect( 0,Size );
	}
};

