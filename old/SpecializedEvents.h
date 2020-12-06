class SEvents
{
	public:
		
	
	map<int, vector< Delegate > > Delegates;
	
	template< class T, class U > void Connect( int hook_id, T* elm, U method)
	{
		vector<Delegate>& lst = Delegates[hook_id];
		lst.push_back( Delegate() );
		lst.back().Connect( elm, method );
	}
	
	void Call( int ID )
	{
		auto it = Delegates.find(ID);
		if (it == Delegates.end()) return;
		vector<Delegate>& lst = it->second;
		for ( auto &v : lst )
		{
			v();
		}
	}
	
	template<class T> void Call( int ID, T arg )
	{
		auto it = Delegates.find(ID);
		if (it == Delegates.end()) return;
		vector<Delegate>& lst = it->second;
		for ( auto &v : lst )
		{
			v(arg);
		}
	}
	
	void Clear()
	{
		Delegates.clear();
	};
	
};
