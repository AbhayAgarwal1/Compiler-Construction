gcc driver.c
./a.out t.txt code.asm
nasm -f elf32 code.asm
ld -m elf_i386 -s -o code code.o
./code
