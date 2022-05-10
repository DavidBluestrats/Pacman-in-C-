#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

#define TAMC 11
struct Jugador{
	char nombre[TAMC];
	char contrasena[TAMC];
	char fecha[TAMC];
	int score;
	int vidas;
	int nivel;
	int bonus;
};
int main(){
	Jugador jug;
	fstream arch;
	
	strcpy(jug.nombre,"mario");
	strcpy(jug.contrasena,"123abc");
	strcpy(jug.fecha," ");
	jug.score=100;
	jug.vidas=3;
	jug.nivel=1;
	jug.bonus=0;
	
	arch.open("archgeneral.dat",ios::binary|ios::out);
	arch.seekp(0);
	arch.write((char *)&jug,sizeof(Jugador));
	arch.close();
}
