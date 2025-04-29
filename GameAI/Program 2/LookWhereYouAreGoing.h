#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Kinematic.h"



class LookWhereYouAreGoing
{

public:
	Kinematic character;
	Kinematic target;				//unused by this

	float maxAngularAcceleration;
	float maxRotation;

	float targetRoRadius;
	float slowRoRadius;

	float timeToTarget;

	LookWhereYouAreGoing();
	SteeringOutput getSteering();


};
