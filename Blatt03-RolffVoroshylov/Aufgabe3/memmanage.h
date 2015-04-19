#ifndef MEMMANAGE_H
#define MEMMANAGE_H

typedef struct MMspaceblock {
  void *ptr;
  char *file;
  unsigned long line;
  unsigned long size;
  unsigned long number;
} MMspaceblock;

typedef struct MMspacetable {
  MMspaceblock * blockspace;
  unsigned long next;
  unsigned long number;
} MMspacetable;

MMspacetable* mem_man_new(unsigned long numberofblocks);
void *mem_man_alloc(MMspacetable *st, char *file, unsigned long line, void *ptr,
    unsigned long size, unsigned long number);
void mem_man_delete_ptr(MMspacetable *st, char *file, unsigned long line,
    void *ptr);
void mem_man_info(const MMspacetable *st);
void mem_man_check(const MMspacetable *st);
void mem_man_delete(MMspacetable *st);

#endif
