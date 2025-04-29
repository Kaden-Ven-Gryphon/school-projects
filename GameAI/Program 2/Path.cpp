#include "Path.h"

Path::Path()
{
	startPoint.set(Vector2());
	totalLength = 0;
	segments = nullptr;
	coherence = 0.05;
}

void Path::setPathStart(Vector2 start)
{
	startPoint.set(start);
}


//takes a vector2 as the postion for the next point on the path
void Path::addPoint(Vector2 newPoint)
{
	Segment* newSeg = new Segment;						//The new segment to created with the new point
	Segment* segPointer = segments;						//a pointer to the previus segment

	if (segments == nullptr)							//If there are no existing segments
	{
		newSeg->A.set(startPoint);						//Point A of the new segment is the starting point of the path
		newSeg->previousSeg = nullptr;					//The pointer to the prevoius segment is null
		segments = newSeg;								//the Segments list needs to point to this first segment
	}
	else												//Other wise there are other existing segments
	{
		while (true)									//This will find the previous segment and store it as segPointer
		{
			if (segPointer->nextSeg == nullptr)			//If the segment being looked at has a null for the nextSeg then this is the curnent last segment
			{
				newSeg->A.set(segPointer->B);			//The new seg starts on the end of the last
				segPointer->nextSeg = newSeg;			//The curent last seg needs to point to this new segment
				newSeg->previousSeg = segPointer;		//The new seg points back to the curent last seg
				break;									//the new seg has been insterted into the list so break the loop
			}
			else
			{
				segPointer = segPointer->nextSeg;		//else move the pointer to the next segment
			}											//AS THIS LIST HAS FORWAD AND BACKWARD POINTERS THERE SHOULD BE A END POINT POINTER TO CLEAN ^ THAT UP
		}
	}

	newSeg->B.set(newPoint);							//Set the new segs end point to the added point
	newSeg->nextSeg = nullptr;							//it is now the last segment so set next pointer to null
	totalLength += (newSeg->B.minus(newSeg->A)).length();	//update the total length of the path
	updatePathParam();									//go update all the path params for the end points
}

//This will normilize the params of the end points of the segments in segments
//This will require the total length of the path to be acurate
void Path::updatePathParam()
{
	Segment* pathPointer = segments;					//A pointer to go through the path, starts at the first segment
	float runningTotal = 0;								//The length of the path up the most recently updated endpoint

	while (segments != nullptr)							//if there are not segments do nothing
	{
		pathPointer->paramA = runningTotal / totalLength;					//The param for the endpoint A of a segment, if this is the first segment the runing total is still 0
		runningTotal += (pathPointer->B.minus(pathPointer->A)).length();	//add the length of the segment to the runing total
		pathPointer->paramB = runningTotal / totalLength;					//set teh param of endpoint B of the segmentt should be 1 for last segment
		if (pathPointer->nextSeg == nullptr)								//The last semgent was reached break
		{
			break;
		}
		pathPointer = pathPointer->nextSeg;									//move to the next segment
	}


}


