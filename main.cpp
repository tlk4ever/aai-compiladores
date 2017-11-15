//Bibliotecas
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

//Define's
#define oper "+/-*><"
#define num  "0123456789"
#define letra  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define stateFinal 24
int scanner(char automato[100]);
bool initBlock(char automato[100], int i);
bool endBlcok (char automato[100], int i);
bool twoBar(char automato[100], int i);
int spaceWhite(char automato[100], int i);
bool finalLine(char automato[100],int i);
int ifLetter(char letter);
int ifNumber(char number);
int ifOperator(char o);
int numbers(char automato[100], int i);
int main (void){
    char resultados[200], *expressoes[200];

	printf("\tCompiladores - Scanner AAI \n");

	FILE * pFile, *pFile2;
	char mystring[100];
	pFile = fopen("entrada.txt", "r");

	pFile2 = fopen("saida.txt", "w+");

	if (pFile == NULL)
		perror("Error opening file");
	else {
		int i = 0;
		while (fgets(mystring, 100 * 100, pFile) != NULL) {
			if (scanner(mystring) == -1) {
				printf("\nExpressao correta - %s", mystring);
				fprintf(pFile2, "\nExpressao correta - %s", mystring);
			} else {
				printf("\nExpressao Incorreta - State: %d - %s",
						scanner(mystring), mystring);
				fprintf(pFile2, "\nExpressao Incorreta State: %d - %s",
						scanner(mystring), mystring);
			}
			printf(" > %d", i);
			i++;
		}
		fclose(pFile);
		fclose(pFile2);
		getch();
	}
}
int search(char a, const char * str) {
	int j = 0, ret = 0;
	while (str[j] && !ret) {
		ret = (str[j] == a);
		j++;
	}
	return (ret);
}
bool initBlock(char automato[100], int i){
    return automato[i] == '/' && automato[i+1] == '*';
}
bool endBlcok (char automato[100], int i){
    return automato[i-1] == '*' && automato[i] == '/';
}

bool twoBar(char automato[100], int i){
	return automato[i] == '/' && automato[i+1] == '/';
}

int spaceWhite(char automato[100], int i){
    while (automato[i] == ' ') {
        i++;
    }
    return i;
}
int letterOrNumber(char automato[100], int i){
     while (ifLetter(automato[i]) || ifNumber(automato[i])) {
        i++;
    }
    return i;
}
int numbers(char automato[100], int i){
     while (ifNumber(automato[i])) {
        i++;
    }
    return i;
}
bool finalLine(char automato[100],int i){
    return automato[i] == 10 || automato[i] == 13 || automato[i] == ';' || twoBar(automato,i);
}
int ifLetter(char letter){
    return search(letter, letra);
}
int ifNumber(char number){
    return search(number,num);
}

