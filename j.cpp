 #include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXFILAS 23
#define MAXCOLS 29
#define TAM 5
#define TAMC 11
#define TOTAL 265
#define BLANCO makecol(255, 255, 255)

//Imágenes
BITMAP *buffer;
BITMAP *pared;
BITMAP *pacbmp;
BITMAP *pacman;
BITMAP *muerbmp;
BITMAP *fantabmp[6];
BITMAP *fanta[4];
BITMAP *comida[2];
BITMAP *frutas[2];
BITMAP *gameover;
BITMAP *menu[5];
BITMAP *cursor;
BITMAP *altas;
BITMAP *rank[2];

//Sonido
SAMPLE *comer; 
SAMPLE *inicio; 
SAMPLE *muere; 

//Fuentes
FONT* font1[3];

//Fruta
int fruit=0,flagg=0,cont=1,timer;

//Fantasma vulnerable
int contPasos = 0,bandera[4];

//Coordenadas Pacman
int dir=0, puntuacion=0;
int px=30*10, py=30*19;
//Coordenadas Fantasma Rojo
int xdir= 0; 
int _x= 30*14, _y=30*19; 
//Coordenadas Fantasma Azul
int xAdir= 0; 
int _xA= 30*14, _yA=30*19; 
//Coordenadas Fantasma Rosa
int xVdir= 0; 
int _xV= 30*14, _yV=30*19; 
//Coordenadas Fantasma Amarillo
int xMdir= 0; 
int _xM= 30*14, _yM=30*19; 

bool aux=true;
int auxposx, auxposy;


//ESTRUCTURAS
struct Jugador{
	char nombre[TAMC];
	char contrasena[TAMC];
	int score;
	int vidas;
	int nivel;
	int bonus;
};
struct Posicion{
	char nombre[TAMC];
	int score;
};

//ENUMERACIONES
enum tipodeJugador{NUEVO,CONTINUAR};
enum altanoc{NOMBRE,CONTRAS};

//PROTOTIPOS DE FUNCIONES
void captura(Jugador &jug,int n);
int validacion(Jugador &jug,FILE *arch,int tipo);
void datos(Jugador &jug,FILE *arch);
char **creardin_mapa();
char **obtener_mapa(char **mapa,Jugador &jug);
void com_frutas(char **mapa,Jugador jug);
void dibujar_mapa(char **mapa,Jugador &jug);
void pantalla();
void dibujar_pacman();
bool finalizar();
void dibujar_fantasma();
void chocar(char **mapa,Jugador &jug);
void fantasma(char **mapa,Jugador &jug);
void dibujar_fantasma_azul();
void chocar_A(char **mapa,Jugador &jug);
void fantasma_azul(char **mapa,Jugador &jug);
void dibujar_fantasma_Rosa();
void chocar_V(char **mapa,Jugador &jug);
void fantasma_Rosa(char **mapa,Jugador &jug);
void dibujar_fantasma_ama();
void chocar_M(char **mapa,Jugador &jug);
void fantasma_ama(char **mapa,Jugador &jug);
void guardar_mapa(char **mapa, Jugador &jug);
int num_registro();
void ordenar_rank(Posicion pos[],int n);
void mostrar_ranking(Posicion pos[],int n);
void ranking();
void play(Jugador &jug,char **mapa);
void manu();

