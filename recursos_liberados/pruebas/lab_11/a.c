#include <stdio.h>
#include <string.h>

int buscar_fuerza_bruta(char* T, char* P){
	int n = strlen(T), m = strlen(P), j = 0;
	for( int i = 0; i <= n - m; i++ ){
		for( j = 0; j < m; j++ )
			if( T[i + j] != P[j] )
				break;
	if( j == m )
		return i;
	}
	return -1;
}

int main() {
    char T1[] = "ABCABD";
    char T2[] = "AAAAAAB";
    char T3[] = "ABCDEFG";
    char T4[] = "AABABC";
    char P1[] = "ABD";
    char P2[] = "AAAB";
    char P3[] = "XYZ";
    char P4[] = "AB";

    int pos =  buscar_fuerza_bruta(T4, P4);
    printf("La posicion es: %d\n", pos);
    return 0;
}
