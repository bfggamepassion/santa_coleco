/**************************************************/
/*** HELIFIRE *************************************/
/**************************************************/
#include <coleco.h>
#include <getput1.h>

#define chrtab  0x1800 /* écran en linéaire */
#define chrgen  0x0000 /* table des caractères */
#define coltab  0x2000 /* couleur des caractères */
#define sprtab  0x3800 /* sprite_pattern_table */
#define sprgen  0x1b00 /* sprite_attribute_table */
#define buffer  0x1c00 /* buffer screen 2 */

#define S_TRUE 1
#define S_FALSE 0

#define GAUCHE	0
#define DROITE	1

#define INACTIF 204
#define INVISIBLE 203

#define EN_GIFT1 1	// gift déplacement horizontal vitesse 1 gauche
#define EN_GIFT2 2	// gift déplacement horizontal vitesse 2 gauche
#define EN_GIFT3 3	// gift déplacement horizontal vitesse 3 gauche
#define EN_BOMB 4



#define MAXENNEMYSPRITE 25 	/* Nombre d'ennemies et de tir ennemis en sprite maximum*/
#define MAXSCROLLOBJ 5

/* from sound.s */
extern const void snd_table[];

extern const byte scrollObj_Init[];
extern const byte* scrollObj_Pointer[];
extern const byte SPATTERNRLE[];
extern const byte PATTERNRLE[];
extern const byte COLORRLE[];

/* from title.c */
extern const byte TITLE_PATRLE[];
extern const byte TITLE_COLRLE[];

/* from scroll.c : decors pre-decales au pixel */
extern const byte* const scrollObj_Frames[];
extern const byte SKY_FIRST;
extern const byte SKY_PATRLE[];
extern const byte SKY_COLRLE[];
extern const byte GND_FIRST;
extern const byte GND_PATRLE[];
extern const byte GND_COLRLE[];

#define SND_MELODY 0x15	/* Jingle Bells, voir xmas.s */
#define SND_BASS 0x16
void playMusic();
void stopMusic();

#define SCROLLOBJ_ARBRE 0
#define SCROLLOBJ_BONHOMME 1

/* from vdpfast.s */
void vdp_put_rect(unsigned vram_addr, void *src, byte n, byte w, byte h);

// Variables pour gestion du flickering
byte currentFlicker;
sprite_t bsprites[32];
byte odd;
byte timer8;
byte timer16;
byte timer32;
byte timer64;
byte timer128;
byte timer2;
byte timer256;
byte gameOver;

byte current_wave; // Wave courante à afficher
byte var1_wave;		// Variable globale pour la wave courante
byte var2_wave;		// variable globale pour la wave courante

byte ligne[11];

byte catched;
unsigned level;

typedef struct
{
	byte sprno1;	
	byte sprno2;	
	unsigned score;	
	byte cpt;
	byte miss;
} s_player;

s_player player;

typedef struct
{
	//byte actif;
	byte sprno;
} s_shoot;

// 1 Shoot par joueur
s_shoot shoot;

typedef struct
{
	byte type; // Type d'ennemie (INACTIF possible)
	byte sprno;
	byte timer;			// Timer qui va servir à pas mal de chose ...
	byte sens;
	byte ligne;	// Ligne occupée par le sprite
} s_ennemySprite;

s_ennemySprite ennemySprite[MAXENNEMYSPRITE];

typedef struct
{
	//byte w; Tout est walkable
	byte p;	// Pattern à utiliser
	byte h;	//On va les mettre en fixe ici.
	byte l;
	char x;
	char y;
} s_scrollObj;

s_scrollObj scrollObj[MAXSCROLLOBJ];

// Decalage en pixels (7..0) a l'interieur du caractere, commun a tous les decors
byte scrollPhase;

// Affichage du score par la NMI (pas pendant l'écran titre)
byte showHud;


char getFreeScrollObj()
{
	byte i;
	
	for (i=0;i<MAXSCROLLOBJ;i++)
	if (scrollObj[i].x<=-10) return i;
	
	return -1;
}

byte createScrollObj(byte pattern,char cy,byte ha, byte la)
{
	char res;
	
	res = getFreeScrollObj();
	if (res==-1) return S_FALSE;
	
	scrollObj[res].p = pattern;
	scrollObj[res].h = ha;
	scrollObj[res].l = la;
	scrollObj[res].x = 32; // On crée en dehors de l'écran à droite
	scrollObj[res].y = cy;
	
	return S_TRUE;
}