//IMPLEMENTACIÓN
/*Se cargan todas las imágenes, sonidos u 
recursos paar el funcionamiento del juego*/
int main() {

	allegro_init();
	install_keyboard();
	install_mouse();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1040, 700, 0, 0);
	
	//Fuentes
	font1[0]=load_font("font3.pcx",NULL,NULL);
	font1[1]=load_font("font2.pcx",NULL,NULL);
	
	//Sonido
	 if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error en el sonido\n%s\n", allegro_error);
       return 1;
    }
	set_volume(80, 80);
	inicio= load_wav("pacmaninicio.wav");
	comer= load_wav("pacmancome.wav");
	muere= load_wav("pacmanmuere.wav");	
	
	//Cargar de pacman y pared
	buffer = create_bitmap(1040,700);
	pared = load_bitmap("Pared.bmp",NULL);
	pacbmp = load_bitmap("pacman.bmp",NULL);
	pacman = create_bitmap(33,33);
	
	//Comidas y bonus
	comida[0]= load_bitmap("comida.bmp",NULL);
	comida[1]= load_bitmap("comidaGran.bmp",NULL);
	frutas[0]=load_bitmap("cereza.bmp",NULL);	
	frutas[1]=load_bitmap("fresa.bmp",NULL);
	
	//Fantasmas
	for(int i=0;i<4;i++){
		fanta[i]=create_bitmap(30,30);
	}
	fantabmp[0]= load_bitmap("enemigo.bmp",NULL);
	fantabmp[1]= load_bitmap("enemigoAzul.bmp",NULL);
	fantabmp[2]= load_bitmap("enemigoRosa.bmp",NULL);
	fantabmp[3]= load_bitmap("enemigoAma.bmp",NULL);
	fantabmp[4]=load_bitmap("enemigoVul.bmp",NULL);
	fantabmp[5]=load_bitmap("enemigoMuerto.bmp",NULL);
	
	gameover=load_bitmap("game over.bmp",NULL);
	muerbmp= load_bitmap("muerte.bmp",NULL);
	
	//Menús
	menu[0]=load_bitmap("menu.bmp",NULL);
	menu[1]=load_bitmap("menu1.bmp",NULL);
	menu[2]=load_bitmap("menu2.bmp",NULL);
	menu[3]=load_bitmap("menu3.bmp",NULL);
	menu[4]=load_bitmap("menu4.bmp",NULL);
	cursor=load_bitmap("cursor.bmp",NULL);
	rank[0]=load_bitmap("rank1.bmp",NULL);
	rank[1]=load_bitmap("rank2.bmp",NULL);
	
	manu();

}
END_OF_MAIN();


//CAPTURA DE NOMBRE Y CONTRASEÑA
//(guarda el nombre y contraseña en la variable jug)
void captura(Jugador &jug,int n){
	//Variables
	int i=0;
	char cadena[TAMC]={"          "};
	
	if(n==NOMBRE){
		altas=load_bitmap("jugador.bmp",NULL);
	}
	else{
		altas=load_bitmap("contrasena.bmp",NULL);
	}
	blit(altas,screen, 0, 0, 0, 0, 1040, 700);
    do{
        int in= readkey();
        char letra=in & 0xff;
        char sc=in >> 8;
        if(letra >= 32 && letra <= 126){
            if(i<TAMC-1){
            	cadena[i]=letra;
                i++;
            }
        }
        else 
			if(sc==KEY_BACKSPACE){
            	if (i>0){
            		i--;	
            		cadena[i]=' ';
         		}
      	 	}
      	clear(altas);
      	if(n==NOMBRE){
      		textout(screen,font1[0],cadena,100,180,BLANCO);
      		strcpy(jug.nombre,cadena);
		  }
		else{
			textout(screen,font1[0],jug.nombre,100,180,BLANCO);
			textout(screen,font1[0],cadena,100,380,BLANCO);
			strcpy(jug.contrasena,cadena);
		}
    	
    }while(!(key[KEY_ENTER] & KB_NORMAL)&& i-1<TAMC);
	destroy_bitmap(altas);
}

//VALIDACIÓN DE TIPO DE PARTIDA
//(recopilación de datos o creación de un nuevo jugador)
int validacion(Jugador &jug,FILE *arch,int tipo){
	
	Jugador aux;
	/*Variable a retornar
	opc=0 nuevo jugador es válido
	opc=1 continuar es válido
	opc=2 no se cumple con los requisitos*/
	
	int opc;

	arch=fopen("archgeneral.dat","rb");
	
	if(arch==NULL){
		return 3;
	}
	
	rewind(arch);
	
	fread(&aux,sizeof(Jugador),1,arch);
	while(!(feof(arch))){
		if(tipo==NUEVO){
			opc=0;
			if(strcmp(jug.nombre,aux.nombre)==0){
				opc=2;
				break;
			}
		}
		else{
			opc=2;
			if(strcmp(jug.nombre,aux.nombre)==0 && strcmp(jug.contrasena,aux.contrasena)==0){
				jug.score=aux.score;
				jug.vidas=aux.vidas;
				jug.nivel=aux.nivel;
				jug.bonus=aux.bonus;
				opc=1;
				break;
			}
		}
		fread(&aux,sizeof(Jugador),1,arch);	
	}
	fclose(arch);
	if(opc==0){
		jug.score=0;
		jug.vidas=3;
		jug.nivel=1;
		jug.bonus=0;
	}
	return opc;
}

