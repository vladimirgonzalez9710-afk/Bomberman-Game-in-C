//Using libs SDL, glibc
#include <SDL.h>	//SDL version 2.0
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>




// Numbers bitmap, some routines and and SDL initialization taken from
// https://github.com/flightcrank/pong


// Please follow the coding guidelines described in:
// https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html



/* Do not use magic numbers in the code, define the meaning of the number
   in this section and the use the define across the code. This increases
   readability accross the code
*/


// The constants used to define the size of the window
#define SCREEN_WIDTH 1480	//window height
#define SCREEN_HEIGHT 1000	//window width

// The constants used for the movement of the block
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3


// Block size that will be used for drawing on the screen
#define BLOCK_SIZE 30


// Coordinates of the starting player
#define PLAYER_START_X 80
#define PLAYER_START_Y 128

// For the return of the functions
#define SUCCESS 0
#define FAILURE 1

// Defines of the game states of each screen
#define START_SCREEN 0
#define LEVEL_1 1
#define LEVEL_2 2
#define LEVEL_3 3
#define GAME_OVER 6
#define next_level 10

// Defines for states
#define FALSE 0
#define TRUE 1

// The colors used in the game
#define BLACK 0x000000ff
#define WHITE 0xffffffff

// Timing delays
#define INPUT_DELAY_MS 500

// Time to render a frame in ms = 1000/60
#define FRAME_TIME_MS 16

// Movement diferential
#define MOVEMENT_DELTA 5

// PUSH the bomb
#define PUSH 0

// More bombs
#define MAX_BOMBS 3

//Enemies
#define ENEMIES 3

//player lifes
#define LIFES 5

// Width of the fire
#define F_WIDTH 60  

// Height of the fire
#define F_HEIGHT 44

// Time of the levels 
#define TIME_LEVEL 250

//function prototypes
//initialise SDL
int init_SDL(int w, int h, int argc, char *args[]);


typedef struct player_t {

	int x; 
	int y;
	int w; 
	int h;

} player_t;

typedef struct game_element_t {
	int x;
	int y;
	int w;
	int h;

} game_element_t;

struct coord{
	int x;
	int y;
};

// This is one of the few cases where it makes sense to use magic numbers
// Avoid the use of global variables at maximum
int g_score[] = {0,0}; 
// Avoid the use of global variables, modify the code to avoid its use.
int g_width, g_height;		//used if fullscreen

int push_bomb = PUSH;   // Push bomb

int push_bomb1 = PUSH;	// Push the second bomb

int speed = MOVEMENT_DELTA;	// speed of the player

//Lifes player
int life = LIFES;

//Counter global
int kills_counter;

// Declared the fire variables

int fire_width = F_WIDTH;
int fire_R_width = F_WIDTH;
int fire_L_width = F_WIDTH;

int fire_height = F_HEIGHT;
int fire_U_height = F_HEIGHT; 
int fire_D_height = F_HEIGHT;

bool power_up_active = false;  // For the bombs power up

bool power_fire = false;   // For the fire power up


// Declared the bomb time variables

Uint32 init_time_bomb = 0; 
Uint32 init_time_bomb1 = 0;
Uint32 init_time_1 = 0;

// Declared the timer variables

int timer[] = {0,0,0};

int temp = TIME_LEVEL;

//Puntaje
int score = 0;

Uint32 init_time = 0;
Uint32 game_time = 0;



SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen

//surfaces
static SDL_Surface *screen;
static SDL_Surface *title;
static SDL_Surface *jugador;
static SDL_Surface *bmpSolido;
static SDL_Surface *bmpDoor;
static SDL_Surface *bmpExplot;
static SDL_Surface *bmpEnemy;
static SDL_Surface *bmpDestruible;
static SDL_Surface *bmpBomba;
static SDL_Surface *bmplava;
static SDL_Surface *grass;
static SDL_Surface *heart;
static SDL_Surface *numbermap;
static SDL_Surface *bmpnext;
static SDL_Surface *bmpwin;
static SDL_Surface *bmpwater;
static SDL_Surface *gameover;

//textures 
SDL_Texture *screen_texture;



