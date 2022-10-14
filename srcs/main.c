#include "../inc/ft_nm.h"

/* void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

   int munmap(void *addr, size_t length); */

// stat -> file_info

struct stat buf;

#define iter(l, r) for (int i = l; i < r; i++)

int main(int ac, char **av) {
    int fd;
    if ((fd = open(av[1], O_RDONLY)) == -1)
        R_ERR("Error: open() failed\n", 1);

    fstat(fd, &buf);
    if (buf.st_size == 0)
        R_ERR("Error: file is empty\n", 1);

    printf("buf.st_size : %ld\n", buf.st_size);

    // mmap put the content of the file in a new virtual memory space and ret a ptr to it
    char *file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == MAP_FAILED) {
        R_ERR("Error: mmap() failed\n", 1);
    }

    int bits = file[4] == 1 ? 32 : 64;

    printf("magic:");
    iter(0, 4) {
        printf("%c", file[i]);
    }
    printf("\nEI_Class: %d (%dbit)\n",file[4], file[4] * 32);

    printf("endian: %d\n", (char)file[EI_DATA]);
    printf("e_type: %X\n", (short)file[16]);
    printf("e_machine: %X\n", (char)file[18]);
    printf("e_entry: %p\n", (void *)file[24]);
    printf("e_phnum (number of entry): %d\n", (short)file[0x38]);

}