class TUI_BitmapButton : public TUI_Button
{
	public:
		TBitmapTex bitmap;

		TUI_BitmapButton(TUI_ParentLess* parent) : TUI_Button(parent)
		{

		};
		
		
		~TUI_BitmapButton()
		{
			
		};
		
		
		virtual void LoadTexture(char* filename)
		{
			bitmap.LoadTexture(filename);
			//if (modulate) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
		};
				
		virtual void AutoSize()
		{
			SetSize( bitmap.Size );	
		}
		
		virtual void OnDraw()
		{
			IRGB textcolor;
			IRGB bgcolor;
			IRGB bitcolor;
			
			if (toggle)
			{
				textcolor = UI_Text_Dark;
				bgcolor = UI_BG_Dark;
				bitcolor = UI_BG_Light;
			}
			else
			{
				if (presents)
				{
					if (hold)
					{
						textcolor = UI_Text_Dark;
						bgcolor = UI_BG_Dark;
						bitcolor = UI_BG_Light;
					} 
					else
					{
						textcolor = UI_Text_Light;
						bgcolor = UI_BG_Light;
						bitcolor = UI_BG_Bright;
					}
				} else
				{
					textcolor = UI_Text;
					bgcolor = UI_BG_Medium;
					bitcolor = UI_BG_Bright;
				}
			};
			
			glColor(bgcolor);
			drawrect(0,Size);
						
			if (boarder) drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
		
			glColor(textcolor);
			drawtext(textpos, Size.y*0.5 + MainFont_Height*0.5, caption);
							
			glColor(bitcolor);
			drawrect(0,Size,bitmap.texture);				
		}
	
};
