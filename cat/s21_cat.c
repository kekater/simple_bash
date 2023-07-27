#include "s21_cat.h"

int main(int argc, char **argv) {
  const char *short_options = "benstETv";
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};      // long_options
  struct flag str_flags = {0};  //значения флагов 0
  argscollector(argc, argv, short_options, long_options, &str_flags);
  return 0;
}

void argscollector(int argc, char **argv, const char *short_options,
                   const struct option *long_options, struct flag *str_flags) {
  int rez = 0;
  int option_index = 0;
  while (rez != -1) {
    rez = getopt_long(argc, argv, short_options, long_options, &option_index);
    switch (rez) {
      case 'b':
        str_flags->f_b = 1;
        break;
      case 'n':
        str_flags->f_n = 1;
        break;
      case 'E':
        str_flags->f_E = 1;
        break;
      case 'e':
        str_flags->f_E = 1;
        str_flags->f_v = 1;
        break;
      case 's':
        str_flags->f_s = 1;
        break;
      case 't':
        str_flags->f_T = 1;
        str_flags->f_v = 1;
        break;
      case 'T':
        str_flags->f_T = 1;
        break;
      case 'v':
        str_flags->f_v = 1;
        break;
    }  // switch
  }
  for (; optind < argc; optind++) {
    cat(argv[optind], str_flags);
    // printf("%s\n", argv[optind]);
  }  // for
}

void cat(char argv[], struct flag *str_flags) {
  FILE *f = NULL;
  int ch = 0;
  int old_ch = -2;
  int n = 1, r = 0;
  if ((f = fopen(argv, "r")) != NULL) {  //открытие файла
    while ((ch = fgetc(f)) != EOF) {  //пока не конец файла выводить символы
      if (str_flags->f_s == 1) {
        if (((old_ch == -2) || (old_ch == '\n')) && ch == '\n') {
          r++;
        } else {
          r = 0;
        }
      }
      if (r < 2) {
        if (str_flags->f_n == 1 && str_flags->f_b == 0) {
          if ((old_ch == -2) || (old_ch == '\n')) {
            printf("%6d\t", n);
            n++;
          }
        }
        if (str_flags->f_b == 1) {
          if (((old_ch == -2) || (old_ch == '\n')) && ch != '\n') {
            printf("%6d\t", n);
            n++;
          }
        }
        if (str_flags->f_E == 1 && ch == '\n') {
          printf("$");
        }
        if (str_flags->f_T == 1) {
          if (ch == '\t') {
            ch = 'I';
            printf("^");
          } else if (ch == 12) {
            printf("^");
            ch = 'L';  // это
          }
        }
        if (str_flags->f_v == 1 && ch != '\n' && ch != '\t') {
          if ((ch >= 0) && (ch <= 31)) {
            printf("^");
            ch = ch + 64;
          } else if (ch == 127) {
            printf("^");
            ch = '?';  // это
          }
        }
        printf("%c", ch);
      }
      old_ch = ch;
    }
    fclose(f);  //закрытие файла
  } else {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv);
  }  // else
}