/******************************************************************************************************************************
* Modual: skeleton.c

* Author: Kaden Gryphon

* Date:

* Version: 1.0

* Usage: n/a

* Procedure:
* 1. creat a float variable containting the probabity of win
* 2. build a function that calculates a homeOrAway() probability
* 3. add 1 to 2
* 4. build a function that calculates a noLosses() probability
* 5. add 3 to 4
* 6. build a function that calculates a opponentLosses() probability
* 7. add 5 to 6
* 8. build a function that calculates a ranking() probability
* 9. add 7 to 8
* 10. build a function that calculates a totalDefense() probability
* 11. add 9 to 10
* 12. build a function that calculates a totalOffence() probability
* 13. add 11 to 12
* 14. print a table dispalying the data
* 15. q to quit


********************************************************************************************************************************/


//Libaries*******************************************************************************************************************

#include <stdio.h>

//Defines********************************************************************************************************************

#define HOLD_SCREEN while(getchar() != 'q')
#define DEBUG 1

//Functions******************************************************************************************************************

float homeOrAway(int Home);
float noLosses(int wins, int losses);
float opponentLosses(int wins, int losses);
float ranking(int a, int b);
float totalDefense(int a, int b);
float totalOffence(int a, int b);
char homeLetter(int home);



//Main**********************************************************************************************************************

int main(void)
{
	float winProb = 0.5;															//*1. creat a float variable containting the probabity of win
	int alHome = 1;
	int alWins = 0;
	int alLosses = 0;
	int louWins = 0;
	int louLosses = 0;
	int alRank = 1;
	int louRank = 26;
	int alDefRank = 2;
	int louDefRank = 22;
	int alOffRank = 13;
	int louOffRank = 4;
	char home;
	

	winProb += homeOrAway(alHome);													//*3. add 1 to 2		
	winProb += noLosses(alWins, alLosses);											//*5. add 3 to 4
	winProb += opponentLosses(louWins, louLosses);									//*7. add 5 to 6
	winProb += ranking(alRank, louRank);											//*9. add 7 to 8
	winProb += totalDefense(alDefRank, louDefRank);									//*11. add 9 to 10
	winProb += totalOffence(alOffRank, louOffRank);									//*13. add 11 to 12
	home = homeLetter(alHome);

	printf("\n\t# H # Team Name # Next Opponent # Records # Tot. Def. # Tot. Off. # Win Prob. #");
	printf("\n\t# %c # Alabama Crimson Tide # Louisville Cardinals # %i-%i / %i-%i # %i / %i # %i / %i # %i / %i # %3.2f #",home, alWins, alLosses, louWins, louLosses, alRank, louRank, alDefRank, louDefRank, alOffRank, louOffRank, winProb);

	if (DEBUG) printf("\n\tprobability = %3.2f\n", winProb);


	printf("q to quit ");
	HOLD_SCREEN;																	//*15. q to quit

	return (0);
}//int main(vio...

//Functions*****************************************************************************************************************

float homeOrAway(int home)   														//*2. build a function that calculates a homeOrAway() probability
{
	if (home)
	{
		return(0.1f);
	}
	return(0.0f);
}//float homeOrA...

float noLosses(int wins, int losses) 												//*4. build a function that calculates a noLosses() probability
{
	if (wins > 4 && losses ==0)
	{
		return(0.1f);
	}
	return(0.0f);
}//float noLosses...

float opponentLosses(int wins, int losses)   										//*6. build a function that calculates a opponentLosses() probability
{
	if ((wins + losses) >= 4 && losses > 1)
	{
		return(-0.1f);
	}
	return(0.0f);
}//float oppon...

float ranking(int a, int b)															//*8. build a function that calculates a ranking() probability
{
	if (a < b)
	{
		return(0.1f);
	}
	return(0.0f);
}//float ranking(in...

float totalDefense(int a, int b)													//*10. build a function that calculates a totalDefense() probability
{
	if (a < b)
	{
		return(0.1f);
	}
	return(-0.1f);
}//float totalDefense(in...

float totalOffence(int a, int b)													//*12. build a function that calculates a totalOffence() probability
{
	if (a < b)
	{
		return(0.1f);
	}
	return(-0.1f);
}//float totalOffence(int...

char homeLetter(int home)
{
	if (home)
	{
		return('Y');
	}
	return('N');
}