static void init_game(game_element_t *player, game_element_t *map_element, game_element_t *blocks_destroyer, game_element_t *enemies, game_element_t *door) {




struct coord destroyer_coordinates[78] = {
        {795, 133}, {693, 133}, {489, 133}, {387, 133}, {285, 133}, {897, 133}, {999, 133}, {1102, 133}, {1203, 133}, {1305, 133},  
        {282, 220}, {489, 220}, {690, 220}, {894, 220}, {1098, 220}, {1302, 220},  
        {489, 306}, {387, 306}, {285, 306}, {183, 306}, {693, 306}, {795, 306}, {897, 306}, {999, 306}, {1101, 306}, 
        {78, 393}, {285, 393}, {486, 393}, {690, 393}, {894, 393}, {1098, 393}, {1302, 393}, 
        {78, 480}, {180, 480}, {1203, 480}, {387, 480}, {489, 480}, {588, 480}, {690, 480}, {792, 480}, {897, 480},{999, 480}, {1098, 480}, {1200, 480}, {1302, 480}, 
        {78, 567}, {285, 567}, {690, 567}, {894, 567}, {1098, 567}, {1302, 567},
        {180, 654}, {282, 654}, {387, 654}, {489, 654}, {588, 654}, {690, 654}, {897, 654}, {999, 654}, {1098, 654}, {1302, 654}, 
        {285, 741}, {489, 741}, {690, 741}, {894, 741}, {1098, 741}, {1302, 741}, 
        {78, 828}, {285, 828}, {387, 828}, {588, 828}, {690, 828}, {792, 828}, {897, 828}, {999, 828}, {1098, 828}, {1203, 828}, {1302,828}
};
struct coord enemy_coordinates[11] = {    
       {591, 133}, {591, 306}, {282, 480}, {489, 567}, {78, 654},
        {792, 654}, {78, 741}, {489, 828}, {180, 828}, {1203, 654},
        {1203, 306}
};
  
              
	 // Initialize player's position
	 player->x = PLAYER_START_X;
	 player->y = PLAYER_START_Y;
	 player->w = 85;
	 player->h = 84;
    
	//Pared Superior
		
	map_element[1].x = 1;
	map_element[1].y = 67;
	map_element[1].w = 80*BLOCK_SIZE;
	map_element[1].h = 2.1*BLOCK_SIZE;
	
	//Pared inferior
		
	map_element[2].x = 1;
	map_element[2].y = 914;
	map_element[2].w = 80*BLOCK_SIZE;
	map_element[2].h = 3*BLOCK_SIZE;
	
	//Pared derecha
		
	map_element[3].x = 1409;
	map_element[3].y = 70;
	map_element[3].w = 5*BLOCK_SIZE;
	map_element[3].h = 100*BLOCK_SIZE;
	
	//Pared Izquierda
		
	map_element[4].x = -73;
	map_element[4].y = 80;
	map_element[4].w = 5*BLOCK_SIZE;
	map_element[4].h = 100*BLOCK_SIZE;
	
	//Primer Bloque

	map_element[5].x = 180;
	map_element[5].y = 220;
	map_element[5].w = 100;
	map_element[5].h = 84;
	
	
	//Segundo bloque
	
	map_element[6].x= 385;
	map_element[6].y= 220;
	map_element[6].w= 100;
	map_element[6].h= 84;
	
	//Tercer bloque
	
	map_element[7].x= 590;
	map_element[7].y= 220;
	map_element[7].w= 100;
	map_element[7].h= 84;
	
	//Cuarto bloque
	
	map_element[8].x= 795;
	map_element[8].y= 220;
	map_element[8].w= 100;
	map_element[8].h= 84;
	
	//Quinto bloque
	
	map_element[9].x= 1000;
	map_element[9].y= 220;
	map_element[9].w= 100;
	map_element[9].h= 84;
	
	//sexto bloque
	
	map_element[10].x= 1205;
	map_element[10].y= 220;
	map_element[10].w= 100;
	map_element[10].h= 84;
	
	// Segunda linea de bloques 
	//septimo bloque 
	
	map_element[11].x= 180;
	map_element[11].y= 393;
	map_element[11].w= 100;
	map_element[11].h= 84;
	
	//octavo bloque
	
	map_element[12].x= 385;
	map_element[12].y= 393;
	map_element[12].w= 100;
	map_element[12].h= 84;
	
	//noveno bloque
	
        map_element[13].x= 590;
	map_element[13].y= 393;
	map_element[13].w= 100;
	map_element[13].h= 84;
	
	//decimo bloque
	
        map_element[14].x= 795;
	map_element[14].y= 393;
	map_element[14].w= 100;
	map_element[14].h= 84;
	
	//undecimo bloque
	
	map_element[15].x= 1000;
	map_element[15].y= 393;
	map_element[15].w= 100;
	map_element[15].h= 84;
	
	//duodecimo bloque
	
	map_element[16].x= 1205;
	map_element[16].y= 393;
	map_element[16].w= 100;
	map_element[16].h= 84;
	
	//tercera linea 
	//bloque 13
	
	map_element[17].x= 180;
	map_element[17].y= 566;
	map_element[17].w= 100;
	map_element[17].h= 84;
	
	//bloque 14
	
	map_element[18].x= 385;
	map_element[18].y= 566;
	map_element[18].w= 100;
	map_element[18].h= 84;
	
	//bloque 15
	
	map_element[19].x= 590;
	map_element[19].y= 566;
	map_element[19].w= 100;
	map_element[19].h= 84;
	
	//bloque 16
	
	map_element[20].x= 795;
	map_element[20].y= 566;
	map_element[20].w= 100;
	map_element[20].h= 84;
	
	//bloque 17
	
	map_element[21].x= 1000;
	map_element[21].y= 566;
	map_element[21].w= 100;
	map_element[21].h= 84;
	
	//bloque 18
	
	map_element[22].x= 1205;
	map_element[22].y= 566;
	map_element[22].w= 100;
	map_element[22].h= 84;
	
	//bloque 19
	
	map_element[23].x= 180;
	map_element[23].y= 739;
	map_element[23].w= 100;
	map_element[23].h= 84;
	
	//bloque 20
	
	map_element[24].x= 385;
	map_element[24].y= 739;
	map_element[24].w= 100;
	map_element[24].h= 84;
	
	//bloque 21
	
	map_element[25].x= 590;
	map_element[25].y= 739;
	map_element[25].w= 100;
	map_element[25].h= 84;
	
	//bloque 22
	
	map_element[26].x= 795;
	map_element[26].y= 739;
	map_element[26].w= 100;
	map_element[26].h= 84;
	
	//bloque 23
	
	map_element[27].x= 1000;
	map_element[27].y= 739;
	map_element[27].w= 100;
	map_element[27].h= 84;
	
	//bloque 24
	
	map_element[28].x= 1205;
	map_element[28].y= 739;
	map_element[28].w= 100;
	map_element[28].h= 84;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int num_destroyer_coords = 78;

    srand(time(0)); 
    //int new_coord = 0;
    for (int new_coord = 1; new_coord < 26; new_coord++) {
        int coord_rand = rand() % 78;
        blocks_destroyer[new_coord].x = destroyer_coordinates[coord_rand].x;
        blocks_destroyer[new_coord].y = destroyer_coordinates[coord_rand].y;
        blocks_destroyer[new_coord].w = 100;
        blocks_destroyer[new_coord].h = 84;
        /*for (int i = coord_rand; i < 26 - 1; i--) {
            destroyer_coordinates[i].x = destroyer_coordinates[i + 1].x;
            destroyer_coordinates[i].y = destroyer_coordinates[i + 1].y;
        }*/
 
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

         int random_door = rand() % 26;
    door->x = blocks_destroyer[random_door].x;
    door->y = blocks_destroyer[random_door].y;
    door->w = 100; 
    door->h = 84;  
    
/////////////////////////////////////////////////////////////////

    srand(time(0)); 
    for (int i = 1; i <= ENEMIES; i++) {

        int coord_rand = rand() % (sizeof(enemy_coordinates) / sizeof(enemy_coordinates[0]));


        enemies[i].x = enemy_coordinates[coord_rand].x;
        enemies[i].y = enemy_coordinates[coord_rand].y;
        enemies[i].w = 60; 
        enemies[i].h = 44; 

        for (int j = coord_rand; j < (sizeof(enemy_coordinates) / sizeof(enemy_coordinates[0])) - 1; j++) {
            enemy_coordinates[j] = enemy_coordinates[j + 1];
        }
    }
}

/* Function: check_collision
 * --------------------------
 * This function verifies if there is collision between two game
 * elements.
 *
 * Arguments:
 * 	a: game element to be checked
 *	b: game element to be checked
 *
 * Return:
 *	TRUE if there is collision 
 * 	FALSE if there isn't collision
 */
int check_collision(game_element_t a, game_element_t b){
	
	int left_a = a.x;
	int right_a = a.x + a.w;
	int top_a = a.y;
	int bottom_a = a.y + a.h;

	int left_b = b.x;
	int right_b = b.x + b.w;
	int top_b = b.y;
	int bottom_b = b.y + b.h;

	if (bottom_a <= top_b)
		return FALSE;

	if (top_a >= bottom_b)
		return FALSE;

	if (right_a <= left_b)
		return FALSE;

	if (left_a >= right_b)
		return FALSE;

	// If none of the conditions worked, return TRUE

	return TRUE;

}


/* Function: move_player
 * ---------------------
 * This function is in charge of relocating the player position
 * across the screen
 *
 * Arguments:
 *	d: direction on which the player is going to be moved.
 *
 * Return:
 *	void.
 */


void move_player(int d, game_element_t *player, game_element_t *skate, game_element_t *map_element, game_element_t *Blocks_Destroyer){

	if(check_collision(*player, *skate) == TRUE){
		
		
		skate->x = -3000;			// modifies the speed of the player when the power up is activated
		skate->w = 0;
		skate->h = 0;
		speed = MOVEMENT_DELTA * 1.7;

	}
	
	if (d == LEFT) {
	
		player->x -= speed;
		
		for( int i = 1; i < 29; i++){
		
			int collision = FALSE;		
			collision = check_collision(*player, map_element[i]);
		
			if(TRUE == collision){
			
				player->x += speed;
				
			break;
			
			}
	     	}
		
		for( int i = 1; i < 26; i++){
		
			int collision = FALSE;		
			collision = check_collision(*player, Blocks_Destroyer[i]);
			
			if(TRUE == collision){
			
				player->x += speed;
				break;
			}
		}
	}
	
	if (d == RIGHT) {
	
		player->x += speed;
		
		for( int i = 1; i < 29; i++){
		
			int collision = FALSE;		
			collision = check_collision(*player, map_element[i]);
		
			if(TRUE == collision){
			
				player->x -= speed;
				
			break;
			
			}
	     	}
	     	
	     	
	
		for( int i = 1; i < 26; i++){
		
			int collision= FALSE;		
			collision = check_collision(*player, Blocks_Destroyer[i]);
			
			if(TRUE == collision){
			
				player->x -= speed;
				break;
			}
		}
	}
		
	if (d == UP) {
	
		player->y -= speed;
		
		for( int i = 1; i < 29; i++){
		
			int collision = FALSE;		
			collision = check_collision(*player, map_element[i]);
		
			if(TRUE == collision){
			
				player->y += speed;
				
			break;
			
			}
	     	}
	
		for( int i = 1; i < 26; i++){
		
			int collision = FALSE;		
			collision = check_collision(*player, Blocks_Destroyer[i]);
			
			if(TRUE == collision){
			
				player->y += speed;
				break;
			}
		}
	}

	if (d == DOWN) {
	
		player->y += speed;
		
		for( int i = 1; i < 29; i++){
		
			int collision = FALSE;		
			collision = check_collision(*player, map_element[i]);
		
			if(TRUE == collision){
			
				player->y -= speed;
				
			break;
			
			}
	     	}
	     	
		for( int i = 1; i < 26; i++){
		
			int collision = FALSE;		
			collision =  check_collision(*player, Blocks_Destroyer[i]);
			
			if(TRUE == collision){
			
				player->y -= speed;
				break;
			}
		}
	}
}

void move_enemies(game_element_t *player,game_element_t *map_element, game_element_t *Blocks_Destroyer, game_element_t *enemies,game_element_t *bomb){
 		
	srand(time(0));
	
	int move = rand() % 3;
	int move_1 = rand() % 4;
	int move_2 = rand() % 5;
	
	if (move == LEFT ){
	
		enemies[1].x -= MOVEMENT_DELTA;
		for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[1].x += MOVEMENT_DELTA;
				
			break;
			
			}
	     	}

		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[1].x += MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], *bomb);
		
		if(TRUE == collision){
			
			enemies[1].x += MOVEMENT_DELTA;
				
			}
	     	
	}
	/////////////////////////////////////////////
	if (move_1==LEFT){
		 enemies[2].x -= MOVEMENT_DELTA;
		for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[2].x += MOVEMENT_DELTA;
				
			break;
			
			}
	     	}

		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[2].x += MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], *bomb);
		
		if(TRUE == collision){
			
			enemies[2].x += MOVEMENT_DELTA;
				
			
			}
	     	
	}
	///////////////////////////////////////////////////////////////
	if (move_2==LEFT){
		enemies[3].x -= MOVEMENT_DELTA;
		for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[3].x += MOVEMENT_DELTA;
				
			break;
			
			}
	     	}

		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[3].x += MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
		
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], *bomb);
		
		if(TRUE == collision){
			
			enemies[3].x += MOVEMENT_DELTA;
			
			}
	     		     	
	}
		
	 if (move == RIGHT){
	 
	 	enemies[1].x += MOVEMENT_DELTA;
		for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[1].x -= MOVEMENT_DELTA;
			
			}
	     	}
	     	
		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[1].x -= MOVEMENT_DELTA;
				
			break;
			} 
		}

		
		int collision = FALSE;		
		collision = check_collision(enemies[1], *bomb);
		
		if(TRUE == collision){
			
			enemies[1].x -= MOVEMENT_DELTA;
			} 
		
	}
	//////////////////////////////////////////////////////////
	 if(move_1 == RIGHT){
		
		enemies[2].x += MOVEMENT_DELTA;
		for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[2].x -= MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	     	
		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[2].x -= MOVEMENT_DELTA;
				
			break;
			} 
		}
		
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], *bomb);
		
		if(TRUE == collision){
			
			enemies[2].x -= MOVEMENT_DELTA;
				
			} 
		
	}
	///////////////////////////////////////////////////////////
	  if(move_2 == RIGHT){
		enemies[3].x += MOVEMENT_DELTA;
		for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[3].x -= MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	     	
		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[3].x -= MOVEMENT_DELTA;
				
			break;
			} 
		}
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], *bomb);
		
		if(TRUE == collision){
			
			enemies[3].x -= MOVEMENT_DELTA;
				
			} 
		
	}

		
	  if (move == DOWN){
	 
	 	enemies[1].y += MOVEMENT_DELTA;
	 	for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[1].y -= MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	
		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[1].y -= MOVEMENT_DELTA;
				
			break;
		}
	    } 
	    
		
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], *bomb);
		
		if(TRUE == collision){
			
			enemies[1].y -= MOVEMENT_DELTA;
				
		}
	       
	}
	//////////////////////////////////////////////////////////////
	  if(move_1 == DOWN){
	    	enemies[2].y += MOVEMENT_DELTA;
	 	for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[2].y -= MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	
		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[2].y -= MOVEMENT_DELTA;
				
			break;
		}
	    }
		
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], *bomb);
		
		if(TRUE == collision){
			
			enemies[2].y -= MOVEMENT_DELTA;
				
		}
	    
	}
	////////////////////////////////////////////////////////////////
	   if(move_2 == DOWN){ 
	    	enemies[3].y += MOVEMENT_DELTA;
	 	for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[3].y -= MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	
		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[3].y -= MOVEMENT_DELTA;
				
			break;
		}
	    } 
		
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], *bomb);
		
		if(TRUE == collision){
			
			enemies[3].y -= MOVEMENT_DELTA;
				
		}
	    
	}
	
	 if (move == UP){
	 
	 	enemies[1].y -= MOVEMENT_DELTA;
	 	for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[1].y += MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	
		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[1], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[1].y += MOVEMENT_DELTA;
				
			break;
			
			}
		}
		int collision = FALSE;		
		collision = check_collision(enemies[1], *bomb);
		
		if(TRUE == collision){
			
			enemies[1].y += MOVEMENT_DELTA;
		}	
	}
	
	  if(move_1 == UP){
		enemies[2].y -= MOVEMENT_DELTA;
	 	for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[2].y += MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	
		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[2].y += MOVEMENT_DELTA;
				
			break;
			
			}
		}
		
		
		int collision = FALSE;		
		collision = check_collision(enemies[2], *bomb);
		
		if(TRUE == collision){
			
			enemies[2].y += MOVEMENT_DELTA;
				
			
			}
		
	}
	////////////////////////////////////////////////////////////////
	  if(move_2 == UP){
		enemies[3].y -= MOVEMENT_DELTA;
	 	for( int i = 1; i < 29; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], map_element[i]);
		
		if(TRUE == collision){
			
			enemies[3].y += MOVEMENT_DELTA;
				
			break;
			
			}
	     	}
	
		for( int i = 1; i < 26; i++){
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], Blocks_Destroyer[i]);
		
		if(TRUE == collision){
			
			enemies[3].y += MOVEMENT_DELTA;
				
			break;
			
			}
		}
		
		
		int collision = FALSE;		
		collision = check_collision(enemies[3], *bomb);
		
		if(TRUE == collision){
			enemies[3].y += MOVEMENT_DELTA;
		}
	}
}

