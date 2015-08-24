//Author: Nikolas Szejna
//header file for the Image class
//the Image class instance is an object that represents
// a pgm file
#ifndef IMAGE_PGM
#define IMAGE_PGM

#include <iostream>
using std::istream;
using std::cout;
using std::endl;
using std::ios;

#include <fstream>
using std::ifstream;
using std::ofstream;


#include <vector>
using std::vector;

#include <string>
using std::getline;
using std::string;
using std::find;

#include <cstring>
using std::memcpy;

#include <sstream>
using std::stringstream;
using std::isdigit;

#include <cmath>
using std::ceil;
using std::floor;

typedef unsigned char Byte;

//typedef std::basic_ofstream<Byte, std::char_traits<Byte> > Byteofstream;



class Image{
public:
    
    //default constructor: all values initialized to 0;
    Image();
    
    //copy constructor
    // Image(const Image & oldImage);
    
    //overloaded assignment operator
    
    //destructor
    
    //reads a file in and fills this image object with a pgm file
    int imageReader(ifstream& in);
    //outputs this image object to a pgm format file
    int imageWriter(string filename);
    //scales all of the pixel values in the file to maximize contrast
    int scaleImage();

    //--------------Simple Getters------------------
    //----------------------------------------------
    inline int getWidth() const {return width;}
    inline int getHeight() const {return height;}
    inline int getDeclaredMax() const {return declaredMax;}
    inline int getMin() const {return pixelMin;}
    inline int getMax() const {return pixelMax;}
    inline double getAvg() const {return pixelAvg;}
    inline int pixelCount() const {return pixelValues.size();}
    inline int getPixelSum() const {return pixelSum;}
    inline bool getInputFormat() const {return inputFormatBinary;}
    inline bool getOutputFormat() const {return outputToBinaryPGM;}
    inline int getBytesInFile() const {return bytesInFile;}
    inline int pixelAt(int i) const {return pixelValues.at(i);}

    //inline bool pixelsModifiedCheck() const {return pixelsModified;}
    
    
    //convert to x, y coordinates
    inline int iToX(int index) const {return index % getWidth();}
    
    inline int iToY(int index) const {return index / getWidth();}
    
    //-------------NOT SIMPLE GETTERS-------------------------------------------
    //--------------------------------------------------------------------------
    
    //Assumes a zero based array
    //Formula for position is add y-1 rows(width pixels ea.) to pos
    //then add x for the row the pixel is in and subtract 1 to account for 
    //a zero based array
    //
    //NOTE!!!!!!!!!!! NOT THE SAME AS PIAZZA POST, LOOK AT THIS AGAIN
    //on piazza the formula is index = y*Width + x
    //inline int pixelAt(int x,int y) const {return (((getWidth()*(y-1))+x)-1);}  
    inline int pixelAt(int x,int y) const {return y*getWidth() + x;} 
    
    //-------------Simple setters------------------------
    //---------------------------------------------------
    void setBytesInFile(ifstream& in);
    inline void setBytesInFile(int bytecount){bytesInFile = bytecount;}
    inline void setWidth(int widthArg){width = widthArg;}
    inline void setHeight(int heightArg){height = heightArg;}
    inline void setDeclaredMax(int declaredMaxArg){declaredMax = declaredMaxArg;}
    inline void setMin(int min){pixelMin = min;}
    inline void setMax(int max){ pixelMax = max;}
    inline void setAvg(double avg){ pixelAvg =  avg;}
    
    inline void setInputFormat(bool isBinary){inputFormatBinary = isBinary;}
    inline void setOutputFormat(bool isBinary){outputToBinaryPGM = isBinary;}
    inline void setFileName(string infilename){fileName = infilename;}
    
    
    inline void fillPixels(const std::vector<unsigned int>& vec){pixelValues = vec;}
    inline void addPixel(int pixel){pixelValues.push_back(pixel);}
    inline void setPixelAt(int index, int value){pixelValues.at(index) = value;}
    inline void addToPixelSum(int pixel){pixelSum += pixel;}
    //inline void pixelWasModified(){pixelsModified = true;}
    
    

    //-------------Recalculate from Vector---------------
    //---------------------------------------------------
    //calls following methods to recalculate stats
    //and sets the private member variables corresponding
    void calculateMinMaxAvg();
    int calculateMin();
    int calculateMax();
    int calculateAvg();

