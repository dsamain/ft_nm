#include "../inc/ft_nm.h"

void ft_nm_64(void *raw_file, size_t size) {
    struct elf64_hdr *header = (struct elf64_hdr *)raw_file;
    struct elf64_shdr *section_header_table = (struct elf64_shdr *)((char *)header + header->e_shoff); 

    size_t shdr_size = header->e_shentsize; // size of a section header
    size_t shdr_num = header->e_shnum; // number of entry
    size_t shdr_strndx = header->e_shstrndx; // index of the section header string table

    struct elf64_shdr shstrtab = section_header_table[shdr_strndx]; // section header string table

    dbg("header section size : %ld, num : %ld, strndx : %ld\n", shdr_size, shdr_num, shdr_strndx);

    struct elf64_shdr *symtab_hdr = NULL;
    struct elf64_shdr *strtab_hdr = NULL;

    // iteratate over all section header to find symtab_header and strtab_header
    for (int i = 0; i < shdr_num; i++) {
        struct elf64_shdr *section_header = section_header_table + i;
        char *name = raw_file + shstrtab.sh_offset + section_header->sh_name;
        if (!ft_strcmp(name, ".symtab")) {
            symtab_hdr = section_header;
        } else if (!ft_strcmp(name, ".strtab")) {
            strtab_hdr = section_header;
        }
    }

    if (!symtab_hdr || !strtab_hdr) {
        throw("Error: no symtab or strtab\n");
    }

    struct elf64_sym *symtab = (struct elf64_sym *)(raw_file + symtab_hdr->sh_offset);
    char *strtab = raw_file + strtab_hdr->sh_offset;
    size_t sym_cnt = symtab_hdr->sh_size / sizeof(struct elf64_sym);

    dbg("symtab_cnt : %ld\n", sym_cnt);
    dbg("Symtab : %p, strtab : %p\n", symtab, strtab);
    
    t_symbol *symbols = ft_malloc(sizeof(t_symbol) * sym_cnt);
    int idx = 0;

    // iterate over all symbol table entry
    for (int i = 0; i < sym_cnt ; i++) {
        struct elf64_sym sym = symtab[i];
        if (sym.st_name == 0) {
            continue;
        }
        symbols[idx].name = strtab + sym.st_name;
        symbols[idx].type = get_sym_type(section_header_table, sym);
        symbols[idx].value = sym.st_value;
        idx++;
    }
    sort_symbols(symbols, idx);
    show_symbols_64(symbols, idx);
}

void sort_symbols(t_symbol *symbols, int cnt) {
    t_symbol tmp;
    for (int _ = 0; _ < cnt; _++) {
        for (int i = 0; i < cnt - 1; i++) {
            if (ft_strcmp(symbols[i].name, symbols[i + 1].name) > 0) {
                tmp = symbols[i];
                symbols[i] = symbols[i + 1];
                symbols[i + 1] = tmp;
            }
        }
    }
}

void show_symbols_64(t_symbol *symbols, int cnt) {
    for (int i = 0; i < cnt; i++) {
        if (symbols[i].type == 'U' || (symbols[i].type == 'w' && symbols[i].value == 0)) {
            put_rep(" ", 16);
        } else {
            put_hex_n(symbols[i].value, 16);
        }
        put(" ");
        putn(&symbols[i].type, 1);
        put(" ");
        put(symbols[i].name);
        put("\n");


    }
}

// https://stackoverflow.com/questions/15225346/how-to-display-the-symbols-type-like-the-nm-command
char get_sym_type(struct elf64_shdr *shdr, struct elf64_sym sym) {
    char c = '?';
    //if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
        //c = 'u';
    if (ELF64_ST_BIND(sym.st_info) == STB_WEAK) {
        c = 'W';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'w';
    } else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
        c = 'V';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'v';
    } else if (sym.st_shndx == SHN_UNDEF) {
        c = 'U';
    } else if (sym.st_shndx == SHN_ABS) {
        c = 'A';
    } else if (sym.st_shndx == SHN_COMMON) {
        c = 'C';
    } else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) {
        c = 'B';
    } else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == SHF_ALLOC) {
        c = 'R';
    } else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) {
        c = 'D';
    } else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) {
        c = 'T';
    } else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
        c = 'D';
    else {
        c = '?';
    } 

    if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
        c += 32;

    return c;
}

   
   