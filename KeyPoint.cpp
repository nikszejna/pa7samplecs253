//KeyPoint.cpp

//Author: Nikolas Szejna
//
//Implementation of KeyPoint class which:
//Holds a key point which consists of the integer (x,y) coordinates
//of an input source coordiante and an output destination coordinate
#include "KeyPoint.h"

//default constructor
KeyPoint::KeyPoint()
{
   //create (0,0) coordinates with the default of Coordinate class
   std::vector<Coordinate> v;
   locations = v;
   locations.reserve(10);
}


//constructor with inCoords and outCoords
KeyPoint::KeyPoint(const std::vector<Coordinate> keys)
{
    locations = keys;
}

// keyPoint(std::vector<int> intCoordinates)
// {
//     //temporary vector of coordinates to give back to keyPoint class
//     std::vector<Coordinate> tempCoords;

//     //this is less than the size of the int vector minus one
//     //because each iteration of the loop takes 2 integers
//     for(unsigned int i = 0; i < intCoordinates.size()-1;i++)
//     {
//         //check that each coordinate has an x and a y
//         if(!(intCoordinates.size() %2 == 0)) exit(-1);
//         else
//         {
//             Coordinate temp(intCoordinates.at(i),intCoordinates.at(i+1));
//             tempCoords.pushBack(temp);
//         }

//     }
//     keyPoints = tempCoords;
// }

//calculates the difference between the keypoint at position J
//and if useSource is true, the source coordinate Xs,Ys
///if source is false then it uses dest coordinates
Coordinate KeyPoint::calcDifferences( bool useSource, int j)
{
    Coordinate temp;
    Coordinate co1;

    if(useSource)
    {
        //use the source coordinate
        co1 = locationAt(0);
    }
    else
    {
        //get dest keypoint coordinate
        co1 = locationAt(locations.size()-1);
    }
    
    Coordinate co2 =  locationAt(j);

    //changing this on 4.21 to see if it makes a difference
   // temp.x = ( co1.x - co2.x );
   // temp.y = ( co1.y - co2.y);
    temp.x = ( co2.x - co1.x );
    temp.y = ( co2.y - co1.y);
    return temp;
}

//calculates the weight of this key point based on given coordinates
//the result is the Inverse of squared distance
//will result in 0 in the denominator if called with coords of
//a keypoint output location
double KeyPoint::weight(Coordinate coord, unsigned int CoordinateIndex )
{
    Coordinate current = locationAt(CoordinateIndex);

    double squared_distance_x = pow((coord.x - current.x),2);
    double squared_distance_y = pow((coord.y - current.y),2);

    if((squared_distance_x+squared_distance_y) > 0)
    {
        return (1.0/(squared_distance_x + squared_distance_y));
    }
    else //0 would be in denominator, should only happen with KP out locations
    {
        return 0.0;
    }
    
}