    //-----------Output Statistics----------------------------
    //--------------------------------------------------------
    void printImageStatistics(string note) const;
    void printImage()const;
    
    //round a double to the nearest int
    double roundDouble(double n);

private:
    //----------------PRIVATE MEMBER VARIABLES----------------
    //--------------------------------------------------------
   // bool pixelsModified;
    //width specified in the file
    int width;

    //height specified in the file
    int height;

    //declared maximum pixel value specified in the file
    int declaredMax;

    //minimum pixel value in the file calculated with reader
    //or later updated with calculateMin
    int pixelMin;

    //maximum pixel value in the file calculated with reader
    //or later updated with calculateMax
    int pixelMax;
    
    //sum of all pixels added together, calculated on read in
    int pixelSum;

    //average pixel value in the file calculated with reader
    //or later updated with calculateAvg
    double pixelAvg;
    
    //number of bytes in the file including the header
    //number of pixels in the file should be total minus header
    int bytesInFile;
    
    //INPUT FORMAT FLAG
    //true if input was binary, signified by magic number 'P''5'
    //false if input was ascii, signified by magic number 'P''2'
    //default:true
    bool inputFormatBinary;
    
    //OUTPUT FORMAT FLAG
    //true: if file should output to binarypgm format
    //false: if file should output to asciipgm format
    //this is currently determined:: if file extension is
    //.pgma on output file then false
    //default:true
    bool outputToBinaryPGM;
    
    //filename of the file
    string fileName;
    

    //vector that holds all of the pixel values specified in
    //the pgm file
    vector<unsigned int> pixelValues;

    //----------------PRIVATE METHODS-------------------------
    //--------------------------------------------------------
    int charArrayToInt(unsigned char* array,int offset, int length);
    
    std::vector<unsigned int> charArrayToPixelVector
                (unsigned char* array,int pixelCount);

    int checkPixelValue(int pixel);
    
    int checkMagicNumber(ifstream& in, char secondCharacter);
    
    //checks if the magic number is for binary pgm:'P' '5'
    int checkBinaryMagicNumber(ifstream& in);
    
    int checkAsciiMagicNumber(ifstream& in);
    
    //given a line of text, checks if there is a decimal
    //returns 0 if decimal is in comment -1 if outside
    int checkForDecimal(const string& line);
    //given a reference to a stream, extracts the next integer
    //returns the int if successful, negative if fails
    //ignores comments
    int extractStreamInt(ifstream& in);
    
    int extractStreamInt(stringstream& lineStream);
    
    //writes header minus magic number then pixels 
    //in binary format to a file
    int binaryImageWriter(ofstream& out);
    
    int checkFileExtension(string filename);
    
    //writes header minus magic number
    int writeBinaryHeader(ofstream& out);
    
    int writeBinaryPixels(ofstream& out);
    
    //converts 1 int to an char array of size btyecount
    std::vector<Byte> intByteArray(int input,int byteCount);

    //calls extractStreamInt and adds height requirements
    int readHeight(ifstream& in);

    //calls extractStreamInt and adds width requirements
    int readWidth(ifstream& in);

    //calls extractStreamInt and adds declaredMax requirements
    int readDeclaredMax(ifstream& in);

    //calls extractStreamInt and adds pixel Value requirements
    int readPixelValues(ifstream& in);
    
    //given an output stream writes pixels in w x h rows to a file
    int pixelWriter(ofstream& out);
    
    //scales a single pixel int value and returns scaled int value
    //will scale based on a maximum value given
    int scalePixel(int pixel, int max);
    
    
    
//-----------------METHODS FOR BINARY IMAGE READING----------------------------
//-----------------------------------------------------------------------------
    
    int binaryImageReader(ifstream& in);
    
    int fileToByteArray(ifstream& in,Byte* dest);
    
    int extractImageFromByteArray(Byte* fileinbytes);
    
    int extractHeaderFromByteArray(Byte* fileinbytes);
    
    int readBinaryHeight(Byte* fileinbytes);
    
    int readBinaryWidth(Byte* fileinbytes);
    
    int readBinaryDeclaredMax(Byte* fileinbytes);
    
    int readBinaryPixels(Byte* fileinbytes);
    
    int convertByteToInt(Byte* input, int bytecount);

};

#endif //IMAGE_PGM