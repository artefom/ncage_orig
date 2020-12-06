int& ui_graph_updaterate = Console.RegisterVar<int>( 10 , "ui_graph_updatedelay");
int& ui_graph_dense = Console.RegisterVar<int>( 100 , "ui_graph_dense");

class TGraphmem;

class TGraphmem_seg
{
	public:
		list<float> values;
		int size;
		int max_size;
		int buf_size;
		int buf_counter;
		float buf;
		TGraphmem_seg* next;
		TGraphmem* parent;
		
		TGraphmem_seg(TGraphmem* iparent, int imax_size, int ibuf_size );
		void push_back( float val );
		
};


class TGraphmem
{
	public:
		
	list<TGraphmem_seg> segs;
	int buf;
	int iterations;
		
	TGraphmem()
	{
		iterations = 50;
		buf = 2;
		segs.push_back( TGraphmem_seg(this,iterations,buf) );
	};
	
	void push_back( float val )
	{
		segs.back().push_back(val);
	};
	
	void clear()
	{
		segs.clear();
		segs.push_back( TGraphmem_seg(this,iterations,buf) );
	};
};

TGraphmem_seg::TGraphmem_seg(TGraphmem* iparent, int imax_size, int ibuf_size )
{
	parent = iparent;
	max_size = imax_size;
	buf_size = ibuf_size;
	size = 0;
	buf_counter = 0;
	buf = 0;
	next = 0;
};

void TGraphmem_seg::push_back( float val )
{
	if (size >= max_size)
	{
		buf+=values.front();
		++buf_counter;
		values.push_back(val);
		values.pop_front();
		if (buf_counter >= buf_size)
		{
			if (next == 0) {
				if (parent)
				{
					parent->segs.push_front(  TGraphmem_seg(parent,max_size,buf_size) );
					next = &parent->segs.front();
				} else
				{
					cout << "parent is void! something wrong, deleting" << endl;
					delete this;
					return;	
				};
			};
			buf = buf/buf_counter;
			next->push_back(buf);
			buf = 0;
			buf_counter = 0;
		};
	} else
	{
		++size;
		values.push_back(val);
	};
};
	
class TUI_Graph  : public TUI_SimpleFrame
{
	public:
		TGraphmem Graphmem;
		float maximum;
		int offset; // upper and down offset in pixels
		int delay;
		int lastupdate;
		float step;
		float mouseval;
		
		TUI_Graph( TUI_ParentLess* parent ) : TUI_SimpleFrame(parent)
		{
			maximum = 0;
			offset = 10;
			step = 4;
			delay = 0;
			lastupdate=0;
			mouseval = 0;
			lastupdate = GameTime();
		}
		
		
		virtual void push_back(float val)
		{			
			Graphmem.push_back(val);
			lastupdate = GameTime();
			Update();
		}
		
		virtual void OnResize()
		{
			//step = (float)Size.x*0.005;
		};
		
