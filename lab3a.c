#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "ext2_fs.h"

#define BUFF_SIZE 2048
#define superblock_offset 1024

int ext2fd; //file descriptor for disk image
char buffer[BUFF_SIZE];

void superblock_summary(void)
{
  size_t toRead = sizeof(ext2_super_block);
  toRead = pread(ext2fd, buffer, toRead, superblock_offset);
  if (toRead < 0)
    {
      //systemCallError
    }
  ext2_super_block* superblock_ptr;
  superblock_ptr = buffer;
  printf("block count: %d", superblock_ptr->s_blocks_count);
}

void group_summary(void) 
{
  exit(1);
}

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