// This function is for the death of the player
void die_player(game_element_t *player, game_element_t *exp1, game_element_t *exp2, game_element_t *exp3, game_element_t *exp4, game_element_t *exp1_1, game_element_t *exp1_2, game_element_t *exp1_3, game_element_t *exp1_4, game_element_t *enemies) {
    if (check_collision(*exp1, *player) == TRUE || check_collision(*exp2, *player) == TRUE || check_collision(*exp3, *player) == TRUE || check_collision(*exp4, *player) == TRUE || check_collision(*exp1_1, *player) == TRUE || check_collision(*exp1_2, *player) == TRUE || check_collision(*exp1_3, *player) == TRUE || check_collision(*exp1_4, *player) == TRUE) {
    
        life -= 1;
        player->x = PLAYER_START_X;
        player->y = PLAYER_START_Y;
    } else {
        for (int i = 1; i <= ENEMIES; ++i) {
            if (check_collision(enemies[i], *player) == TRUE) {
                life -= 1;
                player->x = PLAYER_START_X;
                player->y = PLAYER_START_Y;
                break; 
            }
        }
    }
}

// This function create the bomb
void create_bomb(game_element_t *player, game_element_t *bomb){
	
	//printf("Creando bomba\n");
	bomb->x = player->x + 30;
	bomb->y = player->y + 25;
	bomb->w = 60;
	bomb->h = 61;
	
	
	
}
// This function create the second bomb
void create_bomb1(game_element_t *player, game_element_t *bomb1){
	
	//printf("Creando bomba 1\n");
	bomb1->x = player->x + 30;
	bomb1->y = player->y + 25;
	bomb1->w = 60;
	bomb1->h = 61;
	
}


// The function explode the bomb when the time end
void explode_bomb(game_element_t *player, game_element_t *fire, game_element_t *bomb, game_element_t *exp1, game_element_t *exp2, game_element_t *exp3, game_element_t *exp4, game_element_t *exp1_1, game_element_t *exp1_2, game_element_t *exp1_3, game_element_t *exp1_4){

	if(check_collision(*player, *fire) == TRUE){
		
		power_fire = true;
		
		fire->y = 5000;
		fire->w = 0;
		fire->h = 0;
		
		fire_R_width = F_WIDTH * 7;
		fire_L_width = F_WIDTH * 7;
		fire_U_height = F_HEIGHT * 9;
		fire_D_height = F_HEIGHT * 9;
	}
	
	if(check_collision(*exp1_1, *bomb) == TRUE || check_collision(*exp1_2, *bomb) == TRUE || check_collision(*exp1_3, *bomb) == TRUE || check_collision(*exp1_4, *bomb) == TRUE){
	
		init_time_bomb = 0;
	
	}	
	
	if (SDL_GetTicks() - init_time_bomb > 3000){
	
		//add the blocks of the explotion
		
		exp1->x = bomb->x + 45;
		exp1->y = bomb->y;	 // Right	
		exp1->w = fire_R_width;  
		exp1->h = fire_height;

		exp2->x = bomb->x - 45;	 //Left
		exp2->y = bomb->y;		 
		exp2->w = fire_L_width;  
		exp2->h = fire_height;
	
		exp3->x = bomb->x;
		exp3->y = bomb->y + 45;	 //Up
		exp3->w = fire_width;  
		exp3->h = fire_U_height; 

		exp4->x = bomb->x;
		exp4->y = bomb->y - 45;	 //Down
		exp4->w = fire_width; 
		exp4->h = fire_D_height;

		bomb->x = 3000;
		bomb->y = 3000; 	//the bomb disappears
		bomb->w = 0;
		bomb->h = 0;
		
		push_bomb = 0;
	}
	
}


