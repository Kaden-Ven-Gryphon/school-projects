#include "SteeringBehaviors.h"


//defualt construtor
SteeringBehaviors::SteeringBehaviors()
{

}

//parameter construtor
SteeringBehaviors::SteeringBehaviors(Kinematic newChar, Kinematic newTar, float newMaxAcceleration, float newMaxSpeed, float newTargetRadius, float newSlowRadius, float newTimeToTarget)
{
    character.set(newChar);
    target.set(newTar);
    maxAcceleration = newMaxAcceleration;
    maxSpeed = newMaxSpeed;
    targetRadius = newTargetRadius;
    slowRadius = newSlowRadius;
    timeToTarget = newTimeToTarget;

    targetRoRadius = 0.2;
    slowRoRadius = 7;
    maxAngularAcceleration = 15;
    maxRotation = 20;
}

//Gets the steering for Arrive
//There is no inputs
//The output is a SteeringOurput that only has Linear data
SteeringOutput SteeringBehaviors::getSteeringArrive()
{
    SteeringOutput result;
    Vector2 direction;
    Vector2 targetVel;
    float distance = 0;
    float targetSpeed = 0;

    direction.set(target.position.minus(character.position));   //get the direction that points twoards the target

    distance = direction.length();

    if (distance < targetRadius)                                //if the character is within the targetRadius
    {
        return result;                                          //make no change
    }

    if (distance > slowRadius)                                  //if the character is outside the slowradius
    {
        targetSpeed = maxSpeed;                                 //set the target speed to max speed
    }
    else
    {
        targetSpeed = maxSpeed * distance / slowRadius;         //the character is within the slow radius, target speed is a function of distance to target
    }

    targetVel.set(direction);                                   //set the target velocity to match the direction to target
    targetVel.normalize();                                      //make it a unit vector
    targetVel.scale(targetSpeed);                               //to set the speed to target speed

    result.linear.set(targetVel.minus(character.velocity));     //get the linear accelteration needed
    result.linear.scale(1 / timeToTarget);                      //scale it by the time

    if (result.linear.length() > maxAcceleration)               //if the acceleration is to high
    {
        result.linear.normalize();                              //get unitvector
        result.linear.scale(maxAcceleration);                   //set the accelteration to max
    }

    result.angular = 0;                                         //returns no angular data

    return result;                                              //return result
}

//gets the steering for seek
//there is not inputs
//there is a SteeringOupt output with only linear data
SteeringOutput SteeringBehaviors::getSteeringSeek()
{
    SteeringOutput result;                                          //object to return
       
    result.linear.set(target.position.minus(character.position));   //set the acceleration to the diffrence of the postions

    result.linear.normalize();                                      //get the unitvector
    result.linear.scale(maxAcceleration);                           //to get the max acceleration

    result.angular = 0;                                             //there is not angular data


    return result;                                                  //return result
}

//get the steering for Flee
//there are not inputs
//there is a SteeringOutput ouput with only linear data
SteeringOutput SteeringBehaviors::getSteeringFlee()
{
    SteeringOutput result;                                          //object to return

    result.linear.set(character.position.minus(target.position));   //set the acceleration to the optite dirction of the target

    result.linear.normalize();                                      //get teh unitvector
    result.linear.scale(maxAcceleration);                           //to set the max acceleration

    result.angular = 0;                                             //no angular data


    return result;                                                  //return the result
}

//get the steering for stop
//there are not inputs
//there is a SteeringOutput output with only linear data
SteeringOutput SteeringBehaviors::getSteeringStop()
{
    SteeringOutput result;                                          //object to return
    Vector2 targetVel(0, 0);                                        //target velocity of 0

    result.linear.set(targetVel.minus(character.velocity));         //get the velocity that is the opisite of the characters velicty
    result.linear.scale(1 / timeToTarget);                          //scale it by time


    if (result.linear.length() > maxAcceleration)                   //if the acceration is to hight
    {
        result.linear.normalize();                                  //get the unit vector
        result.linear.scale(maxAcceleration);                       //and set it to max acceleration
    }

    return result;                                                  //return result
}

//get the to rotation needed to look where the charcter is going
//no inputs
//SteeringOutput with only rotation data
SteeringOutput SteeringBehaviors::getRotationLookWhereYoureGoing()
{
    SteeringOutput result;                                          //object to return

    if (character.velocity.length() == 0)                           //if the characters velocity is 0 then no rotaion needed
    {
        return result;
    }

    float targetOrientaiton = atan2(-character.velocity.x, character.velocity.z);   //use the character velecity to get target orientaiton

    float rotation = targetOrientaiton - character.orientation;                     //get the diffrence in character orientaiton and target orientation

    float targetRotation = 0;

    //MAP ROTAITON
    if (rotation < -M_PI)                                                           //if the needed rotaiton is more than -pi than add pi
    {
        rotation += M_PI;
    }
    else if (rotation > M_PI)                                                       //if the needed rotation is more than pi then subtract pi
    {
        rotation -= M_PI;
    }                                                                               //rotaion needed should be within -pi and pi

    if (abs(rotation) < targetRoRadius)                                             //if needed rotaiton is within target
    {
        return result;                                                              //ne change needed
    }

    if (abs(rotation) > slowRoRadius)                                               //if the rotaion needed is greater than the slow radius
    {
        targetRotation = maxRotation;                                               //then use max rotation
    }
    else
    {
        targetRotation = maxRotation * abs(rotation) / slowRadius;                  //other wise the target rotation is a function of distance from target orientation
    }

    targetRotation *= rotation / abs(rotation);                                     //set the direction of the needed roation

    result.angular = targetRotation - character.rotaion;                            //roation acceleration is the diffence target rotaion and character rotaiton
    result.angular /= timeToTarget;                                                 //scale it by time

    if (abs(result.angular) > maxAngularAcceleration)                               //if needed accelteration is to greate
    {
        result.angular /= abs(result.angular);                                      //set the acceleration to max
        result.angular *= maxAngularAcceleration;
    }

    return result;                                                                  //return result
}
