#ifndef MEMMANAGE_H
#define MEMMANAGE_H

#define DYNAMIC_MEMORY 0

typedef struct MMspaceblock MMspaceblock;
typedef struct MMspacetable MMspacetable;


MMspacetable* mem_man_new(unsigned long numberofblocks);
void *mem_man_alloc(MMspacetable *st, char *file, unsigned long line, void *ptr,
    unsigned long size, unsigned long number);
void mem_man_delete_ptr(MMspacetable *st, char *file, unsigned long line,
    void *ptr);
void mem_man_info(const MMspacetable *st);
void mem_man_check(const MMspacetable *st);
void mem_man_delete(MMspacetable *st);

#endif
