//ImageHandler.cpp implements ImageHandler class

#include "ImageHandler.h"

ImageHandler::ImageHandler(std::vector<Image*> images)
{
    sourceImage = images.at(0);
    destImage = images.at(1);
}

ImageHandler::~ImageHandler()
{
    //delete sourceImage;
    sourceImage = NULL;
    //delete destImage;
    destImage = NULL;

    //should possibly delete the vectors of image pointers here:

    //delete image store

    //delete PsPdStore
    for(unsigned int i = 0; i < PsPdStore.size();i++)
    {
        delete PsPdStore.at(i);
        PsPdStore.at(i) = NULL;
    }
    //delete interpolatedStore;
    for(unsigned int i = 0; i < interpolatedStore.size();i++)
    {
        delete interpolatedStore.at(i);
        interpolatedStore.at(i) = NULL;
    }
}


  //pa6 morphing and interpolation combination
  //creates a vector of images
int ImageHandler::pa6Interpolate(const char* keypointfilename)
{
    //unimplemented:: all of these methods need to be checked

    //get the number of output locations
    int N = pa6OutputNumber(keypointfilename);
    
    //this means the kp file has less than 3 keypoint locations for
    //each keypoint
    //could mean it only had source and/or dest or was completely empty
    if(N < 1)
    {
        return -1;
    }

    //fills pspdstore with copies of the source image
    //this guarentees they will be the same size
    populateVector(PsPdStore, (2*N));

    //morphs 2N images so minimum PsPdStore should look like:
    // PsFile1 // morphed image using the source
    // PdFile1 //morphed image using the dest
    if(fillPsPdStore(2*N, keypointfilename)==-1)return -1;

    populateVector(interpolatedStore, N);

    fillInterpolatedStore(N);

    return 0;
}

//populate vector with given # of copies of the source image
//returns -1 for fail, 0 for success
int ImageHandler::populateVector(std::vector<Image*> &v, int n)
{
    if(!v.empty())return -1;

    for(int i = 0; i <n;i++)
    {
        v.push_back(new Image(*sourceImage)); //this is destructed in the imageHandler

    }
    //check size of vector here
    return 0;
}

//morph images to fill the PsPdStore
int ImageHandler::fillPsPdStore(int N,const char* keypointfilename)
{
    int positionJ = 1;
    //bool incrementflag= false;
    for(int i = 0; i < N-1;i++)
    {
        //incrememnt flag  is for positionJ, only want to increment every other iteration
        //if(i%2 == 0) incrementflag = true;
       // else incrementflag = false;

        if(morph(sourceImage,PsPdStore.at(i), keypointfilename,positionJ,true)== -1)return -1;
        i++;
        if(morph(destImage,PsPdStore.at(i),keypointfilename,positionJ,false)==-1)return -1;
        //increment position J, refering to the jth keypoint every other iteration
        //if(incrementflag)
        positionJ++;
    }

    return 0;

}

//interpolate images to fill the InterpolatedStore
int ImageHandler::fillInterpolatedStore(int N)
{


    //loop ends at N-2 because it handles 2 at a time
    for(int i = 0; i < N;i = i+2)
    {
        //interpolate the contents of the PsPdStore
        interpolate(i,i+1,N);

    }
    return 0;

}