void moveAndShowScrollObj()
{
	byte i,w,c0,c1;
	char x;
	s_scrollObj *o;
	
	o = scrollObj;
	for (i=0;i<MAXSCROLLOBJ;i++,o++)
	{
		if (o->x<=-10) continue;
		
		if (scrollPhase==7) o->x--;
		// Le sol avance par pas de 2 pixels : son image ne change qu'aux phases impaires
		if ((o->p<=SCROLLOBJ_BONHOMME) && !(scrollPhase&1)) continue;
		
		// Colonnes visibles c0..c1-1 de l'objet
		x = o->x;
		w = o->l;
		c0 = (x<0) ? -x : 0;
		c1 = (x+w>32) ? 32-x : w;
		if (c1<=c0) continue;
		
		vdp_put_rect(chrtab+(o->y<<5)+x+c0,(void *)(scrollObj_Frames[(o->p<<3)+scrollPhase]+c0),c1-c0,w,o->h);
	}
}

// Avance les decors d'un pixel vers la gauche
void scrollScenery()
{
	if (scrollPhase==0) scrollPhase = 7; else scrollPhase--;
	moveAndShowScrollObj();
}

// Charge les caracteres pre-decales : ciel dans le 1er tiers, sol dans le 3eme
void loadScrollChars()
{
	rle2vram((void *)SKY_PATRLE,chrgen+(SKY_FIRST<<3));
	rle2vram((void *)SKY_COLRLE,coltab+(SKY_FIRST<<3));
	rle2vram((void *)GND_PATRLE,chrgen+4096+(GND_FIRST<<3));
	rle2vram((void *)GND_COLRLE,coltab+4096+(GND_FIRST<<3));
}

void activateSprite(byte s,byte x,byte y,byte p,byte c)
{
	sprites[s].x = x;
	sprites[s].y = y;
	sprites[s].pattern = p;
	sprites[s].colour = c;
}

byte isCollision(byte x1,byte y1,byte h1,byte l1,byte x2,byte y2,byte h2,byte l2)
{
	if(x1 > x2+l2) return S_FALSE;
	if(x1+l1 < x2) return S_FALSE;
	if(y1 > y2+h2) return S_FALSE;
	if(y1+h1 < y2) return S_FALSE;
	
	return S_TRUE;
}

char getFreeSprite()
{
	int i;
	
	if (odd==0)
	{
		odd = 1;
		for (i=0;i<32;i++)
		{
			if (sprites[i].y==INACTIF) 
			{
				return i;
			}
		}
	}
	else
	{
		odd = 0;
		for (i=31;i>0;i--)
		{
			if (sprites[i].y==INACTIF) 
			{
				return i;
			}
		}
	}
	
	return -1;
}

char getFreeEnnemy()
{
	byte i;
	
	for (i=0;i<MAXENNEMYSPRITE;i++) 
	if (ennemySprite[i].type == INACTIF) return i;
	
	return -1;
}

void createEnnemySprite(byte t,byte x,byte y,byte p,byte c,byte se,byte l)
{
	char e,s;
	
	e = getFreeEnnemy();
	s = getFreeSprite();
	if ( (e!=-1) && (s!=-1) )
	{
		ennemySprite[e].type = t;								
		ennemySprite[e].sprno = s;
		activateSprite(ennemySprite[e].sprno,x,y,p,c);
		ennemySprite[e].sens = se;
		ennemySprite[e].ligne = l;
		ligne[l] = 1;
	}
}

