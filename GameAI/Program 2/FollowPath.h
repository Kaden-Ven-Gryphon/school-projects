#pragma once
#include "Seek.h"
#include "Path.h"



class FollowPath : public Seek			//extends seek
{
public:

	Path* path;

	float pathOffset;
	float currentParam;

	FollowPath();

	SteeringOutput getSteering();

};
