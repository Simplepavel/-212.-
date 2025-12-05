#include <iostream>
#include <string>
char base64Char(unsigned char b);
unsigned int base64Encode(char inStr[], int len, char outStr[]);
unsigned char base64Code(char ch);
bool isBase64(char ch);
int base64Decode(const char inStr[], unsigned int Len, char outStr[]);