// Générateur d'ennemies
// Doit fonctionner par vague selon le niveau de difficulté.
void generateEnnemy()
{
	byte ran,sentinelle;
	byte scrollObjNumber;
	byte scrollObjPosition;
	
	if (timer128==0)
	{
		// Sapins et bonshommes au sol (3eme tiers), etoiles et boules dans le ciel (1er tiers)
		scrollObjNumber = rnd_byte(0,3);
		if (scrollObjNumber==SCROLLOBJ_ARBRE) scrollObjPosition = 16;
		else
		if (scrollObjNumber==SCROLLOBJ_BONHOMME) scrollObjPosition = 18;
		else
			scrollObjPosition = rnd_byte(1,5);
		createScrollObj(scrollObjNumber,scrollObjPosition,scrollObj_Init[(scrollObjNumber<<2)+1],scrollObj_Init[(scrollObjNumber<<2)+2]);
	}
	
	// La précédente vague est terminée, on prépare la suivante
	if (var1_wave==0) 
	{	ran = rnd_byte(0,100); // On tire un chiffre entre 0 et 100
		if (player.cpt<=10) // selon le niveau ou l'on est, on tire une
		{					// vague selon le % trouvé.
			if (ran<=33)  current_wave = 0;
			else
			if (ran<=63)  current_wave = 1;
			else
			if (ran<=100) current_wave = 2;						
		}
		if (player.cpt>10)
		{
			if (ran<=25)  current_wave = 0;
			else
			if (ran<=50)  current_wave = 1;
			else
			if (ran<=75) current_wave = 2;						
			else
			if (ran<=100) current_wave = 3;						
		}
		if (player.cpt>20)
		{
			if (ran<=10)  current_wave = 0;
			else
			if (ran<=40)  current_wave = 1;
			else
			if (ran<=70) current_wave = 2;						
			else
			if (ran<=100) current_wave = 3;						
		}
		if (player.cpt>30)
		{
			if (ran<=10)  current_wave = 0;
			else
			if (ran<=36)  current_wave = 1;
			else
			if (ran<=62) current_wave = 2;						
			else
			if (ran<=90) current_wave = 3;						
			else
			if (ran<=100) current_wave = 4;						
		}
		if (player.cpt>40)
		{
			if (ran<=20)  current_wave = 1;
			else
			if (ran<=40)  current_wave = 2;
			else
			if (ran<=60) current_wave = 3;						
			else
			if (ran<=80) current_wave = 4;						
			else
			if (ran<=100) current_wave = 6;						
		}
		if (player.cpt>50)
		{
			if (ran<=25)  current_wave = 1;
			else
			if (ran<=50)  current_wave = 3;
			else
			if (ran<=75) current_wave = 4;						
			else
			if (ran<=100) current_wave = 6;								
		}
		
		
		var1_wave = 1; // Nombre d'objets de la vague
		if (current_wave==2)  var1_wave = 2;
		
		var2_wave = rnd_byte(2,10); // Position y
		
		sentinelle = 0;
		while ( (ligne[var2_wave]==1) && (sentinelle<20) )
		{		// TODO : Possibilité de bloquer le jeu !!
			var2_wave = rnd_byte(2,10);
			sentinelle++;
		}
		
		if (sentinelle==20) current_wave=0;
	}
	
	if ((current_wave==0) && (timer64==0))
	{
		var1_wave = 0;
	}
	else
	if ((current_wave==1)  && (timer64==0))// 1 Horizontal lent
	{

		createEnnemySprite(EN_GIFT1,240,var2_wave<<4,rnd_byte(4,7)<<2,1,GAUCHE,var2_wave);
		var1_wave = 0;
	}
	else
	if ((current_wave==2) && (timer32==0)) // 2 Horizontal lent
	{
		createEnnemySprite(EN_GIFT1,240,(var2_wave<<4),rnd_byte(4,7)<<2,1,GAUCHE,var2_wave);
		var1_wave --;
	}
	else
	if ((current_wave==3)  && (timer64==0))// 1 Horizontal moyen
	{

		createEnnemySprite(EN_GIFT2,240,var2_wave<<4,rnd_byte(4,7)<<2,5,GAUCHE,var2_wave);
		var1_wave = 0;
	}	
	else
	if (( (current_wave==4) || (current_wave==5) )  && (timer64==0))// 1 Horizontal rapide
	{
		ran = rnd_byte(0,100);
		createEnnemySprite(EN_GIFT3,240,var2_wave<<4,rnd_byte(4,7)<<2,13,GAUCHE,var2_wave);
		
		var1_wave = 0;
	}
	if ((current_wave==6) && (timer32==0))
	{
		//var2_wave = rnd_byte(2,10);
		createEnnemySprite(EN_BOMB,240,var2_wave<<4,8<<2,6,GAUCHE,var2_wave);
		var1_wave = 0;
	}
}

// Version optimisée (pointeurs) : même comportement que l'originale
void killEnnemy(s_ennemySprite *e, sprite_t *s)
{
	s->y = INACTIF;
	e->type = INACTIF;
	ligne[e->ligne] = 0;
}

