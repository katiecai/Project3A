#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "ext2_fs.h"

#define BUFF_SIZE 2048
#define superblock_offset 1024

int ext2fd; //file descriptor for disk image
char buffer[BUFF_SIZE];
int block_size;
int inode_count;
int block_count;
int blocks_bitmap;
int inodes_bitmap;

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
  block_count = superblock_ptr->s_blocks_count;
  //inode count
  printf("%d,", superblock_ptr->s_inodes_count);
  inode_count = superblock_ptr->s_inodes_count;
  //block size
  printf("%d,", 1024 << superblock_ptr->s_log_block_size);
  block_size = 1024 << (superblock_ptr->s_log_block_size);
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
  int toRead = sizeof(struct ext2_group_desc);
  toRead = pread(ext2fd, buffer, toRead, superblock_offset+sizeof(struct ext2_super_block));
  if (toRead < 0)
    {
      fprintf(stderr, "hello");
      // syscall error
    }
  struct ext2_group_desc* group_pointer = (struct ext2_group_desc*)buffer;
  printf("GROUP,");
  // group number
  printf("%d,", 0);
  // total number of blocks in the group
  printf("%d,", block_count);
  // total number of inodes in the group
  printf("%d,", inode_count);
  // total number of free blocks
  printf("%d,", group_pointer->bg_free_blocks_count);
  // total number of free inodes
  printf("%d,", group_pointer->bg_free_inodes_count);
  // block number of free block bitmap
  printf("%d,", group_pointer->bg_block_bitmap);
  blocks_bitmap = group_pointer->bg_block_bitmap;
  // block number of the free inode bitmap
  printf("%d,", group_pointer->bg_inode_bitmap);
  inodes_bitmap = group_pointer->bg_inode_bitmap;
  // block number of the inode table
  printf("%d\n", group_pointer->bg_inode_table);
}

void free_inodes(void)
{
  uint8_t *bitmap = malloc(block_size);
  int toRead = pread(ext2fd, bitmap, block_size, block_size * (inodes_bitmap)); 
  if (toRead < 0)
    {
      fprintf(stderr, "Error!");
      // system call error function
    }
  uint32_t bitmap_size = inode_count;
  uint32_t bit_num;
  int inode_ctr = 0;
  int free_inodes = 0;


  for (bit_num = 0; bit_num < bitmap_size; bit_num++)
    {
      uint8_t a_byte = bitmap[bit_num / 8];
      if (!(a_byte & (1 << (bit_num % 8))))
	{
	  printf("IFREE, %d\n", inode_ctr);
	  free_inodes++;
	}
      inode_ctr++;
    }  
  free(bitmap);
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
  group_summary();
  free_inodes();
  return 0;
}
