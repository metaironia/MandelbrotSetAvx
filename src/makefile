.PHONY: all clean rebuild
CC=g++
WARNINGS=-Wshadow -Winit-self -Wcast-align -Wundef -Wfloat-equal -Winline -Wredundant-decls \
 	 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default \
	 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy \
	 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
	 -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing \
	 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE 
SOURCES= main.cpp graphics.cpp mandelbrot_computation.cpp
		 		 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=mandelbrot

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(WARNINGS) $^ -o $@

%.o : %.cpp %.h
	$(CC) -c $(WARNINGS) $< -o $@                    

clean:
	 rm -f *.o mandelbrot.exe

rebuild:
	make clean
	make all