void moveEnnemySprite()
{
	byte i,t;
	s_ennemySprite *e;
	sprite_t *s;
	
	e = ennemySprite;
	for (i=0;i<MAXENNEMYSPRITE;i++,e++)
	{
		t = e->type;
		if (t==INACTIF) continue;
		s = &sprites[e->sprno];
		
		if ((t==EN_GIFT1) || (t==EN_GIFT2))
		{
			if (e->sens==GAUCHE)
			{
				if ((t==EN_GIFT2) || (timer2==0)) s->x--;
			}
			else
			s->x+=3;
			
			if (s->x<5) 
			{
				killEnnemy(e,s);
				player.miss++;
			}
			
			if (s->x>200) e->sens = GAUCHE;
		}
		else
		if (t==EN_GIFT3)
		{
			s->x-=2;
			
			if (s->x<5) 
			{
				killEnnemy(e,s);
				player.miss++;
			}
		}
		else
		if (t==EN_BOMB)
		{
			if (timer2==0) s->x--;
			if (timer8==0) 
			{
				if (s->pattern == 32) s->pattern = 36;
				else s->pattern = 32;
			}

			if (s->x<5) killEnnemy(e,s);
		}
	}
}

byte checkCollision()
{
	byte j;
	
	// Info sur l'ennemi
	s_ennemySprite *this_en;
	byte *e_sprno;
	byte *e_type;	
	byte *e_sens;
	
	// Info sur le sprite de l'ennemi
	sprite_t *this_sen;
	byte *sen_x;
	byte *sen_y;
	
	// Info sur le joueur
	//s_player *this_player;
	byte pl_sprno2;

	// Info sur le sprite du joueur
	sprite_t *this_splayer;
	byte *spl_x;
	byte *spl_y;
	
	//Info sur le shoot
	//s_shoot *this_shoot;
	byte s_sprno;
	
	//Info sur le sprite du shoot
	sprite_t *this_sshoot;
	byte *ss_x;
	byte *ss_y;
	
	
	//if ( vdp_status==159 ) return S_FALSE;
	
	//	for (i=0;i<2;i++) // joueur
	//{
	//this_player = &player[i];
	pl_sprno2 = player.sprno2;
	
	this_splayer = &sprites[pl_sprno2];
	spl_x = &this_splayer-> x;
	spl_y = &this_splayer-> y;
	
	
	for (j=0;j<MAXENNEMYSPRITE;j++)
	{	
		// On récupère le type de l'ennemi
		this_en = &ennemySprite[j];
		e_type = &this_en->type;	
		e_sens = &this_en->sens;
		
		// Le sprite courant est un cadeau
		if ((*e_type) == EN_BOMB)
		{
			// On récupère le sprite de l'ennemi
			e_sprno = &this_en-> sprno;
			this_sen = &sprites[(*e_sprno)];
			sen_x = &this_sen->x;
			sen_y = &this_sen->y;
			
			if (*sen_x<53) // Si l'ennemie à une chance d'atteindre le joueur
			if (isCollision((*spl_x),(*spl_y),16,16,(*sen_x)+7,(*sen_y)+5,8,8) == S_TRUE)
			{
			
				sprites[player.sprno1].pattern = 40;
				sprites[player.sprno2].pattern = 40;
				stopMusic();
				stop_sound(4);stop_sound(5);
				play_sound(4);play_sound(5);
				delay(120);
				gameOver = 2;
			}
		}
		else
		if (( (*e_type)>(EN_GIFT1-1) ) && ((*e_type)<(EN_GIFT3+1)))
		{								
			// On récupère le sprite de l'ennemi
			e_sprno = &this_en-> sprno;
			this_sen = &sprites[(*e_sprno)];
			sen_x = &this_sen->x;
			sen_y = &this_sen->y;
			
			if (*sen_x<53) // Si l'ennemie à une chance d'atteindre le joueur
			if (isCollision((*spl_x),(*spl_y),16,16,(*sen_x),(*sen_y),16,16) == S_TRUE)
			{
				ennemySprite[j].sens = DROITE;
			}	

			// On récupère les infos du shoot
			//this_shoot = &shoot[i];
			s_sprno = shoot.sprno;
			this_sshoot = &sprites[s_sprno];
			ss_x = &this_sshoot->x;
			ss_y = &this_sshoot->y;
			
			if ((*ss_y)!=INVISIBLE)
			{				
				if ((*e_sens)==GAUCHE) // Le cadeau va t'il à gauche ?
				if (isCollision((*ss_x)+14,(*ss_y),16,2,(*sen_x),(*sen_y),16,16) == S_TRUE)
				{
					// On désactive l'ennemie
					if (ennemySprite[j].type == EN_GIFT1) {if (player.score<65000) player.score+=1;}
					else
					if (ennemySprite[j].type == EN_GIFT2) {if (player.score<65000) player.score+=2;}
					else
					{if (player.score<65000) player.score+=3;}
					
					stop_sound(3);play_sound(3);
					ennemySprite[j].type = INACTIF;
					(*sen_y) = INACTIF;
					ligne[ennemySprite[j].ligne] = 0;
					
					// On rend invisible le shoot sous-marin
					(*ss_y) = INVISIBLE; 	

					if (player.cpt<120) player.cpt++;
				}
			}
		}
	}		
	//}
	
	return S_FALSE;
}

