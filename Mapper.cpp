//Mapper.cpp
//Implementation of class Mapper 
#include "Mapper.h"

// constructor taking the name of a file containing keypoints
Mapper::Mapper(const char* filename, int numOutput)
{
    if(ReadKeyPointsFromFile(filename) == 0)
    {
        keypointFileWasBad = false;
    }
    else
    {
        keypointFileWasBad = true;
    }

    if(keyPointDuplicateCheck() == -1)
    {
        keypointFileWasBad = true;
    }

    //set int that keeps track of which jth intermediate kP to use
    numberOfOutput = numOutput;


}

    //reads KeyPoints into keypoints vector from fileName given
    //format is: x1 y1 x2 y2 for each line in the file
    //where (x1,y1) are the input coordinates for a keypoint
    //and (x2,y2) are the output coordinates for a keypoint
int Mapper::ReadKeyPointsFromFile(const char* fileName)
{
    //open key points file
    ifstream keysIn(fileName);
    //if file did not open correctly return failure
    if(!keysIn) return -1;
    
    string lineInFile = "";
    
    //each iteration of this loop reads a line from file with name
    //fileName and should contain four integers
    while(getline(keysIn,lineInFile))
    {
        //create a new stringstream with each line 
        stringstream lineStream(lineInFile);
        
        int inX,inY;
        std::vector<Coordinate> CoordinateVector;
        CoordinateVector.reserve(10);
        while(!(lineStream.fail() || lineStream.eof()))
        {
            //read int a coordinate at a time
            lineStream >> inX;
            lineStream >> inY;  
            //make a coordinate from the ints
            Coordinate tempCo(inX,inY);
            CoordinateVector.push_back(tempCo);

        }

        
        
        if(lineStream.fail())
        {
            lineStream.clear();
            char tester;
            lineStream >> tester;
            if(!lineStream.fail()) return -1; //found a char in the KP file
            
        }
        
        //int to try a bad read on
        int ErrorTester;
        //if this read works the file format is bad
        //or reading is not working correctly
        lineStream >> ErrorTester;
    if(lineStream.fail() && lineStream.eof()); //no problems
    else return -1;
        
        
        //Check that the keyPoint has at least 3 coordinates
        //otherwise not valid
        if(CoordinateVector.size() < 3) return -1;
        //check if each size is the same as the first
        

        //create a keypoint with CoordinateVector
        KeyPoint temp(CoordinateVector);
        //store the keypoint in the mapper
        addKeyPoint(temp);
        
        
    }
    keysIn.close();
    return 0;
}




//returns the distanceVector (change in x, change in y)
//Using coordinate 
Coordinate Mapper::WeightedAverageDifference(Coordinate coord,bool useSource)
{
    double summedWeightDifferencesX =0.0;
    double summedWeightDifferencesY = 0.0;
    double summedWeight = 0.0;
    
    //loop through each keypoint in the mapper and sum W.a.d
    for(int i = 0; i < KeyPointCount(); i++)
    {
        //calculate the weight this keypoint has on given coord
        //number of output here is the Jth keypoint location
        double currentWeight = KeyPointAt(i).weight(coord,numberOfOutput);

        //calculate the differences of the keypoints inCoords and Outcoords
        //
        //modify this so that it calculates the right difference->
        //source or destination
        Coordinate differences = KeyPointAt(i).calcDifferences(useSource,numberOfOutput);
        
        //add results to sums
        summedWeightDifferencesX += (currentWeight *  (double)differences.x);
        summedWeightDifferencesY += (currentWeight * (double)differences.y);
        summedWeight += currentWeight;
    }
    
    //round change in x and chnage in y to the nearest integer 
    //to be used as coordinates
    int deltaX = roundDouble(summedWeightDifferencesX/summedWeight);
    int deltaY = roundDouble(summedWeightDifferencesY/summedWeight);
    
    Coordinate deltas(deltaX, deltaY);
    
    return deltas;

}

//Returns the input source pixel after calculating
//dest Pixel x and y minus Weighted Average distance x and y respectively
Coordinate Mapper::inputPixelValue(Coordinate destCoords,bool useSource){
    
    //if is a keypoint returns an index to find the keypoint
    int destIndex = KeyPointDestIndex(destCoords);
    
    //if is a keypoint returns the input source coordinates of the KP
    if(destIndex != -1) 
    {
        if(useSource)
        {
            return KeyPointAt(destIndex).locationAt(0);
        }
        else
        {
            return KeyPointAt(destIndex).locationAt(KeyPointAt(destIndex).locations.size()-1);
        }
    }
    
    Coordinate WeightedAverageDifs = WeightedAverageDifference(destCoords,useSource);
    Coordinate inputSourceCoordinate((destCoords.x - WeightedAverageDifs.x),
                                    (destCoords.y - WeightedAverageDifs.y));
                        
    return inputSourceCoordinate;   
}


//returns true if given coordinate is a defined keypoint output destination
//returns false if it is not
bool Mapper::coordIsKeyPointDest(Coordinate dest)
{
    //loop through each key point and check if it is the same
    for(int i = 0; i < KeyPointCount(); i++)
    {
        if(KeyPointAt(i).locationAt(numberOfOutput).x == dest.x)
        {
            if(KeyPointAt(i).locationAt(numberOfOutput).y == dest.y)
            {
                return true;
            }
        }
        
    }
    
    return false;
}

int Mapper::KeyPointDestIndex(Coordinate dest)
{
    if(coordIsKeyPointDest(dest))
    {
        
        for(int i = 0; i < KeyPointCount(); i++)
        {
            if(KeyPointAt(i).locationAt(numberOfOutput).x == dest.x)
            {
                if(KeyPointAt(i).locationAt(numberOfOutput).y == dest.y)
                {
                    return i;
                }
            }
            
        }
    }
    else
    {
        return -1;
    }
    return -1;
}

//rounds a double to the nearest int
//at halfway, will currently round up
double Mapper::roundDouble(double n)
{
      double t = 0.0;
      t = n - floor(n);
      if(t >= 0.5)  
      {
              
              n=ceil(n);
              
      }
      else 
      {
              
              n=floor(n);
              
      }
      return n;
}



    //checks for the same keypoint destination at every keypoint location
    //if two coordinates are the same in the same column returns false or -1
   // the desired result for this program is true or 0
    int Mapper::keyPointDuplicateCheck()
    {
        for(unsigned int loc = 1; loc< KeyPointAt(0).locations.size()-1;loc++)
        {
            std::vector<Coordinate> alreadySeen;
            for(int kpI=0;kpI < KeyPointCount();kpI++)
            {
                Coordinate current =KeyPointAt(kpI).locationAt(loc);
                int cX = current.x;
                int cY = current.y;
                for(unsigned int as = 0; as < alreadySeen.size(); as++)
                {
                    if(alreadySeen.at(as).x == cX && alreadySeen.at(as).y == cY)
                    {
                        return -1;
                    }
                }
                alreadySeen.push_back(current);
            }
        }
        return 0;
    }


