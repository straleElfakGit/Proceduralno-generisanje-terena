#ifndef MIN_MAX_H
#define MIN_MAX_H

#include <cfloat>

class MinMax
{
private:
	float minValue = FLT_MAX;
	float maxValue = -FLT_MAX;

public:
	MinMax() { Reset(); }

	void AddValue(float v)
	{
		if (v > maxValue)
			maxValue = v;
		if (v < minValue)
			minValue = v;
	}

	float Min() const { return minValue; }
	float Max() const { return maxValue; }

	bool IsEmpty() const { return minValue > maxValue; }

	void Reset() 
	{
		minValue = FLT_MAX;
		maxValue = -FLT_MAX;
	}
};

#endif

