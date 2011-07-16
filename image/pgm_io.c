#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>


int parse_pgm(FILE* f, size_t* w, size_t* h, size_t* bpp)
{
  char *line = NULL;
  char buf[1024];
  // read and check for "P5" magic number
  if (!fgets(buf, 10, f))
  {
    return -1;
  }
  if (strncmp(buf, "P5", 2))
  {
    return -1;
  }

  // read next non comment line
  do
  {
    line = fgets(buf, 1024, f);
  }while(line && line[0] == '#');
  if (!line)
  {
    return -1;
  }

  // read width height
  sscanf(line,"%u %u", w, h);

  // read next non comment line
  do
  {
    line = fgets(buf, 1024, f);
  }while(line && line[0] == '#');
  if (!line)
  {
    return -1;
  }

  int maxval;
  sscanf(line,"%d", &maxval);
  *bpp = log2(maxval+1);
  return 0;
}

int size_pgm(char* path, size_t* w, size_t* h, size_t* bpp)
{
  FILE* f = fopen(path, "rb");
  if (f == NULL)
  {
    perror("open");
    return -1;
  }
  int ret = parse_pgm(f, w, h, bpp);
  fclose(f);
  return ret;
}

int load_pgm_uint8(char* path, uint8_t* image, size_t w, size_t stride, size_t h, size_t bpp)
{
  return -1;
}

int load_pgm_uint16(char* path, uint16_t* image, size_t w, size_t stride, size_t h, size_t bpp)
{
  FILE* f = fopen(path, "rb");
  if (f == NULL)
  {
    perror("open");
    return -1;
  }
  size_t _w;
  size_t _h;
  size_t _bpp;
  int ret = parse_pgm(f, &_w, &_h, &_bpp);
  if (ret)
  {
    return ret;
  }
  if (bpp != _bpp || w != _w || h != _h)
  {
    fprintf(stderr, "mismatch expected %dx%dx%d but file is %dx%dx%d\n",
            w,h,bpp, _w,_h,_bpp);
    fclose(f);
    return -1;
  }
  if (stride < w)
  {
    fprintf(stderr, "Invalid image stride\n");
  }
  size_t i;
  for (i = 0; i < h; ++i)
  {
    size_t n = w*bpp/8;
    if (fread(image, n, 1, f) != 1)
    {
      fprintf(stderr, "failed to read from file\n");
      fclose(f);
      return -1;
    }
    image += stride;
  }
  fclose(f);
  return 0;
}

int save_pgm_uint8(char* path, const uint8_t* image, size_t w, size_t stride, size_t h)
{
  return -1;
}

int save_pgm_uint16(char* path, const uint8_t* image, size_t w, size_t stride, size_t h)
{
  return -1;
}