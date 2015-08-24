#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <fstream>
using std::ifstream;

#include "Image.h"
#include "ImageHandler.h"


/// Print the correct usage in case of user syntax error.
int Usage(char* arg0)
{
    cout << "Usage: " << arg0 << " filename1"<<" filename2" << "filename3" << endl;
    cout << "filename1: a pgm image to read in" << endl;
    cout << "filename2: file to output a pgm image" << endl;
    cout << "filename3: file with keypoints"  << endl;

    return -1;
}



int main(int argc, char* argv[]) {

    // check for the correct number of arguments
    if (argc != 4) return Usage(argv[0]);
    
    std::vector<Image*> inputs;
    
    for(int i = 1; i <3;i++)
    {

            //open the first file to read in
            ifstream in(argv[i]);
            //check if the input file failed
            if(in.fail())
            {
                return Usage(argv[0]);
            }
            else
            {
                //cout << "file open successful" << endl;
            }
            //instatiate an empty image object
            Image newImage;
        
            //read file into image object
            if(newImage.imageReader(in) == 0)
            {
                //cout << "format ok" << endl;
                //newImage.printImage();
            }
            else
            {
                //cout << "format bad" << endl;
                return -1;
            }
    
            //close input file
            in.close();

        
            inputs.push_back(new Image(newImage));
        
        
    }
    
    //create an ImageHandler to morph the image
    ImageHandler morpher(inputs);
    
    //morph the image at index 0, index 1 will be the output image
    //argv[3] should contain a keypoint file name
    if(morpher.pa6Interpolate(argv[3]) == 0)
    {
        //morph/interpolate was successful
    }
    else
    {
        return -1; //morph/interpolate failed!
    }
    

    if(morpher.outputInterpolatedStore() == 0)
    {
       //morpher.outputpspdStore(); //output of morphed/interpolated image was successful
    }
    else
    {
        return -1; //output of morphed/interpolated image failed
    }

    for(unsigned int i = 0; i <inputs.size();i++)
    {
        delete inputs.at(i);
        inputs.at(i) = NULL;
    }
    
    return 0;
}