class TUI_Memo : public TUI_ScrollFrame // uneditable
{
	public:
		vector<string*>* lines;
		
		//list<char*>::reverse_iterator rbegin_old;	// these two are to handle
		unsigned int size_old;						// removals from the back
		
		int dtext_x;
		int dtext_y;
		int lines_gap;

		TUI_Memo(TUI_ParentLess* parent) : TUI_ScrollFrame( parent )
		{
			dtext_x = 5;
			dtext_y = 5;
			lines_gap = MainFont_Height+dtext_y;
			lines = 0;
			size_old = 0;
			Update();
		}
		
		virtual void OnDraw()
		{
			if (!lines) return;
			if (size_old != lines->size()) {SetVirtualSize(lines->size()*lines_gap);	size_old = lines->size();};
			glColor(UI_Text_Medium);
			int top_line = clamp(PosToLine(0),0,lines->size()-1);
			int bottom_line = clamp(PosToLine(Size.y),0,lines->size()-1);
			int cury = lines_gap+Container->Position.y+top_line*lines_gap;
			string** ptr = &(*lines)[top_line];
			string** end = &(*lines)[bottom_line]+1;
			for (ptr; ptr != end; ++ptr)
			{
				drawtext( dtext_x,cury, **ptr );
				cury += lines_gap;
			}
		};
		
		void InitLines( vector<string*>* ilines )
		{
			lines = ilines;
		};
		
		int PosToLine( int pos )
		{
			return (pos-Container->Position.y)/lines_gap;
		};
};
