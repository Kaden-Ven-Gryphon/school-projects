// StateMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>>
#include <string>
#include <random>

#define FILE_ONE_NAME "output Scenerio 1.txt"
#define FILE_TWO_NAME "output Scenerio 2.txt"


using namespace std;

void runInteration(bool trace, int scenario);
void follow(bool trace);
void pullOut(bool trace);
void pullInBehind(bool trace);
void accelerate(bool trace);
void decelerate(bool trace);
void pullInahead(bool trace);
void done(bool trace);

int followCount = 0;
int pullOutCount = 0;
int pullInBehindCount = 0;
int accelerateCount = 0;
int decelerateCount = 0;
int pullInaheadCount = 0;
int doneCount = 0;

ofstream fp;

random_device rd;
default_random_engine eng(rd());
uniform_real_distribution<float> distr(0, 1);

int main()
{
    bool trace = true;
    int scenario = 1;
    

    fp.open(FILE_ONE_NAME);
    fp << "Kaden Beaver\nCS 330 - 02\n04.20.2021\n\n\n";                //HEADER

    fp << "Scenario\t= 1\nTrace\t\t= true\nIterations\t= 100\n\n";       //Scenerio header
    fp.close();

    for (int i = 1; i <= 100; i++)                                      //run 100 interation with trace  
    {
        fp.open(FILE_ONE_NAME, ios_base::app);
        fp << "Iteration= " << i << endl;
        fp.close();
        runInteration(trace, scenario);                                 //run state
    }

    fp.open(FILE_ONE_NAME, ios_base::app);                              //print the totals
    fp << "State Counts\t\t= " << followCount << ", " << pullOutCount << ", " << accelerateCount << ", " << pullInaheadCount << ", " << pullInBehindCount << ", " << decelerateCount << ", " << doneCount << endl;
    int total = followCount + pullOutCount + accelerateCount + pullInaheadCount + pullInBehindCount + decelerateCount + doneCount;
    fp << "State frequencies\t= " << (float)followCount/total << ", " << (float)pullOutCount / total << ", " << (float)accelerateCount / total << ", " << (float)pullInaheadCount / total << ", " << (float)pullInBehindCount / total << ", " << (float)decelerateCount / total << ", " << (float)doneCount / total << endl;
    fp.close();

    followCount = 0;                                                    //reset counters
    pullOutCount = 0;
    accelerateCount = 0;
    pullInaheadCount = 0;
    pullInBehindCount = 0;
    accelerateCount = 0;
    doneCount = 0;

    trace = false;                                                      //trace is false
    scenario = 2;                                                       //using scenario two

    fp.open(FILE_TWO_NAME);
    fp << "Kaden Beaver\nCS 330 - 02\n04.20.2021\n\n\n";                //HEADER
    fp << "Scenario\t= 2\nTrace\t\t= false\nIterations\t= 1,000,000\n\n";    //scenerio two header
    fp.close();

    for (int i = 1; i <= 1000000; i++)
    {
        runInteration(trace, scenario);
    }

    fp.open(FILE_TWO_NAME, ios_base::app);                              //print the totals
    fp << "State Counts\t\t= " << followCount << ", " << pullOutCount << ", " << accelerateCount << ", " << pullInaheadCount << ", " << pullInBehindCount << ", " << decelerateCount << ", " << doneCount << endl;
    total = followCount + pullOutCount + accelerateCount + pullInaheadCount + pullInBehindCount + decelerateCount + doneCount;
    fp << "State frequencies\t= " << (float)followCount / total << ", " << (float)pullOutCount / total << ", " << (float)accelerateCount / total << ", " << (float)pullInaheadCount / total << ", " << (float)pullInBehindCount / total << ", " << (float)decelerateCount / total << ", " << (float)doneCount / total << endl;
    fp.close();
}

