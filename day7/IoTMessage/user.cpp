//
// Created by zhangji on 3/20/18.
//
#define STRINGTYPE         0x01
#define SETTYPE            0x02

#define MAX_KEY_LENGTH     15
#define MAX_VALUE_LENGTH   127
#define MAX_MESSAGE_LENGTH 81920



//the below functions are provided for your convenience
/*const char hex[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};*/

/*void printHex(const char message[], const int length) {
	for (int i = 0; i < length; i += 16) {
		for (int j = 0; j < 16 && i + j < length; ++j)
			printf("%c%c ", hex[message[i + j] >> 4], hex[message[i + j] & 0x0f]);
		printf("\n");
	}
}*/

/*void ztrncpy(char dest[], const char src[], const int offset, const int length) {
  int pos = 0;
  while (pos < length && src[pos + offset] != '\0') {
		dest[pos] = src[pos + offset];
      ++pos;
  }
  dest[pos] = '\0';
}*/

/*int ztrlen(const char str[]) {
  int pos = 0;
  while(str[pos] != '\0')
 		++pos;
 	return pos;
}*/


void parse(char in[MAX_MESSAGE_LENGTH], int size) {

}

void set(char targetkey[MAX_KEY_LENGTH + 1], char newvalue[MAX_VALUE_LENGTH + 1]) {

}

void add(char parentkey[MAX_KEY_LENGTH + 1], char childkey[MAX_KEY_LENGTH + 1], char childvalue[MAX_VALUE_LENGTH + 1]) {

}

void erase(char targetkey[MAX_KEY_LENGTH + 1]) {

}

int generate(char targetkey[MAX_KEY_LENGTH + 1], char out[MAX_MESSAGE_LENGTH]) {
    return 0;  // return the size of the encoded IoT Message which is stored in the array of out. 
}