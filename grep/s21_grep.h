#ifndef S21_GREP_H
#define S21_GREP_H

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>  //  include
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flag {
  int f_e;
  int f_i;
  int f_v;
  int f_c;
  int f_l;
  int f_n;
  int f_h;
  int f_s;
  int f_o;
  int f_prefix;
  int f_f;
};
void argscollector(int argc, char **argv, const char *short_options,
                   struct flag *str_flags);
void grep(char *argv, struct flag *str_flags, char *pat);
void readf(char *pat, char *optarg);

#endif  //  S21_GREP_H