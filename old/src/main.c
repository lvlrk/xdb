#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xdb.h"
#include "debug.h"

#define VER_MAX 0
#define VER_MIN 3

int main(int argc, char **argv) {
  const char *usage = "Usage: xdb [-?vctd]\n"
    "\t[-f] <file>\n"
    "\txdb [--help]\n";
  const char *help = "Usage: xdb [OPTION...] [FILE...]\n"
    "The best FOSS p*rn viewer\n\n"
    "Examples:\n"
    "  xdb -f archive.xdb -c foo bar  # Create archive.xdb from files foo and bar.\n\n"
    "  -?, --help              display this help and exit\n"
    "  -v, --version           output version information and exit\n"
    "  -f, --file              use xdb file\n"
    "  -c, --create            create a new xdb file\n"
    "  -t, --list              list the contents of a xdb file\n"
    "  -d, --debug             print debug messages\n";

  char *file = NULL;
  int file_set = 0;
  file = malloc(512);
  memset(file, 0, 512);

  int file_count = 0;
  char **files = NULL;
  files = malloc(sizeof(char*));

  xdb_file *xp = NULL;

  if(argc < 2) {
    fputs(usage, stderr);

    return 1;
  }

  for(int i = 1; i < argc; i++) {
    if(!strcmp(argv[i], "--debug") || !strcmp(argv[i], "-d")) {
      debug_print = !debug_print;
    } else if(!strcmp(argv[i], "--help") || !strcmp(argv[i], "-?")) {
      fputs(help, stdout);

      return 0;
    } else if(!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")) {
      printf("xdb-%d.%d\n", VER_MAX, VER_MIN);

      return 0;
    } else if(!strcmp(argv[i], "--file") || !strcmp(argv[i], "-f")) {
      if(argv[i + 1]) {
        strncpy(file, argv[i + 1], 512);
        
        file_set = 1;
      } else {
        fputs("xdb error: required argument for -f\n", stderr);
        fputs(usage, stderr);

        return 1;
      }
    } else if(!strcmp(argv[i], "--create") || !strcmp(argv[i], "-c")) {
      if(file_set) {
        if(argv[i + 1]) {
          int j = i + 1; // start index for file search

          FILE *tfp = NULL;
          tfp = fopen(argv[j], "rb");

          while(tfp) {
            file_count++;

            files = realloc(files, sizeof(char*) * file_count);
            files[file_count - 1] = malloc(512);
            memset(files[file_count - 1], 0, 512);

            strncpy(files[file_count - 1], argv[j], 512);

            fclose(tfp);

            j++;

            tfp = fopen(argv[j], "rb");
          }

          if(file_count < 1) goto err_create_file_args;

          xp = xdb_init();
          if(xp) {
            for(int k = 0; k < file_count; k++) xdb_append(xp, files[k]);
            xdb_write(xp, file);
          } else {
            fputs("xdb error: cannot create new xdb file\n", stderr);
            fputs(usage, stderr);

            return 1;
          }
        } else {
err_create_file_args:
          fputs("xdb error: required file arguments for -c\n", stderr);
          fputs(usage, stderr);

          return 1;
        }
      } else {
        fputs("xdb error: file must be set for -c\n", stderr);
        fputs(usage, stderr);

        return 1;
      }
    } else if(!strcmp(argv[i], "--list") || !strcmp(argv[i], "-t")) {
      if(file_set) {
        xp = xdb_open(file);
        if(xp) {
          for(int i = 0; i < xp->header.file_count; i++) puts(xp->file_tables[i].name);
        } else {
          fputs("xdb error: cannot open xdb file for reading\n", stderr);
          fputs(usage, stderr);

          return 1;
        }
      } else {

      }
    }
  }

  xdb_free(xp);

  return 0;
}
