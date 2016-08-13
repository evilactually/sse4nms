nasm -f elf -F dwarf -g adder.nasm
nasm -f elf -F dwarf -g test.nasm
nasm -f elf -F dwarf -g roundps.nasm
gcc -g adder.o test.o roundps.o