// The function explode the bombs of the power up when the time end
void explode_bomb1(game_element_t *player, game_element_t *fire, game_element_t *bomb1, game_element_t *exp1_1, game_element_t *exp1_2, game_element_t *exp1_3, game_element_t *exp1_4, game_element_t *exp1, game_element_t *exp2, game_element_t *exp3, game_element_t *exp4){
	
	if(power_fire == true){
	
		fire_R_width = F_WIDTH * 7;
		fire_L_width = F_WIDTH * 7;
		fire_U_height = F_HEIGHT * 9;
		fire_D_height = F_HEIGHT * 9;
		
	}	
	if(check_collision(*exp1, *bomb1) == TRUE || check_collision(*exp2, *bomb1) == TRUE || check_collision(*exp3, *bomb1) == TRUE || check_collision(*exp4, *bomb1) == TRUE){
	
		init_time_bomb1 = 0;
	
	}

	if (SDL_GetTicks() - init_time_bomb1 > 3000){
	
		//add the blocks of the explotion
		
		exp1_1->x = bomb1->x + 45;
		exp1_1->y = bomb1->y;	 	// Right	
		exp1_1->w = fire_R_width;  
		exp1_1->h = fire_height;

		exp1_2->x = bomb1->x - 45;	 //Left
		exp1_2->y = bomb1->y;		 
		exp1_2->w = fire_L_width;  
		exp1_2->h = fire_height;
		
		exp1_3->x = bomb1->x;
		exp1_3->y = bomb1->y + 45;	 //Up
		exp1_3->w = fire_width;  
		exp1_3->h = fire_U_height; 

		exp1_4->x = bomb1->x;
		exp1_4->y = bomb1->y - 45;	 //Down
		exp1_4->w = fire_width; 
		exp1_4->h = fire_D_height;

		bomb1->x = 3000;
		bomb1->y = 3000; 	//the bomb disappears
		bomb1->w = 0;
		bomb1->h = 0;
		
		push_bomb1 = 0;
	}
}


// The blocks of the explotion collision with the destroyable 
void explode_block(game_element_t *player, game_element_t *exp1, game_element_t *exp2, game_element_t *exp3, game_element_t *exp4, game_element_t *exp1_1, game_element_t *exp1_2, game_element_t *exp1_3, game_element_t *exp1_4, game_element_t *Blocks_Destroyer, game_element_t *enemies){
	
	
		for( int i = 1; i < 26; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1, Blocks_Destroyer[i]);
			
			if(TRUE == explotion){
			
				exp1->y = -8000;
				exp1->w = 0;
				exp1->h = 0;
				Blocks_Destroyer[i].x = 3000;
				break; 
			}
		}
		for( int i = 1; i < 4; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1, enemies[i]);
			
			if(TRUE == explotion){
			
				exp1->y = -8000;
				exp1->w = 0;
				exp1->h = 0;
				enemies[i].x =-3000;
				kills_counter += 1;
				score += 1;
				break; 
			}
		}		
		for( int i = 1; i < 26; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp2, Blocks_Destroyer[i]);
			
			if(TRUE == explotion){
			
				exp2->y = -8000;
				exp2->w = 0;
				exp2->h = 0;
				Blocks_Destroyer[i].x = 3000;
				break; 
			}
		}
		for( int i = 1; i < 4; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp2, enemies[i]);
			
			if(TRUE == explotion){
			
				exp2->y = -8000;
				exp2->w = 0;
				exp2->h = 0;
				enemies[i].x = -3000;
				kills_counter += 1;
				score += 1;
				break; 
			}
		}
		for( int i = 1; i < 26; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp3, Blocks_Destroyer[i]);
			
			if(TRUE == explotion){
			
				exp3->y = -8000;
				exp3->w = 0;
				exp3->h = 0;
				Blocks_Destroyer[i].x = 3000;
				break; 
			}
		}
		for( int i = 1; i < 4; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp3, enemies[i]);
			
			if(TRUE == explotion){
			
				exp3->y = -8000;
				exp3->w = 0;
				exp3->h = 0;
				enemies[i].x = -3000;
				kills_counter += 1;
				score += 1;
				break; 
			}
		}
		for( int i = 1; i < 26; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp4, Blocks_Destroyer[i]);
			
			if(TRUE == explotion){
			
				exp4->y = -8000;
				exp4->w = 0;
				exp4->h = 0;
				Blocks_Destroyer[i].x = 3000;
				break; 
			}
		}
		for( int i = 1; i < 4; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp4, enemies[i]);
			
			if(TRUE == explotion){
			
				exp4->y = -8000;
				exp4->w = 0;
				exp4->h = 0;
				enemies[i].x = -3000;
				kills_counter += 1;
				score += 1;
				break; 
			}
		}
				for( int i = 1; i < 25; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1_1, Blocks_Destroyer[i]);
			
			if(TRUE == explotion){
			
				exp1_1->y = -8000;
				exp1_1->w = 0;
				exp1_1->h = 0;
				Blocks_Destroyer[i].x = 3000;
				break; 
			}
		}
		for( int i = 1; i < 4; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1_1, enemies[i]);
			
			if(TRUE == explotion){
			
				exp1_1->y = -8000;
				exp1_1->w = 0;
				exp1_1->h = 0;
				enemies[i].x =-3000;
				kills_counter += 1;
				score += 1;
				break; 
			}
		}		
		for( int i = 1; i < 26; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1_2, Blocks_Destroyer[i]);
			
			if(TRUE == explotion){
			
				exp1_2->y = -8000;
				exp1_2->w = 0;
				exp1_2->h = 0;
				Blocks_Destroyer[i].x = 3000;
				break; 
			}
		}
		for( int i = 1; i < 4; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1_2, enemies[i]);
			
			if(TRUE == explotion){
			
				exp1_2->y = -8000;
				exp1_2->w = 0;
				exp1_2->h = 0;
				enemies[i].x = -3000;
				kills_counter += 1;
				score += 1;
				break; 
			}
		}
		for( int i = 1; i < 26; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1_3, Blocks_Destroyer[i]);
			
			if(TRUE == explotion){
			
				exp1_3->y = -8000;
				exp1_3->w = 0;
				exp1_3->h = 0;
				Blocks_Destroyer[i].x = 3000;
				break; 
			}
		}
		for( int i = 1; i < 4; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1_3, enemies[i]);
			
			if(TRUE == explotion){
			
				exp1_3->y = -8000;
				exp1_3->w = 0;
				exp1_3->h = 0;
				enemies[i].x = -3000;
				kills_counter += 1;
				score += 1;
				break; 
			}
		}
		for( int i = 1; i < 26; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1_4, Blocks_Destroyer[i]);
			
			if(TRUE == explotion){
			
				exp1_4->y = -8000;
				exp1_4->w = 0;
				exp1_4->h = 0;
				Blocks_Destroyer[i].x = 3000;
				break; 
			}
		}
		for( int i = 1; i < 4; i++){
		
			int explotion = FALSE;		
			explotion =  check_collision(*exp1_4, enemies[i]);
			
			if(TRUE == explotion){
			
				exp1_4->y = -8000;
				exp1_4->w = 0;
				exp1_4->h = 0;
				enemies[i].x = -3000;
				kills_counter += 1;
				score += 1;
				break; 
			}
		}
}


