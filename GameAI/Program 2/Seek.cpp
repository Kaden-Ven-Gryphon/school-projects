#include "Seek.h"

//default constructor
Seek::Seek()
{
	maxAcceleration = 2;
}

//paramenterized constructor
Seek::Seek(Kinematic newCharater, Kinematic newTarget, float newMaxAcceleration)
{
	character.set(newCharater);
	target.set(newTarget);
	maxAcceleration = newMaxAcceleration;
}

//seek get steering
SteeringOutput Seek::getSteering()
{
	SteeringOutput result;								//objrect to return

	result.linear.set(target.position.minus(character.position));		//get vector from character pos to target pos

	result.linear.normalize();							//get the direction of that vector
	result.linear.scale(maxAcceleration);				//set it to the max acceration

	result.angular = 0;									//set liniear to 0


	return result;										//return ther steering
}
