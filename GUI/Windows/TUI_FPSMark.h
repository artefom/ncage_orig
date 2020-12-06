
class TUI_FPSMark : public TUI_SimpleFrame
{
	public:
		TUI_Button* fpsMark;
		TUI_BitmapButton* pbtn;
		
		float fps;
		
		int oldtime;
		int frames;
		
		static const int avg = 20;
		int timepassed[avg];
		int iter;
		
		char* caption;
		
		TUI_FPSMark(TUI_ParentLess* parent) : TUI_SimpleFrame(parent)
		{
			fps = 0;
			
			fpsMark = new TUI_Button(this);
			fpsMark->SetOrientation(1);
			
			pbtn = new TUI_BitmapButton(this);
			pbtn->LoadTexture("pause");
			
			pbtn->sevents.Connect(HOOK_OnClick,this, &TUI_FPSMark::OnPausePress);
			
			for (int i = 0; i != avg; ++i)
			{
				timepassed[i] = 0;
			};
			iter = 0;
			
			caption = new char[20];
			caption[0] = 'F';
			caption[1] = 'P';
			caption[2] = 'S';
			caption[3] = ':';
			caption[4] = ' ';
			caption[5] = '9';
			caption[6] = '9';
			caption[7] = '9';
			caption[8] = 0;
			
			fpsMark->SetCaption(caption);
		};
		
		~TUI_FPSMark(){ delete[] caption; };
		
		virtual void OnResize()
		{
			pbtn->SetBounds( Vec2i(0,0), Size.y );
			fpsMark->SetBounds(Vec2i(Size.y-1,0), Size-Vec2i(Size.y-1,0) );
		}
		
		virtual void UpdateGlobalPosHelpers()
		{
			Position.x = Parent->Size.x - Size.x;
			TUI_SimpleFrame::UpdateGlobalPosHelpers();
		}
		
		virtual void OnPausePress()
		{
			pbtn->toggle = !pbtn->toggle;
			SetSim(!pbtn->toggle);
		}
		
		virtual void UpdateFPS()
		{
			int time = CurTime();
			timepassed[iter] = time;
			
			int delta = timepassed[iter];
			++iter; if (iter >= avg) iter = 0;
			delta -= timepassed[iter];
			fps = ((float)avg/delta)*1000;
			if (fps >= 999) fps = 999;
			itoch((int)fps,caption+5);
			caption[19] = 0;
			fpsMark->caption = caption;
		}
		
};