//REGISTRO DE DATOS EN EL ARCHIVO
//(guardar los datos del jugador en archivo general)
void datos(Jugador &jug,FILE *arch){
	
	Jugador aux;
	int cont=0;
	
	arch=fopen("archgeneral.dat","rb+");

	if(arch==NULL){
		return;
	}

	rewind(arch);

	fread(&aux,sizeof(Jugador),1,arch);
	
	while(!(feof(arch))){

		if(strcmp(jug.nombre,aux.nombre)==0){
			cont=1;
			
			if(jug.score>aux.score){
			fseek(arch,-sizeof(Jugador),SEEK_CUR);
			
			fwrite(&jug,sizeof(Jugador),1,arch);
			}
			break;
		}
		fread(&aux,sizeof(Jugador),1,arch);
	}
	
	if(cont==0){
		fseek(arch,0,SEEK_END);
		fwrite(&jug,sizeof(Jugador),1,arch);
	}
	fclose(arch);
}

//CREAR MATRIZ DINÁMICA(mapa)
char **creardin_mapa(){
	char**mapa;
	mapa= new char*[MAXFILAS];
	for(int i=0; i<MAXFILAS; i++){
		mapa[i]= new char[MAXCOLS];
	}
	return mapa;
}

//LIBERAR ESPACIO DE MAPA
void liberarMapa(char **mapa){
	for(int i=0; i<MAXFILAS; i++){
		delete [] mapa[i];
	}
	delete [] mapa;
}
//MAPA DE ACUERDO AL NIVEL(obtener mapa desde un archivo txt)
char **obtener_mapa(char **mapa,Jugador &jug){
	
	FILE*archm;
	char aux[TAMC+4];
	
	//Dependiendo del nivel se abrira un mapa distinto
	if(jug.nivel==1){
		archm=fopen("mapa1.txt","r");
	}
	else{
		if(jug.nivel==2){
			archm=fopen("mapa2.txt","r");
		}
		else{
			strcpy(aux,jug.nombre);
			strcat(aux,".txt");
			archm=fopen(aux,"r");
			if(jug.score-jug.bonus<TOTAL){
				jug.nivel=1;
			}
			else{
				jug.nivel=2;
			}
		}
	}
	
	//Verificar
	if(archm==NULL){
		return mapa;
	}
	
	rewind(archm);
	for(int i=0;i<MAXFILAS; i++){
		for(int j=0;j<MAXCOLS;j++){
			mapa[i][j]=getc(archm);
		}
	}
	fclose(archm);
	
	return mapa;
}

//FUNCIÓN DE FRUTAS(aparición de frutas en el juego)
void com_frutas(char **mapa,Jugador jug){
	if(jug.score>=30){			
		if(flagg!=1 or (flagg==1 && jug.score==timer+10)){			
			if(flagg==1 && jug.score==timer+10){				
				flagg=0;							
			}
			//Aparecer fruta
			switch(cont){
				case 1: mapa[1][3]='B';fruit=0;break;
				case 2: mapa[19][26]='B';fruit=1;break;
				case 3: mapa[7][5]='B';fruit=0;break;
				case 4: mapa[18][3]='B';fruit=1;break;
			}
			timer =jug.score;	
		}
	}
}

