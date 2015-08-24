//KeyPoint.h
//Author: Nikolas Szejna
//
//Holds a key point which consists of the integer (x,y) coordinates
//of an input source coordiante and an output destination coordinate
#ifndef KEY_POINT
#define KEY_POINT

#include "Coordinate.h"
#include <cmath>
using std::pow;
#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

class KeyPoint
{
public:

    //default constructor
    KeyPoint();
    
    //constructor with a vector of Coordinates in the form
    //inCoordinate intermediateCoord1... intermediateCoordN outCoordinate
    KeyPoint(std::vector<Coordinate> keys);

    //keyPoint(std::vector<int> intCoordinates);
    
    //default destructor
    //~KeyPoint();
    
    //calculates the weight of this key point based on given coordinates
    //the result is the Inverse of suqared distance
    //will result in 0 in the denominator if called with coords of
    //a keypoint output location
    double weight(Coordinate coord,unsigned int CoordinateIndex);
    
    //Will calculate the difference of the x and y coordinates
    //of two of the coordinates in the keyPoints resulting in a difference vector
    //the resulting coordinate is coordinate at index i - coordinate at index j
    Coordinate calcDifferences( bool useSource, int j);

//-----------------------------Simple Getters----------------------------------
//-----------------------------------------------------------------------------

    inline Coordinate locationAt(const int i){return locations.at(i);}

//-----------------------------Simple Getters----------------------------------
//-----------------------------------------------------------------------------

    inline void setLocationAt(const int i, Coordinate in){locations.at(i) = in;}

    

//----------------------------PUBLIC DATA---------------------------------------

    std::vector<Coordinate> locations; //represents one line of keypoints
   
protected:

//--------------------------PROTECTED METHODS------------------------------------

    
   
};

#endif //KEY_POINT
