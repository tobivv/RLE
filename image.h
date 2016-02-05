#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED
#include <string>


class Imagecon
{
   public:
       Imagecon();
       std::string nazwa;
       void entropia();
       void RLE();
       void reverseRLE();

};



#endif // IMAGE_H_INCLUDED
