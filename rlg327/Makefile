$HEADERS = dijkstra.h dungeon.h heap_a.h main.h monster.h player.h proj_incl.h

default: dungeon

dungeon: main.c dijkstra.c heap_a.c main.c dungeon.c player.c monster.c ${HEADERS}
	gcc -c main.c dungeon.c heap_a.c dijkstra.c player.c monster.c
	gcc main.o heap_a.o dungeon.o dijkstra.o player.o monster.o -o dungeon

clean:
	rm -rf main.o dungeon.o heap_a.o dijkstra.o player.o monster.o
	rm -rf dungeon