		virtual void OnDraw()
		{
			glColor(UI_BG_Medium);
			drawrect(0,Size);
			
			float curx;
			if ((delay != 0) && Simulating) curx = Size.x-step*( (float)(GameTime()-lastupdate)/delay );
			else curx = Size.x;
			int bottom = Size.y-offset;
			int size = Size.y-offset;
			float ostep = step; // step with offset;
			maximum = GetMaxValue();
			glColor(UI_Text_Medium);
			glLineWidth(2);
			glBegin(GL_LINE_STRIP);
			
			for (auto seg = Graphmem.segs.rbegin(); seg != Graphmem.segs.rend(); ++seg)
			{
				for (auto val = seg->values.rbegin(); val != seg->values.rend(); ++val)
				{
					glVertex2f(curx,Size.y*(1-(*val)/maximum));
					if (curx < 0) goto endloop;
					curx -= ostep;
				};
				ostep*=Graphmem.buf;
				if (seg->buf_counter > 0) glVertex2f(curx,Size.y*(1-(seg->buf/seg->buf_counter)/maximum));
				curx=curx-(ostep*seg->buf_counter/seg->buf_size);
			};
			endloop:;
				
			glEnd();
			
			glColor(UI_BG_Bright);
			glLineWidth(1);
			if ((delay != 0) && Simulating) curx = Size.x-step*( (float)(GameTime()-lastupdate)/delay );
			else curx = Size.x;
			ostep = step;
			glBegin(GL_LINES);
			for (auto seg = Graphmem.segs.rbegin(); seg != Graphmem.segs.rend(); ++seg)
			{
				for (auto val = seg->values.rbegin(); val != seg->values.rend(); ++val)
				{
					if (curx < 0) goto endloop2;
					curx -= ostep;
				};
				ostep*=Graphmem.buf;
				curx=curx-(ostep*seg->buf_counter/seg->buf_size);
				glVertex2f(curx,0);
				glVertex2f(curx,Size.y);
			};
			endloop2:;
			glEnd();
			
			glLineWidth(1);
			
			glColor(UI_Text);
			if (GlobalMouseGui == this)
			{
				mouseval = GetVal( GPos2.x - MousePos.x);
				fcircle(MousePos.x - GPos1.x,Size.y*(1-(mouseval)/maximum),3);
				char str[50];
				snprintf(str,50,"%g",mouseval);
				drawtext(MousePos.x-GPos1.x,MousePos.y-GPos1.y-MainFont_Height*1.5,str);
			};
			Update();
		};
				
		virtual float GetMaxValue()
		{
			float result = 0;
			
			float curx;
			if ((delay != 0) && Simulating) curx = Size.x-step*( (float)(GameTime()-lastupdate)/delay );
			else curx = Size.x;
			float ostep = step;
			
			for (auto seg = Graphmem.segs.rbegin(); seg != Graphmem.segs.rend(); ++seg)
			{
				for (auto val = seg->values.rbegin(); val != seg->values.rend(); ++val)
				{
					if (result < *val) result = *val;
					if (curx < 0) goto endloop;
					curx -= ostep;
				};
				ostep*=Graphmem.buf;
				if ((seg->buf_counter > 0) && (result < seg->buf/seg->buf_counter)) result = seg->buf/seg->buf_counter;
				curx=curx-(ostep*seg->buf_counter/seg->buf_size);
			};
			endloop:;
			return result;
		}
		
		virtual float GetVal(float x)
		{
			float ostep = step; // step with offset;
			float curx;
			
			if ((delay != 0) && Simulating) curx = step*( (float)(GameTime()-lastupdate)/delay );
			else curx = 0;
			
			float curx_old = 0;
			int seg_id = 0;
			for (auto seg = Graphmem.segs.rbegin(); seg != Graphmem.segs.rend(); ++seg)
			{
				curx_old = curx;
				curx+=ostep*Graphmem.iterations;
				ostep*=Graphmem.buf;
				curx=curx+(ostep*seg->buf_counter/seg->buf_size);
				if (curx > x) 
				{
					curx = x-curx_old;
					ostep/=Graphmem.buf;
					int id = curx/ostep;
					
					//cout << "segment num: " << seg_id << endl;
					//cout << "value num: " << id << endl;
						
					if (((*seg).next == 0) && (id >= (*seg).values.size())) return -1;
					if (id < 0) return -1;
						auto it = seg->values.rbegin();
						while(id >= 1)
						{
							if (it == seg->values.rend()) break; else ++it;
							--id;
						};
						if (it == seg->values.rend())
						{
							if (!seg->next) return -1;
							++seg;
							it = seg->values.rbegin();
							if (it == seg->values.rend()) return -1;
						};
						float val1 = *it;
						float val2;
						++it;
						if (it == seg->values.rend())
						{
							if (seg->buf_counter > 0) 
							{
								val2 = seg->buf/seg->buf_counter; 
							}else
							{
								if (seg->next != 0)
								{
									it = seg->next->values.rbegin();
									if (it == seg->next->values.rend()) val2 = val1; else val2 = *it;
								} else {val2 = val1;};
							}
						} else val2 = *it;
						curx = (curx - ostep*(int)(curx/ostep))/ostep;
						return (val2*curx+val1*(1-curx));
						
				};
				++seg_id;
			};
			return 0;
		};
		
