#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "ext2_fs.h"

#define BUFF_SIZE 2048
#define superblock_offset 1024

int ext2fd; //file descriptor for disk image
char buffer[BUFF_SIZE];

void superblock_summary(void)
{
  int toRead = sizeof(struct ext2_super_block);
  toRead = pread(ext2fd, buffer, toRead, superblock_offset);
  if (toRead < 0)
    {
      fprintf(stderr, "hello");
      //systemCallError
    }
  struct ext2_super_block* superblock_ptr;
  superblock_ptr = (struct ext2_super_block*) buffer;
  printf("SUPERBLOCK,");
  //block count
  printf("%d,", superblock_ptr->s_blocks_count);
  //inode count
  printf("%d,", superblock_ptr->s_inodes_count);
  //block size
  printf("%d,", 1024 << superblock_ptr->s_log_block_size);
  //inode size
  printf("%d,", superblock_ptr->s_inode_size);
  //blocks per group
  printf("%d,", superblock_ptr->s_blocks_per_group);
  //inodes per group
  printf("%d,", superblock_ptr->s_inodes_per_group);
  //first non-reserved i-node
  printf("%d\n", superblock_ptr->s_first_ino);
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
  superblock_summary();
  return 0;
}
