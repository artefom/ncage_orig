
class TPoison : public TBaseSingleCell
{
	public:
		MColorID;
		TPoison()
		{
			flags = ENT_EDIBLE | ENT_POISON;
			InitColor(colorID);
		}
};
DefaultColor(TPoison,50,150,50);
