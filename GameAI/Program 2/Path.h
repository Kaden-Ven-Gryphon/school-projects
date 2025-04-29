#pragma once
#include <string>
#include "Vector2.h"



class Path
{
public:

	class Segment
	{
	public:
		Segment* previousSeg = nullptr;
		Vector2 A;
		Vector2 B;
		float paramA = 0;
		float paramB = 0;
		Segment* nextSeg = nullptr;
	};

	float totalLength;
	Segment* segments;				//the start of the linked list
									//should add a pointer to the end of the list to use to deconstruct
	Vector2 startPoint;

	float coherence;

	Path();
	//NEEDS A DECONSTRUCTOR the path never get distroyed in this example, but this would need to be added for further use

	void setPathStart(Vector2 start);
	void addPoint(Vector2 newPoint);
	void updatePathParam();

	float getParm(Vector2 position, float lastParam);
	Vector2 getPosition(float param);

	std::string toString();


};