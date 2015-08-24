//Author: Nikolas Szejna
//implementation of Image.h for the Image class
//the Image class instance is an object that represents
// a pgm file
#include "Image.h"
//--------------------------------Constructor----------------------------------
//-----------------------------------------------------------------------------
Image::Image()
{
    bool outputFormatBinary = true;
    bool inputFormatWasBinary = true;
    
   // pixelsModified = false;
    
    width = 0;
    height = 0;
    declaredMax = 0;
    pixelMin = 0;
    pixelMax = 0;
    pixelAvg = 0.0;
    pixelSum = 0;
    //these remain unless changed by the input method
    //input format depends on the magic number in the file
    setInputFormat(inputFormatWasBinary);
    //output format depends on if the file extension is .pgma
    setOutputFormat(outputFormatBinary);
    pixelValues.reserve(50000);
}

// //copy constructor
// Image::Image(const Image & oldImage)
// {
//     setBytesInFile(oldImage.getBytesInFile());
//     setWidth(oldImage.getWidth());
//     setHeight(oldImage.getHeight());
//     setDeclaredMax(oldImage.getDeclaredMax());
//     setMin(oldImage.getMin());
//     setMax(oldImage.getMax());
//     setAvg(oldImage.getAvg());
    
//     setInputFormat(oldImage.getInputFormat());
//     setOutputFormat(oldImage.getOutputFormat());
    
//     for(int i = 0; i < oldImage.pixelCount(); i++)
//     {
//         addPixel(oldImage.pixelAt(i));
//         addToPixelSum(oldImage.pixelAt(i));
//     }
    
// }

// //assignment operator overload

// //destructor
// Image::~Image()
// {
    
// }

int Image::scaleImage()
{
    declaredMax = 255;
    
    
    //cout << "scaler test " << scalePixel(130,255) <<endl;
    
    //scale each pixel in the pixelValues
    for(int i = 0; i < pixelCount(); i++)
    {
        int newPixel = scalePixel(pixelValues.at(i),declaredMax);
        if(newPixel >= 0 && newPixel < (declaredMax +1))
        {
           pixelValues.at(i) = newPixel;
           //cout << "pixel #:" << i << " set to:" << pixelValues.at(i) << endl;
        }
        else
        {
            //cout << " pixel value out of range:" << newPixel << endl;
            return -1;
        }
        
    }
    pixelMin = 0;
    pixelMax = 255;
    return 0;
}

//scales one pixel given int input and max, outputs int rounded from a double
int Image::scalePixel(int pixel, int max)
{ 
    double tempPixel;
    double numerator = (max * (pixel - pixelMin));
    
    double denominator = (pixelMax - pixelMin);
    if(denominator > 0)
    {
        tempPixel = numerator /denominator;
        
    }
    else return -1; //divide by zero error
    
    //rounding to the nearest integer, halfway rounds to 0;
    tempPixel = roundDouble(tempPixel);
    if(tempPixel > max) tempPixel = max;
    if(tempPixel < 0) return -1;
    
    
    return (int)tempPixel;
    
}