// Cretaed the block of the skate power up
void power_up_skate(game_element_t *skate,  game_element_t *blocks_destroyer){
	
	skate->x = blocks_destroyer[11].x;
	skate->y = blocks_destroyer[11].y;
	skate->w = 80;
	skate->h = 64; 
	
}
// Cretaed the block of the more bombs power up
void power_up_bombs(game_element_t *morebombs, game_element_t *blocks_destroyer){

	morebombs->x = blocks_destroyer[12].x;
	morebombs->y = blocks_destroyer[12].y;
	morebombs->w = 80;
	morebombs->h = 64;

}
// Cretaed the block of the fire power up
void power_up_fire(game_element_t *fire, game_element_t *blocks_destroyer){

	fire->x = blocks_destroyer[13].x;
	fire->y = blocks_destroyer[13].y;
	fire->w = 80;
	fire->h = 64;

}

/* Function: draw_game_over
 * ----------------------------
 * Currently this function is not being used but it will function
 * when the developed game requires a game over screen.
 *
 * Arguments: 
 *	none
 *
 * Return:
 *
 *	void.
 */
static void draw_game_over() { 

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = gameover->w;
	src.h = gameover->h;

	dest.x = (screen->w / 2) - (src.w/ 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = gameover->w;
	dest.h = gameover->h;
	
	SDL_BlitSurface(gameover, &src, screen, &dest);
	
}
void display_score() {
    // Código para mostrar el puntaje en la pantalla, por ejemplo:
    printf("Puntaje: %d\n", score);
}
static void draw_water(){
    SDL_Rect src;
    SDL_Rect dest;
    
  
    src.x = 0;
    src.y = 0;
    src.w = 100;
    src.h = 86;
    
    // Calcula la cantidad de filas y columnas segun el tamaño del pasto de de la pantalla
    int rows = 1000/ src.h;
    int cols = 1500 / src.w;
    
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {

            dest.x = x * src.w;
            dest.y = y * src.h;
            dest.w = src.w;
            dest.h = src.h;
            

            SDL_BlitSurface(bmpwater, &src, screen, &dest);
        }
    }
}
static void draw_lava(){
    SDL_Rect src;
    SDL_Rect dest;
    
  
    src.x = 0;
    src.y = 0;
    src.w = 110;
    src.h = 90;
    
    // Calcula la cantidad de filas y columnas segun el tamaño del pasto de de la pantalla
    int rows = 1000/ src.h;
    int cols = 1500 / src.w;
    
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {

            dest.x = x * src.w;
            dest.y = y * src.h;
            dest.w = src.w;
            dest.h = src.h;
            

            SDL_BlitSurface(bmplava, &src, screen, &dest);
        }
    }
}
static void draw_grass(){
    SDL_Rect src;
    SDL_Rect dest;
    
  
    src.x = 0;
    src.y = 0;
    src.w = 100;
    src.h = 86;
    
    // Calcula la cantidad de filas y columnas segun el tamaño del pasto de de la pantalla
    int rows = 1000/ src.h;
    int cols = 1500 / src.w;
    
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {

            dest.x = x * src.w;
            dest.y = y * src.h;
            dest.w = src.w;
            dest.h = src.h;
            

            SDL_BlitSurface(grass, &src, screen, &dest);
        }
    }
}


// This function draw the heart
static void draw_heart(){
	
	SDL_Rect src;
	SDL_Rect dest;					
	
	src.x = 0;
	src.y = 0;
	src.w = 400;
	src.h = 380;

	dest.x = (screen->w / 2) - 130;
	dest.y = 10;
	dest.w = 400;
	dest.h = 450;

	SDL_BlitSurface(heart, &src, screen, &dest);
}

/* Function: draw_menu
 * --------------------------------
 * This function is in charge of drawing the first screen that is
 * presented to the player.
 *
 * Arguments:
 *	none
 *
 * Return:
 *	void.
 */
static void draw_menu() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = title->w;
	src.h = title->h;

	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = title->w;
	dest.h = title->h;

	SDL_BlitSurface(title, &src, screen, &dest);
}
static void draw_win(){

	SDL_Rect src;
	SDL_Rect dest;
	
	src.x = 0;
	src.y = 0;
	src.w = bmpwin->w;
	src.h = bmpwin->h;
	
	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = bmpwin->w;
	dest.h = bmpwin->h;
	
	SDL_BlitSurface(bmpwin, &src, screen, &dest);
	
}
static void draw_next_level(){

	SDL_Rect src;
	SDL_Rect dest;
	
	src.x = 0;
	src.y = 0;
	src.w = bmpnext->w;
	src.h = bmpnext->h;
	
	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = bmpnext->w;
	dest.h = bmpnext->h;
	
	SDL_BlitSurface(bmpnext, &src, screen, &dest);
	
}
static void draw_game_element_Walls(game_element_t *element) {
	
	SDL_Rect src;
	int i;

	for (i = 1; i < 2; i++) {
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
	
		int r = SDL_FillRect(screen, &src, 0x40FFFFFF);   //BLUE : FF880000
								//PINK : FFFF88FF
	}
}

/* Function: draw_game_element
 * -------------------------------
 * This functions draws a game element on the screen according to its
 * properties and coordinates. Please note that the element to be 
 * drawn is passed as an reference insted of a value.
 *
 * Arguments:
 *	element: Element to be drawn on the screen.
 *
 * Return:
 *	void.
 */
static void draw_game_element_Bomb(game_element_t *element) {

	SDL_Rect src;
	SDL_Rect dest;
	int i;

	for (i = 0; i < 2; i++) {
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = element->w;
		dest.h = element->h;
		
	
		SDL_BlitSurface(bmpBomba, &dest, screen, &src);
		
	}
}
static void draw_game_element_Explot(game_element_t *element) {

	SDL_Rect src;
	SDL_Rect dest;
	int i;

	for (i = 0; i < 2; i++) {
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = element->w;
		dest.h = element->h;
		
	
		SDL_BlitSurface(bmpExplot, &dest, screen, &src);
		
	}
}
static void draw_game_element_Destroyer(game_element_t *elements) {
    
        SDL_Rect src;
        SDL_Rect dest;
        
        
		for (int i = 0; i < 2; i++) {
        src.x = 0;
        src.y = 0;
        src.w = elements->w;
        src.h = elements->h;

        dest.x = elements->x;
        dest.y = elements->y;
        dest.w = elements->w;
        dest.h = elements->h;

        SDL_BlitSurface(bmpDestruible, &src, screen, &dest);
    }
}
static void draw_game_element(game_element_t *element) {

	SDL_Rect src;
	SDL_Rect dest;
	int i;

	for (i = 0; i < 2; i++) {
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = element->w;
		dest.h = element->h;
		
	
		SDL_BlitSurface(bmpSolido, &dest, screen, &src);
		
	}
}
 static void draw_game_jugador(game_element_t *element, int dire) {

	SDL_Rect src; 
	SDL_Rect dest;
	
	dest.x = 270;
	dest.y = 15;
	dest.w = 83;
	dest.h = 84;

	for (int  i= 0; i < 2; i++) {
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
		
	
		if (dire == LEFT){
			dest.x = 380;
		}
		
		if (dire == RIGHT){
			dest.x = 160;
		}
		if ( dire == UP){
			dest.x = 35;
		}
		if (dire == DOWN){
			dest.x = 270;
		}
		
	}
	
	SDL_BlitSurface(jugador, &dest, screen, &src);
	
}
 static void draw_game_enemies(game_element_t *element) {

	srand(time(0));
	
	int move = rand() % 4;
	
	SDL_Rect src; 
	SDL_Rect dest;
	
	dest.x = 270;
	dest.y = 15;
	dest.w = 55;
	dest.h = 63;
	
	src.x = element->x;
	src.y = element->y;
	src.w = element->w;
	src.h = element->h;
	
	
		if (move == LEFT){
			dest.x = 175;
		}
		
		if (move == RIGHT){
			dest.x = 61;
		}
		if (move == UP){
			dest.x = 19;
		}
		if (move == DOWN){
			dest.x = 130;
		}
	SDL_BlitSurface(bmpEnemy, &dest, screen, &src);
}
static void draw_game_enemies_1(game_element_t *element) {

	srand(time(0));
	
	int move = rand() % 3;
	
	SDL_Rect src; 
	SDL_Rect dest;
	
	dest.x = 270;
	dest.y = 15;
	dest.w = 55;
	dest.h = 63;

	
	src.x = element->x;
	src.y = element->y;
	src.w = element->w;
	src.h = element->h;

		if (move == LEFT){
			dest.x = 175;
		}
		
		if (move == RIGHT){
			dest.x = 61;
		}
		if (move == UP){
			dest.x = 19;
		}
		if (move == DOWN){
			dest.x = 130;
		}
		
	SDL_BlitSurface(bmpEnemy, &dest, screen, &src);
}
static void draw_game_enemies_2(game_element_t *element) {

	srand(time(0));
	
	int move = rand() % 3;
	
	SDL_Rect src; 
	SDL_Rect dest;
	
	dest.x = 270;
	dest.y = 15;
	dest.w = 55;
	dest.h = 63;
	
	src.x = element->x;
	src.y = element->y;
	src.w = element->w;
	src.h = element->h;
	
	
		if (move == LEFT){
			dest.x = 175;
		}
		
		if (move == RIGHT){
			dest.x = 61;
		}
		if (move == UP){
			dest.x = 19;
		}
		if (move == DOWN){
			dest.x = 130;
		}
		
	SDL_BlitSurface(bmpEnemy, &dest, screen, &src);
}

