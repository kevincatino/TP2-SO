// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>

#define GENERAL_SCREEN 0
#define GAMES_SCREEN 5
#define GAME_SCREEN_SL 1
#define GAME_SCREEN_SR 2
#define GAME_SCREEN_IL 3
#define GAME_SCREEN_IR 4
#define GAME_SCREEN_DELETE_S 2		// Cantidad de espacio que hay para escribir dentro de cada pantalla 
#define GAME_SCREEN_DELETE_G 4
#define GAME_SCREEN_DELETE_I 17
#define IS_LEFT_SCREEN 1
#define IS_RIGHT_SCREEN 0



static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;

static const uint32_t width = 80;
static const uint32_t height = 25 ;
static int screenInUse = 0;
static int cursor = 0;

 
void displayCursor() {
	if (!cursor) {
		*currentVideo = '|';
		cursor = 1;
	}
	else 
		{
			*currentVideo = ' ';
			cursor = 0;
		}
}


void ncPrint(const char * string) {
	if (cursor) {

	}
	int i;
	for (i = 0; string[i] != 0; i++) {
		ncPrintChar(string[i]);
	}
}



void ncPrintChar(char character) {
	*currentVideo = character;
	currentVideo += 2;
}



void ncClear() {
	int i;
	for (i = 0; i < height * width; i++) {
		video[i * 2] = ' ';
		video[i * 2 + 1] = 0x00;

	}
	currentVideo = video;
}



void ncNewline() {
	do {
		ncCheckTerminalPosition(); 
		ncPrintChar(' ');
	} while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}



void ncPrintDec(uint64_t value) {
	ncPrintBase(value, 10);
}



void ncPrintHex(uint64_t value) {
	ncPrintBase(value, 16);
}



void ncPrintBin(uint64_t value) {
	ncPrintBase(value, 2);
}



void ncPrintBase(uint64_t value, uint32_t base) {
    uintToBase(value, buffer, base);
    ncPrintStringColour(buffer, WHITE);
}



static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) {
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;
	
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	*p = 0;

	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
} 

// --------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------


void ncPrintCharColour(char c, int colour){ 
	if(c == '\0') { 
		return;
	}

	ncCheckTerminalPosition(); 		// va a chequear si tengo lugar para escrbir, sino llama a scrollUp
	uint8_t * auxCurrent = currentVideo;

	if(c == '\n'){
		ncNewline();
		return;
	} else if(c == '\b'){
		ncDelete();
		return;
	} else if(c == '\t') {			
		*auxCurrent = ' ';
		*(auxCurrent +1) = 0x00;
		auxCurrent += 2;
	} else {
		*auxCurrent = c;
		*(auxCurrent +1) = colour;
		auxCurrent += 2;
	}
	currentVideo = auxCurrent;
}



void ncPrintCharGames(char c, int colour){ 
	if(c == '\0') { 
		return;
	}

	uint8_t * auxCurrent = currentVideo;
	*auxCurrent = c;
	*(auxCurrent +1) = colour;
	auxCurrent += 2;
	currentVideo = auxCurrent;
}



void ncPrintStringColour(char * message, int colour) {
    while (*message != '\0') {             
		ncPrintCharColour(*message, colour);
        message++;
    }
}

// Division de pantalla
void ncScreenDivition() {
	ncClearScreen();
	ncPrintHeight();
	ncPrintWidth();
}

void ncPrintHeight() {	// PARA MODIFICAR SOLO CAMBIAR EL screen_enters
	int screen_enters = 4;
	uint8_t * screenHeight = video;
	screenHeight += (width*2*screen_enters);	

	for(int i = 0; i < width; i++) {
		*screenHeight = '-';
		*(screenHeight+1) = LIGHT_BLUE;
		screenHeight += 2;
	}

	screen_enters = 2;
	screenHeight += (width*2*screen_enters);
	for(int i = 0; i < width; i++) {
		*screenHeight = '-';
		*(screenHeight+1) = LIGHT_BLUE;
		screenHeight += 2;
	}
}

void ncPrintWidth() {		// PARA MODIFICAR SOLO CAMBIAR EL screen_spaces
	int screen_spaces = 5;
	uint8_t * screenWidth = video;
	screenWidth += (width + (width*2)*screen_spaces);		
	for(int i = 0; i < height; i++) {
		*screenWidth = '|';
		*(screenWidth+1) = LIGHT_BLUE;
		screenWidth += (width*2);
	}
}



