#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xdb.h"
#include "debug.h"

xdb_file *xdb_init() {
  xdb_file *xp = NULL;
  xp = malloc(sizeof(xdb_file));
  if(!xp) {
    debug("cannot alloc xp");
    return NULL;
  }

  memcpy(xp->header.magic, "XDBA", 4);
  xp->header.tag_count = 0;
  xp->header.file_count = 0;
  xp->header.zero = 0;

  xp->tags = NULL;
  xp->tags = malloc(sizeof(xdb_tag));
  if(!xp->tags) {
    debug("cannot alloc tags");
    goto error;
  }

  xp->file_tables = NULL;
  xp->file_tables = malloc(sizeof(xdb_table));
  if(!xp->file_tables) {
    debug("cannot alloc tables");
    goto error;
  }

  xp->data = NULL;
  xp->data = malloc(sizeof(char*));
  if(!xp->data) {
    debug("cannot alloc data");
    goto error;
  }

  debug("no error");

  return xp;
error:
  if(xp) xdb_free(xp);

  debug("error");

  return NULL;
}

int xdb_append(xdb_file *xp, char *filename) {
  if(!xp || !filename) {
    debug("invalid xp or filename");
    return 1;
  }

  xp->header.file_count++;

  FILE *fp = NULL;
  char *buf = NULL;
  fp = fopen(filename, "rb");
  if(!fp) {
    debug("cant open file");
    goto error;
  }

  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  if(size < 1) {
    debug("size small");
    goto error;
  }
  fseek(fp, 0, SEEK_SET);

  buf = malloc(size);
  if(!buf) {
    debug("bad buffer");
    goto error;
  }
  // will return to tomorrow on 7/16

  fread(buf, 1, size, fp);

  fclose(fp);

  xp->file_tables = realloc(xp->file_tables, xp->header.file_count * sizeof(xdb_table));
  if(!xp->file_tables) goto error;

  xdb_table *table = &xp->file_tables[xp->header.file_count - 1];

  int tmp = sizeof(xdb_header);
  tmp += sizeof(xdb_table) * xp->header.file_count;
  for(int i = 0; i < xp->header.file_count - 1; i++) tmp += xp->file_tables[i].size;

  table->off = 0; // will be set in xdb_write()
  table->size = size;
  table->tag_index = 0;
  strncpy(table->name, filename, 244);

  xp->data = realloc(xp->data, sizeof(char*) * xp->header.file_count);
  if(!xp->data) goto error;

  char **data = &xp->data[xp->header.file_count - 1];
  *data = malloc(size);
  if(!(*data)) goto error;

  memcpy(*data, buf, size);

  return 0;
error:
  if(fp) fclose(fp);
  if(buf) free(buf);
  if(xp) xdb_free(xp);

  return 1;
}

int xdb_write(xdb_file *xp, char *filename) {
  if(!xp || !filename) return 1;
  FILE *fp = NULL;
  fp = fopen(filename, "wb");
  if(!fp) goto error;

  fwrite(&xp->header, 1, sizeof(xdb_header), fp);

  int tmp = sizeof(xdb_header);
  tmp += sizeof(xdb_tag) * xp->header.tag_count;
  tmp += sizeof(xdb_table) * xp->header.file_count;

  for(int i = 0; i < xp->header.file_count; i++) {
    xp->file_tables[i].off = tmp;

    for(int j = 0; j < xp->header.file_count - 1; j++) tmp += xp->file_tables[j].size;
  }

  for(int i = 0; i < xp->header.file_count; i++) {
    fwrite(&xp->file_tables[i], 1, sizeof(xdb_table), fp);
  }

  for(int i = 0; i < xp->header.file_count; i++) {
    fwrite(xp->data[i], 1, xp->file_tables[i].size, fp);
  }

  fclose(fp);

  return 0;
error:
  if(fp) fclose(fp);

  return 1;
}

void xdb_free(xdb_file *xp) {
  if(xp) {
    if(xp->tags) free(xp->tags);
    if(xp->file_tables) free(xp->file_tables);
    if(xp->data) {
      for(int i = 0; i < xp->header.file_count; i++) {
        if(xp->data[i]) free(xp->data[i]);
      }

      free(xp->data);
    }

    free(xp);
  }
}

xdb_file *xdb_open(char *filename) {
  xdb_file *xp = NULL;
  xp = xdb_init();
  if(!xp) return NULL;

  FILE *fp = NULL;
  fp = fopen(filename, "rb");
  if(!fp) goto error;

  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  if(size < sizeof(xdb_header)) goto error;

  char magic[4];

  fread(magic, 1, 4, fp);

  // debug checkpoints to detect segfaults / errors cause im a dumbass
  debug("before magic check");
  if(memcmp(magic, "XDBA", 4)) goto error;
  debug("after magic check");

  fseek(fp, 0, SEEK_SET);

  fread(&xp->header, 1, sizeof(xdb_header), fp);
  debug("after header read");

  xp->tags = realloc(xp->tags, sizeof(xdb_tag) * xp->header.tag_count);
  if(!xp->tags && xp->header.tag_count > 0) goto error;
  debug("after tags alloc");

  for(int i = 0; i < xp->header.tag_count; i++) {
    fread(&xp->tags[i], 1, sizeof(xdb_tag), fp);
  }

  debug("after tags read");

  xp->file_tables = realloc(xp->file_tables, sizeof(xdb_table) * xp->header.file_count);
  if(!xp->file_tables) goto error;

  debug("after file table alloc");

  for(int i = 0; i < xp->header.file_count; i++) {
    fread(&xp->file_tables[i], 1, sizeof(xdb_table), fp);
  }

  debug("after tables read");

  xp->data = realloc(xp->data, sizeof(char*) * xp->header.file_count);
  if(!xp->data) goto error;

  debug("after master data alloc");

  for(int i = 0; i < xp->header.file_count; i++) {
    xp->data[i] = malloc(xp->file_tables[i].size);
    if(!xp->data[i]) goto error;
    debug("after data[i] data alloc");

    fread(xp->data[i], 1, xp->file_tables[i].size, fp);
    debug("after data[i] read");
  }

  debug("end");

  fclose(fp);

  return xp;
error:
  if(fp) fclose(fp);
  if(xp) xdb_free(xp);

  return NULL;
}