void cls(void)
{
		byte i,j;
		
		for (i=0;i<32;i++)
		for (j=0;j<24;j++)
		put_char(i,j,32);
}

void changeFontColor(byte col1,byte col2)
{
	byte finalColor;
	
	finalColor = col1 << 4;
	finalColor+= col2;
	
	fill_vram(coltab+(32*8),finalColor,59*8);
	fill_vram(coltab+(32*8)+2048,finalColor,59*8);
	fill_vram(coltab+(32*8)+4096,finalColor,59*8);
}

void renderMiss(void)
{
	char *pointage;
	pointage = str(player.miss);

	print_at(16,0,"MISS : ");
	put_char(23,0,pointage[4]);

} 

void renderNumber(byte x,byte y,byte n)
{
	char *pointage;
	pointage = str(n);
	
	put_char(x,y,pointage[4]);
}

void loadCharset()
{
	load_patternrle(PATTERNRLE);
	duplicate_pattern();
	rle2vram(COLORRLE,coltab);
	rle2vram(COLORRLE,coltab+2048);
	rle2vram(COLORRLE,coltab+4096);
}

void playMusic()
{
	stop_sound(SND_MELODY);
	stop_sound(SND_BASS);
	play_sound(SND_MELODY);
	play_sound(SND_BASS);
}

void stopMusic()
{
	stop_sound(SND_MELODY);
	stop_sound(SND_BASS);
}

void titleScreen()
{
	screen_off();
	disable_nmi();
	rle2vram((void *)TITLE_PATRLE,chrgen);
	rle2vram((void *)TITLE_COLRLE,coltab);
	set_default_name_table(chrtab);
	screen_on();
	enable_nmi();
	playMusic();
	
	pause();
	
	screen_off();
	disable_nmi();
	loadCharset();
	cls();
	screen_on();
	enable_nmi();
}

void presentation()
{
	screen_off();
	disable_nmi();
	changeFontColor(6,15);
	cls();
	center_string(2,"CRAPAHUTE & BFG");
	center_string(4,"PRESENT");
	center_string(6,"SANTA MUST SAVE XMAS");
	center_string(10,"SANTA HAS LOST ALL");
	center_string(11,"THE XMAS GIFT");	
	center_string(14,"HELP HIM CATCH THEM ALL");
	center_string(15,"AND SAVE XMAS !");
	center_string(20,"PRESS FIRE TO START");
	
	screen_on();
	enable_nmi();
	
	pause();
}

void doGameOver()
{
	byte i;
	
	stopMusic();
	screen_off();
	disable_nmi();
	for (i=0;i<32;i++) sprites[i].y = 204;
	loadCharset();
	changeFontColor(6,15);
	cls();
	center_string(2,"GAME OVER");
	if (gameOver==1)	center_string(4,"YOU MISS 5 GIFTS");
	else center_string(4,"THE GRINCH EXPLODED SANTA !");
	
	center_string(10,"FINAL SCORE");
	center_string(11,str(player.score));
	print_at(19,11,"00");
	
	center_string(14,"GREETS GOES TO ALL COLECO FANS !");
	center_string(15,"PARTICULAR GREETINGS TO :");
	center_string(17,"YOUKI");
	center_string(18,"NEWCOLECO");
	center_string(22,"MERRY CHRISTMAS 2011 AND 2026 !");
	
	screen_on();
	enable_nmi();
	
	pause();
}

