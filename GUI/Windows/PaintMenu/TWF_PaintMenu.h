#include <GUI/Windows/PaintMenu/Tools.h>
class TWF_PaintMenu : public TUI_WFrame
{
	public:
		
		TUI_ScrollFrame* ScrollFrame;
		TUI_ParameterBox* parbox1;
		TUI_ParameterBox* parbox2;

		static const int btn_count = 3;
		static const int inst_count = Instance_SingleCellCount+1;
		TUI_BitmapButton** btn_Tools;

		int cur_tool_id;
		int cur_instance_id;
		
		TPaintTool_Base* tools[btn_count];
		TPaintTool_Base* cur_tool;
		
		TBaseSingleCell* instances[inst_count];
		TBaseEntity* cur_instance;
		Vec2i Padding;
		
		TWF_PaintMenu(TUI_ParentLess* parent ) : TUI_WFrame(parent)
		{
			caption = "Paint Menu";
			
			Size = Vec2i(150,200);
			ScrollFrame = new TUI_ScrollFrame(this);
			ScrollFrame->SetBounds( 0, Size);
			
			btn_Tools = new TUI_BitmapButton*[btn_count];
			int btn_size = 32;
			int btn_num = ScrollFrame->Container->Size.x / btn_size;
			Padding.x = (ScrollFrame->Container->Size.x - btn_size*btn_num) * 0.5;
			Padding.y = Padding.x;
						
			// creating button cage
			
			Vec2i pos = Padding;
			for (int i = 0; i != btn_count; i++)
			{
				btn_Tools[i] = new TUI_BitmapButton(ScrollFrame->Container);
				btn_Tools[i]->SetBounds(pos,btn_size);
				btn_Tools[i]->sevents.Connect(HOOK_OnClick, this, &TWF_PaintMenu::ChooseTool);
				btn_Tools[i]->SetArgument(i);
				pos += Vec2i(btn_size-1,0);
				if (pos.x > (ScrollFrame->Container->Size.x - btn_size) )
				{
					pos.x = 9;
					pos.y+=btn_size-1;
				}
			}
			
			btn_Tools[0]->LoadTexture("cursor");
			btn_Tools[1]->LoadTexture("pen");
			btn_Tools[2]->LoadTexture("spray");
			
			tools[0] = 0;
			tools[1] = new TPaintTool_Pen;
			tools[1]->Connect(this,&TWF_PaintMenu::PaintCell);
			tools[2] = new TPaintTool_Spray;
			tools[2]->Connect(this,&TWF_PaintMenu::PaintCell);
				
			instances[0] = 0;
			Instance.FillInstance_SingleCell(instances);
				
			TUI_ContextButton* cbtn = new TUI_ContextButton(ScrollFrame->Container);
			cbtn->AddContext("Erase");
			Instance.FillContext_SingleCell(cbtn);
			cbtn->SetContext(0);
			cbtn->sevents.Connect(HOOK_OnClick, this, &TWF_PaintMenu::ChoosePaint);
			//cbtn->AutoSize();
			//cbtn->SetPos(Vec2i(10,50));
			pos.y += btn_size+Padding.y;
			cbtn->SetBounds( Vec2i(Padding.x,pos.y),Vec2i(ScrollFrame->Container->Size.x-Padding.x*2,21) );
			pos.y += 21;
			
			parbox1 = new TUI_ParameterBox( ScrollFrame->Container );
			parbox1->SetBounds( Vec2i(0,cbtn->Position.y+cbtn->Size.y+20), Vec2i(Size.x,16) );
			parbox1->SetCaption("Tool parameters");
			parbox1->sevents.Connect(HOOK_OnResize, this, &TWF_PaintMenu::ParBoxToggle);
			
			parbox2 = new TUI_ParameterBox( ScrollFrame->Container );
			parbox2->SetBounds( Vec2i(0,parbox1->Position.y+parbox1->Size.y), Vec2i(Size.x,16) );
			parbox2->SetCaption("Paint parameters");
			parbox2->sevents.Connect(HOOK_OnResize, this, &TWF_PaintMenu::ParBoxToggle);
			
			ParBoxToggle();
			//ScrollFrame->SetVirtualSize(pos.y+Padding.y);
			//ScrollFrame->AutoSize();
			Size.y = ScrollFrame->Container->Size.y;
			
			EnableGlobalHooks();
			cur_tool_id = 0;
			cur_instance_id = 0;
			cur_instance = 0;
			cur_tool = 0;
			SetToolID(0);
		
			Padding = 5;
			bstyle = 2;
		}
		
