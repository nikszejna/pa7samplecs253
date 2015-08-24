//Morpher.h
//Author: Nikolas Szejna
#ifndef NLS_MORPHER
#define NLS_MORPHER

#include "Mapper.h"
#include "Image.h"

class Morpher
{
public:

//constructor with input image pointer,, output image pointer and
//a filename with the keyPoints 
Morpher(Image* input, Image* output, const char* keypointfile, int J,bool useSource);

//checks if the input image has a source pixel with the 
//coordinates given 
bool inImageHasCoord(Coordinate inCoord);

//morphs the input image using the keypoints in the keypointfile
//to map every pixel to a new location in the output Image
//returns 0 for success
//returns -1 for failure
int morphImage();

inline void setkpfilename(const char* name){keypointfilename = name;}

inline const char* getkpfilename(){return keypointfilename;}

bool useSource;

//---------------------------PUBLIC DATA-------------------------------------------
int jthPoint;

protected:
    Image* inImage;
    Image* outImage;
    //file name of key points which will be used in the Mapper
    const char* keypointfilename;
    //Mapper map;
};
#endif //NLS_MORPHER