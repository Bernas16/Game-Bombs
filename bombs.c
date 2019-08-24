#include <pg/graphics.h>
#include <pg/events.h>
#include <pg/audio.h>
#include <stdbool.h>


#include "components.h"
#include "bombs_model.h"

#define TIME_BASE 50

#define START_BOARD_X 50
#define START_BOARD_Y 80

#define LIVES_X 	200
#define LIVES_Y		20

#define MSG_X		100
#define MSG_Y		200


#define BOMB_BLOW "bomb.wav"
#define INACTIVE_BOMB "punch.wav"

// Cores

#define BACK_BOARD_COLOR c_gray
#define BOMB_COLOR c_orange
#define BACK_COLOR graph_rgb(0,0, 128)

#define BIG_BOMB_COLOR graph_rgb(200, 90, 0)

// variáveis globais
Counter lives;
bool game_end;
MsgView final_msg;

void bomb_erase(Bomb b, bool explosion) {
	int x = b.center.x + START_BOARD_X;
	int y = b.center.y + START_BOARD_Y;
	graph_circle(x, y, b.radius, BACK_BOARD_COLOR, true);
	if (explosion) sound_play(BOMB_BLOW);
	//else sound_play(INACTIVE_BOMB);
}

void bomb_draw(Bomb b) {
	int x = b.center.x + START_BOARD_X;
	int y = b.center.y + START_BOARD_Y;
	graph_circle(x, y, b.radius, b.radius > 20 ? BIG_BOMB_COLOR: BOMB_COLOR, true);
	graph_circle(x, y, b.radius-1, c_black, false);
	
	// draw remaining time
	/*
	char text[3];
	
	sprintf(text, "%.1lf", (double)b.remainingTicks/(1000/TIME_BASE));
	graph_text(b.center.x-10+START_BOARD_X, b.center.y +6+START_BOARD_Y, c_black, text, SMALL_FONT);
	* */
}

void process_timer() {
	
	if (game_end) return;
	
	Bomb created[MAX_ACTIVE_BOMBS];
	Bomb removed[MAX_ACTIVE_BOMBS];
	
	Sizes sz = game_tick(removed, created);
	//percorrer bombas removidas (que rebentaram)
	for(int i=0; i < sz.nRemoved; ++i) {
		bomb_erase(removed[i], true);
		lives = ct_dec(lives);
		if (lives.val ==0) {
			mv_show_text(final_msg, "Perdeu!", ALIGN_CENTER);
			game_end=true;
		}
	}
			
	
	//percorrer bombas criadas
	for(int i=0; i < sz.nCreated; ++i)
		bomb_draw(created[i]);
}

void process_mouse(MouseEvent me) {
	if (game_end) return;
	if (me.button == BUTTON_LEFT &&
		me.type == MOUSE_BUTTON_EVENT &&
		me.state == BUTTON_PRESSED) {
		
		int x = me.x - START_BOARD_X;
		int y = me.y - START_BOARD_Y;
		
		Bomb b = game_select(x, y);
		if (b.radius > 0) { // acertámos numa bomba!
			bomb_erase(b, false);
			lives = ct_inc(lives);
			if (game_bombs() ==0) {
				mv_show_text(final_msg, "Venceu. Parabens!", ALIGN_CENTER);
				game_end=true;
			}
		}
	}
}

void draw_board() {
	// background
	graph_rect(0, 0, 
				GRAPH_WIDTH, GRAPH_HEIGHT, BACK_COLOR, true);
	graph_rect(START_BOARD_X, START_BOARD_Y, 
				GAME_WIDTH, GAME_HEIGHT, BACK_BOARD_COLOR, true);
}


void create_components() {
	lives = ct_create(LIVES_X, LIVES_Y, 10, "Vidas");
	final_msg = mv_create(MSG_X, MSG_Y, 20, 10, 10, LARGE_FONT, c_orange, c_black);
	ct_show(lives);
}
int main() {
	graph_init();
	
	graph_regist_timer_handler(process_timer, TIME_BASE);
	graph_regist_mouse_handler(process_mouse);
	game_init();
	draw_board();
	create_components();
	graph_start();
	return 0;
}