int ImageHandler::pa6OutputNumber(const char* keypointfilename)
  {
    
    //open key points file
    ifstream keysIn(keypointfilename);
    //if file did not open correctly return failure
    if(!keysIn) return -1;

    string lineInFile = "";

    //each iteration of this loop reads a line from file with name
    //fileName and should contain four integers
    bool firstLine = true;
    unsigned int keyPointCounter = 0;
    while(firstLine)
    {
        
        getline(keysIn,lineInFile);
        //create a new stringstream with each line 
        stringstream lineStream(lineInFile);
        
        int inX,inY;
    
        //read
        while(!lineStream.fail() && !lineStream.eof()) 
        {

            lineStream >> inX;
            if(!inX)break;
            lineStream >> inY;
            keyPointCounter++;
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
    
    firstLine = false;
        
        
    }
    keysIn.close();
    if(keyPointCounter < 3) return -1;//this mean that there is not
                                      //at least one output file

    return keyPointCounter-2;         //subtracting 2 because source and dest will be counter
  }




  //morph the input image and put morphed image in the output image
  //using the keypoints defined in the file with name keypointfilename
int ImageHandler::morph(Image* source, Image* dest, const char* keypointfilename, int J,bool useSource)
{
    //make a new Morpher object for the output image in the position J
    Morpher morphMachine(source, dest,keypointfilename,J,useSource);
                         
    return morphMachine.morphImage();
}


int ImageHandler::interpolate(int firstImageIndex, int secondImageIndex, int N)
{
    if(checkImageDimensions(pspdImageAt(firstImageIndex),pspdImageAt(secondImageIndex)) == -1)
    {
        return -1;// dimesions of input images are not equal, program exit
    }
    
    //fillImageStore(interpolationOutputs(firstImageIndex,secondImageIndex,N));
    //imageCount here will be N+2 after the call to interpolationOutputs
    //to fill the vector s
    for(int j = 0; j < interpolatedImageCount(); j++)
    {
         
          if(linearlyScale(j,N)==-1) return -1;  
          
    }
    return 0;
}

//linearly scales a single image determined by J
int ImageHandler::linearlyScale(int J, int N)
{
    std::vector<unsigned int> temp ;
    for(int i = 0; i < sourceImage->pixelCount(); i++)
    {

        int newPixel = pixelLinearScaler(i,J,N);
        temp.push_back(newPixel);
    }
    interpolatedImageAt(J)->fillPixels(temp);
    return 0;
}

//handles individual pixels, returns the integer value of a pixel
int ImageHandler::pixelLinearScaler(int position, int J, int N)
{
    double increment = (double)J*2.0;
    double Image1Pixel = (double)pspdImageAt(increment)->pixelAt(position);
    double Image2Pixel = (double)(pspdImageAt(1+increment)->pixelAt(position));
    double outputImageNumber = (double)J+1;
    double totalOutputCount = (double)(N+1);//not N+2 because of 0 index

    double interpolateFormula1 = (double)((N-outputImageNumber+1)/totalOutputCount);
    double interpolateFormula2 = (double)(outputImageNumber/totalOutputCount);

    // if(position == 10)
    // {
    //     cout << increment << endl<<Image1Pixel << endl
    //     <<Image2Pixel <<endl<< outputImageNumber<<endl << totalOutputCount << endl
    //     <<interpolateFormula1 <<endl << interpolateFormula2<<endl;
    // }
    
   return interpolatedImageAt(0)->roundDouble((interpolateFormula1 * Image1Pixel) +
                                 (interpolateFormula2 * Image2Pixel));
}


//checks the image dimensions for input images to be interpolated
//returns -1 if height or width do not match
int ImageHandler::checkImageDimensions(Image* image1, Image* image2) const
{
    if(image1->getWidth() == image2->getWidth())
    {
        if(image1->getHeight() == image2->getHeight())
        {
            return 0;
        }
        else return -1;
    }
    else return -1;
}

int ImageHandler::outputImages() const
{
    bool finalOutputFormatBinary = imageAt(0)->getInputFormat();
    
    for(int i = 0; i < imageCount(); i++)
    {
        //converts the index into a string containing only the index
        //conversion code pulled from cplusplus.com article
        //http://www.cplusplus.com/articles/D9j2Nwbp/
        string outName = static_cast<stringstream*>( 
                         &(stringstream() << i) )->str();
        if(finalOutputFormatBinary)
        {
            outName += ".pgm";
            imageAt(i)->setOutputFormat(true);
        }
        else
        {
            //****shouldnt depend on file extension here
            //because of a pa4 issues, removed the a in resubmit****
            outName += ".pgma";
            imageAt(i)->setOutputFormat(false);
        }
        //imageAt(i).printImageStatistics();
        //output each image through its writer function
        //outName is just a string filename but imageWriter checks
        //the file extension, so make sure they are all consistent
        
        if(imageAt(i)->imageWriter(outName) == -1)
        {
            return -1;
        }
    }
    return 0;
}

int ImageHandler::outputInterpolatedStore()
{
    for(unsigned int i = 0; i < interpolatedStore.size(); i++)
    {
        stringstream intString;
        intString << i+1;
        string outfilename = "";
        intString >> outfilename;
        outfilename = outfilename + ".pgm";
        if(interpolatedImageAt(i)->imageWriter(outfilename) == -1) return -1;
    }
    return 0;
}
int ImageHandler::outputpspdStore()
{
    for(unsigned int i = 0; i < PsPdStore.size(); i++)
    {
        stringstream intString;
        intString << i+1;
        string outfilename = "";
        intString >> outfilename;
        outfilename = outfilename + ".pspd";
        if(pspdImageAt(i)->imageWriter(outfilename) == -1) return -1;
    }
    return 0;
}

void ImageHandler::imageStoreToLog()
{
    for(int i = 0; i < imageCount();i++)
    {
        string note = "Image at index:";
        int indexint = i;
        stringstream in;
        in << indexint;
        string indexstring;
        in >> indexstring;
        note.append((indexstring));
        imageAt(i)->printImageStatistics(note);
    }
}