//FUNCIONES DE DIBUJO EN PANTALLA
//(tablero con la comida de Pacman,mostrar vidas y score de jug)
void dibujar_mapa(char **mapa,Jugador &jug){
	for(int i=0;i<MAXFILAS; i++){
		for(int j=0;j<MAXCOLS;j++){
			if(mapa[i][j]=='X'){
				draw_sprite(buffer,pared,j*30,i*30);
			}
			if(mapa[i][j]=='o'){
				draw_sprite(buffer,comida[0],j*30,i*30);
				if(py/30==i && px/30 ==j){
					play_sample(comer,300,150,1000,0);
					mapa[i][j]=' ';
					jug.score++;
				}
			}
			if(mapa[i][j]=='B'){
				draw_sprite(buffer,frutas[fruit],j*30,i*30);
				if(py/30==i && px/30 ==j){
					//Condicionales
					flagg=1;cont++;
				    if(cont==5){
					cont=1;
				    }
					play_sample(comer,300,150,1000,0);
					mapa[i][j]=' ';
					jug.bonus+=70;jug.score+=70;
					timer=jug.score;
				}
			}
			if(mapa[i][j]=='Y'){
				draw_sprite(buffer,comida[1],j*30,i*30);
				if(py/30==i && px/30 ==j){
					mapa[i][j]=' ';
					jug.score++;
					contPasos=1500;
					bandera[0]=0;bandera[1]=0;bandera[2]=0;bandera[3]=0;
				}
			}
		}
	}
	textprintf_right_ex(buffer,font1[1],140,630,BLANCO,-1,"Score: %d",jug.score);
	textprintf_right_ex(buffer,font1[1],800,630,BLANCO,-1,"Vidas: %d",jug.vidas);
}

//PANTALLA, MUESTRA LO QUE SE MANDA A PANTALLA
void pantalla(){
	blit(buffer, screen, 0,0,0,0,1040,700);
}

//PACMAN EN PANTALLA
void dibujar_pacman(){
	blit(pacbmp,pacman,dir*33,0,0,0,33,33);
	draw_sprite(buffer,pacman,px,py);
}

//FINALIZAR PARTIDA (ninguna comida en el tablero)
bool finalizar(char **mapa, Jugador &jug){
	if(jug.nivel==1){
		for(int i=0;i<MAXFILAS; i++){
			for(int j=0;j<MAXCOLS;j++){
				if(mapa[i][j]=='o'|| mapa[i][j]=='B'|| mapa[i][j]=='Y')return true;
			}
		}
		jug.nivel=2;
		play(jug,mapa);
		return false;
	}
	else{
		for(int i=0;i<MAXFILAS; i++){
			for(int j=0;j<MAXCOLS;j++){
				if(mapa[i][j]=='o' || mapa[i][j]=='B'|| mapa[i][j]=='Y')return true;
			}
		}
		return false;
	}
}

//FANTASMA ROJO

//DIBUJAR EN PANTALLA EL FANTASMA ROJO(ojitos,azul,original)
void dibujar_fantasma(){
	if(contPasos>0){
		if(bandera[0]!= 0){
			blit(fantabmp[5],fanta[0],0,0,0,0,30,30);
			draw_sprite(buffer,fanta[0],_x,_y);
		}
		else{
			blit(fantabmp[4],fanta[0],0,0,0,0,30,30);
			draw_sprite(buffer,fanta[0],_x,_y);
		}
	}
	else{
		blit(fantabmp[0],fanta[0],0,0,0,0,30,30);
		draw_sprite(buffer,fanta[0],_x,_y);
	}
}

//CUANDO PACMAN Y FANTASMA ROJO CHOCAN
void chocar(char **mapa,Jugador &jug){
	if((_y== py && _x==px) || (_y== auxposy && _x==auxposx) ){
		if(contPasos<=0){
			play_sample(muere,300,150,1000,0);
			for(int i=0; i<TAM; i++){
				clear(buffer);
				clear(pacman);
				dibujar_mapa(mapa,jug); 
				blit(muerbmp,pacman,i*33,0,0,0,33,33);
				draw_sprite(buffer,pacman,px,py);
				pantalla();
				rest(60);
				
			}
			px=30*10; 
			py=30*10;
			jug.vidas--;
		}
		else{
			bandera[0]++;
		}
	}

}

