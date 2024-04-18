.PHONY: all clean rebuild benchmark_O0 benchmark_O3
CC=g++
CFLAGS = -mavx -mavx2
WARNINGS=-Wshadow -Winit-self -Wcast-align -Wundef -Wfloat-equal -Winline -Wredundant-decls \
 	 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default \
	 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy \
	 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
	 -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing \
	 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE 
SOURCES= main.cpp graphics.cpp mandelbrot_computation.cpp		 		 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=mandelbrot

all: set_O3

build: $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(WARNINGS) $^ -o $@

%.o : %.cpp
	$(CC) -c $(CFLAGS) $(WARNINGS) $< -o $@                    

set_O3: CFLAGS += -O3
set_O3: rebuild 

set_O0: CFLAGS += -O0
set_O0: rebuild

benchmark_O0: CFLAGS += -DBENCHMARK
benchmark_O0: set_O0

benchmark_O3: CFLAGS += -DBENCHMARK
benchmark_O3: set_O3

clean:
	 rm -f *.o mandelbrot.exe

rebuild: clean build



