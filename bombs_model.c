#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "components.h"
#include "bombs_model.h"



// Variáveis globais com o estado do jogo

Bomb bombs[MAX_ACTIVE_BOMBS];
int nBombs;

int height = GAME_HEIGHT;
int width = GAME_WIDTH;

int radius_min;
int radius_max;

int min_x;
int max_x;

int min_y;
int max_y;

int min_ticks;
int max_ticks;

int ticksCreation = 10;

void init_limits() {
	radius_min = 12;
	radius_max = 25;

	min_x = radius_max;
	max_x = width - radius_max;

	min_y = radius_max;
	max_y = height - radius_max;

	min_ticks = 10;
	max_ticks = 50;
}

// só para indicar a inexistência de bomba
Bomb noBomb = { .radius = 0 };


// função auxiliar para ger um valor aleatório dentro
// de um intervalo
int rand_range(int li, int ls) { 
	int r = rand() % (ls - li +1) + li;
	return r;
}

bool bomb_contains(Bomb b, int x, int y) {
	int bx = b.center.x;
	int by = b.center.y;
	int dist = sqrt((bx-x)*(bx-x) + (by-y)*(by-y));
	return  dist <= b.radius;
}

bool bomb_intersects(Bomb b1, Bomb b2) {
	int b1x = b1.center.x;
	int b1y = b1.center.y;
	int b2x = b2.center.x;
	int b2y = b2.center.y;
	int dist = (b1x-b2x)*(b1x-b2x) + (b1y-b2y)*(b1y-b2y);
	return  dist <= ((b1.radius+b2.radius+1)*(b1.radius+b2.radius+1));
}

bool bomb_colides(Bomb b) {
	for(int i=0; i < nBombs; ++i)  
		if (bomb_intersects(b, bombs[i])) return true;
	return false;
}

void bomb_remove(int i) {
	for (int j = i +1; j < nBombs; ++j) 
		bombs[j-1] = bombs[j];
	nBombs--;
}

Bomb game_select(int x, int y) {
	for(int i=0; i < nBombs; ++i) {
		if (bomb_contains(bombs[i], x, y)) {
			// acertámos numa bomba
			Bomb hitted = bombs[i];
			bomb_remove(i); // tirar a bomba!
			return hitted;
		}
	}
	return noBomb;
}

Bomb create_bomb_random() {
	Bomb nb;
	do {
		nb.remainingTicks = rand_range(min_ticks, max_ticks);
		nb.radius = rand_range(radius_min, radius_max);
		nb.center.x = rand_range(min_x, max_x);
		
		
		nb.center.y = rand_range(min_y, max_y);
	}
	while(bomb_colides(nb));
	
	return nb;
}

/**
 * Chamada por cada tick do relógio
 * preenche o array removed com as bombas que expiraram
 * prenche o array created com as bombas que foram criadas neste tick
 * retorna a dimensão dos dois arrays numa variável do tipo Sizes
 * 
 */
Sizes game_tick(Bomb removed[], Bomb created[]) {
	int nr = 0, nc = 0;
	// percorrer o arrays das bombas e actualizar o remainingTicks
	for(int i=0; i < nBombs; ++i) {
		bombs[i].remainingTicks--;
		if (bombs[i].remainingTicks == 0) { // bomba_expirada
			removed[nr] = bombs[i]; // acrescentar ao array removed
			nr++;
			bomb_remove(i);	// remover do array bombs
		}
	}
	
	if (nBombs < MAX_ACTIVE_BOMBS && --ticksCreation == 0) {
		ticksCreation =10;
		Bomb nb = create_bomb_random();
		bombs[nBombs++] = nb; // inserir nova bomba no array bombs
		created[0] = nb;
		nc = 1;
	}
	
	Sizes s = {.nRemoved = nr, .nCreated = nc };
	return s;
		
}

int game_bombs() {
	return nBombs;
}

void game_init() {
	init_limits();
}
