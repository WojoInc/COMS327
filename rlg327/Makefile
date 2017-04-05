$HEADERS = dijkstra.h dungeon.h heap_a.h main.h monster.h player.h proj_incl.h control_IO.h
$HEADERSCPP = dijkstra.h dungeon.h heap.h main.h proj_incl.h control_IO.h character.h npc.h pc.h
default: cpp

c: main.c dijkstra.c heap_a.c main.c dungeon.c player.c monster.c control_IO.c ${HEADERS}
	gcc -c main.c dungeon.c heap_a.c dijkstra.c player.c monster.c control_IO.c
	gcc main.o heap_a.o dungeon.o dijkstra.o player.o monster.o control_IO.o -lncurses -o dungeon

cpp: main.cpp dijkstra.cpp heap.cpp dungeon.cpp npc.cpp pc.cpp character.cpp control_IO.cpp ${HEADERSCPP}
	g++ -g -std=gnu++0x -c main.cpp dungeon.cpp heap.cpp dijkstra.cpp pc.cpp character.cpp npc.cpp control_IO.cpp
	g++ main.o heap.o dungeon.o dijkstra.o pc.o npc.o character.o control_IO.o -o dungeon -lncurses

clean:
	rm -rf main.o dungeon.o heap.o dijkstra.o control_IO.o character.o pc.o npc.o
	rm -rf dungeon


