FLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

all: Assembler

Assembler : main.o buffer.o stack.o spu.o
	g++ main.o buffer.o stack.o spu.o -o Assembler $(FLAGS)

main.o : main.cpp buffer.h stack.h commands.h spu.h
	g++ main.cpp -c $(FLAGS)

buffer.o : buffer.cpp buffer.h
	g++ -c buffer.cpp $(FLAGS)

stack.o : stack.cpp stack.h
	g++ -c stack.cpp $(FLAGS)

spu.o : spu.cpp spu.h stack.h
	g++ -c spu.cpp $(FLAGS)

.PHONY: clean

clean:
	rm -f *.o