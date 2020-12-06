//############# PARAMETER FLAGS

class IParBox_Parameter
{
	public:
	void* var_ptr;
	void* copy_from;
	char* caption;
	bool valid;
	Delegate deleg;
	IParBox_Parameter(){caption=0; var_ptr=0;copy_from=0;valid=0;};
	template< class T, class U > void Connect( T* i_class, U i_method )
	{
		deleg.Connect(i_class, i_method);
	}
	virtual void Transmit(){};
};

template< class T, class F > class ParBox_Parameter : public IParBox_Parameter
{
	public:
		
	T* to;
	F* from;
	ParBox_Parameter(){from = 0; to = 0;};
	ParBox_Parameter(T* ito, F* ifrom){from = ifrom; to = ito; valid = 1;};
	virtual void Transmit()
	{
		if (!to || !from || !valid) return;
		*to = static_cast<T>(*from);
		if (deleg.Connected()) deleg();	
	};
};


class TUI_ParameterBox : public TUI_SimpleFrame
{
	
	private: 
		vector<IParBox_Parameter*> parameters;
		vector<TUI_ValueBox*> valboxes;
		vector<TUI_ContextButton*> cbuttons;
		vector<TUI_Label*> labels;
		vector<TUI_ParameterBox*> paramboxes;
		vector<int> paramboxes_oldsize;
		TUI_Button* button;
		TUI_SimpleFrame* Container;
		int cury;
		TBitmapTex arrow_up;
		TBitmapTex arrow_down;
	public:
		bool visible;
		int bar_height;
		int con_height;
		int con_height_min;
	
		TUI_ParameterBox(TUI_ParentLess* parent) : TUI_SimpleFrame(parent)
		{
			button = new TUI_Button(this);
			button->SetOrientation(-1);
			button->sevents.Connect(HOOK_OnClick,this,&TUI_ParameterBox::Toggle);
			bar_height = 18;
			con_height = 10;
			con_height_min = 10;
			visible = false;
			Container = new TUI_SimpleFrame(this);
			SetCaption("TUI_ParameterBox");
			cury = 0;
			Container->SetVisible(visible);
			arrow_up.LoadTexture("arrow_up");
			arrow_down.LoadTexture("arrow_down"); 
		};

		~TUI_ParameterBox()
		{
			for (auto &v : parameters) delete v;
			//ClearParameters();
		};
		
		virtual void OnDraw()
		{
			if (visible) 
			{
				glColor(UI_BG_Dark);
				drawrect(0,bar_height);
				glColor(UI_BG_Light);
				drawrect(4,arrow_up);
			} else
			{
				glColor(UI_BG_Medium);
				drawrect(0,bar_height);
				glColor(UI_BG_Bright);
				drawrect(4,arrow_down);	
			};
			drawbevel(UI_Outline,UI_Outline_Shadow,0,Size);
			drawbevel(UI_Outline,UI_Outline_Shadow,0,bar_height);
		};
		
		virtual void OnResize()
		{
			button->SetBounds(Vec2i(bar_height,0),Vec2i(Size.x-bar_height,bar_height));
			Container->SetBounds( Vec2i(0,bar_height), Vec2i(Size.x,con_height));
		};
		
		virtual void SetCaption( string text )
		{
			button->SetCaption(text);
		};
		
		virtual void Show()
		{
			visible = true;
			Container->SetVisible(true);
			SetRealSize(Vec2i(Size.x,con_height+bar_height));
		};
		
		virtual void Hide()
		{
			visible = false;
			Container->SetVisible(false);
			SetRealSize(Vec2i(Size.x,bar_height));
			Update();
		};

		virtual void SetSize(Vec2i isize)
		{
			bar_height = isize.y;
			SetRealSize(isize);
		}
		
		virtual void SetBounds( Vec2i ipos, Vec2i isize)
		{
			bar_height = isize.y;
			SetRealBounds(ipos,isize);
		}
		
		virtual void SetRealSize(Vec2i size)
		{
			TUI_SimpleFrame::SetSize(size);	
		};
		
		virtual void SetRealBounds(Vec2i pos, Vec2i size)
		{
			TUI_SimpleFrame::SetBounds(pos,size);	
		};
		
		virtual void SetVisible( bool ivis )
		{
			if (ivis) Show(); else Hide();
			Update();
		};
		
		virtual void SetContainterHeight( int height )
		{
			con_height = height;
			Container->SetSize(Vec2i(Size.x,height));	
			if (visible) Show();
		};
		
		template<class T> inline unsigned int GetType()
		{
			return 0;
		};
		
		virtual void EnableTransmit()
		{
			for (auto &v : parameters) v->valid = true;
		};
		
		virtual void DisableTransmit()
		{
			for (auto &v : parameters) v->valid = false;
		};
		