int ifOperator(char o){
    return search(o, oper);
}
int scanner(char automato[100]) {
    int i = 0, state = 0;
    while (state != stateFinal) {
        switch(state){
        case 0:
            i = spaceWhite(automato,i);

			if(initBlock(automato,i)){
                state = 1;
                i+=2;
			}else if (ifLetter(automato[i])) {
			    state = 3;
			    i++;
			}else if(finalLine(automato,i)){
			    state = stateFinal;
			}else{
			    return state;
			}
            break;
        case 1:
            while (automato[i] != '*') {
				i++;
			}
			if(endBlcok(automato,i)){
                state = 0;
                i+=2;
			}else if(automato[i] == '*' ){
                state = 2;
                i++;
            }else{
                return state;
            }
            break;
        case 2:
			if(automato[i] == '/'){
                state = 0;
			}else{
			    state = 1;
			}
			i++;
            break;
        case 3:
            i = letterOrNumber(automato,i);
            i = spaceWhite(automato,i);

            if(initBlock(automato,i)){
                state = 4;
                i+=2;
            }else if(automato[i] == '='){
                state = 6;
                i++;
            }else if(finalLine(automato,i)){
                state = stateFinal;
            }else{
                return state;
            }
            break;
        case 4:
            while (!endBlcok(automato,i)) {
				i++;
			}
			state = 5;
			i++;
            break;
        case 5:
            i = spaceWhite(automato,i);

            if(initBlock(automato,i)){
                state = 4;
                i+=2;
            }else if(automato[i] == '='){
                state = 6;
                i++;
            }else if(finalLine(automato,i)){
                state = stateFinal;
            }else{
                return state;
            }
            break;
        case 6:
            i = spaceWhite(automato,i);
            if(initBlock(automato,i)){
                state = 7;
                i+=2;
            }else if(ifLetter(automato[i])){
                state = 18;
                i++;
            }else if(ifNumber(automato[i])){
                state = 9;
                i++;
            }else{
                return state;
            }
            break;
        case 7:
            if(finalLine(automato,i)) return state;
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            if(initBlock(automato,i)){
                state = 6;
                i+=2;
            }else if(automato[i]==' '){
                state = 8;
                i++;
            }else if(ifLetter(automato[i])){
                state = 18;
                i++;
            }else if(ifNumber(automato[i])){
                state = 9;
                i++;
            }else{
                return state;
            }
            break;
        case 8:
            i = spaceWhite(automato,i);
            if(initBlock(automato,i)){
                state = 6;
                i+=2;
            }else if(ifLetter(automato[i])){
                state = 18;
                i++;
            }else if(ifNumber(automato[i])){
                state = 9;
                i++;
            }else if(finalLine(automato,i)){
                return state;
            }
            break;
        case 9:
            i = numbers(automato,i);
            i = spaceWhite(automato,i);
            if(automato[i] == ';'){
                state = 12;
                i++;
            }else if(initBlock(automato,i)){
                state = 10;
                i+=2;
            }else if(ifOperator(automato[i])){
                state = 15;
                i++;
            }else {
                return state;
            }
            break;
        case 10:
            if(finalLine(automato,i)){
                return state;
            }
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            if(initBlock(automato,i)){
                state = 11;
                i+=2;
            }else if(automato[i] == ';' || automato[i]==' '){
                state = 12;
                i++;
            }else if(ifOperator(automato[i])){
                state = 15;
                i++;
            }else{
                return state;
            }
            break;
        case 11:
            if(finalLine(automato,i)){
                return state;
            }
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            if(initBlock(automato,i)){
                state = 10;
                i+=2;
            }else if(automato[i] == ';' || automato[i]==' '){
                state = 12;
                i++;
            }else if(ifOperator(automato[i])){
                state = 15;
                i++;
            }else{
                return state;
            }
            break;
        case 12:
            i = spaceWhite(automato,i);
            if(finalLine(automato,i)){
                state = stateFinal;
            }else if(initBlock(automato,i)){
                state = 13;
                i+=2;
            }else{
                return state;
            }
            break;
        case 13:
            if(finalLine(automato,i)){
                return state;
            }
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            if(initBlock(automato,i)){
                state = 14;
                i+=2;
            }else if(finalLine(automato,i)){
                state = 12;
            }else if(automato[i] == ' '){
                state = 12;
                i++;
            }else{
                return state;
            }
            break;
        case 14:
            if(finalLine(automato,i)){
                return state;
            }
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            if(initBlock(automato,i)){
                state = 13;
                i+=2;
            }else if(finalLine(automato,i)){
                state = 12;
            }else if(automato[i] == ' '){
                state = 12;
                i++;
            }else{
                return state;
            }
            break;
        case 15:
            i = spaceWhite(automato,i);
            if(initBlock(automato,i)){
                state = 16;
                i+=2;
            }else if(ifLetter(automato[i])){
                state = 18;
                i++;
            }else if(ifNumber(automato[i])){
                state = 9;
                i++;
            }else{
                return state;
            }
            break;
        case 16:
            if(finalLine(automato,i)){
                return state;
            }
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            if(automato[i] == ' '){
                state = 15;
                i++;
            }else if(initBlock(automato,i)){
                state = 17;
                i+=2;
            }else if(ifLetter(automato[i])){
                state = 18;
                i++;
            }else if(ifNumber(automato[i])){
                state = 9;
                i++;
            }else{
                return state;
            }
            break;
        case 17:
            if(finalLine(automato,i)){
                return state;
            }
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            if(automato[i] == ' '){
                state = 15;
                i++;
            }else if(initBlock(automato,i)){
                state = 16;
                i+=2;
            }else if(ifLetter(automato[i])){
                state = 18;
                i++;
            }else if(ifNumber(automato[i])){
                state = 9;
                i++;
            }else{
                return state;
            }
            break;
        case 18:
            i = letterOrNumber(automato,i);
            i = spaceWhite(automato,i);
            if(automato[i] == ';'){
                state = 19;
                i++;
            }else if(initBlock(automato,i)){
                state = 22;
                i+=2;
            }else if(ifOperator(automato[i])){
                state = 15;
                i++;
            }else{
                return state;
            }
            break;
        case 19:
            i = spaceWhite(automato,i);
            if(initBlock(automato,i)){
                state = 20;
                i+=2;
            }else if(finalLine(automato,i)){
                state = stateFinal;
            }
            break;
        case 20:
            if(finalLine(automato,i)){
                return state;
            }else{
                while(!endBlcok(automato,i)){
                    i++;
                }
                i++;
                if(finalLine(automato,i)){
                    state = 19;
                }else if(automato[i] == ' '){
                    state = 19;
                    i++;
                }else if(initBlock(automato,i)){
                    state = 21;
                    i+=2;
                }else{
                    return state;
                }
            }
            break;
        case 21:
            if(finalLine(automato,i)){
                return state;
            }
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            i = spaceWhite(automato,i);
            if(initBlock(automato,i)){
                state = 20;
                i+=2;
            }else if(finalLine(automato,i)){
                state = 19;
            }else{
                return state;
            }
            break;
        case 22:
            if(finalLine(automato,i)){
                return state;
            }
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            if(initBlock(automato,i)){
                state = 23;
                i+=2;
            }else if(ifOperator(automato[i])){
                state = 15;
                i++;
            }else if(finalLine(automato,i)){
                state = 19;
                i++;
            }else if(automato[i] == ' '){
                state = 19;
                i++;
            }else{
                return state;
            }
            break;
        case 23:
            if(finalLine(automato,i)){
                return state;
            }
            while(!endBlcok(automato,i)){
                i++;
            }
            i++;
            if(initBlock(automato,i)){
                state = 22;
                i+=2;
            }else if(ifOperator(automato[i])){
                state = 15;
                i++;
            }else if(finalLine(automato,i)){
                state = 19;
                i++;
            }else if(automato[i] == ' '){
                state = 19;
                i++;
            }else{
                return state;
            }
            break;
        }
    }
	return -1;
}
