//Morpher.cpp
//Author: Nikolas Szejna
//Implementation of the Morpher class

#include "Morpher.h"

Morpher::Morpher(Image* input, Image* output, const char* keypointfile, int J,bool useSrc)
{
    inImage = input;
    outImage = output;
    jthPoint = J;
    setkpfilename(keypointfile);
    useSource = useSrc;

    //Mapper temp(keypointfile);
    //makeMap(temp);
    //map = temp;
}

bool Morpher::inImageHasCoord(Coordinate inCoord)
{
    int imageWidth = inImage->getWidth() -1;
    int imageHeight = inImage->getHeight()-1;
    
    //if coordinates are outside of the height or the width
    //there is not a pixel to take so it will be black or 0
    //
    //check x coord
    if(inCoord.x > imageWidth || inCoord.x < 0) return false;
    //check y coord
    if(inCoord.y > imageHeight|| inCoord.y < 0) return false;
    
    //Coordinate is in the image 
    return true;
}

int Morpher::morphImage()
{
    Mapper map(getkpfilename(),jthPoint);
    //return -1 and exit if the keypoint file was bad
    if(map.keypointFileWasBad) return -1;
    
    //loop through each pixel in destination and map to input
    //source coordinate
    for(int i = 0; i < outImage->pixelCount();i++)
    {
        //input source location to be used
        Coordinate InputSource;
        //convert the pixel index to xy coordinates
        Coordinate location(outImage->iToX(i), outImage->iToY(i));
        
        if(useSource)
        {
            //find the input source location using the mapper
            InputSource = map.inputPixelValue(location,true);
        }
        else //use the destImage to morph
        {
            //find the input source location using the mapper
            InputSource = map.inputPixelValue(location,false);
        }
        

        if(inImageHasCoord(InputSource))
        {
            //find the index of the input source pixel
            int SourceIndex = inImage->pixelAt(InputSource.x,InputSource.y);
            
            //set output pixel to input source pixel
            outImage->setPixelAt(i,inImage->pixelAt(SourceIndex));
            
        }
        else
        {
            //set output pixel to a black pixel
            //to represent a pixel off of the original input image
            outImage->setPixelAt(i,0);
        }
        
        
    }
    
    return 0;
}