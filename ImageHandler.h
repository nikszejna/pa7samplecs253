//Author: Nikolas Szejna
//header file for the ImageHandler class
//the ImageHandler class instance is an object can manipulate objects 
//of type Image 
#ifndef IMAGE_HANDLER
#define IMAGE_HANDLER

#include "Image.h"
#include "Morpher.h"

class ImageHandler{
public:

  //default constructor
  ImageHandler();
  
  //copy constructor
  
  //overloaded assignment operator
  
  //destructor
  
  //constructor that takes a vector of Image pointers
  ImageHandler(std::vector<Image*> images);

  //destructor of imageHandler that deletes all pointer data in it
  ~ImageHandler();


  //main pa6 method, combines morphing and interpolation
  int pa6Interpolate(const char* keypointfilename);

  //method to calculate # of output images for pa6
  //given keypoint file this # is the # of intermediate kp locations
  //returns N
  int pa6OutputNumber(const char* keypointfilename);

  //populate vector with given # of copies of the source image
  //returns -1 for fal, 0 for success
  int populateVector(std::vector<Image*> &v, int n);

  int fillPsPdStore(int N,const char* keypointfilename);

  int fillInterpolatedStore(int N);

  int outputInterpolatedStore();

  int outputpspdStore();

  
  //method that given the indices two start Images will interpolate the two with 
  //N+2 output images as a result
  //this method does the error checking between the two images:
  //check the size of the images
  //returns -1 if an error is found
  int interpolate(int firstImageIndex, int secondImageIndex, int N);
  
  //outputs the images in the image store to files 
  //names will be the number of output image between 0.pgm and N+1.pgm
  //assumes the images in the imageStore are all files to be output
  //format: the format of the first image in the imageStore
  int outputImages() const;
  
  //prints image store information to ImageLog
  void imageStoreToLog();
  
  //morph the input image and put morphed image in the output image
  //using the keypoints defined in the file with name keypointfilename
  int morph(Image* source, Image* dest, const char* keypointfilename, int J,bool useSource);
  
//---------------------SIMPLE GETTERS-------------------------------------------
//------------------------------------------------------------------------------
  inline int imageCount() const {return ImageStore.size();}
  inline int interpolatedImageCount() const {return interpolatedStore.size();}
  //should probably be a reference to an image
  //NOT DOING THIS RIGHT NOW
  inline Image* imageAt(int index) const{return ImageStore.at(index);}
  inline Image* imageAt(int index){return ImageStore.at(index);}

  //constant version of getter
  inline Image* interpolatedImageAt(int index) const{return interpolatedStore.at(index);}
  //non constant version
  inline Image* interpolatedImageAt(int index){return interpolatedStore.at(index);}

    //constant version of getter
  inline Image* pspdImageAt(int index) const{return PsPdStore.at(index);}
  //non constant version
  inline Image* pspdImageAt(int index){return PsPdStore.at(index);}
  
  
  
//-----------------------SIMPLE SETTERS-----------------------------------------
//------------------------------------------------------------------------------  
  inline void fillImageStore(std::vector<Image*> vec){ImageStore = vec;}
  
protected:

//-----------------------PROTECTED DATA-------------------------------------------
//------------------------------------------------------------------------------

    //holds a pointer to the first input Image, containing the "source"
    Image* sourceImage;

    //holds a pointer to the second input Image, containing the "Destination"
    Image* destImage;

    //stores the images the handler will work with
    //maybe should be a vector of Image pointers for efficiency
    std::vector<Image*> ImageStore;

    // vector of intermediate images that have been morphed
    //should be of size 2N where N is # of intermediate keypoint locations
    //every 2 images in the vector is a pair Ps and Pd
    std::vector<Image*> PsPdStore;

    //vector of images interpolated from each pair of Ps and Pd
    //the size of the vector should be N
    std::vector<Image*> interpolatedStore;

    
   
    
    
//-----------------------PROTECTED METHODS----------------------------------------
//------------------------------------------------------------------------------

    //method to Linearly scale each pixel in an image
    //using J-output image number
    //N- total Number of output images not including two originals
    //returns -1 if fails for some reason
    int linearlyScale(int J, int N);
    
    //method to linearly scale a single pixel 
    //x and y are the pixel position
    //using J-output image number
    //N- total Number of output images not including two originals
    //sets actual pixels in the image at index J in the Image Store
    int linearlyScalePixel(int i, int J, int N);
    
    //contains the function to specificly calculate the value
    //of a scaled pixel and returns the integer value
    //does NOT set any pixels 
    int pixelLinearScaler(int position, int J, int N);
    
    //given two indices for images in the image store:
    //returns 0 if width and height are equal
    //returns -1 if width or height are not equal
    int checkImageDimensions(Image*,Image*) const;
    
    //given two images this method builds the imageStore with firstImageIndex
    //at position 0, then fills in N images with a copy of the first Image
    //and adds secondImageIndex at the end of the vector
    //**this will get more complicated with more input Images
    //returns the vector in this form that can be copied into imageStore
    std::vector<Image*> interpolationOutputs(int firstImageIndex, 
                                                  int secondImageIndex, int N);
};


#endif //IMAGE_HANDLER