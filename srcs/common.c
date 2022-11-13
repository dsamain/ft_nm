#include "../inc/ft_nm.h"

void sort_symbols(t_symbol *symbols, int cnt, int flags) {
    t_symbol tmp;
    for (int _ = 0; _ < cnt; _++) {
        for (int i = 0; i < cnt - 1; i++) {
            if (((flags & FLAG_R) && ft_strcmp(symbols[i].name, symbols[i + 1].name) < 0) ||
                (!(flags & FLAG_R) && ft_strcmp(symbols[i].name, symbols[i + 1].name) > 0)) {
                tmp = symbols[i];
                symbols[i] = symbols[i + 1];
                symbols[i + 1] = tmp;
            }
        }
    }
}

void show_symbols(t_symbol *symbols, int cnt, int flags, int bits) {
    for (int i = 0; i < cnt; i++) {

        // -u -> print only undefined
        if ((flags & FLAG_U) && !(symbols[i].type == 'u' || symbols[i].type == 'U' || symbols[i].type == 'w'))
            continue;
        
        // -g -> print only external
        if ((flags & FLAG_G) && !(symbols[i].type >= 'A' && symbols[i].type <= 'Z'))
            continue;

        if (symbols[i].type == 'U' || (symbols[i].type == 'w' && symbols[i].value == 0)) {
            put_rep(" ", bits);
        } else {
            put_hex_n(symbols[i].value, bits);
        }
        put(" ");
        putn(&symbols[i].type, 1);
        put(" ");
        put(symbols[i].name);
        put("\n");
    }
}

// https://stackoverflow.com/questions/15225346/how-to-display-the-symbols-type-like-the-nm-command