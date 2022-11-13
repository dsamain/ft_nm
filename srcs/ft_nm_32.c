#include "../inc/ft_nm.h"

void ft_nm_32(void *raw_file, char *file_name, size_t size, u_int32_t flags) {
    struct elf32_hdr *header = (struct elf32_hdr *)raw_file;


    if (!header->e_shoff || !header->e_phoff) {
        err(cat("ft_nm: ", file_name, ": File format not recognized\n"));
        return;
    }

    struct elf32_shdr *section_header_table = (struct elf32_shdr *)(raw_file + header->e_shoff); 

    if ((void *)section_header_table + sizeof(struct elf32_shdr) * header->e_shnum > (void *)header + size) {
        err(cat("ft_nm: ", file_name, ": File truncated\n"));
        return;
    }


    size_t shdr_size = header->e_shentsize; // size of a section header
    size_t shdr_num = header->e_shnum; // number of entry
    size_t shdr_str_idx = header->e_shstrndx; // index of the section header string table

    if (shdr_str_idx >= shdr_num) {
        err(cat("ft_nm: ", file_name, ": invalid index for section header string table\n"));
        return;
    }

    struct elf32_shdr shstrtab = section_header_table[shdr_str_idx]; // section header string table


    // iteratate over all section header to find symtab_header and strtab_header
    struct elf32_shdr *symtab_hdr = NULL, *strtab_hdr = NULL;
    for (int i = 0; i < shdr_num; i++) {
        struct elf32_shdr *section_header = section_header_table + i;
        char *name = raw_file + shstrtab.sh_offset + section_header->sh_name;
        // check NULL terminated name
        char *tmp = name;
        while (*tmp != '\0') {
            if ((void *)tmp >= (void *)header + size) {
                err(cat("ft_nm: ", file_name, ": File truncated\n"));
                return;
            }
            tmp++;
        }

        if (!ft_strcmp(name, ".symtab")) {
            symtab_hdr = section_header;
        } else if (!ft_strcmp(name, ".strtab")) {
            strtab_hdr = section_header;
        }
    }

    if (!symtab_hdr || !strtab_hdr) {
        err(cat("ft_nm:  ", file_name, ":  no symbols found\n"));
        return ;
    }


    // symbol table and string table associated with it
    struct elf32_sym *symtab = (struct elf32_sym *)(raw_file + symtab_hdr->sh_offset);
    char *strtab = raw_file + strtab_hdr->sh_offset;

    // check overflow of symtab and strtab
    if ((void *)symtab + strtab_hdr->sh_size > (void *)header + size
        || (void *)strtab + strtab_hdr->sh_size > (void *)header + size) {
        err(cat("ft_nm: ", file_name, ": File truncated\n"));
        return;
    }

    size_t sym_cnt = symtab_hdr->sh_size / sizeof(struct elf32_sym);
    t_symbol *symbols = ft_malloc(sizeof(t_symbol) * sym_cnt);
    int idx = 0;

    // iterate over all symbol table entry
    for (int i = 0; i < sym_cnt ; i++) {
        struct elf32_sym sym = symtab[i];
        if (sym.st_name == 0) {
            continue;
        }
        symbols[idx].name = strtab + sym.st_name;
        symbols[idx].type = get_sym_type_32(section_header_table, sym);
        symbols[idx].value = sym.st_value;

        if (symbols[idx].type == 'a') {
            continue;
        }

        idx++;
    }

    if (!(flags & FLAG_P))
        sort_symbols(symbols, idx, flags);

    if (!(flags & FLAG_UNIQUE)) {
        put(cat("\n", file_name, ":\n"));
    }
    show_symbols(symbols, idx, flags, 8);
}

char get_sym_type_32(struct elf32_shdr *shdr, struct elf32_sym sym) {

    char c = '?';
    if (ELF32_ST_BIND(sym.st_info) == STB_WEAK) {
        c = 'W';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'w';
    } else if (ELF32_ST_BIND(sym.st_info) == STB_WEAK && ELF32_ST_TYPE(sym.st_info) == STT_OBJECT) {
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
    } else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC) { 
        c = 'D';
    }


    if (ELF32_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
        c += 32;

    if (ELF32_ST_BIND(sym.st_info) == STB_LOCAL && c == '?')
        c = 'd';

    return c;
}

   
   