//FUNCIÓN GENERAL DE FANTASMA ROJO
void fantasma(char **mapa,Jugador &jug){
	dibujar_fantasma();
	chocar(mapa,jug);
	if(xdir==0){
		if(mapa[_y/30][(_x-30)/30] !='X') _x-=30;
		else{
			xdir= rand()%4;
		}
	}
	
	if(xdir==1){
		if(mapa[_y/30][(_x+30)/30] !='X') _x+=30;
		else{
			xdir= rand()%4;
		}
	}
	if(xdir==2){
		if(mapa[(_y-30)/30][_x/30] !='X') _y-=30;
		else{
			xdir= rand()%4;
		}
	}
	if(xdir==3){
		if(mapa[(_y+30)/30][_x/30] !='X') _y+=30;
		else{
			xdir= rand()%4;
		}
	}
	
	if (mapa[(_y)/30][_x/30]== 'C'){
		xdir=rand()%4;
	}	
	
	if(_x<=-30){ 
		_x=870; 
	}
	else if(_x>=870){
			_x= -30;	
		}
	
}

//FANTASMA AZUL

//DIBUJAR EN PANTALLA EL FANTASMA AZUL(ojitos,azul,original)
void dibujar_fantasma_azul(){
	if(contPasos>0){
		if(bandera[1]!= 0){
			blit(fantabmp[5],fanta[1],0,0,0,0,30,30);
			draw_sprite(buffer,fanta[1],_xA,_yA);
		}
		else{
			blit(fantabmp[4],fanta[1],0,0,0,0,30,30);
			draw_sprite(buffer,fanta[1],_xA,_yA);
		}
	}
	else{
		blit(fantabmp[1],fanta[1],0,0,0,0,30,30);
		draw_sprite(buffer,fanta[1],_xA,_yA);
	}
}

//CUANDO PACMAN Y FANTASMA AZUL CHOCAN
void chocar_A(char **mapa,Jugador &jug){
	
	if((_yA== py && _xA==px) || (_yA== auxposy && _xA==auxposx) ){
		if(contPasos<=0){
			play_sample(muere,300,150,1000,0);
			for(int i=0; i<TAM; i++){
				clear(buffer);
				clear(pacman);
				dibujar_mapa(mapa,jug); 
				blit(muerbmp,pacman,i*33,0,0,0,33,33);
				draw_sprite(buffer,pacman,px,py);
				pantalla();
				rest(60);
				
			}
			px=30*10; 
			py=30*10;
			jug.vidas--;
		}
		else{
			bandera[1]++;
		}
	}

}

//FUNCIÓN GENERAL DE FANTASMA AZUL
void fantasma_azul(char **mapa,Jugador &jug){
	dibujar_fantasma_azul();
	chocar_A(mapa,jug);
	if(xAdir==0){
		if(mapa[_yA/30][(_xA-30)/30] !='X') _xA-=30;
		else{
			xAdir=rand()%4;
		}
	}
	
	if(xAdir==1){
		if(mapa[_yA/30][(_xA+30)/30] !='X') _xA+=30;
		else{
			xAdir=rand()%4;
		}
	}
	if(xAdir==2){
		if(mapa[(_yA-30)/30][_xA/30] !='X') _yA-=30;
		else{
			xAdir=rand()%4;
		}
	}
	if(xAdir==3){
		if(mapa[(_yA+30)/30][_xA/30] !='X') _yA+=30;
		else{
			xAdir=rand()%4;
		}
	}
	
	if (mapa[(_yA)/30][_xA/30]== 'C'){
		xAdir=rand()%4;
	}
		
	if(_xA<=-30){ 
		_xA=870; 
	}
	else if(_xA>=870){
			_xA= -30;	
		}	
}

//FANTASMA ROSA

//DIBUJAR EN PANTALLA EL FANTASMA ROSA(ojitos,azul,original)
void dibujar_fantasma_Rosa(){
	if(contPasos>0){
		if(bandera[2]!= 0){
			blit(fantabmp[5],fanta[2],0,0,0,0,30,30);
			draw_sprite(buffer,fanta[2],_xV,_yV);
		}
		else{
			blit(fantabmp[4],fanta[2],0,0,0,0,30,30);
			draw_sprite(buffer,fanta[2],_xV,_yV);
		}
	}
	else{
		blit(fantabmp[2],fanta[2],0,0,0,0,30,30);
		draw_sprite(buffer,fanta[2],_xV,_yV);
	}
}

