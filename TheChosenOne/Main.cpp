#include <allegro.h>
#include <math.h>

volatile long counter = 0;
void Increment () { counter++; }

int main()
{
	allegro_init ();
	install_keyboard();
	set_color_depth(16);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,900,600,0,0);
	BITMAP *buffer = create_bitmap(900,600);

	LOCK_VARIABLE(counter);
	LOCK_FUNCTION(Increment);
	install_int_ex(Increment,BPS_TO_TIMER (35));

	FONT *font1 = load_font("assets/fonts/font1.pcx",NULL,NULL);
	srand(time(NULL));   

pocetak:

	int tezina = 0;
	bool pzagenerisanjecol = true,provera1 = true;
	
	while(provera1 = true)
	{
		textout_ex(buffer,font1,"Izaberite tezinu",430,295,makecol(0,0,255),-1);
		textout_ex(buffer,font1,"Q = easy",430,325,makecol(0,0,255),-1);
		textout_ex(buffer,font1,"W = hard",430,355,makecol(0,0,255),-1);
		textout_ex(buffer,font1,"E = nightmare",430,385,makecol(0,0,255),-1);
		
		blit(buffer,screen,0,0,0,0,900,600);
		clear_bitmap(buffer);

		if(key[KEY_Q]) 
		{
			tezina = 5;
			goto pocetak2;
		}
		if(key[KEY_W]) 
		{
			tezina = 8;
			goto pocetak2;
		}
		if(key[KEY_E]) 
		{
			tezina = 11;
			goto pocetak2;
		}
		if(key[KEY_ESC]) goto kraj2;
	}

