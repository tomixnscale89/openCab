#include <string>
#pragma once
class Image
{
public:
  Image(const std::string& filename);
  Image();
  ~Image();
  bool Load(const std::string& filename);

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
