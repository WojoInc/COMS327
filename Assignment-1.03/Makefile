$HEADERS = dijkstra.h dungeon.h heap_a.h main.h proj_incl.h

default: dungeon

dungeon: main.c dijkstra.c heap_a.c main.c dungeon.c ${HEADERS}
	gcc -c main.c dungeon.c heap_a.c dijkstra.c
	gcc main.o heap_a.o dungeon.o dijkstra.o -o dungeon

clean:
	rm -rf main.o dungeon.o heap_a.o dijkstra.o
	rm -rf dungeon


