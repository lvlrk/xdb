#ifndef _XDB_H
#define _XDB_H

typedef struct {
  char magic[4]; // "XDBA"
  int tag_count;
  int file_count;
  int zero;
} xdb_header;

typedef struct {
  int off;
  int size;
  int tag_index;
  char name[244];
} xdb_table;

typedef struct {
  char name[256];
} xdb_tag;

typedef struct {
  xdb_header header;
  xdb_tag *tags;
  xdb_table *file_tables;
  char **data;
} xdb_file;

xdb_file *xdb_init();
int xdb_append(xdb_file *xp, char *filename);
int xdb_write(xdb_file *xp, char *filename);
xdb_file *xdb_open(char *filename);
void xdb_free(xdb_file *xp);

#endif