//CUANDO PACMAN Y FANTASMA ROSA CHOCAN
void chocar_V(char **mapa,Jugador &jug){
	if((_yV== py && _xV==px) || (_yV== auxposy && _xV==auxposx) ){
		if(contPasos<=0){
			play_sample(muere,300,150,1000,0);
			for(int i=0; i<TAM; i++){
				clear(buffer);
				clear(pacman);
				dibujar_mapa(mapa,jug); 
				blit(muerbmp,pacman,i*33,0,0,0,33,33);
				draw_sprite(buffer,pacman,px,py);
				pantalla();
				rest(60);
				
			}
			px=30*10; 
			py=30*10;
			jug.vidas--;
		}
		else{
			bandera[2]++;
		}
	}

}

//FUNCIÓN GENERAL DE FANTASMA ROSA
void fantasma_Rosa(char **mapa,Jugador &jug){
	dibujar_fantasma_Rosa();
	chocar_V(mapa,jug);
	if(xVdir==0){
		if(mapa[_yV/30][(_xV-30)/30] !='X') _xV-=30;
		else{
			xVdir=rand()%4;
		}
	}
	
	if(xVdir==1){
		if(mapa[_yV/30][(_xV+30)/30] !='X') _xV+=30;
		else{
			xVdir=rand()%4;
		}
	}
	if(xVdir==2){
		if(mapa[(_yV-30)/30][_xV/30] !='X') _yV-=30;
		else{
			xVdir=rand()%4;
		}
	}
	if(xVdir==3){
		if(mapa[(_yV+30)/30][_xV/30] !='X') _yV+=30;
		else{
			xVdir=rand()%4;
		}
	}
	
	if (mapa[(_yV)/30][_xV/30]== 'C'){
		xVdir=rand()%4;
	}
	
	if(_xV<=-30){ 
		_xV=870; 
	}
	else if(_xV>=870){
			_xV= -30;	
		}
	
}

//FANTASMA AMARILLO

//DIBUJAR EN PANTALLA EL FANTASMA AMARILLO(ojitos,azul,original)
void dibujar_fantasma_ama(){
	if(contPasos>0){
		if(bandera[3]!= 0){
			blit(fantabmp[5],fanta[3],0,0,0,0,30,30);
			draw_sprite(buffer,fanta[3],_xM,_yM);
		}
		else{
			blit(fantabmp[4],fanta[3],0,0,0,0,30,30);
			draw_sprite(buffer,fanta[3],_xM,_yM);
		}
	}
	else{
		blit(fantabmp[3],fanta[3],0,0,0,0,30,30);
		draw_sprite(buffer,fanta[3],_xM,_yM);
	}
}

//CUANDO PACMAN Y FANTASMA AMARILLO CHOCAN
void chocar_M(char **mapa,Jugador &jug){
	
	if((_yM== py && _xM==px) || (_yM== auxposy && _xM==auxposx) ){
		if(contPasos<=0){
			play_sample(muere,300,150,1000,0);
			for(int i=0; i<TAM; i++){
				clear(buffer);
				clear(pacman);
				dibujar_mapa(mapa,jug); 
				blit(muerbmp,pacman,i*33,0,0,0,33,33);
				draw_sprite(buffer,pacman,px,py);
				pantalla();
				rest(60);
				
			}
			px=30*10; 
			py=30*10;
			jug.vidas--;
		}
		else{
			bandera[3]++;
		}
	}
}

//FUNCIÓN GENERAL DE FANTASMA AMARILLO
void fantasma_ama(char **mapa,Jugador &jug){
	dibujar_fantasma_ama();
	chocar_M(mapa,jug);
	if(xMdir==0){
		if(mapa[_yM/30][(_xM-30)/30] !='X') _xM-=30;
		else{
			xMdir=rand()%4;
		}
	}
	
	if(xMdir==1){
		if(mapa[_yM/30][(_xM+30)/30] !='X') _xM+=30;
		else{
			xMdir=rand()%4;
		}
	}
	if(xMdir==2){
		if(mapa[(_yM-30)/30][_xM/30] !='X') _yM-=30;
		else{
			xMdir=rand()%4;
		}
	}
	if(xMdir==3){
		if(mapa[(_yM+30)/30][_xM/30] !='X') _yM+=30;
		else{
			xMdir=rand()%4;
		}
	}
	
	if (mapa[(_yM)/30][_xM/30]== 'C'){
		xMdir=rand()%4;
	}
	
	if(_xM<=-30){ 
		_xM=870; 
	}
	else if(_xM>=870){
			_xM= -30;	
		}
}