		~TWF_PaintMenu()
		{
			if (btn_Tools) delete btn_Tools;
			parbox1->DisableTransmit();
			parbox2->DisableTransmit();
			for (int i = 0; i != btn_count; ++i) if (tools[i])  delete tools[i];
			for (int i = 0; i != inst_count; ++i) if (instances[i]) {instances[i]->ClearParameters(); delete instances[i];};
		};
		
		virtual void ChoosePaint(int ID)
		{
			SetPaintID(ID);
		}
		
		virtual void SetPaintID(int ID)
		{
			cur_instance_id = ID;
			if (cur_instance) cur_instance->ClearParameters();
			cur_instance = instances[cur_instance_id];
			parbox2->ClearParameters();
			if (cur_instance) cur_instance->RegisterParameters(parbox2);
		};
		
		virtual void ChooseTool(int ID)
		{
			SetToolID(ID);
		}
		
		virtual void SetToolID(int ID)
		{
			btn_Tools[cur_tool_id]->toggle = 0;
			cur_tool_id = ID;
			btn_Tools[cur_tool_id]->toggle = 1;
			cur_tool = tools[cur_tool_id];
			
			parbox1->ClearParameters();
			if (cur_tool) cur_tool->RegisterParameters(parbox1);
		}
		
		virtual void OnResize()
		{
			ScrollFrame->SetSize( Size );
		};
		
		virtual void PaintCell(Vec2i cell)
		{
			cell = World.BoundsClamp(cell);
			
			TBaseEntity* ent = World.GetCellContent( cell );
								
			if ( cur_instance_id == 0 )
			{
				if ( ent != 0 ) delete ent;
				return;	
			};
			
			if ( ent != 0 ) return;
			if (cur_instance && dynamic_cast<TBaseSingleCell*>(cur_instance))
			{
				TBaseSingleCell* cent = 0;
				//if (cur_instance_id == 2/*bot type id in instancing.cpp*/) cent = new TBot(); 
				//else cent = (TBaseSingleCell*)MakeInstance(cur_instance,cur_instance_id);
				cent = (TBaseSingleCell*)MakeInstance(cur_instance,cur_instance_id);
				if (cent)
				{
					//cout << "spawning" << endl;
					cent->InitPos(cell);
					cent->Spawn();
				};
			};
		}
		
		virtual bool CanPaint(MouseRes res){if ( (res.btn == GLUT_LEFT_BUTTON) && (World.hasmouse) && cur_tool) return true; return false;};
		virtual bool CanPaint(){	if (  MouseStates[0] && (World.hasmouse) && cur_tool) return true; return false;};
		
		virtual void OnMouseDown(MouseRes res)
		{
			if (CanPaint(res)) cur_tool->StartPaint( World.ScreenToCell_Unclamped(MousePos) );
		}
		
		virtual void OnMouseUp(MouseRes res)
		{
			if (CanPaint(res)) cur_tool->EndPaint( World.ScreenToCell_Unclamped(MousePos) );
		}
		
		virtual void OnMouseMove(MouseRes res)
		{
			if (CanPaint()) cur_tool->Paint( World.ScreenToCell_Unclamped(MousePos) );
		};
		
		virtual void Think()
		{
			if (CanPaint()) cur_tool->ThinkPaint( World.ScreenToCell_Unclamped(MousePos) );
		};
		
		virtual void ParBoxToggle()
		{
			parbox2->SetPos(parbox1->Position + Vec2i(0,parbox1->Size.y));
			ScrollFrame->AutoSize();
		};
		
};

UI_Hook(HOOK_G_Think,TWF_PaintMenu,Think);
UI_Hook(HOOK_G_OnMouseDown,TWF_PaintMenu,OnMouseDown);
UI_Hook(HOOK_G_OnMouseMove,TWF_PaintMenu,OnMouseMove);

