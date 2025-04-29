#pragma once
#include "Vector2.h"
#include "SteeringOutput.h"
#include <string>
#include <string>

class Kinematic
{
public:
	Vector2 position;
	float orientation;
	Vector2 velocity;
	float rotaion;

	Kinematic();
	Kinematic(Vector2 startPos, float startOrien, Vector2 startVel, float startRot);

	std::string toString();

	void update(SteeringOutput steering, float maxSpeed, float maxRotaion, float time);
	void set(Kinematic b);



private:

};