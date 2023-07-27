#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    const char *short_options = "e:ivclnhsof:";
    struct flag str_flags = {0};  // значения флагов 0
    argscollector(argc, argv, short_options, &str_flags);
  }
  return 0;
}

void argscollector(int argc, char **argv, const char *short_options,
                   struct flag *str_flags) {
  int rez = 0;  // заводим переменную для гетопт
  int option_index = 0;
  const struct option long_options[] = {{NULL, 0, NULL, 0}};
  char pat[10000] = {0};  // массив для шаблона
  while (rez != -1) {     // пока есть символы опций
    rez = getopt_long(argc, argv, short_options, long_options, &option_index);
    switch (rez) {
      case 'e':
        strcat(pat, optarg);  // объединяет шаблоны
        strcat(pat, "|");     // добавляет "|"
        str_flags->f_e = 1;
        break;
      case 'i':
        str_flags->f_i = 1;
        break;
      case 'v':
        str_flags->f_v = 1;
        break;
      case 'c':
        if (str_flags->f_l == 0) str_flags->f_c = 1;
        break;
      case 'l':
        str_flags->f_l = 1;
        str_flags->f_c = 0;
        break;
      case 'n':
        str_flags->f_n = 1;
        break;
      case 'h':
        str_flags->f_h = 1;
        break;
      case 's':
        str_flags->f_s = 1;
        break;
      case 'o':
        str_flags->f_o = 1;
        break;
      case 'f':
        readf(pat, optarg);
        str_flags->f_f = 1;
        break;
    }
  }
  if (str_flags->f_e == 0 &&
      str_flags->f_f == 0) {    // если флаг е f отсутствует
    strcat(pat, argv[optind]);  // соединяет шаблоны
    optind++;
  } else {
    pat[strlen(pat) - 1] = '\0';  // вычитаем 1, чтобы убрать знак "или" (|)
  }
  if ((argc - optind) > 1) {
    str_flags->f_prefix = 1;
  }
  for (; optind < argc; optind++) {  // перебираем файлы
    // вызывает функцию и передаем файлы, флаги, шаблоны
    grep(argv[optind], str_flags, pat);
  }
}

void readf(char *pat, char *optarg) {
  FILE *f = NULL;
  if ((f = fopen(optarg, "r")) != NULL) {
    char *line = NULL;  // сюда сохраним строку
    size_t len;
    int status;
    while ((status = getline(&line, &len, f)) != -1) {
      strcat(pat, line);  // объединяет шаблоны
      strcat(pat, "|");
    }
    if (line) {
      free(line);
    }
    fclose(f);
  } else {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", optarg);
  }
}

void grep(char *argv, struct flag *str_flags, char *pat) {
  FILE *f = NULL;
  char *line = NULL;  // сюда сохраним строку
  size_t len = 0;
  int status = 0;
  int counter = 0;
  int counter_line = 0;
  int statusreg = 0;
  if ((f = fopen(argv, "r")) != NULL) {  // проверяем на наличие файла
    regex_t reg;  // переменная для скомпилированного выражения
    if (!regcomp(&reg, pat,  // для компиляции выражения
                 str_flags->f_i ? REG_EXTENDED | REG_ICASE | REG_NEWLINE
                                : REG_EXTENDED | REG_NEWLINE)) {
      regmatch_t pmatch[1] = {0};
      while ((status = getline(&line, &len, f)) != -1) {  // получаем строку
        counter_line++;
        statusreg = regexec(&reg, line, 1, pmatch, 0);  //
        //  Сравниваем строку с скомпилированным регулярным выражением
        if ((!statusreg && !str_flags->f_v) ||
            (statusreg == REG_NOMATCH && str_flags->f_v)) {
          counter++;
          if (str_flags->f_c) continue;

          if (str_flags->f_l) {
            printf("%s\n", argv);
            break;
          }

          if (str_flags->f_o) {
            char *sub_line = line;
            while (!statusreg) {
              if (str_flags->f_prefix == 1 && str_flags->f_h == 0) {
                printf("%s:", argv);
              }
              if (str_flags->f_n) {
                printf("%d:", counter_line);
              }
              for (int i = pmatch->rm_so; i < pmatch->rm_eo; i++)
                printf("%c", sub_line[i]);
              printf("\n");
              sub_line = sub_line + pmatch->rm_eo;
              statusreg = regexec(&reg, sub_line, 1, pmatch, 0);
            }
          }
          if (str_flags->f_o == 0) {
            if (str_flags->f_prefix == 1 && str_flags->f_h == 0) {
              printf("%s:", argv);
            }
            if (str_flags->f_n) {
              printf("%d:", counter_line);
            }
            if (str_flags->f_c == 0 || str_flags->f_h == 1) {
              printf("%s", line);
            }
          }
          if (str_flags->f_c == 0 && str_flags->f_o == 0 &&
              (line[strlen(line) - 1] != '\n')) {
            printf("\n");
          }
        }
      }

      if (str_flags->f_c) {
        if (str_flags->f_prefix && !str_flags->f_h) {
          printf("%s:", argv);
        }
        printf("%d\n", counter);
      }
      // if((line[strlen(line) - 1]) != '\n') printf("\n");
      if (line) {
        free(line);
      }
    }
    fclose(f);
    regfree(&reg);
  } else {
    if (!str_flags->f_s) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv);
    }
  }
}