pocetak2:

	int genplat = 0; //generator koji odredjuje na kojoj ce platformi da se generise collectable
	int x=250,y=135; // koordinate igraca
	int velx = 0 , vely = 0 ; // pomocne kordinate za kretanje
	int groundlevel2=135,groundlevel1=285,groundlevel3=385;; // visine zemlje 1 , 2 , 3 
	int en2x = 400,en2y = 135,pomEn2=tezina; //koordinate neprijatelja 2
	int en1x = 100,en1y = 285,pomEn1=tezina-3; // koordinate neprijatelja 1
	int en3x = 800,en3y = 385,pomEn3=tezina-2; // koordinate neprijatelja 3
	int collx = 0 , colly = 0; // koordinate collectabla
	int skor=-1;
	const int Gravity = 1; // gravitacija
	
	while(!key[KEY_ESC]) // glavna while petlja
	{
		while(counter > 0)
		{

		textout_ex(buffer,font1,"Z = jump",0,0,makecol(0,0,255),-1); //uputstvo
		textout_ex(buffer,font1,"R = reset",0,25,makecol(0,0,255),-1); //uputstvo
		textout_ex(buffer,font1,"-> = move right",0,50,makecol(0,0,255),-1); //uputstvo
		textout_ex(buffer,font1,"<- = move left",0,75,makecol(0,0,255),-1); //uputstvo
		textout_ex(buffer,font1,"Jump in pit to win :)",0,100,makecol(0,255,0),-1); //uputstvo

		if(key[KEY_RIGHT]) velx = 5; //movement right
		else if(key[KEY_LEFT]) velx = -5; //movement left 
		else velx = 0; //movement stay
		if(key[KEY_Z] && y == groundlevel2 && x>235 && x<600 || key[KEY_Z] && y == groundlevel1 && x>-15 && x<200 || key[KEY_Z] && y == groundlevel3 && x>635 && x<900  ) vely = -10; //movement jump

		if(vely<10) //ogranicenje gravitacije
		vely += Gravity; //gravitacija
		x += velx; //kretanje
		y += vely; //kretanje

		if(y>=135 && y<=150 && x >= 240 && x<=600) y=groundlevel2; //zemlja 2
		if(y>=285 && y<=300 && x >= -15 && x<=199) y=groundlevel1; //zemlja 1
		if(y>=385 && y<=400 && x >= 635 && x<=900) y=groundlevel3; //zemlja 3
		
		if(y>585) //kraj igre (pad u rupu)
		{
			clear_to_color(screen,makecol(0,0,0));
			goto kraj;
		}

		if(y>150 && x>=235 && x<=400) x=235; // ogranicenje platforme 2 sa leve strane
		if(y>150 && x<=600 && x>=250) x=600; //ogranicenje platforme 2 sa desne strane
		if(y>300 && x<=200 && x>=0) x=200; // ogranicenje platforme 1 sa desne strane
		if(y>400 && x<=885 && x>=635) x=635; //ogranicenje platforme 3 sa leve strane 

		if(x<0) x = 0;     //ogranicenje buffera sa leve strane 
		if(x>=885) x = 885; //ogranicenje buffera sa desne strane 

		if(key[KEY_R]) //reset game
		{
			x=250;
			y=135;
			skor = 0;
		}

		en2x = en2x + pomEn2;                                   // pomeranje neprijatelja 2
		if(en2x >= 570) pomEn2 = pomEn2 - pomEn2 - pomEn2;      // pomeranje neprijatelja 2
		if(en2x <= 290) pomEn2 = (pomEn2 * -1) ;                // pomeranje neprijatelja 2

		en1x = en1x + pomEn1;                                   // pomeranje neprijatelja 1
		if(en1x >= 185) pomEn1 = pomEn1 - pomEn1 - pomEn1;      // pomeranje neprijatelja 1
		if(en1x <= 0)   pomEn1 = (pomEn1 * -1);                 // pomeranje neprijatelja 1

		en3x = en3x + pomEn3;                                   // pomeranje neprijatelja 3
		if(en3x >= 890) pomEn3 = pomEn3 - pomEn3 - pomEn3;      // pomeranje neprijatelja 3
		if(en3x <= 650) pomEn3 = (pomEn3 * -1);                 // pomeranje neprijatelja 3
			
		if( x<=en2x+15 && x>=en2x-15 && y>=en2y-15 && y<=en2y ) goto kraj; // napad neprijatelja 2
		if( x<=en1x+15 && x>=en1x-15 && y>=en1y-15 && y<=en1y ) goto kraj; // napad neprijatelja 1
		if( x<=en3x+15 && x>=en3x-15 && y>=en3y-15 && y<=en3y ) goto kraj; // napad neprijatelja 3
															   										   
		if( x<=collx+10 && x>=collx-15 && y>=colly-15 && y<=colly+10) pzagenerisanjecol = true;   //skupljanje kocke
		
		if( pzagenerisanjecol == true ) // generisanje nove collectable
		{
			genplat = rand() % 3 + 1; 
			pzagenerisanjecol = false;

			if(genplat == 2) // generisanje nove collectable plat 2                  
			{                                  
				collx = rand() % 340 + 250;    
				colly = rand() % 30 + 110;     
				skor++;                        
			}                                  
			if(genplat == 1)  // generisanje nove collectable plat 1                 
			{
				collx = rand() % 160 + 30;
				colly = rand() % 30 + 260;
				skor++;
			}
			if(genplat == 3) // generisanje nove collectable plat 3
			{
				collx = rand() % 220 + 650;
				colly = rand() % 30 + 360;
				skor++;
			}
		} 
		
		if(x<=5+20 && x>=5-15 && y>=240-15 && y<=240+20) //ulaz u portal levi
		{
			x=350;
			y=135;
		}
		if(x<=875+20 && x>=875-15 && y>=360-15 && y<=360+20) //ulaz u portal desni
		{
			x=550;
			y=135;
		}

		rect(buffer,250,150,600,750,makecol(0,255,0)); //zemlja 2
		rect(buffer,0,300,200,600,makecol(0,255,0)); // zemlja 1
		rect(buffer,650,400,900,600,makecol(0,255,0)); // zemlja 3
		rect(buffer,5,240,25,260,makecol(255,50,255)); //portal levi
		rect(buffer,875,360,895,380,makecol(255,50,255)); //portal desni
		rectfill(buffer,x,y,x+15,y+15,makecol(200,200,0));// player
		rectfill(buffer,en2x,en2y,en2x+15,en2y+15,makecol(0,0,255)); // enemy 2
		rectfill(buffer,en1x,en1y,en1x+15,en1y+15,makecol(0,0,255)); //enemy 1
		rectfill(buffer,en3x,en3y,en3x+15,en3y+15,makecol(0,0,255)); // enemy 3
		rectfill(buffer,collx,colly,collx+10,colly+10,makecol(200,50,255)); // collectable

		textprintf_ex(buffer,font1,550,3,makecol(200,50,50),-1,"Skor = %d",skor); //skor
		textprintf_ex(buffer,font1,250,3,makecol(200,50,50),-1,"X = %d",x); // trenutni X
		textprintf_ex(buffer,font1,400,3,makecol(200,50,50),-1,"Y = %d",y); // trenutni Y
		
		blit(buffer,screen,0,0,0,0,900,600);
		
		clear_bitmap(buffer);// brisanje trenutnog buffera da nebi dosto do stackovanje slika
		counter--;
	}
	
	}
kraj:

	while(!key[KEY_ESC]) 
	{
		textout_ex(screen,font1,"GAME OVER",440,300,makecol(255,0,0),-1);
		rest(2000);
		goto pocetak;
	}
	
kraj2:	

	destroy_bitmap(buffer);
	return 0;
	
	
}
END_OF_MAIN()