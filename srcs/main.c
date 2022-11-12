#include "../inc/ft_nm.h"

/* void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

   int munmap(void *addr, size_t length); */

// stat -> file_info

struct stat buf;

#define iter(l, r) for (int i = l; i < r; i++)

char *read_file(char *name) {
    int fd;
    if ((fd = open(name, O_RDONLY)) == -1)
        throw("Error: open() failed\n");

    fstat(fd, &buf);

    if (buf.st_size == 0)
        throw("Error: file is empty\n");

    dbg("File size: %ld\n", buf.st_size);
    printf("buf.st_size : %ld\n", buf.st_size);

    // mmap put the content of the file in a new virtual memory space and ret a ptr to it
    char *file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == MAP_FAILED) {
        throw("Error: mmap() failed\n");
    }
}


int main(int ac, char **av) {

    char *file = read_file(av[1]);
    struct elf64_hdr *header = (struct elf64_hdr *)file;

    // check magic number 
    if (header->e_ident[EI_MAG0] != ELFMAG0 ||
        header->e_ident[EI_MAG1] != ELFMAG1 ||
        header->e_ident[EI_MAG2] != ELFMAG2 ||
        header->e_ident[EI_MAG3] != ELFMAG3) {
        throw("Error: not an ELF file\n");
    }

    if (header->e_ident[EI_CLASS] == ELFCLASS64) {
        ft_nm_64((void *)header, buf.st_size);
    } else {
        throw("Error: not a 64-bit ELF file\n");
    }


    int bits = file[4] == 1 ? 32 : 64;

    printf("magic:");
    iter(0, 4) {
        printf("%c", file[i]);
    }
    printf("\nEI_Class: %d (%dbit)\n",file[4], file[4] * 32);

    //printf("endian: %d\n", (char)file[EI_DATA]);
    //printf("e_type: %X\n", (short)file[16]);
    //printf("e_machine: %X\n", (char)file[18]);
    //printf("e_entry: %p\n", (void *)file + 24);
    //printf("e_phnum (number of entry): %d\n", (short)file[0x38]);

}