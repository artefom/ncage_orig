class TWF_Console : public TUI_WFrame
{
	public:
		
		TUI_Edit* edit;
		TUI_Memo* memo;
		
		int last_command;
		
		TWF_Console(TUI_ParentLess* parent ) : TUI_WFrame(parent)
		{
			caption = "Console";
			Size = Vec2i(150,200);
			bstyle = 3; // 0 - unscaleable, 1 - vertical, 2-horizontal 3 - both.
			EnableGlobalHooks();
			
			memo = new TUI_Memo(this);
			memo->SetBounds(0,10);
			memo->InitLines( &Console.lines );
			
			edit = new TUI_Edit(this);
			edit->SetBounds( 10, 20);
			edit->sevents.Connect( HOOK_OnEnter, this, &TWF_Console::OnEnter);
			edit->sevents.Connect( HOOK_OnChange, this, &TWF_Console::UpdateContext);
			
			last_command = -1;
		};
		
		virtual void OnResize()
		{
			memo->SetBounds( 0, Vec2i(Size.x,Size.y-20) );
			edit->SetBounds( Vec2i(0,Size.y-20) , Vec2i(Size.x,20) );
		};
		
		virtual void OnEnter()
		{
			Console.EnterText( edit->GetText() );
			edit->clear();
		}
		
		virtual void OnConsoleEnter( const char* text )
		{
			memo->Update();
		}
		
		pair<int,int> bounds(string text)
		{
			if (text.size() == 0) return pair<int,int>(-1,-1);
			int first = -1;
			int last = Console.functions.size();
			int mid = (first+last)*0.5;
			int top = 0;
			int bottom = 0;
			while((mid != first) && (mid != last))
			{
				if (text < Console.functions_names[mid])
				{
					last = mid;
				} else
				if (text > Console.functions_names[mid])
				{
					first = mid;
				} else break;
				mid = ceil((first+last)*0.5); 
			};
			top = mid;
			text += (char)(255);
			first = 0;
			last = Console.functions.size();
			mid = (first+last)*0.5;
			while((mid != first) && (mid != last))
			{
				if (text < Console.functions_names[mid])
				{
					last = mid;
				} else
				if (text > Console.functions_names[mid])
				{
					first = mid;
				} else break;
				mid = floor((first+last)*0.5); 
			};
			bottom = mid;
			return pair<int,int>(top,bottom);
		};
		
		virtual void UpdateContext()
		{
			edit->ClearContext();
			pair<int,int> p = bounds(edit->GetText());
			int top = p.first;
			int bot = p.second;
			for (int i = top; i <= bot; ++i)
			{
				if ((i >= 0) && (i < Console.functions_names.size())) edit->AddContext(Console.functions_names[i]);	
			};
			//edit->UpdateContext();
		}
		
};

UI_Hook(HOOK_GC_OnEnterText,TWF_Console,OnConsoleEnter);
//UI_Hook(HOOK_GC_OnMove,TWF_Console,OnConsoleMove);