		virtual void Clear()
		{
			Graphmem.clear();
			maximum = 0;
		}
		
		virtual void OnMouseDown( MouseRes res )
		{
			if (res.btn == 3) step*=1.3; // Closer
			if (res.btn == 4) step/=1.3; // Futher
			
			if (res.btn == 1)
			{
				float val = GetVal( GPos2.x - res.pos.x );
				//cout << "pos: " << GPos2.x - res.pos.x << endl;
				cout << "val: " << val << endl;
			};
		};
		
		virtual void OnMouseUp( MouseRes res )
		{
			
		};
		
		virtual void OnMouseMove( MouseRes res )
		{
		};
};		

UI_Hook(HOOK_OnMouseDown,TUI_Graph,OnMouseDown);
UI_Hook(HOOK_OnMouseUp,TUI_Graph,OnMouseUp);
UI_Hook(HOOK_OnMouseMove,TUI_Graph,OnMouseMove);

#define avgmeasure(Class, Member)                                  \
avgmeasure_func<Class, decltype(Class::Member), &Class::Member>()
    	
class TWF_GraphView : public TUI_WFrame
{
	public:
		
		TUI_MenuPanel* Panel;
		TUI_Graph* Graph;
		
		int update_time;
		
		int mtype;
		
		TWF_GraphView( TUI_ParentLess* parent ) : TUI_WFrame(parent)
		{
			caption = "Graph View";	
			bstyle = 3;
			Size = Vec2i(400,200);
			
			update_time = 0;
			
			mtype = -1;
			
			TUI_Button* btn;
			TUI_MenuButton* mbtn;
			TUI_Menu* menu;
	
			Panel = new TUI_MenuPanel(this);
		
			TUI_ContextButton* cbtn = new TUI_ContextButton(Panel);
			cbtn->SetCaption("Monitor thing");
			cbtn->AutoSize();
			cbtn->AddContext("Bot count"); // ID: 0
			cbtn->AddContext("Allocated memory"); // ID: 1
			cbtn->sevents.Connect(HOOK_OnClick, this,&TWF_GraphView::SelectMonitorType);
			
			Panel->Init();
			
			Graph = new TUI_Graph(this);
			Graph->SetPos( Vec2i(0,Panel->Size.y) );
			Graph->delay = ui_graph_updaterate;
		}
		
		virtual void OnResize()
		{
			Graph->SetSize( Size - Vec2i(0,Panel->Size.y) );
		}
		
		virtual void SelectMonitorType( int ID )
		{
			Graph->Clear();
			mtype = ID;
			//cout << "selected " << ID << endl;
		}
		
		virtual float GetValue()
		{
			if (mtype == -1) return 0;
			float result = 0;
			if (mtype == 0)
			{
				result = 0;
				for (auto &v : World.thinkers)
				{
					if ( v->flags == ENT_ALIVE )
					{
						result+=1;
					}
				}
				return result;
			};
			
			//if (mtype == 1) return avgmeasure(TBot,mutation_rate);
			//if (mtype == 2) return avgmeasure(TBot,overall_size);
			//if (mtype == 3) return avgmeasure(TBot,breedsatiety);
		}

		template <class C, class T, T C::*m > T avgmeasure_func()
		{
			float result = 0;
			C* ptr;
			int count = 0;
			for (auto &v : World.ents)
			{
				ptr = dynamic_cast<C*>(v);
				if (ptr)
				{
					result+=ptr->*m;
					++count;
				}
			}
			result = result/count;
			return result;
		};
		
		
		virtual void OnDraw()
		{
			if ( update_time < GameTime() )
			{
				if (Simulating)
				{
					Graph->push_back( GetValue() );
					update_time = GameTime()+ui_graph_updaterate;
				};
			};
			Update();
		}
};
