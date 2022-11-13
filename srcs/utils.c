#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#define throw(x) { ft_fputstr(x, 2); exit(1); }

void put_hex_n(unsigned long num, size_t n) {
    char *hex = "0123456789abcdef";
    for (int i = n -1; i >= 0; i--) {
        write(1, hex + ((num >> (i * 4)) & 0xf), 1);
    }
}

int ft_strlen(const char *s) {
    int i = 0;
    while (s && s[i])
        i++;
    return i;
}

void ft_fputstr(const char *s, int fd) {
    write(fd, s, ft_strlen(s));
}

int ft_strcmp(const char *s, const char *t) {
    int i = 0;
    while (s[i] && s[i] == t[i])
        i++;
    return s[i] - t[i];
}

int ft_strncmp(const char *s, const char *t, int n) {
    int i = 0;
    while (s[i] && s[i] == t[i] && i < n)
        i++;
    return (i < n ? s[i] - t[i] : 0);
}

void *ft_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) throw("Error: malloc() failed\n");
    return ptr;
}

char *ft_join(char *s, char *t) {
    char *ret = malloc(ft_strlen(s) + ft_strlen(t) + 1);
    if (!ret) {
        throw("malloc error\n");
    }

    int i = 0;
    for (int j = 0; s && s[j]; j++, i++) 
        ret[i] = s[j];
    for (int j = 0; t && t[j]; j++, i++) 
        ret[i] = t[j];
    ret[i] = 0;
    return ret;
}

char *cat_f(char *s, ...) {
    va_list l;
    va_start(l, s);
    char *tmp = NULL;
    do {
        s = ft_join(s, tmp);
    } while ((tmp = va_arg(l, char *)));
    va_end(l);
    return s;
}
