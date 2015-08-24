//Mapper.h
//Author: Nikolas Szejna
//Keeps track of all key points given a keypoints file
//
#ifndef NLS_MAPPER
#define NLS_MAPPER

#include "KeyPoint.h"

#include<vector>
using std::vector;

#include <string>
using std::string;

#include <fstream>
using std::ifstream;


#include <sstream>
using std::stringstream;
using std::isdigit;


class Mapper
{
public:
    // constructor taking the name of a file containing keypoints
    Mapper(const char* filename, int numOuts);
    //default destructor
    //~Mapper();
    
    //reads KeyPoints into keypoints vector from fileName given
    //format is: x1 y1 x2 y2 for each line in the file
    //where (x1,y1) are the input coordinates for a keypoint
    //and (x2,y2) are the output coordinates for a keypoint
    int ReadKeyPointsFromFile(const char* fileName);
    
    //returns the distanceVector (change in x, change in y)
    //Using coordinate 
    Coordinate WeightedAverageDifference(Coordinate coord, bool useSource);
    
    //Returns the input source pixel after calculating
    //dest Pixel x and y minus Weighted Average distance x and y respectively
    Coordinate inputPixelValue(Coordinate destCoords,bool useSource);
    
    //returns true if given coordinate is a defined keypoint output destination
    //returns false if it is not
    bool coordIsKeyPointDest(Coordinate dest);
    
    //returns -1 if coord is not a keypoint dest, otherwise returns index
    //of keypoint with same coords
    int KeyPointDestIndex(Coordinate dest);

    //rounds a double to the nearest int result is still double.
    //EX: in-3.4 out-3.0
    double roundDouble(double n);

    //checks for the same keypoint destination at every keypoint location
    //if two coordinates are the same in the same column returns true
    //the desired result for this program is false
    int keyPointDuplicateCheck();
//------------------------public data-------------------------------------------

    bool keypointFileWasBad;
    int numberOfOutput;//to get the jth intermediate key point

//-----------------------simple getters-----------------------------------------

KeyPoint KeyPointAt(int index){return keypoints.at(index);}
int KeyPointCount(){return keypoints.size();}

//-----------------------simple setters-----------------------------------------

//add a keypoint to the keypoints vector
void addKeyPoint(KeyPoint kp){keypoints.push_back(kp);}

protected:
    /* data */
    vector<KeyPoint> keypoints;
    
};

#endif //NLS_MAPPER
