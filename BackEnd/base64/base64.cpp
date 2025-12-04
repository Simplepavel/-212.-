#include <iostream>
#include <memory>
#include <string>

// ######## кодирование ########

char base64Char(unsigned char b)
{
  b &= 0x3F;
  if (b < 26)
    return 'A' + b;
  if (b < 52)
    return 'a' + (b - 26);
  if (b < 62)
    return '0' + (b - 52);
  if (b == 62)
    return '+';
  return '/';
}

unsigned int base64Encode(const std::string &str, char outStr[])
{
  unsigned int inIndex = 0, outIndex = 0;
  unsigned int len = str.length();
  unsigned char a1, a2, a3;
  unsigned char b1, b2, b3, b4;

  for (inIndex = 0; inIndex < len; inIndex += 3)
  {
    a1 = a2 = a3 = 0;
    a1 = str[inIndex];

    if (inIndex + 1 < len)
    {
      a2 = str[inIndex + 1];
    }
    if (inIndex + 2 < len)
    {
      a3 = str[inIndex + 2];
    }

    b1 = a1 >> 2;
    b2 = ((a1 & 3) << 4) | (a2 >> 4);
    b3 = ((a2 & 0xF) << 2) | (a3 >> 6);
    b4 = a3 & 0x3F;

    outStr[outIndex++] = base64Char(b1);
    outStr[outIndex++] = base64Char(b2);
    outStr[outIndex++] = (inIndex + 1 < len) ? base64Char(b3) : '=';
    outStr[outIndex++] = (inIndex + 2 < len) ? base64Char(b4) : '=';
  }

  return outIndex;
}

unsigned char base64Code(char ch)
{
  if (ch >= '0' && ch <= '9')
    return (ch - '0') + 52;
  if (ch >= 'A' && ch <= 'Z')
    return ch - 'A';
  if (ch >= 'a' && ch <= 'z')
    return (ch - 'a') + 26;
  if (ch == '+')
    return 62;
  if (ch == '/')
    return 63;
  return 64;
}

bool isBase64(char ch)
{
  if (ch >= 'A' && ch <= 'Z')
    return true;
  if (ch >= 'a' && ch <= 'z')
    return true;
  if (ch >= '0' && ch <= '9')
    return true;
  if (ch == '+')
    return true;
  if (ch == '/')
    return true;
  if (ch == '=')
    return true;
  return false;
}

int base64Decode(char inStr[], unsigned int Len, char outStr[])
{
  if (Len % 4 > 0)
    throw "Wrong data for decoding";

  unsigned int inIndex = 0, outIndex = 0;
  unsigned char b1, b2, b3, b4;
  int s = 0;

  for (inIndex = 0; inIndex < Len; inIndex += 4)
  {
    b1 = base64Code(inStr[inIndex]);
    b2 = base64Code(inStr[inIndex + 1]);
    b3 = base64Code(inStr[inIndex + 2]);
    b4 = base64Code(inStr[inIndex + 3]);

    if (b3 == 64)
    {
      b3 = 0;
      b4 = 0;
      s = 2;
    }
    if (b4 == 64)
    {
      b4 = 0;
      s = 1;
    }

    outStr[outIndex++] = (b1 << 2) | (b2 >> 4);
    outStr[outIndex++] = (b2 << 4) | (b3 >> 2);
    outStr[outIndex++] = (b3 << 6) | b4;

    if (s > 0)
      break;
  }

  return outIndex - s;
}

std::string decode(const char str[])
{
  char inStr[4];
  char outStr[3];
  int cnt = 0;
  std::string res;

  for (size_t i = 0; str[i] != '\0'; ++i)
  {
    char ch = str[i];
    if (isBase64(ch))
    {
      inStr[cnt++] = ch;
      if (cnt == 4)
      {
        int decoded = base64Decode(inStr, 4, outStr);
        res.append(outStr, outStr + decoded);
        cnt = 0;
      }
    }
  }
  return res;
}
