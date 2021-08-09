CFLAGS = -std=c20
LDFLAGS =-lSDL2 -lSDL2_image -lSDL2_ttf

Test: main.c
	gcc main.c $(LDFLAGS)

Bug: main.c
	gcc $(CFLAGS) -g DDEBUG main.c $(LDFLAGS)

.Phony: test bug clean

test: Test
	./a.out

bug: Bug
	./a.out

clean:
	rm -f a.out