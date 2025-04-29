#include "Vector2.h"

//default constructor
Vector2::Vector2()
{
	x = 0;
	z = 0;
}

//parameter contstructor
Vector2::Vector2(float newX, float newZ)
{
	x = newX;
	z = newZ;
}

//returns a float that is the dot Product of this and vector w
float Vector2::dotProduct(Vector2 w)
{
	float product = 0;
	product = x * w.x;
	product += z * w.z;
	return product;
}

//get the magnitued of the vector
float Vector2::length()
{
	return sqrt(pow(x,2)+pow(z,2));
}

//the diffrence of two vectors, does not change the vectors
Vector2 Vector2::minus(Vector2 b)
{
	Vector2 result;
	result.x = x - b.x;
	result.z = z - b.z;
	return result;
}

//multiplies the elements of two ectors, deos not change the vectors
Vector2 Vector2::mult(float scaler)
{
	Vector2 result;
	result.x = x * scaler;
	result.z = z * scaler;

	return result;
}

//set a vector to anouther vector
void Vector2::set(Vector2 b)
{
	x = b.x;
	z = b.z;
}

//set a vector by tis elements
void Vector2::set(float newX, float newZ)
{
	x = newX;
	z = newZ;
}

//get the unit vector, does change the vector
void Vector2::normalize()
{
	float mag = length();
	x = x / mag;
	z = z / mag;
}

//add a vector to vector, does changethe vector
void Vector2::plusequal(Vector2 b)
{
	x += b.x;
	z += b.z;
}

//sca;les the vector, doeschange the vector
void Vector2::scale(double scaler)
{
	x = x * scaler;
	z = z * scaler;
}
