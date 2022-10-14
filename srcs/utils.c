#include "../inc/utils.h"

int ft_strlen(const char *s) {
    int i = 0;
    while (s && s[i])
        i++;
    return i;
}

void ft_putstr(const char *s) {
    write(1, s, ft_strlen(s));
}