#include "../inc/ft_nm.h"

/* void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

   int munmap(void *addr, size_t length); */

// stat -> file_info


#define iter(l, r) for (int i = l; i < r; i++)

char *read_file(char *name, size_t *size) {

    struct stat buf;

    int fd;
    if ((fd = open(name, O_RDONLY)) == -1)
        ret_err(cat("ft_nm:  ", name, ": No such file or directory\n"), NULL);

    fstat(fd, &buf);

    *size = buf.st_size;

    if (buf.st_size == 0)
        ret_err(cat("ft_nm:  ", name, ": file is empty"), NULL);

    dbg("File size: %ld\n", buf.st_size);
    dbg("buf.st_size : %ld\n", buf.st_size);

    // mmap put the content of the file in a new virtual memory space and ret a ptr to it
    char *file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == MAP_FAILED)
        throw("Error: mmap() failed\n");
}

void parse(int ac, char **av, int *flags, char ***files, int *file_cnt) {
    for (int i = 1; i < ac; i++) {
        if (av[i][0] == '-') {
            if (!ft_strcmp(av[i], "-g")) {
                *flags |= FLAG_G;
            } else if (!ft_strcmp(av[i], "-u")) {
                *flags |= FLAG_U;
            } else if (!ft_strcmp(av[i], "-r")) {
                *flags |= FLAG_R;
            } else if (!ft_strcmp(av[i], "-p")) {
                *flags |= FLAG_P;
            } else {
                throw("Error: invalid flag\n");
            }
        } else {
            (*files)[(*file_cnt)++] = av[i];
        }
    }

    if (*file_cnt == 0) {
        (*files)[(*file_cnt)++] = "a.out";
    }
    
    *flags |= (*file_cnt == 1 ? FLAG_UNIQUE : 0);
}

int main(int ac, char **av) {
    char **files = ft_malloc(sizeof(char*) * ac);
    int files_size = 0;
    int flags = 0;

    parse(ac, av, &flags, &files, &files_size);

    for (int i = 0; i < files_size; i++) {

        size_t size;
        char *file = read_file(files[i], &size);

        if (!file)
            continue;

        if (size < sizeof(struct elf64_hdr)) {
            err(cat("ft_nm: ", files[i], ": File truncated\n"));
            continue;
        }
            
        struct elf64_hdr *header = (struct elf64_hdr *)file;

        // check magic number 
        if (header->e_ident[EI_MAG0] != ELFMAG0 ||
            header->e_ident[EI_MAG1] != ELFMAG1 ||
            header->e_ident[EI_MAG2] != ELFMAG2 ||
            header->e_ident[EI_MAG3] != ELFMAG3) {
            err(cat("ft_nm: ", files[i], ": File format not recognized\n"));
            continue;
        }

        // check arch 
        dbg("arch : %d\n", header->e_ident[EI_CLASS]);
        if (header->e_ident[EI_CLASS] == ELFCLASS64) {
            if (header->e_machine != EM_X86_64) {
                err(cat("ft_nm: ", files[i], ": File format not recognized\n"));
                continue;
            }
            ft_nm_64((void *)header, files[i], size, flags);
        } else if (header->e_ident[EI_CLASS] == ELFCLASS32) {
            if (header->e_machine != EM_386) {
                err(cat("ft_nm: ", files[i], ": File format not recognized\n"));
                continue;
            }
            dbg("32 bit\n");
            ft_nm_32((void *)header, files[i], size, flags);
        } else {
            err(cat("ft_nm: ", files[i], ": File format not recognized\n"));
        }
    } 

    //printf("endian: %d\n", (char)file[EI_DATA]);
    //printf("e_type: %X\n", (short)file[16]);
    //printf("e_machine: %X\n", (char)file[18]);
    //printf("e_entry: %p\n", (void *)file + 24);
    //printf("e_phnum (number of entry): %d\n", (short)file[0x38]);

}