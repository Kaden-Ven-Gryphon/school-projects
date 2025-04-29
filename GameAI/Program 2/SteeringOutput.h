#pragma once
#include "Vector2.h"

class SteeringOutput
{
public:
	Vector2 linear;
	float angular;

	SteeringOutput();

	void set(SteeringOutput newSteering);
	void sum(SteeringOutput b);
};