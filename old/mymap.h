class TFileLine
{
	public:
	const char* file;
	size_t line;
	
	TFileLine()
	{
		file = "unknown";
		line = 0;
	};
	
	TFileLine(const char* i_file, size_t i_line)
	{
		file = i_file;
		line = i_line;
	};
	
	bool operator == (TFileLine& comp)
	{
		if ((file == comp.file) && (line == comp.line)) return true;
		return false;
	};
};

bool operator > (const TFileLine& lhs, const TFileLine& rhs)
{
	int result = strcmp (rhs.file,lhs.file);
	if (result > 0) return true;
	if ((result == 0) && (rhs.line > lhs.line)) return true;
	return false;
}

bool operator < (const TFileLine& lhs, const TFileLine& rhs)
{
	int result = strcmp (rhs.file,lhs.file);
	if (result < 0) return true;
	if ((result == 0) && (rhs.line < lhs.line)) return true;
	return false;
}
