//Coordinate.h
//Author: Nikolas Szejna
#ifndef NLS_COORDINATE
#define NLS_COORDINATE

class Coordinate
{
public:
    Coordinate(){x =0; y = 0;}
    //constructor with initializer list, default is (0,0)
    //but can set with any values
    Coordinate(int i, int j){x=i;y=j;} 
    int x;
    int y;
    
};

#endif //NLS_COORDINATE
