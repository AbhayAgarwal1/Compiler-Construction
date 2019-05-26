# Compiler-Construction
compiler implementation of simple language in c.

NOTE : Working on 64 bit ubuntu 16.04

# Running Instructions

sh script.sh

    or
    
gcc driver.c

./a.out t.txt code.asm

nasm -f elf32 code.as
m
ld -m elf_i386 -s -o code code.o

./code

