#include <stdio.h>
#include <string.h>

int fuerza_bruta(){
	return 0;
}

int main(void){
	char T[] = "ABCABD";
	char P[] = "ABD";
	int posicion = fuerza_bruta(T,P);
	printf("La posicion es: %d", posicion);
	return 0;
}