void runInteration(bool trace, int scenario)
{
    enum state_e {followS, pullOutS, accelerateS, pullInAheadS, decelerateS, pullInBehindS, doneS};
    float check = 0;
    state_e state = followS;

    

    bool isDone = false;
    while (!isDone)
    {
        switch(state)
        {
        case followS:                                                       //STATE follow
            follow(trace);                                                  //follow action


            check = distr(eng);                                       //check data to detimine which state to switch too
            if (scenario == 1)                                              //scenario 1
            {
                if (check < 0.8) state = pullOutS;                          //close to car, no oncoming trafic condition met
            }
            else if (scenario == 2)                                         //scenario 2
            {
                if (check < 0.8) state = pullOutS;                          //close to car, no oncoming trafic condition met
            }
            else state = doneS;                                             //unknow scenario go stright to done
            break;

        case pullOutS:                                                      //STATE puul out
            pullOut(trace);                                                 //pull out action


            check = distr(eng);                                       //check data to detimine which state to switch too
            if (scenario == 1)                                              //scenario 1
            {
                if (check < 0.4) state = accelerateS;                       //no oncoming traffic condition met
                else if (check < 0.8) state = pullInBehindS;                //oncoming traffic condition met
            }
            else if (scenario == 2)                                         //scenario 2
            {
                if (check < 0.5) state = accelerateS;                       //no oncoming traffic condition met
                else if (check < 0.8)  state = pullInBehindS;               //oncoming traffic condition met
            }
            break;

        case accelerateS:                                                   //STATE ACCELERATE
            accelerate(trace);                                              //accelerate action

            check = distr(eng);                                       //check data to detimine which state to switch too
            if (scenario == 1)                                              //scenario 1
            {
                if (check < 0.3) state = pullInAheadS;                      //in fornt of car condition met
                else if (check < 0.6) state = pullInBehindS;                //oncoming traffic, behind car condition met
                else if (check < 0.9) state = decelerateS;                  //oncomming traffic, alongside car condition met
            }
            else if (scenario == 2)                                         //scenario 2
            {
                if (check < 0.4) state = pullInAheadS;                      //in front of car condition met
                else if (check < 0.6) state = pullInBehindS;                //oncomint trafic, behing car condtion met
                else if (check < 0.9) state = decelerateS;                  //oncomeint trafic alongside car condition met
            }
            break;

        case pullInAheadS:                                                  //STATE PULL IN AHEAD
            pullInahead(trace);                                             //pull in ahead action

            check = distr(eng);                                       //check data to detimine which state to switch too
            if (scenario == 1)                                              //scenario 1
            {
                if (check < 0.8) state = doneS;                             //in travel lane condition met
            }
            else if (scenario == 2)                                         //scenario 2
            {
                if (check < 0.8) state = doneS;                             //in travel lane condition met
            }
            break;

        case decelerateS:                                                   //STATE DECELERATE
            decelerate(trace);                                              //decealerte action

            check = distr(eng);                                       //check data to detimine which state to switch too
            if (scenario == 1)                                              //scenario 1
            {
                if (check < 0.8) state = pullInBehindS;                     //behind car condition met
            }
            else if (scenario == 2)                                         //scenario 2
            {
                if (check < 0.8) state = pullInBehindS;                     //behind car condition met
            }
            break;

        case pullInBehindS:                                                 //STATE PULL IN BEHIND
            pullInBehind(trace);                                            //pull in behind action

            check = distr(eng);                                       //check data to detimine which state to switch too
            if (scenario == 1)                                              //scenario 1
            {
                if (check < 0.8) state = followS;                           //done pulling in behind condition met
            }
            else if (scenario == 2)                                         //scenario 2
            {
                if (check < 0.8) state = followS;                           //done pulling in behind condition met
            }
            break;

        case doneS:                                                         //STATE DONE
            done(trace);                                                    //done action
            isDone = true;
            break;
        }
    }
}



void follow(bool trace)
{
    if (trace)
    {
        fp.open(FILE_ONE_NAME, ios_base::app);
        fp << "State= 1 Follow\n";
        fp.close();
    }
    followCount++;
}

void pullOut(bool trace)
{
    if (trace)
    {
        fp.open(FILE_ONE_NAME, ios_base::app);
        fp << "State= 2 Pull Out\n";
        fp.close();
    }
    pullOutCount++;
}

void pullInBehind(bool trace)
{
    if (trace)
    {
        fp.open(FILE_ONE_NAME, ios_base::app);
        fp << "State= 5 Pull In Behind\n";
        fp.close();
    }
    pullInBehindCount++;
}

void accelerate(bool trace)
{
    if (trace)
    {
        fp.open(FILE_ONE_NAME, ios_base::app);
        fp << "State= 3 Accelerate\n";
        fp.close();
    }
    accelerateCount++;
}

void decelerate(bool trace)
{
    if (trace)
    {
        fp.open(FILE_ONE_NAME, ios_base::app);
        fp << "State= 6 Decelerate\n";
        fp.close();
    }
    decelerateCount++;
}

void pullInahead(bool trace)
{
    if (trace)
    {
        fp.open(FILE_ONE_NAME, ios_base::app);
        fp << "State= 4 Pull In Ahead\n";
        fp.close();
    }
    pullInaheadCount++;
}

void done(bool trace)
{
    if (trace)
    {
        fp.open(FILE_ONE_NAME, ios_base::app);
        fp << "State= 7 Done\n\n\n";
        fp.close();
    }
    doneCount++;
}
