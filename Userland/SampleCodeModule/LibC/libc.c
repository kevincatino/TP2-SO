// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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



void clearScreen() {
	sys_screen_clear();
}


char getChar() {
	return sys_get_char();
}


void putChar(char c) {
    sys_write(&c, 1);
}



void reverse(char *str, uint64_t len) {
	
	for(int i=0; i<len/2; i++){
		int temp = str[i];
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

static int stringsMatch(char * s1, char * toMatch) {
	if (strlength(s1) < strlength(toMatch))
		return 0;
	while (*toMatch) {
		if (*toMatch != *s1)
			return 0;
		toMatch++;
		s1++;
	}
	return 1;
}

int strtok(char *s, char * delim, char *array[], int arraySize) {
	int arrayIndex = 0;
	int sIdx = 0;
	int sLength = strlength(s);
	int delimLen = strlength(delim);
	// if (stringsMatch(s, delim) && *s != '\0')
	// 	sIdx+=delimLen;
	array[arrayIndex++] = s;
	while (s[sIdx] != '\0' && sIdx < sLength)
	{

		if (stringsMatch(&s[sIdx], delim))
		{
			s[sIdx] = 0;

			// if (!stringsMatch(s + delimLen, delim) && (*(s + delimLen +1) != '\0'))
			// {
				if (arrayIndex >= arraySize)
					return arrayIndex;
				array[arrayIndex++] = s + sIdx + delimLen;
			// }
		sIdx+=delimLen;
		}
		else
		sIdx++;
		
	}
	return arrayIndex;
}


void readInput(char* inputBuffer, int maxSize, int flag) {	
    int size = 0;
    char c;
    while (size < (maxSize - 1) && (c = getChar()) != '\n') {
		if(flag != 0) {
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




void sleep(uint64_t seconds) {
	int ticks = sys_get_ticks();

	while (sys_get_ticks() < (ticks + seconds * 18));

}

char* strCpy(char* destination, const char* source)
{
    // return if no memory is allocated to the destination
    if (destination == 0) {
        return 0;
    }
 
    // take a pointer pointing to the beginning of the destination string
    char *ptr = destination;
 
    // copy the C-string pointed by source into the array
    // pointed by destination
    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }
 
    // include the terminating null character
    *destination = '\0';
 
    // the destination is returned by standard `strcpy()`
    return ptr;
}