//GUARDAR JUEGO(guardar matriz tipo char en un arch txt)
void guardar_mapa(char **mapa, Jugador &jug){
	FILE *arch;
	char aux[TAMC+4];
	strcpy(aux,jug.nombre);
	strcat(aux,".txt");
	arch=fopen(aux,"w");
	if(arch==NULL){
		return ;
	}
	rewind(arch);
	for(int i=0;i<MAXFILAS;i++){
			for(int j=0;j<MAXCOLS;j++){
				fputc(mapa[i][j],arch);
			}
	}
	fclose(arch);
	jug.nivel=3;
}

//FUNCIÓN PLAY/CONTINUE(ciclo de juego)
void play(Jugador &jug,char **mapa){
	
	mapa=obtener_mapa(mapa,jug);
	
	while(!key[KEY_ESC] && finalizar(mapa,jug) && jug.vidas>0){
		auxposx= px;
		auxposy= py;
		
		
		if(key[KEY_RIGHT]) dir = 1;
		else if(key[KEY_LEFT]) dir = 0;
		else if(key[KEY_UP]) dir = 2;
		else if(key[KEY_DOWN]) dir = 3;
		
		if(dir == 0){
			if(mapa[py/30][(px-30)/30]!='X'){
				px-=30;
				contPasos-=30;
			}
			else dir=4;
		}
		if(dir == 1){
			if(mapa[py/30][(px+30)/30]!='X'){
				px+=30;
				contPasos-=30;
			}
			else dir=4;
		}
		if(dir == 2){
			if(mapa[(py-30)/30][px/30]!='X'){
				py-=30;
				contPasos-=30;
			}
			else dir=4;
		}
		if(dir == 3){
			if(mapa[(py+30)/30][px/30]!='X'){
				py+=30;
				contPasos-=30;
			}
			else dir=4;
		}
		
		if(px<=-30){ 
		px=870; 
		}
		else if(px>=870){
		px= -30;	
		}
		
		clear(buffer);
		dibujar_mapa(mapa,jug); 
		dibujar_pacman();
		fantasma(mapa,jug);
		fantasma_azul(mapa,jug);
		fantasma_Rosa(mapa,jug);
		fantasma_ama(mapa,jug);
		pantalla();
		rest(70);
		clear(pacman);
		blit(pacbmp,pacman,4*33,0,0,0,33,33);
		draw_sprite(buffer,pacman,px,py);			
		pantalla();
		rest(90);
		com_frutas(mapa,jug);	
	}
	guardar_mapa(mapa,jug);
	
	//Pantalla de fin de juego
	if(jug.vidas==0){
		while(!key[KEY_ENTER]){
			draw_sprite(buffer,gameover,0,0);
			pantalla();
		}	
	}
	clear(buffer);
}

//TOTAL DE REGISTROS EN ARCHIVO
int num_registro(){

	FILE *arch;
	
	arch=fopen("archgeneral.dat","rb");
	if(arch==NULL){
		return 1;
	}

	fseek(arch,0,SEEK_END);
	int n=ftell(arch)/sizeof(Jugador);
	
	return n;
}

//ORDENAR RANKING
//(Se guardan los datos jug.score y jug.nombre en un vector)
void ordenar_rank(Posicion pos[],int n){
	
	FILE *arch;
	Jugador aux;
	
	arch=fopen("archgeneral.dat","rb");
	if(arch==NULL){
		return;
	}
	
	rewind(arch);
	for(int i=0;i<n;i++){
		fread(&aux,sizeof(Jugador),1,arch);
		strcpy(pos[i].nombre,aux.nombre);
		pos[i].score=aux.score;
	}	
	fclose(arch);
	
	//Ordenar los datos de mayor a menor score
	Posicion pos1;
	for(int j=0; j<n; j++){
		for(int k=0; k<n-1; k++){
			if (pos[k].score <= pos[k+1].score){
				
                strcpy(pos1.nombre,pos[k].nombre);
                pos1.score = pos[k].score;
                
                strcpy(pos[k].nombre,pos[k+1].nombre);
                pos[k].score = pos[k+1].score;
                
				strcpy(pos[k+1].nombre,pos1.nombre);
                pos[k+1].score = pos1.score;
			}
		}
	}
	
}
	
