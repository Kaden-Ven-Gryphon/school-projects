#include "FollowPath.h"

//default constructor
FollowPath::FollowPath()
{
    Seek::Seek();
    pathOffset = 0.05;
    currentParam = 0;
    path = nullptr;
}

//the getsteering for followpath, uses seek with path target
SteeringOutput FollowPath::getSteering()
{
    currentParam = path->getParm(character.position, currentParam);         //get the curent param within the coherance of the previus param

    target.position.set(path->getPosition(currentParam + pathOffset));      //get the position of the next param step and set that as the target postion

    return Seek::getSteering();                                             //call the get steering of seek
}
