#include "LookWhereYouAreGoing.h"

LookWhereYouAreGoing::LookWhereYouAreGoing()
{


    targetRoRadius = 0.2;
    slowRoRadius = 2;
    maxAngularAcceleration = 15;
    maxRotation = 20;
}

SteeringOutput LookWhereYouAreGoing::getSteering()
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
        targetRotation = maxRotation * abs(rotation) / slowRoRadius;                  //other wise the target rotation is a function of distance from target orientation
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
