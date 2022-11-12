#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include  <elf.h>
#include "./elf.h"


#define dbg(...) {dprintf(2, __VA_ARGS__);}
#define throw(x) { ft_fputstr(x, 2); exit(1); }
#define put(x) { ft_fputstr(x, 1); }
#define putn(x, n) { write(1, x, n); }
#define put_rep(c, n) { for (int i = 0; i < n; i++) { put(c); } }
#define ERR(x) {ft_fputstr(x, 2); return ;}
#define R_ERR(x, ret) {ft_fputstr(x, 2); return ret;}


typedef struct s_symbol {
    char *name;
    char type;
    size_t value;
} t_symbol;

void ft_nm_64(void *raw_file, size_t size);
void show_symbols_64(t_symbol *symbols, int cnt);
char get_sym_type(struct elf64_shdr *shdr, struct elf64_sym sym);
void sort_symbols(t_symbol *symbols, int cnt);

// utils
void ft_fputstr(const char *s, int fd);
int ft_strlen(const char *s);
int ft_strncmp(const char *s, const char *t, int n);
int ft_strcmp(const char *s, const char *t);
void *ft_malloc(size_t size);
void put_hex_n(unsigned long num, size_t n);
