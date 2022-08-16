#include <string>
#pragma once
class Image
{
 

public:
  Image();
  Image(const std::string& filename);
  ~Image();

  inline unsigned int GetGLHandle() const;
  inline int GetWidth() const;
  inline int GetHeight() const;

private:
  unsigned int handle;
  int width;
  int height;

  
};

unsigned int Image::GetGLHandle() const
{
  printf("Handle: %d\n", handle);
  return handle;
}

int Image::GetWidth() const
{
  return width;
}

int Image::GetHeight() const
{
  return height;
}
