./cmmc $1 -o prog.s
as prog.s -o prog.o
ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 \
	  /usr/lib/x86_64-linux-gnu/crt1.o \
	  /usr/lib/x86_64-linux-gnu/crti.o \
	  -lc \
	  prog.o \
	  stdcminusminus.o \
	  /usr/lib/x86_64-linux-gnu/crtn.o \
    -o prog.exe