//gets the param of the closest point within the coherance
//take the vector pos of the point and the last param
//return the new param limited by the coherance
//should return ~0 if there are no segmetns
float Path::getParm(Vector2 position, float lastParam)
{
	float targetParam = 0;
	float nearestLength = 99999;					// in the unlikely case that the entire path is more than 99999 away from the point this will fail
	Vector2 nearestPos;
	Segment* nearestSeg = segments;
	Segment* pathPointer = segments;

	while (segments != nullptr)						//only if there are segments that exist
	{
		float lamdaS = 0;							//this is used in finding the nearest point, acts as off set from A
		Vector2 pointS;								//the nearest point on the segment

		lamdaS = ((position.minus(pathPointer->A)).dotProduct(pathPointer->B.minus(pathPointer->A)));			//(X-A) dot (B-A)
		lamdaS /= ((pathPointer->B.minus(pathPointer->A)).dotProduct((pathPointer->B.minus(pathPointer->A))));	// /(B-A) dot (B-A)

		if (lamdaS <= 0) pointS.set(pathPointer->A);						//if lamda is <= 0 then the closest point is the endpoint A
		else if (lamdaS >= 1) pointS.set(pathPointer->B);					//if lamda is >= 1 then the closest point is the endpoint B
		else																//else the point is on the segment
		{
			pointS.set(pathPointer->B.minus(pathPointer->A));				//get the segment from A to B
			pointS.scale(lamdaS);											//scale it by the percent from lamda
			pointS.plusequal(pathPointer->A);								//add this off set to A to get the cloeset point
		}

		if ((position.minus(pointS).length()) < nearestLength)				//if the nearest point on this seg is closer than any point sofar
		{
			nearestLength = (position.minus(pointS).length());				//get the dictance of the new point
			nearestPos.set(pointS);											//get the pos of the new point
			nearestSeg = pathPointer;										//get the segment of the new point
		}

		if (pathPointer->nextSeg == nullptr) break;							//if there are no more segmetns to check break
		pathPointer = pathPointer->nextSeg;									//move to the next segment
	}

	targetParam = nearestSeg->paramB - nearestSeg->paramA;					//get the portion of param for the segmetn
	float AtoSLength = (nearestPos.minus(nearestSeg->A).length());			//Was trying to do to much on one line
	float AtoBLength = (nearestSeg->B.minus(nearestSeg->A)).length();
	float percentOfAB = AtoSLength / AtoBLength;
	targetParam *= percentOfAB;	//scale by the ration of the length A to S : length A to B,  this will get the off set of param from A to point S

	targetParam += nearestSeg->paramA;										//this should be the param of the nearest point

	if (targetParam > lastParam + coherence) targetParam = lastParam + coherence;	//if the real closest point is outside of the coherence then use the radius of the coherence
	else if (targetParam < lastParam - coherence) targetParam = lastParam - coherence;	//same but limit the lower bound


	return targetParam;														//return the param of nearest point within coherance
}

//take a float param 0 to 1  It will be fored to the those bounds
//then retrun the postion on the path that is the param% of the way alnong the path
Vector2 Path::getPosition(float param)
{
	Vector2 posOnPath;
	Segment* pathPointer = segments;
	float paramOffSet = 0;

	if (param > 1) param = 1;							//force param to be 0 to 1
	if (param < 0) param = 0;

	while (segments != nullptr)							//as long as there exitest a segmetn
	{
		if (pathPointer->paramA <= param && pathPointer->paramB >= param) break;		//if the requested param is within this segment
		if (pathPointer->nextSeg == nullptr) break;		//should never be called but is a saftey
		pathPointer = pathPointer->nextSeg;				//if the param is not in that segment go to the next one
	}

	paramOffSet = param - pathPointer->paramA;			//get the percent of path from A to target
	paramOffSet /= (pathPointer->paramB - pathPointer->paramA);	//devied path percent of path of segment, this will get teh percent of the segment to target

	posOnPath.set(pathPointer->A);						//take vector A
	posOnPath.scale(-1);								//make it negative
	posOnPath.plusequal(pathPointer->B);				//to subtract from B to get the vector from A to B
	posOnPath.scale(paramOffSet);						//scale it by the reltive param from A

	posOnPath.plusequal(pathPointer->A);				//add the off set from A to A to get the final pos of param

	return posOnPath;									//Return the postition
}

std::string Path::toString()
{
	using namespace std;


	string output;

	Segment* pathPointer = segments;

	output += "A, B, paramA, paramB\n";
	while (true)
	{
		output += to_string((pathPointer->A.x));
		output += ", ";
		output += to_string((pathPointer->A.z));
		output += ", ";
		output += to_string((pathPointer->B.x));
		output += ", ";
		output += to_string((pathPointer->B.z));
		output += ", ";
		output += to_string((pathPointer->paramA));
		output += ", ";
		output += to_string((pathPointer->paramB));
		output += "\n";
		if (pathPointer->nextSeg == nullptr) break;
		pathPointer = pathPointer->nextSeg;
	}


	return output;
}
