#pragma once
#include "Kinematic.h"

class Seek
{

public:
	Kinematic character;
	Kinematic target;

	float maxAcceleration;

	Seek();
	Seek(Kinematic newCharater, Kinematic newTarget, float newMaxAcceleration);

	SteeringOutput getSteering();


};
