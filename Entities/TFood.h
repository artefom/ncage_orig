int shared_ptr_count = 0;


	class TShared_ptr
	{	
		public:
			
		int ptr_count;
		
		bool mortal = 0;
			
		TShared_ptr()
		{
			ptr_count = 0;
			++shared_ptr_count;
			//cout << "created " << shared_ptr_count << " shared ptr" << endl;
		}
		
		TShared_ptr(bool ib)
		{
			ptr_count = 0;
			mortal = ib;
			++shared_ptr_count;
			//cout << "created " << shared_ptr_count << " shared ptr" << endl;
		}
		
		~TShared_ptr()
		{
			--shared_ptr_count;
			//cout << "shared ptrs left: " << shared_ptr_count << endl;
		}
		
		void operator ++ ()
		{
			++ptr_count;	
		}
		
		void operator -- ()
		{
			--ptr_count;
			if (ptr_count <= 0) 
			{
				if (mortal == 1) delete this;
			}
		}
		
		void SetMortal(bool im)
		{
			mortal = im;
			if (im == 0)
			{
				if (ptr_count <= 0) delete this;	
			}
		}
	};

TShared_ptr uncounted_count(0);

class TFood : public TBaseSingleCell
{
	public:
		
		//TShared_ptr& counter;
		MColorID;
		TFood() : TBaseSingleCell()//, counter(uncounted_count)
		{
			flags = ENT_EDIBLE;
			InitColor(colorID);
		}		
		
		TFood( TShared_ptr& ptr ) : TBaseSingleCell()//, counter(ptr)
		{
			flags = ENT_EDIBLE;
			InitColor(colorID);
		}			
		
		~TFood()
		{
			//--food_count;
			//--counter;
		}
};
DefaultColor(TFood,150,150,80);
