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

#ifdef DEBUG
# define dbg(...) {dprintf(2, __VA_ARGS__);}
#else
# define dbg(...)
#endif
#define throw(x) { ft_fputstr(x, 2); exit(1); }
#define put(x) { ft_fputstr(x, 1); }
#define putn(x, n) { write(1, x, n); }
#define put_rep(c, n) { for (int i = 0; i < n; i++) { put(c); } }
#define err(x) {ft_fputstr(x, 2);}
#define ret_err(x, ret) {ft_fputstr(x, 2); return ret;}


#define cat(...) (cat_f(__VA_ARGS__, NULL))

// flags
#define FLAG_G (1 << 1)
#define FLAG_U (1 << 2)
#define FLAG_R (1 << 3)
#define FLAG_P (1 << 4)
#define FLAG_UNIQUE (1 << 5) // for 1 file


typedef struct s_symbol {
    char *name;
    char type;
    size_t value;
} t_symbol;

void ft_nm_64(void *raw_file, char *file_name, size_t size, u_int32_t flags);
void ft_nm_32(void *raw_file, char *file_name, size_t size, u_int32_t flags);
void show_symbols(t_symbol *symbols, int cnt, int flags, int bits);
void sort_symbols(t_symbol *symbols, int cnt, int flags);
char get_sym_type_32(struct elf32_shdr *shdr, struct elf32_sym sym);
char get_sym_type_64(struct elf64_shdr *shdr, struct elf64_sym sym);

// utils
void ft_fputstr(const char *s, int fd);
int ft_strlen(const char *s);
int ft_strncmp(const char *s, const char *t, int n);
int ft_strcmp(const char *s, const char *t);
void *ft_malloc(size_t size);
void put_hex_n(unsigned long num, size_t n);
char *cat_f(char *s, ...);
char *ft_join(char *s, char *t);