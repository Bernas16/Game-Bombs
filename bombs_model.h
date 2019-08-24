#define MAX_ACTIVE_BOMBS 3

// dimensões da cena
#define GAME_HEIGHT 300
#define GAME_WIDTH	500


typedef struct bomb {
	int remainingTicks;
	int radius;
	Point center;
} Bomb;

typedef struct sizes {
	int nCreated, nRemoved;
} Sizes;

/**
 * retorna a bomba que contém o ponto (x,y)
 * ou noBomb se o ponto não estiver contido em nenhuma
 * bomba
 */
Bomb game_select(int x, int y);

/**
 * Chamada por cada tick do relógio
 * preenche o array removed com as bombas que expiraram
 * prenche o array created com as bombas que foram criadas neste tick
 * retorna a dimensão dos dois arrays numa variável do tipo Sizes
 * 
 */
Sizes game_tick(Bomb removed[], Bomb created[]);

/**
 * Chamada para iniciar o estado do jogo
 */
void game_init();


int game_bombs();
