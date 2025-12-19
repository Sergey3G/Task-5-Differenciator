CC = g++

CFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

TARGET = program

OBJS = main.o trees.o graph.o tex.o calculator.o simplifier.o differenciator.o taylor.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c trees.h graph.h tex.h calculator.h simplifier.h differenciator.h taylor.h my_assert.h
	$(CC) $(CFLAGS) -c main.c -o main.o

trees.o: trees.c trees.h my_assert.h
	$(CC) $(CFLAGS) -c trees.c -o trees.o

graph.o: graph.c graph.h trees.h
	$(CC) $(CFLAGS) -c graph.c -o graph.o

tex.o: tex.c tex.h trees.h my_assert.h
	$(CC) $(CFLAGS) -c tex.c -o tex.o

calculator.o: calculator.c calculator.h
	$(CC) $(CFLAGS) -c calculator.c -o calculator.o

simplifier.o: simplifier.c simplifier.h trees.h
	$(CC) $(CFLAGS) -c simplifier.c -o simplifier.o

differenciator.o: differenciator.c differenciator.h trees.h my_assert.h
	$(CC) $(CFLAGS) -c differenciator.c -o differenciator.o

taylor.o: taylor.c taylor.h trees.h simplifier.h differenciator.h my_assert.h
	$(CC) $(CFLAGS) -c taylor.c -o taylor.o

clean:
	rm -f $(OBJS) $(TARGET)
