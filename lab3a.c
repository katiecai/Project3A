#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "ext2_fs.h"

int ext2fd; //file descriptor for disk image

int main(int argc, char* argv[])
{
  if (argc != 2)
    {
      fprintf(stderr, "Wrong number of parameters!");
      exit(1);
    }

  ext2fd = open (argv[1], O_RDONLY);
  if (ext2fd < 0)
    {
      fprintf(stderr, "Error opening disk image!");
      exit(1);
    }
  return 0;
}
