#pragma once

#include <math.h>

class Vector2
{
public:
	float x;
	float z;

	Vector2();
	Vector2(float newX, float newZ);

	float length();
	Vector2 add(Vector2 b);
	Vector2 minus(Vector2 b);
	Vector2 mult(float scaler);

	void set(Vector2 b);
	void set(float newX, float newZ);
	void normalize();
	void plusequal(Vector2 b);
	void scale(double scaler);


private:
	
};