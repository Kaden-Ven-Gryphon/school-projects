#include "SteeringOutput.h"

//default construtor
SteeringOutput::SteeringOutput()
{
	linear.set(0, 0);
	angular = 0;
}

// used as = might try to overload the = operator at some point
void SteeringOutput::set(SteeringOutput newSteering)
{
	linear.set(newSteering.linear);
	angular = newSteering.angular;
}

//effectivly +=
void SteeringOutput::sum(SteeringOutput b)
{
	linear.plusequal(b.linear);
	angular += b.angular;
}
