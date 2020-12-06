class TUI_Bitmap : public TUI_Childless
{
	public:
		TBitmapTex bitmap;
		
		TUI_Bitmap(TUI_ParentLess* parent) : TUI_Childless(parent)
		{
			
		};
		
		virtual void LoadTexture(char* filename)
		{
			bitmap.LoadTexture(filename);
		};
		
		virtual void OnDraw()
		{
			glEnable( GL_TEXTURE_2D );
			
			glBindTexture( GL_TEXTURE_2D, bitmap.texture );
				
							glColor3f(1,0,0);
				glBegin( GL_QUADS );
					glTexCoord2d(0.0,1.0); glVertex2d(0,0);
					glTexCoord2d(1.0,1.0); glVertex2d(Size.x,0);
					glTexCoord2d(1.0,0.0); glVertex2d(Size.x,Size.y);
					glTexCoord2d(0.0,0.0); glVertex2d(0,Size.y);
				glEnd();
				
			glDisable( GL_TEXTURE_2D );
		};
};