static void draw_game_element_Door(game_element_t *element) {

	SDL_Rect src;
	SDL_Rect dest;
	int i;

	for (i = 0; i < 2; i++) {
	
		src.x = element->x;
		src.y = element->y;
		src.w = element->w;
		src.h = element->h;
		
		dest.x = 0;
		dest.y = 0;
		dest.w = element->w;
		dest.h = element->h;
		
	
		SDL_BlitSurface(bmpDoor, &dest, screen, &src);
		
	}
}

static void draw_numbers_score(int score) {

	SDL_Rect src; 
	SDL_Rect dest;
	
	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;
	
	dest.x =(screen->w / 2) - src.w ;
	dest.y = 0;
	dest.w = 65;
	dest.h = 64;
	
	
	if(score == 9){
		
		src.x = src.w * (score);
		
	}else if (score == 8){
	
		src.x = src.w * (score);
		
	}else if (score == 7){
	
		src.x = src.w * (score);
		
	}else if(score == 6){
		
		src.x = src.w * (score);
		
	}else if (score == 5){
	
		src.x = src.w * (score);
		
	}else if (score == 4){
	
		src.x = src.w * (score);
			
	}else if ( score == 3){
	
		src.x = src.w * (score);
		
	}else if (score == 2){
	
		src.x = src.w * (score);
		
	}else if (score == 1){
	
		src.x = src.w * (score);	
	}else {
		src.x = 0;
	}
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);	
}



 static void draw_numbers_life() {

	SDL_Rect src; 
	SDL_Rect dest;
	
	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;
	
	dest.x = ( screen->w / 2)- 500;
	dest.y = 0;
	dest.w = 65;
	dest.h = 64;
	
	if (life == 5){
	
		src.x = 320;
		
	}if (life == 4){
	
		src.x = 256;
			
	}if ( life == 3){
	
		src.x = 192;
		
	}if (life == 2){
	
		src.x = 128;
		
	}if (life == 1){
	
		src.x = 64;	
		
	}if(life == 0) {
	
		src.x = 0;
	}
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);	
}


// The header of this two functions are for the student to complete
// Try changing the coordinates to see the effect that has on the game
// Also, are here the magic numbers justified? Or should be declared
// as constants in the beggining of the file?
static void draw_game_element_0_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) - src.w - 30; //12 is just padding spacing
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;

	if (g_score[0] > 0 && g_score[0] < 10) {
		
		src.x += src.w * g_score[0];
	}
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

static void draw_game_element_1_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) + 12;
	dest.y = 0;
	dest.w = 64; 
	dest.h = 64;
	
	if (g_score[1] > 0 && g_score[1] < 10) {
		
		src.x += src.w * g_score[1];
	}

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

//Draw the units of the timer 
static void draw_game_element_0_time(int temp) { 
	
	SDL_Rect src;
	SDL_Rect dest;
	
	timer[0] = temp % 10; 

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) - 200; //12 is just padding spacing
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;

	
	src.x += src.w * timer[0];
		
	
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

//Draw the tens of the timer
static void draw_game_element_1_time(int temp) { 
	
	SDL_Rect src;
	SDL_Rect dest;
	
	timer[1] = (temp / 10) % 10;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) - 250; //12 is just padding spacing
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;

	src.x += src.w * timer[1];
	
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

//Draw the hundreds of the timer 
static void draw_game_element_2_time(int temp) { 
	
	SDL_Rect src;
	SDL_Rect dest;
	
	timer[2] = (temp / 100) % 10;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) - 300; //12 is just padding spacing
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;
	
	
	src.x += src.w * timer[2];
	
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);
}






// Main function

