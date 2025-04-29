#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Kinematic.h"


class SteeringBehaviors
{
public:
	Kinematic character;
	Kinematic target;

	float maxAcceleration;
	float maxSpeed;
	float maxAngularAcceleration;
	float maxRotation;

	float targetRadius;
	float slowRadius;
	float targetRoRadius;
	float slowRoRadius;

	float timeToTarget;

	SteeringBehaviors();
	SteeringBehaviors(Kinematic newChar, Kinematic newTar, float newMaxAcceleration, float newMaxSpeed, float newTargetRadius, float newSlowRadius, float newTimeToTarget);

	SteeringOutput getSteeringArrive();
	SteeringOutput getSteeringSeek();
	SteeringOutput getSteeringFlee();
	SteeringOutput getSteeringStop();
	SteeringOutput getRotationLookWhereYoureGoing();
};