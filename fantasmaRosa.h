class fantasma_rosa {
	public:
		int _x= 30*14, _y=30*19; 
		int xdir= 0; 
		void dibujar_fantasma(){
			if(contPasos>0){
				if(bandera[2]!= 0){
					blit(fantabmp[5],fanta[2],0,0,0,0,30,30);
					draw_sprite(buffer,fanta[2],_x,_y);
				}
				else{
					blit(fantabmp[4],fanta[2],0,0,0,0,30,30);
					draw_sprite(buffer,fanta[2],_x,_y);
				}
			}
			else{
				blit(fantabmp[2],fanta[2],0,0,0,0,30,30);
				draw_sprite(buffer,fanta[2],_x,_y);
			}
		}		
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
					bandera[2]++;
				}
			}

		}
		void fantasma_mover(char **mapa,Jugador &jug){
			dibujar_fantasma();
			chocar(mapa,jug);
			if(xdir==0){
				if(mapa[_y/30][(_x-30)/30] =='Y' || mapa[_y/30][(_x-30)/30] =='o' || mapa[_y/30][(_x-30)/30] =='C' || mapa[_y/30][(_x-30)/30] =='B' || mapa[_y/30][(_x-30)/30] ==' ') _x-=30;
				else{
					xdir= rand()%4;
				}
			}
			
			if(xdir==1){
				if(mapa[_y/30][(_x+30)/30] =='Y' || mapa[_y/30][(_x+30)/30] =='o' || mapa[_y/30][(_x+30)/30] =='C' || mapa[_y/30][(_x+30)/30] =='B' || mapa[_y/30][(_x+30)/30] ==' ') _x+=30;
				else{
					xdir= rand()%4;
				}
			}
			if(xdir==2){
				if(mapa[(_y-30)/30][_x/30] =='Y' || mapa[(_y-30)/30][_x/30] =='o' || mapa[(_y-30)/30][_x/30] =='C' || mapa[(_y-30)/30][_x/30] =='B'||mapa[(_y-30)/30][_x/30] ==' ') _y-=30;
				else{
					xdir= rand()%4;
				}
			}
			if(xdir==3){
				if(mapa[(_y+30)/30][_x/30] =='Y'||mapa[(_y+30)/30][_x/30] =='o'||mapa[(_y+30)/30][_x/30] =='C'||mapa[(_y+30)/30][_x/30] =='B'||mapa[(_y+30)/30][_x/30] ==' ') _y+=30;
				else{
					xdir= rand()%4;
				}
			}
			
			if (mapa[(_y)/30][_x/30]== 'C'){
				xdir=rand()%4;
			}	
			
			if(_x<=-0){ 
				_x=840; 
			}
			else if(_x>=840){
					_x= 0;	
				}
			
		}
};