int main (int argc, char *args[]) {
	
	int FRONT;
	int front;
	int front_c;
	



	
	//Declaracion arreglo de bloques fijos y destruibles
	game_element_t map_element[28];
	game_element_t Blocks_Destroyer[25];
	

	//Declaraciòn de la funciòn malloc
	game_element_t *enemies = (game_element_t *)malloc(ENEMIES*sizeof(game_element_t));
	
	// Define the player and the maps
	game_element_t player;
        
        //bomb
        game_element_t bomb;
        game_element_t bomb1;

        //explotion
        game_element_t exp1;
        game_element_t exp2;
        game_element_t exp3;
        game_element_t exp4;
        
        game_element_t exp1_1;
        game_element_t exp1_2;
        game_element_t exp1_3;
        game_element_t exp1_4;

        //power ups
        game_element_t skate;
        game_element_t morebombs;
        game_element_t fire;

        //Portal
	game_element_t Door;
        

        
	//SDL Window setup
	if (init_SDL(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == FAILURE) {
		
		return FAILURE;
	}
	
	SDL_GetWindowSize(window, &g_width, &g_height);
	
	int sleep = 0;
	int quit = FALSE;
	int state = START_SCREEN;

	Uint32 next_game_tick = SDL_GetTicks();
	
	// Initialize the ball position data. 
	init_game(&player, map_element, Blocks_Destroyer, enemies, &Door); 
	
	
	power_up_skate(&skate, Blocks_Destroyer);
	power_up_bombs(&morebombs, Blocks_Destroyer); 
	power_up_fire(&fire, Blocks_Destroyer);

	
	//render loop
	while(quit == FALSE) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		
		if (keystate[SDL_SCANCODE_ESCAPE]) {
		
			quit = TRUE;
		}
		
		if (keystate[SDL_SCANCODE_S]) {
			
			move_player(DOWN, &player, &skate, map_element, Blocks_Destroyer);
			
		FRONT = DOWN; 
		}
		

		if (keystate[SDL_SCANCODE_W]) {
			
			move_player(UP, &player, &skate, map_element, Blocks_Destroyer);
				
		FRONT = UP;
		}

		
		if (keystate[SDL_SCANCODE_A]) {
			
			move_player(LEFT, &player, &skate, map_element, Blocks_Destroyer);
		FRONT = LEFT;
		}
		

		if (keystate[SDL_SCANCODE_D]) {
			
			move_player(RIGHT, &player, &skate, map_element, Blocks_Destroyer);
			
		FRONT = RIGHT;
		
		}
		
		if(check_collision(player, morebombs) == TRUE){
		
			power_up_active = true;
				
			morebombs.x = -5000;
			morebombs.w = 0;
			morebombs.h = 0;
			
		}	

		 if(keystate[SDL_SCANCODE_B] && push_bomb == 0 ){
		
			init_time_bomb = SDL_GetTicks();
			push_bomb = 1;
			create_bomb(&player, &bomb);
			
			init_time_1 = SDL_GetTicks();
				
		}
		
		//Logic to place more bombs when the power up is active
		
		if (SDL_GetTicks() - init_time_1 > 850){
		
		
			if(power_up_active == true && keystate[SDL_SCANCODE_B] && push_bomb == 1 && push_bomb1 == 0){
			
				init_time_bomb1 = SDL_GetTicks();
				push_bomb1 = 1; 
				create_bomb1(&player, &bomb1);
				
			}	

		}
	
		die_player(&player, &exp1, &exp2, &exp3, &exp4, &exp1_1, &exp1_2, &exp1_3, &exp1_4, enemies);
		explode_bomb(&player, &fire, &bomb, &exp1, &exp2, &exp3, &exp4, &exp1_1, &exp1_2, &exp1_3, &exp1_4);
		explode_bomb1(&player, &fire, &bomb1, &exp1_1, &exp1_2, &exp1_3, &exp1_4, &exp1, &exp2, &exp3, &exp4);
		explode_block(&player, &exp1, &exp2, &exp3, &exp4, &exp1_1, &exp1_2, &exp1_3, &exp1_4, Blocks_Destroyer, enemies);

		//draw background
		SDL_RenderClear(renderer);
		SDL_FillRect(screen, NULL, BLACK);
		
		//display main menu
		if (state == START_SCREEN ) {
		
			if (keystate[SDL_SCANCODE_SPACE]) {
				state = LEVEL_1;
				life = 3;
				kills_counter = 1;
				init_game(&player, map_element, Blocks_Destroyer, enemies, &Door);
				
           		 }
		//draw menu 
			draw_menu();
		
		//display gameover
		} else if (state == GAME_OVER) {
		
			if (keystate[SDL_SCANCODE_SPACE]) {
			if(init_time == 0){
			
				init_time = SDL_GetTicks();
			
			}
			
			game_time = SDL_GetTicks();		//Timer
			
			if((game_time - init_time) >= 1000){
			
				temp --;
				init_time = game_time;
			}

				state = START_SCREEN;
				score = 0;
				// delay for a little bit so the space bar press doesnt get 
				// triggered twice
				// while the main menu is showing
				
            			SDL_Delay(INPUT_DELAY_MS);
			}

				draw_game_over();
				
		//display the game
		} else if (state == LEVEL_1) {
		
		
		move_enemies(&player, map_element, Blocks_Destroyer, enemies, &bomb);
		
		if(init_time == 0){
		
			init_time = SDL_GetTicks();
		
		}
		
		game_time = SDL_GetTicks();		//Timer
		
		if((game_time - init_time) >= 1000){
		
			temp --;
			init_time = game_time;
		}

		//Suelo
		draw_grass();
		
		//Portal
		draw_game_element_Door(&Door);
		

		
		// Here we draw the player that we move across 
		draw_game_jugador(&player, FRONT);
		

		// We draw the map element that is going to be static
	
		draw_game_element_Walls(&map_element[1]);
		draw_game_element_Walls(&map_element[2]);
		draw_game_element_Walls(&map_element[3]);
		draw_game_element_Walls(&map_element[4]);
	
		draw_game_element(&map_element[5]);
		draw_game_element(&map_element[6]);
		draw_game_element(&map_element[7]);
		draw_game_element(&map_element[8]);
		draw_game_element(&map_element[9]);
		draw_game_element(&map_element[10]);
		draw_game_element(&map_element[11]);
		draw_game_element(&map_element[12]);
		draw_game_element(&map_element[13]);
		draw_game_element(&map_element[14]);
		draw_game_element(&map_element[15]);
		draw_game_element(&map_element[16]);
		draw_game_element(&map_element[17]);
		draw_game_element(&map_element[18]);
		draw_game_element(&map_element[19]);
		draw_game_element(&map_element[20]);
		draw_game_element(&map_element[21]);
		draw_game_element(&map_element[22]);
		draw_game_element(&map_element[23]);
		draw_game_element(&map_element[24]);
		draw_game_element(&map_element[25]);
		draw_game_element(&map_element[26]);
		draw_game_element(&map_element[27]);
		draw_game_element(&map_element[28]);
	
		//Draw power ups
		draw_game_element(&skate);
		draw_game_element(&morebombs);
		
		//destructibles
		for(int i = 0; i <= 25; i++){
		draw_game_element_Destroyer(&Blocks_Destroyer[i]);
		}

	
		// enemies
		draw_game_enemies(&enemies[1]);
		draw_game_enemies_1(&enemies[2]);
		draw_game_enemies_2(&enemies[3]);

		//draw the bomb
		draw_game_element_Bomb(&bomb);
		draw_game_element_Bomb(&bomb1);
		

		//draw the explotion 
		draw_game_element_Explot(&exp1);
		draw_game_element_Explot(&exp2);
		draw_game_element_Explot(&exp3);
		draw_game_element_Explot(&exp4);

		//Draw lifes
		draw_numbers_life();

		//Draw Heart
		draw_heart();
		//score
		draw_numbers_score(score);
		display_score();
		//draw the score
		//draw_game_element_0_score();

		//draw the score
		draw_game_element_1_score();
		
		//draw timer
		draw_game_element_0_time(temp);
		draw_game_element_1_time(temp);
		draw_game_element_2_time(temp);

		if (life == 0) {
			state = GAME_OVER;
		    } 
			if (kills_counter == 4) {
			    if (check_collision(player, Door)==TRUE) {
				if (keystate[SDL_SCANCODE_SPACE]) {
				state = LEVEL_2;
				life = 3;
				kills_counter = 1;
				init_game(&player, map_element, Blocks_Destroyer, enemies, &Door);	
           			 }
           			 draw_next_level();
			    }
			}	
			
		} else if (state == LEVEL_2) {
			
			
			move_enemies(&player, map_element, Blocks_Destroyer, enemies, &bomb);
			if(init_time == 0){
			
				init_time = SDL_GetTicks();
			
			}
			
			game_time = SDL_GetTicks();		//Timer
			
			if((game_time - init_time) >= 1000){
			
				temp --;
				init_time = game_time;
			}

			//score
			draw_numbers_score(score);
			
			//Suelo
			
			draw_water();
			
			//Portal
			draw_game_element_Door(&Door);
			

			
			// Here we draw the player that we move across 
			draw_game_jugador(&player, FRONT);
			

			// We draw the map element that is going to be static
		
			draw_game_element_Walls(&map_element[1]);
			draw_game_element_Walls(&map_element[2]);
			draw_game_element_Walls(&map_element[3]);
			draw_game_element_Walls(&map_element[4]);
		
			draw_game_element(&map_element[5]);
			draw_game_element(&map_element[6]);
			draw_game_element(&map_element[7]);
			draw_game_element(&map_element[8]);
			draw_game_element(&map_element[9]);
			draw_game_element(&map_element[10]);
			draw_game_element(&map_element[11]);
			draw_game_element(&map_element[12]);
			draw_game_element(&map_element[13]);
			draw_game_element(&map_element[14]);
			draw_game_element(&map_element[15]);
			draw_game_element(&map_element[16]);
			draw_game_element(&map_element[17]);
			draw_game_element(&map_element[18]);
			draw_game_element(&map_element[19]);
			draw_game_element(&map_element[20]);
			draw_game_element(&map_element[21]);
			draw_game_element(&map_element[22]);
			draw_game_element(&map_element[23]);
			draw_game_element(&map_element[24]);
			draw_game_element(&map_element[25]);
			draw_game_element(&map_element[26]);
			draw_game_element(&map_element[27]);
			draw_game_element(&map_element[28]);
			
			//Draw power ups
			draw_game_element(&skate);
			draw_game_element(&morebombs);

			//destructibles
			for(int i = 0; i <= 25; i++){
  			draw_game_element_Destroyer(&Blocks_Destroyer[i]);
			}

			
			// enemies
			draw_game_enemies(&enemies[1]);
			draw_game_enemies_1(&enemies[2]);
			draw_game_enemies_2(&enemies[3]);

		
			//draw the bomb
			draw_game_element_Bomb(&bomb);
			draw_game_element_Bomb(&bomb1);
			
			
			//draw the explotion 
			draw_game_element_Explot(&exp1);
			draw_game_element_Explot(&exp2);
			draw_game_element_Explot(&exp3);
			draw_game_element_Explot(&exp4);
			
			draw_game_element_Explot(&exp1_1);
			draw_game_element_Explot(&exp1_2);
			draw_game_element_Explot(&exp1_3);
			draw_game_element_Explot(&exp1_4);

	
			//Draw lifes
			draw_numbers_life();
	
			//Draw Heart
			draw_heart();
			//score
			draw_numbers_score(score);
			display_score();
			
			//draw the score
			//draw_game_element_0_score();
	
			//draw the score
			draw_game_element_1_score();
			
			//draw timer
			draw_game_element_0_time(temp);
			draw_game_element_1_time(temp);
			draw_game_element_2_time(temp);

			
			if (life == 0) {
				state = GAME_OVER;
			} else if (kills_counter >= 4) {
			      if (check_collision(player,Door)==TRUE) {
			      	  if (keystate[SDL_SCANCODE_SPACE]) {
				 	state = LEVEL_3;
					life = 3;
					kills_counter = 1;
					init_game(&player, map_element, Blocks_Destroyer, enemies, &Door);	
           			 }
				draw_next_level();
			  }
		    }
			
		} else if (state == LEVEL_3) {
			
			move_enemies(&player, map_element, Blocks_Destroyer, enemies, &bomb);
			
			if(init_time == 0){
			
				init_time = SDL_GetTicks();
			
			}
			
			game_time = SDL_GetTicks();		//Timer
			
			if((game_time - init_time) >= 1000){
			
				temp --;
				init_time = game_time;
			}

			//Suelo
			
			draw_lava();
			
			//Portal
			draw_game_element_Door(&Door);
			

			
			// Here we draw the player that we move across 
			draw_game_jugador(&player, FRONT);
			

			// We draw the map element that is going to be static
		
			draw_game_element_Walls(&map_element[1]);
			draw_game_element_Walls(&map_element[2]);
			draw_game_element_Walls(&map_element[3]);
			draw_game_element_Walls(&map_element[4]);
		
			draw_game_element(&map_element[5]);
			draw_game_element(&map_element[6]);
			draw_game_element(&map_element[7]);
			draw_game_element(&map_element[8]);
			draw_game_element(&map_element[9]);
			draw_game_element(&map_element[10]);
			draw_game_element(&map_element[11]);
			draw_game_element(&map_element[12]);
			draw_game_element(&map_element[13]);
			draw_game_element(&map_element[14]);
			draw_game_element(&map_element[15]);
			draw_game_element(&map_element[16]);
			draw_game_element(&map_element[17]);
			draw_game_element(&map_element[18]);
			draw_game_element(&map_element[19]);
			draw_game_element(&map_element[20]);
			draw_game_element(&map_element[21]);
			draw_game_element(&map_element[22]);
			draw_game_element(&map_element[23]);
			draw_game_element(&map_element[24]);
			draw_game_element(&map_element[25]);
			draw_game_element(&map_element[26]);
			draw_game_element(&map_element[27]);
			draw_game_element(&map_element[28]);
			
			//Draw power ups
			draw_game_element(&skate);
			draw_game_element(&morebombs);
			
			//destructibles	
			for(int i = 0; i <= 25; i++){
  			draw_game_element_Destroyer(&Blocks_Destroyer[i]);
			}
			
			// enemies
			draw_game_enemies(&enemies[1]);
			draw_game_enemies_1(&enemies[2]);
			draw_game_enemies_2(&enemies[3]);

	
			//draw the bomb
			draw_game_element_Bomb(&bomb);
			draw_game_element_Bomb(&bomb1);
			
			//draw the explotion 
			draw_game_element_Explot(&exp1);
			draw_game_element_Explot(&exp2);
			draw_game_element_Explot(&exp3);
			draw_game_element_Explot(&exp4);
	
			//Draw lifes
			draw_numbers_life();
	
			//Draw Heart
			draw_heart();

			//score
			draw_numbers_score(score);
			display_score();
			
			//draw the score
			//draw_game_element_0_score();
	
			//draw the score
			draw_game_element_1_score();
			
			//draw timer
			draw_game_element_0_time(temp);
			draw_game_element_1_time(temp);
			draw_game_element_2_time(temp);

			
			if (life == 0) {
				state = GAME_OVER;
			} else if (kills_counter >= 4) {
			      if (check_collision(player,Door)==TRUE) {
					if (keystate[SDL_SCANCODE_SPACE]) {
					state = START_SCREEN;	
           			 	}
           			 	draw_win();
				}

			    }
			
		}
	
		SDL_UpdateTexture(screen_texture, NULL, screen->pixels, 
						  screen->w * sizeof (Uint32));
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);

		//draw to the display
		SDL_RenderPresent(renderer);
				
		//time it takes to render  frame in milliseconds
		next_game_tick += FRAME_TIME_MS;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free loaded images
	SDL_FreeSurface(screen);
	SDL_FreeSurface(title);
	SDL_FreeSurface(bmpDestruible);
	SDL_FreeSurface(bmpSolido);
	SDL_FreeSurface(bmpDoor);
	SDL_FreeSurface(bmpEnemy);
	SDL_FreeSurface(bmpBomba);
	SDL_FreeSurface(bmpExplot);
	SDL_FreeSurface(jugador);
	SDL_FreeSurface(grass);
	SDL_FreeSurface(bmplava);
	SDL_FreeSurface(heart);
	SDL_FreeSurface(numbermap);
	SDL_FreeSurface(gameover);
	SDL_FreeSurface(bmpwin);
	SDL_FreeSurface(bmpwater);
	SDL_FreeSurface(bmpnext);

	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);
	
	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	return SUCCESS;
}