void main(void)
{
	byte i;
	byte move;
	
	set_snd_table(snd_table);
	stop_sound(2);
	stop_sound(3);
	stop_sound(4);
	stop_sound(5);
	/************************************************************/
	/** INITIALISATIONS VARIABLES *******************************/
	/************************************************************/
	currentFlicker=0;
	odd = 0;
	timer2=0;
	timer8=0;
	timer16=0;
	timer32=0;
	timer64=0;	
	timer128=0;
	timer256=0;
	catched = 0;
	level = 0;
	current_wave = 0;
	var1_wave = 0;
	var2_wave = 0;
	player.cpt = 0;
	player.miss=0;
	gameOver = 0;
	scrollPhase = 0;
	showHud = 0;

	for (i=0;i<MAXSCROLLOBJ;i++)
	{
		scrollObj[i].p = 0;
		scrollObj[i].x = -10;
		scrollObj[i].y = 0;
	}
	
	for (i=0;i<11;i++) ligne[i]=0;
	
	for (i=0;i<32;i++) sprites[i].y = INACTIF;
	for (i=0;i<MAXENNEMYSPRITE;i++) ennemySprite[i].type = INACTIF;
	for (i=0;i<MAXSCROLLOBJ;i++) scrollObj[i].x=-10;
	/************************************************************/
	/** FIN INITIALISATIONS VARIABLES ***************************/
	/************************************************************/
	
	screen_mode_2_bitmap();
	screen_off();
	rle2vram(SPATTERNRLE,sprtab);
	screen_on();
	enable_nmi();
	
	titleScreen();
	showHud = 1;
	presentation();
	
	disable_nmi();
	screen_off();
	changeFontColor(6,15);
	loadScrollChars();
	cls();
	screen_on();
	enable_nmi();
	
	// Initialisation joueur
	player.sprno1 = getFreeSprite();
	player.sprno2 = getFreeSprite();
	
	activateSprite(player.sprno1,5,100,0,6);
	activateSprite(player.sprno2,21,100,4,6);
	
	// Initialisation du shoot Joueur 1
	shoot.sprno = getFreeSprite();
	activateSprite(shoot.sprno,INVISIBLE,INVISIBLE,12,5);

	//	shoot[1].sprno = getFreeSprite();
	//	activateSprite(shoot[1].sprno,INVISIBLE,INVISIBLE,12,5);
	
	while(gameOver==0) 
	{
		generateEnnemy();
		if (timer2==0) checkCollision();
		
		if (currentFlicker<2) currentFlicker++; else currentFlicker=1;
		move = timer2;
		if ((joypad_1 & UP) && (sprites[player.sprno2].y>8)) { sprites[player.sprno1].y -=2;sprites[player.sprno2].y -=2;}
		if ((joypad_1 & DOWN) && (sprites[player.sprno2].y<(192-16))) { sprites[player.sprno1].y +=2;sprites[player.sprno2].y +=2;	}
		if ( (joypad_1 & FIRE1) || (joypad_1 & FIRE2) )
		{
			if (sprites[shoot.sprno].y==INVISIBLE)
			{
				sprites[shoot.sprno].x = sprites[player.sprno2].x+12;
				sprites[shoot.sprno].y = sprites[player.sprno2].y;
				stop_sound(2);play_sound(2);
			}
		}
		
		for (i=0;i<1;i++)
		{
			if (sprites[shoot.sprno].y!=INVISIBLE)
			{
				sprites[shoot.sprno].x += 2;
				if (sprites[shoot.sprno].x>80) 
				{
					sprites[shoot.sprno].y = INVISIBLE;
				}
			}
		}
		
		moveEnnemySprite();
			
		delay(1);
		
		// Juste apres le VBLANK : les decors avancent d'un pixel par image
		disable_nmi();
		scrollScenery();
		enable_nmi();

		if (timer8<7) timer8++; else timer8=0;			
		if (timer16<15) timer16++; else timer16=0;			
		if (timer32<31) timer32++; else timer32=0;			
		if (timer64<63) timer64++; else timer64=0;
		if (timer128<127) timer128++; else timer128=0;
		if (timer256<255) timer256++; else timer256=0;
		if (player.miss==5) gameOver = 1;
	}
	
	doGameOver();
}

void nmi()
{	
	if (currentFlicker==2) put_vram (0x1b00,sprites,128); 
	else 
	{
		memcpyb(bsprites,sprites+16,64);
		memcpyb(bsprites+16,sprites,64);
		put_vram (0x1b00,bsprites,128);				
	}
	

	if ((timer8==0) && showHud) {
		print_at(5,0,str(player.score));	
		print_at(10,0,"00");
		renderMiss();		
	}
			if (timer2<1) timer2++; else timer2=0;
}