void ncPrintSudokuNumbers(char* line, uint64_t position, uint64_t orientationX) {
	uint8_t * auxCurrentVideo = currentVideo;	// Creo un aux para no perder el current
	currentVideo = video;
	currentVideo += position;		// Como current ahora vale lo de video, le sumo directo la posicion
	
	if(orientationX == 1) {
		while (*line != 0) {
			ncPrintCharGames(*line, WHITE);
			line++;
		}
	} else {
		int flag = 0;
		while (*line != 0) {
			if (flag == 4) {
				currentVideo += ((80*2)-(4*2));
				flag = 0;
			}
			ncPrintCharGames(*line, WHITE);
			line++;
			flag++;
		}
	}

	currentVideo = auxCurrentVideo;	// Restablezco el valor de current
}



void ncPrintInScreenPosition(char* line, uint64_t position) {
	uint8_t * auxCurrentVideo = currentVideo;	// Creo un aux para no perder el current
	currentVideo = video;
	currentVideo += position;		// Como current ahora vale lo de video, le sumo directo la posicion
	
	while (*line != 0) {
		ncPrintCharGames(*line, WHITE);
		line++;
	}

	currentVideo = auxCurrentVideo;	// Restablezco el valor de current
}





void ncClearScreen() {
	ncClear();
	// if(mode == GENERAL_SCREEN) { 	
	// 	ncClear();
	// } else if(mode == GAME_SCREEN_SL) {
	// 	int startSL = (width*2*5);
	// 	int count = GAME_SCREEN_DELETE_S;		// Cantidad de espacio que tiene la pantalla 
	// 	ncClearParticuarScreen(startSL, count, IS_LEFT_SCREEN);
	// 	currentVideo = auxCurrentVideo;
	// 	return;
	// } else if(mode == GAME_SCREEN_SR) {
	// 	int startSR = (width*2*5) + width + 2;
	// 	int count = GAME_SCREEN_DELETE_S;	
	// 	ncClearParticuarScreen(startSR, count, IS_RIGHT_SCREEN);
	// 	currentVideo = auxCurrentVideo;
	// 	return;
	// } else if(mode == GAME_SCREEN_IL) {
	// 	int startIL = (width*2*8);
	// 	int count = GAME_SCREEN_DELETE_I;	
	// 	ncClearParticuarScreen(startIL, count, IS_LEFT_SCREEN);
	// 	currentVideo = auxCurrentVideo;
	// 	return;
	// } else if(mode == GAME_SCREEN_IR) {
	// 	int startIR = (width*2*8) + width + 2;
	// 	int count = GAME_SCREEN_DELETE_I; 		
	// 	ncClearParticuarScreen(startIR, count, IS_RIGHT_SCREEN);
	// 	currentVideo = auxCurrentVideo;
	// 	return;
	// } else if(mode == GAMES_SCREEN) { 
	// 	int j;
	// 	for(j = 0; j < (width*GAME_SCREEN_DELETE_G); j++) {
	// 		video[j * 2] = ' ';
	// 	}
	// 	currentVideo = video;
	// 	return;
	// } else {
	// 	ncPrintStringColour("Invalid Screen", RED);
	// }
	
}



void ncScrollUp(uint64_t screen) {
	uint8_t * auxCurrent = currentVideo;

	if(screen == GENERAL_SCREEN) {
		for(int i=0; i < height; i++) { 	
			for(int j=0; j < width*2 ; j++) { 
				video[j + i * width * 2] = video[j + (i+1) * width * 2]; // se copia lo que esta en el renglon de abajo, en el de arriba. 
			}
		} 
		for (int k = 0; k < width * 2; k++){
			video[(height - 1) * width * 2 + k] = '\0';
		}
		auxCurrent = video + ((height - 1) * width * 2); 	// actual = inicial + cantidad de renglones que se bajaron

	} else if(screen == GAMES_SCREEN) {
		for(int i=0; i < 4; i++) { 			// De cero a 4 ya que es la altura de nuestra shell en los juegos
			for(int j=0; j < width*2 ; j++) { 
				video[j + i * width * 2] = video[j + (i+1) * width * 2];
			}
		} 
		for (int k = 0; k < width * 2; k++){
			video[(4 - 1) * width * 2 + k] = '\0';
		}
		auxCurrent = video + ((4 - 1) * width * 2);
	}
	currentVideo = auxCurrent;
}

void ncCheckTerminalPosition() {
	if(screenInUse == GENERAL_SCREEN) {
		if(currentVideo - video >= height * width * 2){
			ncScrollUp(GENERAL_SCREEN);
		}
	} else { // estoy en Games Screen (5)
		if(currentVideo - video >= 4 * width * 2){ 
			ncScrollUp(GAMES_SCREEN);
		}
	}
}



int ncGetCurrentVideo() {
    uint8_t * pos = currentVideo;
    return ((int)(pos - video))/2; 
}

void ncDelete() {	
	if (ncGetCurrentVideo()) {
		*currentVideo = ' ';
    	currentVideo -= 2;
		*currentVideo = ' ';
	}
}



void ncChangeScreen(int screenMode){
	screenInUse = screenMode;
}