//rounds a double to the nearest int
//at halfway, will currently round up
double Image::roundDouble(double n)
{
      double t;
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

//outputs an image object to a file in pgm image format
//assumes an image has been scaled
int Image::imageWriter(string infileName)
{
    string magicNumber = string();
    //since PA4 this needs to be calculated at time of output
    int newDeclaredMax = 255;
    setDeclaredMax(newDeclaredMax);
    
    
    //****shouldnt depend on file extension here
    //because of a pa4 issues****
    if(checkFileExtension(infileName) != 0)
    {
        bool formatIsAscii = false;
        setInputFormat(formatIsAscii);
        setOutputFormat(formatIsAscii);
    }
    
    
    if(getOutputFormat())//true is binary  output
    {
        magicNumber = "P5";
    }
    else magicNumber = "P2";
    
    
    
    
    
    
    if(getOutputFormat())//true if binary output
    {
       ofstream bOut(infileName, ios::out | ios::binary);
       if( binaryImageWriter(bOut) == -1) return -1;
       bOut.close();
    }
    else
    {
        
       ofstream out(infileName);
        //Write Magic number to file
       out << magicNumber << endl;
       //Write width to file
       out << width << endl;
       //write height to file
       out << height << endl;
       //write 255 as declared max to file
       out << newDeclaredMax << endl;
       //-File should always have 255 declared max after scaling
    
        //write all of the pixels to the file width pixels per line
        // and height # of lines
        if(pixelWriter(out) == -1) return -1;
        if(out.fail()) return -1;
        out.close();
        
    }
    
    
    
   return 0;
}

int Image::pixelWriter(ofstream& out)
{
    int i = 0;
    int j = 0;
    while(i < pixelCount())
    {
      
        out << pixelValues[i] << " ";
        
        //this group of statements roughly alligns pixel values
        //in a file when outut based on how many digits the pixel value is
        /*if(pixelValues[i+1] < 10)
        {
            out << "  ";
        }
        else if(pixelValues[i+1] < 100)
        {
            out << " ";
        }
        */
        if(j == (width-1))
        {
           out << endl; 
        }

        i++;
        j = (j+1) % width;
    }
    if(out.fail()) return -1;
    else return 0;
}

//-----------------------OUTPUT TO BINARY METHODS-------------------------------
//------------------------------------------------------------------------------

int Image::binaryImageWriter(ofstream& out)
{
    //writes width height and declared max in binary format
    //to a file
    if(writeBinaryHeader(out) == -1) return -1;
    //writes w x h pixels in binary format to a file
   if( writeBinaryPixels(out)== -1) return -1;
   
   return 0;
}

//checks if file name ends with .pgma
int Image::checkFileExtension(string filename)
{
    int start = filename.find_last_of('.',filename.length());
    //assuming cannot have a filename with only .pgma
    //in place of what should be at least onechar.pgma
    if(start>1 && (filename.length()-start) == 5)//5  for chars ".pgma"
    {
        string extension;
        for(int i = 1; i <= 4;i++)
        {
            extension += filename.at(start+i);
        }
        
        if(extension.compare("pgma") == 0) return -1;
    }
    return 0;
}

//writes P5 width height and declared max in binary format
//to a file
//stream out should be a file opened in binary mode
int Image::writeBinaryHeader(ofstream& out)
{
    
    int sizeOfHeaderBlocks = 4;
    
   // Byte* init = {NULL};
    
    std::vector<Byte>  widthBinary  = {NULL};
    std::vector<Byte>  heightBinary =  {NULL};
    std::vector<Byte>  dmBinary =  {NULL};
    
    
    widthBinary = intByteArray(getWidth(),sizeOfHeaderBlocks);
    heightBinary = intByteArray(getHeight(),sizeOfHeaderBlocks);
    dmBinary = intByteArray(getDeclaredMax(),sizeOfHeaderBlocks);
    
    
   // intByteArray(getWidth(),4,widthBinary);
    unsigned int counter= 0;
   
    out.put('P');
    out.put('5');
   // out.write(reinterpret_cast<const unsigned char*>(&widthBinary[0]), 4);
   for(int i = 0; i < sizeOfHeaderBlocks; i++)
   {
       unsigned char outchar = static_cast<unsigned char>(widthBinary[i]);
       out.put(outchar);
       counter++;
   }
   for(int i = 0; i < sizeOfHeaderBlocks; i++)
   {
       unsigned char outchar =static_cast<unsigned char>(heightBinary[i]); 
       out.put(outchar);
       counter++;
   }
   for(int i = 0; i < sizeOfHeaderBlocks; i++)
   {
       unsigned char outchar = static_cast<unsigned char>(dmBinary[i]);
       out.put(outchar);
       counter++;
   }
    //cout << "counter:" << counter;
    return 0;
}

//writes w x h pixels in binary format to a file
int Image::writeBinaryPixels(ofstream& out)
{
    Byte onePixel;
    onePixel = ' ';
    unsigned int outputCounter = 0;
    for(int i = 0; i < (pixelCount()); i++)
    {
         onePixel = intByteArray(pixelValues.at(i),1).at(0);
         //out.write((const char*)onePixel,1);
         out.put(onePixel);
         outputCounter++;
        
    }
    //cout << "wrote " << outputCounter << "pixels to file" << pixelCount() <<endl;
    
    if(out.fail()) return -1;
    else return 0;
}

//assumes byte array of size 1 or 4
std::vector<Byte>  Image::intByteArray(int input,int byteCount)
{
    int tempInt = input;
    Byte* byteArray = reinterpret_cast<Byte*>(&tempInt); 
    
    // if(byteCount == 1)
    // {
    //     //return byteArray;
    // }
    // else
    // {
        
        // byteArray[3] = (input >> 24) & 0xFF;
        // byteArray[2] = (input >> 16) & 0xFF;
        // byteArray[1] = (input >> 8) & 0xFF;
        // byteArray[0] = input & 0xFF;        
        
        //largest byte shifted the most, shifts smaller bits out of the way
        byteArray[0] = (input >> 24) & 0xFF;
        byteArray[1] = (input >> 16) & 0xFF;
        byteArray[2] = (input >> 8) & 0xFF;
        byteArray[3] = input & 0xFF;       
        
       
        
        
    // }
    std::vector<Byte> newArray;
    if(byteCount == 1)
    {
        newArray.push_back(0);
        newArray.at(0) = byteArray[3];
    }
    else
    {
        for(int i = 0; i <byteCount;i++)
        {
            newArray.push_back(0);
            newArray.at(i) = (byteArray[i]);
        }
    }
    return newArray;
    
   
  
}





//----------------------------INPUT METHODS-----------------------------------
//----------------------------------------------------------------------------

int Image::imageReader(ifstream& in) {

    int returnValue;

    //check for "P2" as first two characters
    //and whitespace after. if fails returns negative
    if(checkAsciiMagicNumber(in) == 0)
    {
        bool ascii = false;
        //-------------------POTENTIAL ISSUE------------------------------------
        //may be forcing ascii output regardless of image type
        setInputFormat(ascii);
        setOutputFormat(ascii);
    }
    else
    {
        in.clear();
        in.seekg (0, in.beg);
        if(checkBinaryMagicNumber(in) == 0)
        {
            if(binaryImageReader(in) == 0)
            {
                return 0;
            }
            else return -1;
        }
        //cout << "bad magic number" << endl;
        return -1; //Magic number is not Valid
    }
    
    //if not binary or failed reads in ascii file
    if(readWidth(in) == -1)
    {
        //cout << "failed Width extraction" << endl;
        returnValue =  -1;
    }
    else
    {
        //cout << "extracted a pixel width of " << getWidth() << " from stream" << endl;
        returnValue =  0;
    }
    
     if(readHeight(in) == -1)
    {
        //cout << "failed Height extraction" << endl;
        returnValue =  -1;
    }
    else
    {
        //cout << "extracted a pixel height of " << getHeight() << " from stream" << endl;
        returnValue =  0;
    }
    
      if(readDeclaredMax(in) == -1)
    {
        //cout << "failed dMax extraction" << endl;
        returnValue =  -1;
    }
    else
    {
        //cout << "extracted a declared max pixel value of " << getDeclaredMax() << " from stream" << endl;
        returnValue =  0;
    }
    
      if(readPixelValues(in) == -1)
    {
        //cout << "failed pixelValue extraction" << endl;
        returnValue =  -1;
    }
    else
    {
        
        //cout << "extracted pixel value count of " << pixelCount() 
            // << " from stream equal to " << getHeight()<< "*" <<getWidth() << endl;
         
        returnValue =  0;
    }
    
    calculateAvg();
    
    return returnValue;



}

int Image::charArrayToInt(unsigned char* array,int offset, int length)
{
    int bytecount = length;
    unsigned int bytes[bytecount];
    
    for(int i = 0; i <bytecount; i++)
    {
      bytes[i] = (static_cast<unsigned int>(array[i+offset]));
    }
    
    unsigned int theInt = 0;
    
    for(int i = 0; i <bytecount; i++)
    {
      //shift by 8 bits for every byte other than the first
      //decrement shift by 8 bits(1byte) on each iteration
      //for proper alignment
      int bitshift = ((bytecount-1)*8) - (8*i);
      theInt |= (bytes[i] << bitshift);
    }
   return theInt;
}

std::vector<unsigned int> Image::charArrayToPixelVector(unsigned char* array,int pixelCount)
{
  
  int oneByte = 1;
  std::vector<unsigned int> pixels;
  //cout << "pixelcount" << pixelCount <<  endl;
  for(int i = 14; i <pixelCount+14; i++)
  {
      
    unsigned int temp = charArrayToInt(array,i,oneByte);
    ////cout << "pixel value:" << temp <<endl;
      //check for validity of a pixel and update pixel stats
    if(checkPixelValue(temp) != 0)
    {
       // pixels.push_back(-1);
        
    }
    else
    {
        
            pixels.push_back(temp);
        
        
    }
     
     //cout << "pixel:" << pixels.at(i-14);
     
  }
  //cout << "size in charArrayToPixelVector" << pixels.size() << endl;
  return pixels;
}

int Image::binaryImageReader(ifstream& in)
{
    setBytesInFile(in);
    Byte* fileBytes = new Byte[getBytesInFile()];
    if(fileToByteArray(in,fileBytes)!=0) return -1;
    if(extractImageFromByteArray(fileBytes) != 0) return -1;
    
    delete [] fileBytes;
    fileBytes = NULL;
    
    return 0;
}

void Image::setBytesInFile(ifstream& in)
{
    //set the file pointer to the end of the file
    in.seekg (0, in.end);
    //get position of the end of the file
    int length = in.tellg();
    //set the file pointer to the beggining of the file
    in.seekg (0, in.beg);
    
    setBytesInFile(length);
}

//bytes in file:byesInFile must be set before calling this method
int Image::fileToByteArray(ifstream& in, Byte* dest)
{
    //int headerSizeInBytes = 14;
    //set the file pointer to the begining of the file
    in.seekg (0, in.beg);
    for(int i=0;i<(getBytesInFile());i++)
    {
        dest[i] = in.get();
    }
   // in.read((char*)dest,getBytesInFile());
    if(in.fail()) return -1;
    else return 0;
   
}

int Image::extractImageFromByteArray(Byte* fileinbytes)
{
    if(extractHeaderFromByteArray(fileinbytes) != 0) return -1;
    if(readBinaryPixels(fileinbytes) != 0) return -1;
    
    return 0;
}

int Image::extractHeaderFromByteArray(Byte* fileinbytes)
{
    if(readBinaryWidth(fileinbytes) != 0) return -1;
    if(readBinaryHeight(fileinbytes) != 0) return -1;
    if(readBinaryDeclaredMax(fileinbytes) != 0) return -1;
    
    return 0;
}

int Image::readBinaryWidth(Byte* fileinbytes)
{
    int sizeOfWidth = 4;
    int offsetInFile = 2;
    
    int returnVal = charArrayToInt(fileinbytes,offsetInFile,sizeOfWidth);
    
    if(returnVal > 0)
    {
        setWidth(returnVal);
        return 0;
    }
    else return -1;
    
    
    
}

//method should be deleted, new version is charArrayToInt
int Image::convertByteToInt(Byte* input, int bytecount)
{
    string intstring((const char*)input,bytecount);
    int x;
    stringstream converter(intstring);
    converter >> x;
    return x;
}

int Image::readBinaryHeight(Byte* fileinbytes)
{
    int sizeOfHeight = 4;
    int offsetInFile = 6;
    
    
    int returnVal = charArrayToInt(fileinbytes,offsetInFile,sizeOfHeight);
    
    if(returnVal >0)
    {
        setHeight(returnVal);
        return 0;
    }
    return -1;
}

int Image::readBinaryDeclaredMax(Byte* fileinbytes)
{
    int sizeOfDeclaredMax = 4;
    int offsetInFile = 10;
    
    int returnVal = charArrayToInt(fileinbytes,offsetInFile,sizeOfDeclaredMax);
    if(returnVal > 0 && returnVal <=256)
    {
        setDeclaredMax(returnVal);
        return 0;
    }
    return -1;
}


int Image::readBinaryPixels(Byte* fileinbytes)
{
    //int sizeOfBinaryPixel = 1;
    int offsetInFile =14;
    
    
    unsigned int pixelsToRead = (unsigned int)(getBytesInFile() - offsetInFile);
    
    //pixels should be equal to bytes left in the file after
    //the header
    if(pixelsToRead != (unsigned int)(getWidth() * getHeight()))
    {
       return -1; 
    }
    
    
    std::vector<unsigned int> temp(
                     charArrayToPixelVector(fileinbytes,pixelsToRead));
    if(temp.size()!= pixelsToRead)
    {
        //cout << temp.size();
        //cout << "died here in readBinaryPixels" << endl;
        return -1;
    }
    
    fillPixels(temp);
    return 0;
}

//checks a pixel for valid values and sets min/max pixel and adds to the sum
//of pixels
int Image::checkPixelValue(int pixel)
{
        if(pixel < 0)
        {
            return -1;
        }
        else if(pixel <= getDeclaredMax())
        {
           // cout << "comparing pixel:" << pixel << " to dm" << getDeclaredMax() <<endl;
            if(getDeclaredMax() < 256)
            {
                //adding the pixel value to the vector
                addPixel(pixel);
                //checking pixel value against current max pixel 
                if(pixel > getMax()) setMax(pixel);
                //checking pixel value against current mix pixel 
                if(pixel < getMin()) setMin(pixel);
                //adding the pixel to the current sum
                addToPixelSum(pixel);
            }
            else
            {
                if(pixel == getDeclaredMax() && declaredMax == 256)
                {
                    //cout << "Invalid pixel of value 256, must be <255" << endl;
                    return -1;
                }
            }
        }
        else//pixel is greater than dec max
        {
            return -1;
        }
        return 0;
}
//secondCharacter is for now:'2' or '5' assuming P2 or P5 MNs
//should probably write a method that just compares a given char[] 
//to file input
int Image::checkMagicNumber(ifstream & in, char secondCharacter) 
{
    //check for "P2" as first two characters
    //and whitespace after if fails return negative
    char firstChar;
    char secondChar;
    in.get(firstChar);
    if(in.fail())return -1;
    else
    {
        if(firstChar != 'P') return -1;
        else
        {
            in.get(secondChar);
            if(in.fail())return -1;
            else
            {
                
                if(secondChar != secondCharacter) return -1;
            }
        }
    }


    return 0;
}

int Image::checkAsciiMagicNumber(ifstream& in)
{
    return checkMagicNumber(in,'2');
}

int Image::checkBinaryMagicNumber(ifstream& in)
{
    return checkMagicNumber(in, '5');
}
    

//returns int if found or -1 if invalid data found
int Image::extractStreamInt(ifstream& in)
{
    int temp = -1;
    string comment = "";
    in >> temp;
    if(in.fail())
    {
        in.clear();
        char commentChecker;
        in >> commentChecker;
        if(commentChecker == '#')
        {
           //cout << "found a comment" <<endl;
           getline(in,comment);
           //in.ignore(75,'\n');
           in.clear();
           return extractStreamInt(in);
        }
        else return -1; //This case means that the method found non-integer
                        //data that was not in comment form
        
        
    }
    //cout << "extract stream in returning" << temp << endl;
    return temp;
}

int Image::extractStreamInt(stringstream& lineStream)
{
    int temp;
    string comment = "";
    lineStream >> temp;

    if(lineStream.fail())
    {
        lineStream.clear();
        char commentChecker;
        lineStream >> commentChecker;
        if(commentChecker == '#')
        {
           getline(lineStream,comment);
           //in.ignore(75,'\n');
            lineStream >> temp; //Multiple comment in a row will be an issue
        }
        else
        {
            //cout << "lineStream extr error" << endl;
            return -1; //This case means that the method found non-integer
                         //data that was not in comment form
        }
                       
        
        
    }
    return temp;
}



int Image::readWidth(ifstream& in)
{
    //cout << "calling extract stream int in readwidth" << endl;
    int tempWidth = extractStreamInt(in);
    if(tempWidth < 0) return -1;
    else
    {
        //cout << "setting width to:" << tempWidth <<endl;
        this->setWidth(tempWidth);
        return 0;
    }
    
}

int Image::readHeight(ifstream& in)
{
    int tempHeight = extractStreamInt(in);
    if(tempHeight < 0) return -1;
    else
    {
        this->setHeight(tempHeight);
        return 0;
    }
    
}

int Image::readDeclaredMax(ifstream& in)
{
    int tempMax = extractStreamInt(in);
    if(tempMax < 0) return -1;
    else if(tempMax > 256) return -1;
    else
    {
        this->setDeclaredMax(tempMax);
        return 0;
    }
    
}

int Image::checkForDecimal(const string& line)
{
    int decimalPosition = line.find_first_of('.',0);
    
    if(decimalPosition >= 0)
        {
            //cout << "found a decimal value with find" << endl;
            int commentPosition = line.find_first_of('#',0);
            
            if(commentPosition >= 0)
            {
                //cout << line << endl;
               
               if(decimalPosition <= commentPosition)
               {
                   //cout << "found decimal before comment" << endl;
                   return -1;
               }
               else{return 0;} //do nothing
            }
            else
            {
                //cout << " found a decimal in a line without a comment" << endl;
                return -1;
            }
           
        }
    else
    {
        return 0; // no decimal was found in the line
    }
}

int Image::readPixelValues(ifstream& in)
{
    pixelMin = declaredMax;
    int returnValue = 0;
    int tempPixel;
    
    
    int linecounter = 1;
    string textline;
    
    while(getline(in,textline))
    {
        
       if(checkForDecimal(textline) == -1) return -1;
       stringstream line_stream(textline);
        
       while(line_stream >> tempPixel)
       {
            //this else if statement makes sure if it finds a decimal
            //it will only return -1 if it is not in a comment
            
            
            
            //check each pixel value:
             
             if(line_stream.fail())
             {
                 line_stream.clear();
                 char tempChar;
                 string throwaway;
                 line_stream >> tempChar;
                 if(tempChar == '#')
                 {
                     getline(line_stream,throwaway);
                     break;
                     //line_stream >> tempPixel; //Multiple comment in a row will be an issue
                 }
                 //this case should be hanfled in checkfordecimal above
                 else if(tempChar == '.')
                 {
                     //cout << "found decimal without prepended int" << endl;
                     return -1;
                 }
                 else
                 {
                     //cout << "lineStream extraction error" << endl;
                    return -1; //This case means that the method found non-integer
                         //data that was not in comment form
                 }
             }
             //tempPixel = extractStreamInt(line_stream);
             //cout << "temp Pixel: " << tempPixel << endl;
            
            
            //check for validity of a pixel and update pixel stats
            if(checkPixelValue(tempPixel) != 0) return -1;
            
           
        
        }
        //this handles the case where the linestream fails at the end
        //of a line
        //otherwise will not catch a line like:
        // 1 2 1 3 a
        //because the integers were read and it would move to the next line
        if(line_stream.fail())
        {
            char badChar = '0';
            line_stream.clear();
            line_stream >> badChar;
            if(!isdigit(badChar))
            {
                if(badChar != '#')
                {
                    //cout << "found a bad pixel:" << badChar << endl;
                    return -1;
                }
                
            }
        }
       
        
            
           //getline(in,textline);
            linecounter++;
            //cout << "linecounter" << linecounter << endl;
        }
        
         //check pixelCount against height x width
        if(pixelCount() != (getHeight() * getWidth()))
        {
            //cout << "pixel count is not height x width" << endl;
            returnValue = -1;
        }
    
    
    return returnValue;
}

//sets pixelAvg in Image class
int Image::calculateAvg()
{
    double temp = getPixelSum()/(double)pixelCount();
    if(temp > 0) setAvg(temp);
    else return -1;
    return 0;
}


void Image::printImageStatistics(string note) const {
    //cout << "pixelSum" << " "<< getPixelSum() << endl;
    //cout << getMin() << " " <<getAvg() << " " << getMax()  << endl;
    
    string outformat = "ascii";
    string informat = "ascii";
    
    if(getOutputFormat()) outformat = "binary";
    if(getInputFormat()) informat = "binary";
    
    ofstream outstat("ImageLog",std::ofstream::out | std::ofstream::app);
    outstat << note << endl;
    outstat << "Input type:" << informat << endl;
    outstat << "output type:" << outformat << endl;
    //outstat << "file ext"
    outstat << "width:" << getWidth() << endl;
    outstat << "height" << getHeight() << endl;
    outstat << "declaredMax:" << getDeclaredMax() << endl;
    outstat << "pixel count:" << pixelCount() << endl;
    // if(pixelsModifiedCheck())
    // {
    //     outstat << "pixels were modified" << endl;
    // }
    // else
    // {
    //     outstat << "pixels were not modified" << endl;
    // }
    // if(getInputFormat())
    // {
    //     outstat << "bytes in file " << getBytesInFile() << endl;
    // }
    
    //outstat << "location/mem-adress:" << this << endl;
    
    outstat << endl << endl;
    
    outstat.close();
     
}

void Image::printImage() const
{
for(unsigned int i = 0; i<(unsigned int)pixelCount(); i+=1) {

if(pixelAt(i) < getMax() * 0.05) {

cout << "███" ;

}
else if(pixelAt(i) < getMax() * 0.1) {
cout << "█▓█" ;
}
else if(pixelAt(i) < getMax() * 0.15) {
cout << "▓█▓" ;
}
else if(pixelAt(i) < getMax() * 0.2) {
cout << "▓▓▓" ;
}
else if(pixelAt(i) < getMax() * 0.25) {
cout << "▓▒▓" ;
}
else if(pixelAt(i) < getMax() * 0.3) {
cout << "▒▓▒" ;
}
else if(pixelAt(i) < getMax() * 0.35) {
cout << "▒▒▒" ;
}
else if(pixelAt(i) < getMax() * 0.4) {
cout << "▒░▒" ;
}
else if(pixelAt(i) < getMax() * 0.45) {
cout << "░▒░" ;
}
else if(pixelAt(i) < getMax() * 0.5) {
cout << "░░░" ;
}
else if(pixelAt(i) < getMax() * 0.55) {
cout << "░|░" ;
}
else if(pixelAt(i) < getMax() * 0.6) {
cout << "|░|" ;
}
else if(pixelAt(i) < getMax() * 0.65) {
cout << "|||" ;
}
else if(pixelAt(i) < getMax() * 0.7) {
cout << "|.|" ;
}
else if(pixelAt(i) < getMax() * 0.75) {
cout << ".|." ;
}
else if(pixelAt(i) < getMax() * 0.775) {
cout << "..." ;
}
else if(pixelAt(i) < getMax() * 0.85) {
cout << ". ." ;
}
else if(pixelAt(i) < getMax() * 0.875) {
cout << " . " ;
}
else{
cout << "   " ;
}

if((i+1) % getWidth() == 0 && i > 0) {
cout << endl;
}
}
cout << endl;
}