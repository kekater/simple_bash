#ifndef S21_CAT_H
#define S21_CAT_H
#include <getopt.h>
#include <stdio.h>

struct flag {
  int f_b;
  int f_n;
  int f_s;
  int f_E;
  int f_T;
  int f_v;
};
void argscollector(int argc, char **argv, const char *short_options,
                   const struct option *long_options, struct flag *str_flags);
void cat(char argv[], struct flag *str_flags);

#endif  // _S21_CAT_H