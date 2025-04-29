
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include "Vector2.h"
#include "SteeringBehaviors.h"
#include "Path.h"
#include "FollowPath.h"
#include "LookWhereYouAreGoing.h"




using namespace std;

int main()
{

    Path* path = new Path;                                      //The path to be followed
    Kinematic followPathCharacter(Vector2(70, -40), 0, Vector2(0, 0), 0);       //the character that will be folloing the path, start pos 70, -40 start velocity 0
    FollowPath followPath;                                      //the follow path behavior
    LookWhereYouAreGoing lookAhead;                             //the look ahead behavior
    ofstream outFile;                                           //file to write the out to


    path->setPathStart(Vector2(75, -20));                       //adding the points to build up the path
    path->addPoint(Vector2(45, 20));
    path->addPoint(Vector2(15, -40));
    path->addPoint(Vector2(-15, 40));
    path->addPoint(Vector2(-45, -60));
    path->addPoint(Vector2(-75, 60));

    followPath.character.set(followPathCharacter);              //set the character for the follow path behavoir to the path character,  would like to change this to use pointers so it does not have to be constantly updated
    followPath.path = path;                                     //set the path in the behavoir to point to the path created above, want to change the character to be a pointer like this is
    followPath.pathOffset = 0.05;                               //the path offset used to chase the rabit,  had an isue for a bit becuase a forgot to set this which left it as 0. then I set it to 0.5 instead of 0.05 casuing the character to skip the path

    lookAhead.character.set(followPathCharacter);               //set the character for the lookwhereyouaregoing behavoir

    float time = 0;                                             //start at time 0

    outFile.open("KadenBeaverOutput.txt");                      //the output file
                                                                //print the inistail conditions
    outFile << time << "," << "171" << "," << followPathCharacter.toString() << "," << followPath.getSteering().linear.x << "," << followPath.getSteering().linear.z << "," << followPathCharacter.orientation << "," << "3" << endl;


    while (time < 100)                                          //till the time reaches 100 seconds
    {
        SteeringOutput steering;                                //create a steering output
        steering.set(followPath.getSteering());                 //get the steering needed to follow the path
        steering.sum(lookAhead.getSteering());                  //used to get the steering needed to look where going
        followPathCharacter.update(steering, 4, M_PI, 0.5);     //update the character with the summed steering
        followPath.character.set(followPathCharacter);          //update the charcter in the follow behavoir, this is what I want to elemnated by using a pointer
        lookAhead.character.set(followPathCharacter);           //update the charcter in the look where going behavior
        time += 0.5;                                            // increment time

        cout << path->getParm(followPathCharacter.position, followPath.currentParam) << endl;   //print the charcters current param to the concel, used to track the characters progress and debug

                                                                //print the character info to the output file
        outFile << time << "," << "171" << "," << followPathCharacter.toString() << "," << steering.linear.x << "," << steering.linear.z << "," << followPathCharacter.orientation << "," << "3" << endl;
    }
    
    outFile.close();                                            //clsoe the file


    return 0;
}



/* previous assigment
int main()
{
    ofstream outFile;
    

    Kinematic testSeek(Vector2(50,-25), 3*M_PI/2, Vector2(0,-8), 0);            //charcter for the Seek behavoir
    Kinematic testFlee(Vector2(-25, 50), M_PI / 4, Vector2(0, -8), 0);          //character for the flee behavoir
    Kinematic testArrive(Vector2(-50, -75), M_PI, Vector2(-6, 4), 0);           //character for the arrive behavior
    Kinematic testTarget;                                                       //character for the target, using the stop behavior

    SteeringBehaviors seekBehavior(testSeek, testTarget, 2, 8, 2, 25, 0.5);     //Steering with the Seek character and target
    SteeringBehaviors fleeBehavior(testFlee, testTarget, 2, 10, 2, 25, 0.5);    //Steerring with the flee character and target
    SteeringBehaviors arriveBehavior(testArrive, testTarget, 2, 8, 2, 25, 0.5); //steering with the arrive character and target
    SteeringBehaviors stopBehavior(testTarget, testTarget, 2, 8, 2, 25, 0.5);   //steering with the stop character/target
    


    outFile.open("KadenBeaverOutput.txt");                                      //open the output file

    float time = 0;                                                             //set the time to 0
    while (time <= 50)                                                          //loop while the time is less than 50
    {
                                                                                //write out the info on the 4 characters to the file
        outFile << time << "," << "164" << "," << testArrive.toString() << "," << arriveBehavior.getSteeringArrive().linear.x << "," << arriveBehavior.getSteeringArrive().linear.z << "," << testArrive.orientation << "," << "5" << endl;
        outFile << time << "," << "163" << "," << testSeek.toString() << "," << seekBehavior.getSteeringSeek().linear.x << "," << seekBehavior.getSteeringSeek().linear.z << "," << testSeek.orientation << "," << "3" << endl;
        outFile << time << "," << "162" << "," << testFlee.toString() << "," << fleeBehavior.getSteeringFlee().linear.x << "," << fleeBehavior.getSteeringFlee().linear.z << "," << testFlee.orientation << "," << "4" << endl;
        outFile << time << "," << "161" << "," << testTarget.toString() << ",0,0,0," << "1" << endl;

        SteeringOutput steer;                                                   //create a sterringOutput
        steer.linear.set(arriveBehavior.getSteeringArrive().linear);            //get the linear change for arrive
        steer.angular = arriveBehavior.getRotationLookWhereYoureGoing().angular;//get the rotation change for arrive
        testArrive.update(steer, arriveBehavior.maxSpeed, arriveBehavior.maxRotation, 0.5);                 //update arrive character
        arriveBehavior.character.set(testArrive);                               //update the character of arrivebehavior

        steer.linear.set(seekBehavior.getSteeringSeek().linear);                //get teh linear change for seek
        steer.angular = seekBehavior.getRotationLookWhereYoureGoing().angular;  //get the rotaiton chagne for seek
        testSeek.update(steer, seekBehavior.maxSpeed, seekBehavior.maxRotation, 0.5);                     //update seek character
        seekBehavior.character.set(testSeek);                                   //update teh character in the seek behavior

        steer.linear.set(fleeBehavior.getSteeringFlee().linear);                //get teh linear change for flee
        steer.angular = fleeBehavior.getRotationLookWhereYoureGoing().angular;  //get teh rotaiton change for flee
        testFlee.update(steer, fleeBehavior.maxSpeed, fleeBehavior.maxRotation, 0.5);                     //update the flee character
        fleeBehavior.character.set(testFlee);                                   //update teh character in the flee behacior

        steer.linear.set(stopBehavior.getSteeringStop().linear);                //get the linear change for stop
        steer.angular = stopBehavior.getRotationLookWhereYoureGoing().angular;  //get the rotaiton change fro stop
        testTarget.update(steer, stopBehavior.maxSpeed, stopBehavior.maxRotation, 0.5);                   //update teh stop character
        stopBehavior.character.set(testTarget);                                 //update the character in the stop behavior

        time += 0.5;                                                            //Time step 0.5

    }

    outFile.close();                                                            //close the file

}

*/