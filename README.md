# Malloc

Malloc project network42

# Goal

Implement malloc, free and realloc functions using mmap and munmap

# How did I do it?

One of the requirements for this project is to not call mmap and munmap for every malloc and every free respectively.
So we need a way to allocate larger blocks in which we return smaller segments when malloc is called.
Another requirement was that we had to separate allocated memory in blocks of TINY SMALL and LARGE blocks where a tiny malloc is less than n small is less than m but more or equal to n and large are more than m.
Large blocks always represent only one malloc chunk.
The blocks size must also always be a multiple of getpagesize.

Every block starts with a block header (which is also a double linked list) containing information such as:
- prev: Pointer to the previous block
- block_type: One of TINY, SMALL or LARGE
- checksum: A checksum of the structures content (used for parity checking)
- wilderness: Pointer to the closest untouched memory part
- last: Pointer to the last allocated memory chunk
- first_free: Pointer to the first free memory chunk
- num_alloc: Current number of allocated memory chunks
- size: Total size in byte of the block
- available: Total available bytes in the block (they do not have to follow each other)
- next: Pointer to the next block

And before every malloc stands a malloc header (also double linked list) containing these:
- prev: Pointer to the previous malloc header in the block (NULL if first)
- in_use: Integer defining whether the chunk is currently being used
- checksum: A checksum of the structures content (used for parity checking)
- next: Pointer to the next malloc header in the block (never NULL)
- next_free: Pointer to the next free malloc header in the block (NULL by default, unused if in_use is 1)

When malloc is called the type of the malloc (tiny, small or large) is determined and the currently allocated blocks (contained in global variable g_blks) are looked up to see if any can hold the necessary data.
When a block has been found the program will try to lookup free chunks for possible candidates, if a freed chunk is large enough to store the malloc that chunk will be removed from the free chunk list and the program will try to segment the data between the chunk and the next chunk such that if for example, the free chunk is 96 bytes long and the new chunk is 16 bytes long, the total bytes necessary for that new chunk is 16 + the size of the malloc header, if we subtract that number from 96 there should be enough data for a new free block (the block will never be smaller than 1 + chunk header for TINY and n + chunk header for SMALL) and will try to create it.
If no free chunk is available the program will try to allocate it at the end of the block (in the wilderness) pushing it away from the start of the block.
If no available block is able to store the data, the program will try to allocate a new block and go through the same process as above.

When free is called a parity check is being performed on the header of the given pointer aswell as a double free check and checking if the pointer is actually part of any of our blocks.
The number of allocated chunks is reduced and if that number reaches 0 the block will be given back to the system (munmap).
The previous and next chunks will be checked, if they are tagged as non used the block will merge into one bigger block, that bigger block can be larger than the largest chunk the block can allocate because of the segmentation done in malloc.
If the then current chunk (if the previous chunk was free the previous chunk becomes the current chunk to free) is the last allocated chunk in the block, the wilderness is assigned to it and the last allocated chunk to the previous chunk of the current chunk.
Otherwise the chunk is added properly into the free_list (that is at the start if the free_list is empty or by ascending order into the list)

Realloc is quite special, it will, like free, do the same parity check.
If the new size is smaller we will just reduce the size of the chunk and segment the rest just like we would do in malloc. This case is special because it will allow for a chunk to be smaller than the blocks minimum allocated size which is not possible otherwise, this is to reduce the call to malloc in realloc.
If the type of the new size is bigger than the type of the older size, the program will just allocate that new block and copy the old chunks data into the new one.
Otherwise the program will try to keep the same pointer for the new chunk by first:
- Verifying if we are the last chunk and there is enough space in the block for the new chunk, in that case the allocated chunk is just expaned
- Verifying if the next chunk is not being used and if there is enough space by merging those two chunks for the new chunk 
it will merge those two blocks into one and perform the same segmentation algorithm mentioned before
- Otherwise a new chunk is being allocated and the old one gets copied into it

You might have noticed by now that we do have memory loss with that system, indeed we lose (for a 64 bit system) 32 bytes of data per malloc chunk, this is a lot if all of the requested memory are smaller. However we do need a way to keep track of the allocated chunks. Macos (we use macos at our school so I'm used to it now) doesn't store a header for every chunk however due to padding there is still a loss (for example a 1 byte malloc will result in a 16 byte region being allocated).

So like most algorithms it's a game of compromises and I feel like the method I'm using is good enough for understanding how such a project would work.
