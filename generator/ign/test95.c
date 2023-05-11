#include <sysy/sylib.h>
const int width = 8;
const int height = 4;
int image_in[width * height];
int image_out[width * height];

int main()
{
  int i = 0;
  int j = 0;
  int num = getarray(image_in);

  while (j < width)
  {
    // image_out[j] = image_in[j];
    image_out[(height - 1) * width + j] = image_in[(height - 1) * width + j];
    j = j + 1;
  }
  // putarray(width * height, image_in);
  stoptime();

  return 1;
}