#ifndef __MYCOLOURS_H
#define __MYCOLOURS_H

#define BLACK                  0x00 // 0000
#define BLUE                    0x01 // 0001
#define GREEN                 0x02 // 0010
#define CYAN                    0x03 // 0011
#define RED                      0x04 // 0100
#define MAGENTA             0x05 // 0101
#define BROWN                0x06 // 0110
#define LIGHT_GRAY         0x07 // 0111
#define DARK_GRAY          0x08 // 1000
#define LIGHT_BLUE         0x09 // 1001
#define LIGHT_GREEN       0x0A // 1010
#define LIGHT_CYAN          0x0B // 1011
#define LIGHT_RED            0x0C // 1100
#define LIGHT_MAGENTA   0x0D // 1101
#define YELLOW                 0x0E // 1110
#define WHITE                   0x0F // 1111 

inline unsigned char setColour(unsigned char f, unsigned char b)
{
    return (f | (b << 4));
}

#endif  