
patch.exe: patch.c roundps_xmm3_xmm0_09.bin
	gcc patch.c -o patch.exe

%.bin: %.nasm
	nasm -f bin $< -o $@