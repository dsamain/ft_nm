#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include  <elf.h>
#include "utils.h"

#define ERR(x) {ft_putstr(x); return ;}
#define R_ERR(x, ret) {ft_putstr(x); return ret;}
