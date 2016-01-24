/* 
   SGIimage.h

   David Bucciarelli (tech.hmw@plus.it)
   Humanware s.r.l.
   Via XXIV Maggio 62
   Pisa, Italy
   Tel./Fax +39-50-554108
   email: info.hmw@plus.it
   www: www-hmw.caribel.pisa.it
*/

#ifndef __IMAGE_H__
#define __IMAGE_H__

typedef struct
{
    unsigned short imagic;
    unsigned short type;
    unsigned short dim;
    unsigned short sizeX, sizeY, sizeZ;
    char name[128];
	unsigned char *data;
} IMAGE;

IMAGE *ImageLoad(char *);

#endif /* !__IMAGE_H__! */