/* Function: init_SDL
 * ----------------------------
 * This function initialises the required environment for SDL
 * to work.
 *
 * Arguments:
 *	width: The width of the screen.
 * 	height: The height of the screen.
 *	argc: The same parameter from the main function.
 *	args: The same parameter from the main function.
 *
 * Return:
 *	SUCCESS if no problem reported.
 *	FAILURE if any initialization failed
 */
int init_SDL(int width, int height, int argc, char *args[]) {


	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return FAILURE;
	} 
	
	int i;
	
	for (i = 0; i < argc; i++) {
		
		//Create window	
		if(strcmp(args[i], "-f")) {
			
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, 
										SCREEN_HEIGHT, 
										SDL_WINDOW_SHOWN, 
										&window, 
										&renderer);
		
		} else {
		
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, 
										SCREEN_HEIGHT, 
										SDL_WINDOW_FULLSCREEN_DESKTOP, 
										&window, 
										&renderer);
		}
	}

	if (window == NULL) { 
		
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return FAILURE;
	}

	// Create the screen surface where all the elements will be drawn
	screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, 
											SDL_PIXELFORMAT_RGBA32);
	
	if (screen == NULL) {
		
		printf("Could not create the screen surfce! SDL_Error: %s\n", 
			   SDL_GetError());

		return FAILURE;
	}


	// Create the screen texture to render the screen surface to the actual 
	// display
	screen_texture = SDL_CreateTextureFromSurface(renderer, screen);

	if (screen_texture == NULL) {
		
		printf("Could not create the screen_texture! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}

	//Load the title image
	title = SDL_LoadBMP("title.bmp");

	if (title == NULL) {
		
		printf("Could not Load title image! SDL_Error: %s\n", SDL_GetError());

		return FAILURE;
	}
	
	//Imagenes
bmpDoor = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmpDoor.bmp");
bmpEnemy = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmpEnemy.bmp");
bmpDestruible = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmpDestruible.bmp");
bmpBomba = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmpBomba.bmp");
bmpExplot = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmpExplot.bmp");
bmpSolido = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmpSolido.bmp");
jugador = SDL_LoadBMP("/data/data/com.termux/files/home/p1/jugador.bmp");
grass = SDL_LoadBMP("/data/data/com.termux/files/home/p1/grass.bmp");
bmplava = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmplava.bmp");
bmpwater = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmpwater.bmp");
heart = SDL_LoadBMP("/data/data/com.termux/files/home/p1/heart.bmp");
bmpnext = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmpnext.bmp");
gameover = SDL_LoadBMP("/data/data/com.termux/files/home/p1/gameover.bmp");
bmpwin = SDL_LoadBMP("/data/data/com.termux/files/home/p1/bmpwin.bmp");
numbermap = SDL_LoadBMP("/data/data/com.termux/files/home/p1/numbermap.bmp");

	
	//Load the numbermap image
	numbermap = SDL_LoadBMP("numbermap.bmp");

	if (numbermap == NULL) {
		
		printf("Could not Load numbermap image! SDL_Error: %s\n", 
				SDL_GetError());

		return FAILURE;
	}
	


	
	// Set the title colourkey. 
	Uint32 colorkey = SDL_MapRGB(title->format, 255, 0, 255);
	SDL_SetColorKey(title, SDL_TRUE, colorkey);
	SDL_SetColorKey(numbermap, SDL_TRUE, colorkey);
	SDL_SetColorKey(grass, SDL_TRUE, colorkey);
	SDL_SetColorKey(bmplava, SDL_TRUE, colorkey);
	SDL_SetColorKey(heart, SDL_TRUE, colorkey);
	return SUCCESS;
}



