#include <libc.h>
// Salida/Entrada
#define SIZE_CHAR 2
#define MAX_BUFFER 100	

int bufferSize = 0; 
char buffer[MAX_BUFFER];
static int tabCount = 0;
  


uint64_t strlength(char * string) {
	uint64_t i=0;
	while(string[i]!=0) {
		i++;
	}
	return i;
}


void userPrint(char * buffer) {
	sys_print_user(1, buffer, strlength(buffer));
}


void getScreenDivition() {
	sys_screen_divition();
}


void clearScreen() {
	sys_screen_clear();
}


char getChar() {
	return sys_get_char();
}


void putChar(char c) {
    sys_write(&c, 1);
}


void printSudokuNumbers(char* line, uint64_t position, uint64_t orientationX) {
	sys_print_sudoku_numbers(line, position, orientationX);
}


void printInScreenPosition(char* message, uint64_t position) {
	sys_print_in_screen_position(message, position);
}

void changeScreen(uint64_t mode) {
	sys_screen_mode(mode);
}


uint64_t getMilliSeconds(uint64_t time) {
	return sys_get_milli_seconds(time);
}


void reverse(char *str, uint64_t len) {
	int temp;
	for(int i=0; i<len/2; i++){
		temp = str[i];
		str[i] = str[len-i-1];
		str[len-i-1]=temp;
	}
}


void print(char * string) {
    sys_write(string, strlength(string));  
}


int numToStr(int num, int pos, char*buff) {	
	if(num/10 < 1) {
		buff[pos] = num + '0';
		return pos+1;
	}
	int newpos = numToStr((int) num/10, pos, buff);
	buff[newpos] = num%10 + '0';
	return newpos+1;
}


int isDigit(char c) {
    return c <= '9' && c >= '0';
}


int charToDigit(char* buffer) {
    int res = 0;

	for (int i = 0; buffer[i]; ++i) {
       res = res * 10 + buffer[i] - '0';
  	}
	return res;
}


char toLower(char c) {
    if(c >= 'A' && c <= 'Z') {
        c = c + ('a'-'A');
    }
    return c;
}


char toUpper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' + 'A');
    }
    return c;
}


int isWhiteSpace(char c) {
	return (c == ' ' || c == '\t' || c=='\n' || c == '\v' || c == '\f' || c == '\r');
}


int isAlpha(char c) {
	return (c <= 'z' && c>='a') || (c<='Z' && c>='A');
}


void stringToIntArray(char* buffer[], int* positions, int max) {
	int res = 0;
	for(int i = 0; i < max; i++) {
		res = 0;
		for(int j = 0; buffer[i][j] != '\0'; j++) {
			res = res * 10 + buffer[i][j] - '0';
		}
		positions[i] = res;
	}
}


void clearBuffer() {
    bufferSize = 0;
    int c = 0;
    while(c != -1) {
        c = getChar();
    }
    
}


void toStr(int number, char* ret){
	ret[0] = number + '0';
    ret[1] = '\0';
}


void putInBuffer(char c) {
    if(bufferSize < MAX_BUFFER - 1) {
        buffer[bufferSize++] = c;
    }
}


int strCmp( char * s1,  char * s2) {
	if(*s1 == 0 || *s2 == 0){
		return 1;
	}
	 while(*s1 != 0 && *s2 != 0){
        if(*s1 != *s2){
            return 1;
        }
        s1++;
        s2++;
    }
    return *s1 - *s2;
}


void printNum(int num) {
	char buffer[40];
	int sign = 0;
	if (num < 0) {
		num = -num;
		sign = 1;
	}
	uintToBase(num, buffer, 10);
	if (sign)
		print("-");
	print(buffer);
}


uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
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



int digitFormat(char c, int digitBase) {
    int topCantdigit;
    if (digitBase <= 10){
        topCantdigit = digitBase + '0';
	} else {
		 topCantdigit = digitBase - 10 + 'a';
	}
 
    if (c >= '0' && c <= '9' && c <= topCantdigit){
        return (c - '0');
	}
    else if ((c >= 'a' && c <= 'f') && (c <= topCantdigit)) {
		return (10 + c - 'a');
	} else if ((c >= 'A' && c <= 'F') && (c <= topCantdigit)) {
		return (10 + c - 'A');
	} else {
        return -1;
	}
}

uint64_t hexaToInt(char* hexaAddress, int strBase) {
    int toReturn = 0, sign = 1, number;
 
    if (*hexaAddress == '-') {
        sign = -1;
        ++hexaAddress;
    }
 
    while ((number = digitFormat(*hexaAddress, strBase)) >= 0) {
        toReturn *= strBase;
        toReturn += (number * sign);
        ++hexaAddress;
    }
    return toReturn;
}



char * intToHexa(uint64_t num, char * str, int bytes){ 
    // counter for hexadecimal number array
    int i = 0;
    while (i < bytes*2 && num != 0) {
 
        // storing remainder in temp variable.
        int temp = num % 16;
 
        // check if temp < 10
        if (temp < 10)
            str[i++] = temp + '0';
        else
            str[i++] = temp - 10 + 'A';
 
        num = num / 16;
    }

    while (i < bytes*2)
        str[i++] = '0';

    str[i++] = 'x';
    str[i++] = '0';
    str[i] = '\0';
  
    // Reverse the string 
    reverse(str, i); 
    return str;
} 


int power(int num, int p) {
	int res = 1;
	for(int i = 0; i < p; i++) {
		res *= 10;
	}
	return num*res;
}


int strtok(char *s, char delim, char *array[], int arraySize) {
	int arrayIndex = 0;
	if (*s != delim && *s != '\0')
		array[arrayIndex++] = s;
	while (*s != '\0')
	{
		if (*s == delim)
		{
			*s = 0;
			if (*(s + 1) != delim && (*(s + 1) != '\0'))
			{
				if (arrayIndex >= arraySize)
					return arrayIndex;
				array[arrayIndex++] = s + 1;
			}
		}
		s++;
	}
	return arrayIndex;
}


void readInput(char* inputBuffer, int maxSize, int flag) {	
    int size = 0;
    char c;
    while (size < (maxSize - 1) && (c = getChar()) != '\n') {
		if(flag != 0) {
		    // timeGame();
			// stopWatch();
		}
        if (c) { 
            if (c != '\b' && c != '\t') {
                putChar(c);
                inputBuffer[size++] = c;
            } else if (size > 0 && c != '\t') {
                putChar('\b');
                size--;
            } else if(c == '\t') {
				tabCount = 1;
			}
        }
    }
    inputBuffer[size++] = 0;
}


int checkTabCount() {
	return tabCount;
}



void intToString(int num, char *str) {	// Ver como hacer para que imprima el 10
    int power=0;
    int j=num;
    for(power=1; j>=10; j/=10) {
        power *= 10;
	}	
	if(num >= 0 && num <= 9){
		(*str++)='0';
	}
    for(; power > 0; power/=10) {
        (*str++) = '0' + num/power;
        num %= power;
    }
    *str='\0';
}

void sleep(uint64_t seconds) {
	int ticks = sys_get_ticks();

	while (sys_get_ticks() < (ticks + seconds * 18));

}