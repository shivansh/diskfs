#include "fs.h"
#include "disk.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FS_MAGIC 0xf0f03410
#define INODES_PER_BLOCK 128
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK 1024
#define PRINT_LIMIT 5

#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct fs_superblock {
    int magic;
    int nblocks;
    int ninodeblocks;
    int ninodes;
};

struct fs_inode {
    int isvalid;
    int size;
    int direct[POINTERS_PER_INODE];  // pointers in an indirect block
    int indirect;                    // indirect block number
};

union fs_block {
    struct fs_superblock super;
    struct fs_inode inode[INODES_PER_BLOCK];
    int pointers[POINTERS_PER_BLOCK];
    char data[DISK_BLOCK_SIZE];
};

// fs_format creates a new filesystem on the disk, destroying any data already
// present. It writes the superblock, allocates 10 percent of the total blocks
// as inode blocks and marks them all as invalid.
int fs_format() {
    union fs_block block;
    block.super.magic = FS_MAGIC;
    block.super.nblocks = disk_size();
    // Allocate 10 percent of the total number of disk blocks as inode blocks.
    block.super.ninodeblocks = MAX(1, block.super.nblocks / 10);
    block.super.ninodes = 0;
    int blocknum = 0;
    disk_write(blocknum++, block.data);
    for (int i = 0; i < block.super.ninodes; ++i) {
        union fs_block inode_block;
        for (int j = 0; j < INODES_PER_BLOCK; ++j) {
            inode_block.inode[j].isvalid = 0;
        }
        disk_write(blocknum++, inode_block.data);
    }
    return 0;  // TODO: should return -1 when invoked on a mounted filesystem
}

// fs_debug scans a mounted filesystem and reports how the inodes and blocks are
// organised.
void fs_debug() {
    union fs_block block;

    // Read the superblock.
    int blocknum = 0;
    disk_read(blocknum++, block.data);  // adjust block number for the next read

    printf("superblock:\n");
    if (block.super.magic == FS_MAGIC) {
        printf("    magic number is valid\n");
    } else {
        // TODO: handle error ; this should also be handled when mounting.
    }
    printf("    %d blocks\n", block.super.nblocks);
    printf("    %d inode blocks\n", block.super.ninodeblocks);
    printf("    %d inodes\n", block.super.ninodes);

    // Read the inodes within each inode block.
    for (int i = 0; i < block.super.ninodeblocks; ++i) {
        disk_read(blocknum++, block.data);
        for (int j = 0; j < INODES_PER_BLOCK; ++j) {
            if (block.inode[j].isvalid) {
                printf("inode %d:\n", j);
                printf("    size: %d bytes\n", block.inode[j].size);
                printf("    direct blocks:");
                for (int k = 0; k < POINTERS_PER_INODE; ++k) {
                    if (block.inode[j].direct[k] == 0) {
                        break;  // TODO: verify if the empty fields are nil
                    } else {
                        printf(" %d", block.inode[j].direct[k]);
                    }
                }
                printf("\n");
                if (block.inode[j].indirect != 0) {
                    printf("    indirect block: %d\n", block.inode[j].indirect);
                    // Read the indirect block.
                    disk_read(block.inode[j].indirect, block.data);
                    printf("    indirect data blocks:");
                    // We only print the first PRINT_LIMIT indirect data blocks.
                    for (int k = 0; k < POINTERS_PER_BLOCK; ++k) {
                        if (k == PRINT_LIMIT) {
                            printf(" ...");
                            break;
                        } else if (block.pointers[k] == 0) {
                            break;
                        } else {
                            printf(" %d", block.pointers[k]);
                        }
                    }
                    printf("\n");
                }
            }
        }
    }
}

int fs_mount() { return 0; }

int fs_create() { return 0; }

int fs_delete(int inumber) { return 0; }

int fs_getsize(int inumber) { return -1; }

int fs_read(int inumber, char *data, int length, int offset) { return 0; }

int fs_write(int inumber, const char *data, int length, int offset) {
    return 0;
}