		template<class T> TUI_Childless* AddParameter(T* param, char* caption, unsigned int type = 0)
		{
			if (type == 0) type = GetType<T>();
			TUI_Childless* elem = 0;
			
			if (type == PARAM_VALUEBOX)
			{
				cury+=6; // top padding;
				valboxes.push_back( new TUI_ValueBox(Container) );
				elem = valboxes.back();
				valboxes.back()->SetBounds(Vec2i(Size.x-70,cury),Vec2i(60,16));
				valboxes.back()->SetValue(static_cast<float>(*param));
				labels.push_back( new TUI_Label(Container) );
				labels.back()->SetBounds(Vec2i(10,cury),Vec2i( valboxes.back()->Position.x-20,16 ) );
				labels.back()->SetOrientation(-1);
				labels.back()->SetText(caption);
				parameters.push_back( new ParBox_Parameter<T,float>(param,&valboxes.back()->value) ); 
				parameters.back()->caption = caption;
				valboxes.back()->sevents.Connect(HOOK_OnEnter,parameters.back(),&IParBox_Parameter::Transmit);
				cury+=16; // bottom padding;
			};
			
			if (type == PARAM_CONTEXTBUTTON)
			{
				cury+=6;
				cbuttons.push_back( new TUI_ContextButton(Container) );
				elem = cbuttons.back();
				labels.push_back( new TUI_Label(Container) );
				labels.back()->SetOrientation(0);
				labels.back()->SetText(caption);
				
				labels.back()->SetBounds(Vec2i(5,cury),Vec2i( Size.x-10,16 ) );
				cury+=20;
				cbuttons.back()->SetBounds(Vec2i(5,cury),Vec2i(Size.x-10,16));
				cury+=16; 
				
				parameters.push_back( new ParBox_Parameter<T,int>(param,&cbuttons.back()->c_context) ); 
				parameters.back()->caption = caption;
				cbuttons.back()->sevents.Connect(HOOK_OnChange,parameters.back(),&IParBox_Parameter::Transmit);
			};
			
			if (type == PARAM_PARAM)
			{
				cury+=6;
				paramboxes.push_back( new TUI_ParameterBox(Container) );
				paramboxes_oldsize.push_back( 16 );
				elem = paramboxes.back();
				paramboxes.back()->SetBounds( Vec2i(5,cury), Vec2i(Size.x-10,16) );
				paramboxes.back()->SetCaption(caption);
				paramboxes.back()->sevents.Connect(HOOK_OnResize, this, &TUI_ParameterBox::ParBoxToggle);
				cury+=16;
			};
			
			SetContainterHeight(cury+6);
			return elem;
		};
		
		virtual void ParBoxToggle()
		{
			int delta = 0;
			int total_delta = 0;
			int pos;
			for (int i = 0; i != paramboxes.size(); ++i)
			{
				delta = paramboxes[i]->Size.y-paramboxes_oldsize[i];
				if (delta != 0)
				{
					total_delta += delta;
					paramboxes_oldsize[i]=paramboxes[i]->Size.y;
					pos = paramboxes[i]->Position.y;
					TUI_Childless** elem;
					TUI_Childless** end;
					#define check( vec )	\
					for (elem = (TUI_Childless**)&vec[0], end = (TUI_Childless**)&vec[vec.size()]; elem != end; ++elem) 	\
						if ((*elem)->Position.y > pos) (*elem)->SetPos((*elem)->Position+Vec2i(0,delta));
					check( valboxes );
					check( cbuttons );
					check( labels );
					check( paramboxes );
				};
			};
			cury += total_delta;
			SetContainterHeight(cury+6);
		};
		
		template< class T, class M, class U > TUI_Childless* AddParameter(T* param, char* caption, M* i_class, U i_method )
		{
			TUI_Childless* result = AddParameter(param,caption);
			parameters.back()->Connect(i_class,i_method);
			return result;
		};
		
		template< class T, class M, class U > TUI_Childless* AddParameter(T* param, char* caption, M* i_class, U i_method, unsigned int flags )
		{
			TUI_Childless* result = AddParameter(param,caption,flags);
			parameters.back()->Connect(i_class,i_method);
			return result;
		};
		
		void ClearParameters()
		{
			cury = 0;
			for (auto &v : valboxes) v->Suicide();
			for (auto &v : labels) v->Suicide();
			for (auto &v : cbuttons) v->Suicide();
			for (auto &v : paramboxes) v->Suicide();
			for (auto &v : parameters) delete v;
			valboxes.clear();
			parameters.clear();
			labels.clear();
			cbuttons.clear();
			paramboxes.clear();
			paramboxes_oldsize.clear();
			SetContainterHeight(con_height_min);
		};
		
		virtual void Toggle()
		{
			visible = !visible;
			SetVisible(visible);
		};
};

template <> inline unsigned int TUI_ParameterBox::GetType<int>() 
{
	return PARAM_VALUEBOX;
}

template <> inline unsigned int TUI_ParameterBox::GetType<float>() 
{
	return PARAM_VALUEBOX;
}