//MOSTRAR RANKING
void mostrar_ranking(Posicion pos[],int n){
	
	//Mostrar los primeros 5 en la pantalla
	int ejey=330, ejex=0;
	int recorrer=5;
	if(n<recorrer){
		recorrer=n;
	}
	
	for(int l=0; l<recorrer; l++){
		ejex=370;
		textprintf_ex(buffer,font1[0],ejex,ejey,BLANCO,-1,"%s",pos[l].nombre);
		ejex=870;
		textprintf_ex(buffer,font1[0],ejex,ejey,BLANCO,-1,"%d",pos[l].score);
		ejey+=60;
	}
	
}

//FUNCIÓN DE RANKING
void ranking(){
	//Variables
	int n=num_registro();
	Posicion pos[n];
	ordenar_rank(pos,n);
	
	while(aux!=false){
		if(mouse_x>30 && mouse_x<180 &&
        	mouse_y>25 && mouse_y<65){
        	draw_sprite(buffer,rank[0],0,0);
        	if(mouse_b & 1){
        		manu();
        	}	
      	}else draw_sprite(buffer,rank[1],0,0);
      		mostrar_ranking(pos,n);
      		masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,13,22); 	
			pantalla();
	}	
}

//MENU PRINCIPAL(play,continue,scores,exit)
void manu(){
	//Estructura general
	Jugador jug;
	
	//Archivo general
	FILE* archgeneral;
	
	//Mapa
	char **mapa;
	mapa=creardin_mapa();
	
	while(aux!=false){
		if(mouse_x>422 && mouse_x<615 &&
           mouse_y>300 && mouse_y<348){
           	
            blit(menu[1],buffer,0,0,0,0,1040,700);
            play_sample(inicio,300,150,1000,0);
            
            if(mouse_b & 1){
            	
            	captura(jug,NOMBRE);
            	captura(jug,CONTRAS);
            	
            	if(validacion(jug,archgeneral,NUEVO)==NUEVO){
            		/*Variable timer, se da un valor solo
					 cada que es nueva partida*/
					timer=999;
            		play(jug,mapa);
            		datos(jug,archgeneral);
				}
				aux=true;
            }
        }
        else if(mouse_x>340 && mouse_x<693 &&
                mouse_y>387 && mouse_y<439){
                	
            blit(menu[2],buffer,0,0,0,0,1040,700);
            play_sample(inicio,300,150,1000,0);
            
            if(mouse_b & 1){
            	
				captura(jug,NOMBRE);
            	captura(jug,CONTRAS);
            	
            	if(validacion(jug,archgeneral,CONTINUAR)==CONTINUAR){
            		
            		play(jug,mapa);
            		datos(jug,archgeneral);
            		
				}
				aux=true;
            }
        }
        else if(mouse_x>387 && mouse_x<650 &&
                mouse_y>484 && mouse_y<534){
                	
            blit(menu[3],buffer,0,0,0,0,1040,700);
            play_sample(inicio,300,150,1000,0);
            
            if(mouse_b & 1){
            	
            	aux=true;         		
            	ranking();
            	clear(buffer);
            	
            }
        }
        else if(mouse_x>440 && mouse_x<615 &&
                mouse_y>570 && mouse_y<620){
                	
            blit(menu[4],buffer,0,0,0,0,1040,700);
            play_sample(inicio,300,150,1000,0);
            
            if(mouse_b & 1){
            	
            aux=false;
            
            }
        }else blit(menu[0],buffer, 0, 0, 0, 0,1040,700);
        masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,13,22);
        blit(buffer,screen,0,0,0,0,1040,700);
	}
	liberarMapa(mapa);
}



