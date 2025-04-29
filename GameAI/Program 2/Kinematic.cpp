#include "Kinematic.h"

//defualt constructor
Kinematic::Kinematic()
{
	position.set(0, 0);
	orientation = 0;
	velocity.set(0, 0);
	rotaion = 0;
}

//parameter contructor
Kinematic::Kinematic(Vector2 startPos, float startOrien, Vector2 startVel, float startRot)
{
	position.set(startPos);
	orientation = startOrien;
	velocity.set(startVel);
	rotaion = startRot;
}

//convert the information of the character to a string
std::string Kinematic::toString()
{
	std::string result = "";
	result += std::to_string(position.x);
	result += ",";
	result += std::to_string(position.z);
	result += ",";
	result += std::to_string(velocity.x);
	//result += std::to_string(-sin(orientation));
	result += ",";
	result += std::to_string(velocity.z);
	//result += std::to_string(cos(orientation));
	//result += ",";
	//result += "0";
	//result += ",";
	//result += "0";
	//result += ",";
	//result += std::to_string(orientation);
	return result;
}

//update the characters postion/velectioy
//inputs: a steeringOutput, maxSpeed, and time step
//there is no output
void Kinematic::update(SteeringOutput steering, float maxSpeed, float maxRotaion, float time)
{
	position.plusequal(velocity.mult(time));			//update position
	orientation += rotaion * time;						//update orientaiotn

	velocity.plusequal(steering.linear.mult(time));		//update velecity
	rotaion += steering.angular * time;					//update rotaion

	if (velocity.length() > maxSpeed)					//if velocity is faster than maxspeed
	{
		velocity.normalize();							//scale velcity to max speed
		velocity.scale(maxSpeed);
	}

	if (orientation > maxRotaion)						//limit the max rotataion
	{
		rotaion = rotaion / abs(rotaion);
		rotaion *= maxRotaion;
	}

}

//copy the contents of one character to anouteher
void Kinematic::set(Kinematic b)
{
	position.set(b.position);
	orientation = b.orientation;
	velocity.set(b.velocity);
	rotaion = b.rotaion;
}
