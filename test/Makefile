# Get TDM-GCC here: https://jmeubank.github.io/tdm-gcc

all: 1.exe 2.exe

1.exe: 1.c
	gcc $^ -lgdi32 -o $@

2.exe: 2.c
	gcc $^ -lgdi32 -lopengl32 -lglu32 -o $@
