#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#define TAILLE_X 31
#define TAILLE_Y 101
#define TEMPS_AFFICHAGE 1
#define TAUXDEMUR 48	//-->une chance sur X d'avoir un mur

//0 --> vide
//1 --> murs horizontaux de la bordure
//2 --> murs verticaux de la bordure
//3 --> murs obstacles
//4 --> coffres
//5 --> vie
//6 --> ?
//7 --> ?
//11 --> petits monstres (avec 1 point de vie)
//21 ou 22 --> gros monstres (avec 1 ou 2 points de vie)
//1xy --> personnage avec x arme qui regarde y direction

void creerZone(int t[TAILLE_X][TAILLE_Y]){
	int i,j=0;
	int nbInitialMonstres=(rand()%8)+3; //au moins 3 monstres et au plus 10 monstres

	for (i=0;i<TAILLE_X;i++){
		for(j=0;j<TAILLE_Y;j++){
			if(i==0 || (i==TAILLE_X-1 && (j!=0 && j!=TAILLE_Y-1))){	//On définit la bordure d'écran verticale
				t[i][j]=1;		
			}
			else if(j==0 || j==TAILLE_Y-1 ){
				t[i][j]=2;
			}			

			//une chance sur 40 d'avoir un mur, et si il y a déjà un mur à côté, 1 chance sur 2 d'en re-avoir un.
			else if(rand()%TAUXDEMUR==15 || (t[i][j-1]==3 && rand()%3==1) || (t[i-1][j]==3 && rand()%2==1) )
				t[i][j]=3;
			else{
				t[i][j]=0;
			}

		}
	}

	int nbMur = 0;
	int k = 0;	

	t[1][TAILLE_Y-2]=3;
	t[1][1]=3;
	t[TAILLE_X-2][1]=3;
	t[TAILLE_X-2][TAILLE_Y-2]=3;
	for(k=0;k<=10;k++){
		for(i=1;i<TAILLE_X-1;i++){
			for(j=1;j<TAILLE_Y-1;j++){
				nbMur = 0;
				if(t[i][j]==0){
					if(t[i-1][j]<=3 && t[i-1][j]!=0){nbMur++;}
					if(t[i+1][j]<=3 && t[i+1][j]!=0){nbMur++;}
					if(t[i][j-1]<=3 && t[i][j-1]!=0){nbMur++;}
					if(t[i][j+1]<=3 && t[i][j+1]!=0){nbMur++;}
					if(nbMur>=3){t[i][j]=3;}
				}
			}
		}
	}


	int nbTrou = 0;

	for(i=1;i<TAILLE_X-1;i++){		//On enlève les murs tous seuls
		for(j=1;j<TAILLE_Y-1;j++){
			nbTrou = 0;
			if(t[i][j]==3){
				if(t[i-1][j]<=2){nbTrou++;}
				if(t[i+1][j]<=2){nbTrou++;}
				if(t[i][j-1]<=2){nbTrou++;}
				if(t[i][j+1]<=2){nbTrou++;}
				if(nbTrou==4){t[i][j]=0;}
			}
		}
	}
	int xCoffre = 0;
	int yCoffre = 0;
	do{						//On met un coffre au hasard sur la carte
		xCoffre=(rand()%(TAILLE_X-5))+5;
		yCoffre=(rand()%(TAILLE_Y-5))+5;
	}while(t[xCoffre][yCoffre]!=0);
	t[xCoffre][yCoffre]=4;

	t[14][50]=3;t[13][50]=3;t[12][50]=3;t[16][50]=3;t[17][50]=3;t[18][50]=3;
	t[15][49]=3;t[15][48]=3;
	t[15][51]=3;t[15][52]=3;
	t[14][49]=3;t[14][51]=3;t[16][49]=3;t[16][51]=3;
	t[15][50]=111;				//on rajoute le personnage avec une zone de mur autour pour empêcher les monstres de spawnent

	while(nbInitialMonstres>0){
		int typeMonstre=rand()%5;
		if(typeMonstre==4){typeMonstre=22;}	//1 chance sur 5 d'avoir un gros monstre --> 22, sinon un petit monstre --> 11
		else{typeMonstre=11;}
		int xMonstre=(rand()%(TAILLE_X-3))+3;	//on définit des zones de spawns aléatoires DANS le terrain
		int yMonstre=(rand()%(TAILLE_Y-3))+3;
		while(t[xMonstre][yMonstre]!=0){
			xMonstre=(rand()%(TAILLE_X-3))+3;
			yMonstre=(rand()%(TAILLE_Y-3))+3;
		}
		t[xMonstre][yMonstre]=typeMonstre;
		nbInitialMonstres--;

	}
	t[14][50]=0;t[13][50]=0;t[12][50]=0;t[16][50]=0;t[17][50]=0;t[18][50]=0;
	t[15][49]=0;t[15][48]=0;
	t[15][51]=0;t[15][52]=0;
	t[14][49]=0;t[14][51]=0;t[16][49]=0;t[16][51]=0;	

}


void monstre(int t[TAILLE_X][TAILLE_Y], int x, int y){
	if (t[x][y-1]!=0 && t[x][y+1]!=0 && t[x+1][y]!=0 && t[x-1][y]!=0){	//si le monstre est entouré de 4 murs, on le tue
		t[x][y] = 0;			//ATTENTION, il faudra penser à rajouter l'exception si il y a le joueur, pour que le monstre ne se suicide 
	}					//pas si il est entouré de 3murs et le joueur.
	int monstre=t[x][y];		//on regarde si c'est un petit ou un gros monstre (11 ou 21 ou 22)
	int n = (rand()%4);
	if(rand()%3!=0){
		if (n==0 && t[x-1][y]==0){
			//Avancer en haut
			t[x][y] = 0;
			t[x-1][y] = monstre;
		}
		else if(n<=1 && t[x][y+1]==0){
			//Avancer à droite
			t[x][y] = 0;
			t[x][y+1] = monstre;
		}
		else if (n<=2 && t[x][y-1]==0){
			//Avancer à gauche
			t[x][y] = 0;
			t[x][y-1] = monstre;
		}
		else if(n<=3 && t[x+1][y]==0){
			//Avancer en bas
			t[x][y] = 0;
			t[x+1][y] = monstre;
		}
	}
}

int monstreMort(int tab[TAILLE_X][TAILLE_Y],int x, int y){
	if(tab[x][y]==10 ||tab[x][y]==20){
		int type = (tab[x][y])/10;
		tab[x][y]=0;
		int scoreAugmente=type*10;
		return scoreAugmente;
	}
	else{ return 0;}
}


void afficher(int tab[TAILLE_X][TAILLE_Y], int vie, int score){
	int i=0;
	int j=0;
	int compteur = vie;

	system("@cls||clear"); //pour effacer l'écran
	printf("\n\n\n\n");
	printf("      Vie :   ");			//on affiche la barre d'informations.
	while(compteur>0){
		if(compteur%2==0){printf("<");}
		else{printf("3 ");}
		compteur--;
	}
	printf("                      score :   %d \n",score);

	for(i=0;i<TAILLE_X;i++){		//on affiche la zone de jeu
		for(j=0;j<TAILLE_Y;j++){
			switch(tab[i][j]){
				case 1: printf("_"); break;
				case 2: printf("|"); break;
				case 3: printf("."); break;
				case 4: printf("#"); break;
				case 5: printf("+"); break;
				case 11: printf("m"); break;
				case 21: printf("M"); break;
				case 22: printf("M"); break;
				default: if(tab[i][j]>=101 && tab[i][j]<=194){
						 switch(tab[i][j]%10){
							 case 1: printf("^"); break;
							 case 2: printf("<"); break;
							 case 3: printf("v"); break;
							 case 4: printf(">"); break;
						 }
					 }
					 else {printf(" ");}
			}	
		}
		printf("\n");
	}
}


int rechercheMonstresEtMonstresMorts(int tab[TAILLE_X][TAILLE_Y], int listeMonstres[11][2], int score){
	int i,j = 0;
	int compteur = 0;
	for(i=0;i<TAILLE_X;i++){
		for(j=0;j<TAILLE_Y;j++){
			if(tab[i][j]==10 ||tab[i][j]==20){
				score=score+monstreMort(tab,i,j);
			}
			if(tab[i][j]==11 || tab[i][j]==21 || tab[i][j]==22){
				listeMonstres[compteur][0]=i;
				listeMonstres[compteur][1]=j;
				compteur++;
			}
		}
	}
	if(compteur > 10){
		printf("erreur, trop de monstre sur la zone, la limite étant 10\n");	
	}
	else{
		listeMonstres[compteur][0]=-1;
		listeMonstres[compteur][1]=-1;
	}
	return score;
}

int nbMonstres(int listeMonstres[11][2]){ //attention à bien s'asurer que le tableau est à jour quand on utilise la fonction.
	int i, compteur = 0;
	for(i=0;listeMonstres[i][0]!=-1;i++){
		compteur++;
	}
	return compteur;
}

void bougerMonstres(int tab[TAILLE_X][TAILLE_Y], int listeMonstres[11][2], int score){
	score=rechercheMonstresEtMonstresMorts(tab,listeMonstres,score); //sert juste à mettre à jour la liste de monstres
	int nombreMonstres=nbMonstres(listeMonstres);
	while(nombreMonstres>0){
		monstre(tab,listeMonstres[nombreMonstres-1][0],listeMonstres[nombreMonstres-1][1]);
		nombreMonstres--;
	}
}

void deplacement(int tab[TAILLE_X][TAILLE_Y], int x, int y) {
	char c;
        int n;
	int dir;
        n = read(0, &c, 1);
        if (n>=1) {
                if (c=='z') {
			dir = 1;
			if (tab[x-1][y]==0) {
				tab[x][y] = 0;
				x--;
			}
		}
		else if (c=='q') {
			dir = 2;
			if (tab[x][y-1]==0) {
				tab[x][y] = 0;
				y--;
			}
		}
		else if (c=='s') {
			dir = 3;
			if (tab[x+1][y]==0) {
				tab[x][y] = 0;
				x++;
			}
		}
		else if (c=='d') {
			dir = 4;
			if (tab[x][y+1]==0) {
				tab[x][y] = 0;
				y++;
			}
		}

		tab[x][y] = 100 + dir;
        }
}

int main () {
        fcntl(0, F_SETFL, O_NONBLOCK);
        struct termios terminal;
        tcgetattr(0, &terminal);
        terminal.c_lflag &= ~(ICANON|ECHO);
        terminal.c_cc[VTIME] = 1;
        terminal.c_cc[VMIN] = 1;
        tcsetattr(0, TCSANOW, &terminal);

	int t[TAILLE_X][TAILLE_Y];
	int jeuFini = 0;			//variable =1 si le joueur a gagné ou 2 si il est mort
	int listeMonstres[11][2];		//liste de coordonnées des monstres, on considère qu'il y en aura au plus 10 (avec un -1 à la fin)
	srand(time(NULL));
	int nbMonstre = 0;
	int score = 0;
	int vie = 6;

	creerZone(t);	//Initialisation de la zone

	while(jeuFini == 0){
		afficher(t,vie,score);
		score=rechercheMonstresEtMonstresMorts(t,listeMonstres,score); //le score augmentera si un monstre vient de mourrir
		nbMonstre=nbMonstres(listeMonstres);
		bougerMonstres(t,listeMonstres,score);
		deplacement(t, 15, 50);

		if(listeMonstres[0][0]==-1){jeuFini=1;} //le joueur a gagné
		else if(vie==0){jeuFini=2;}		// le joueur a perdu
		sleep(TEMPS_AFFICHAGE);

	}

	if(jeuFini==1){
		printf("\n Bien joué ! \n");
	}
	else if(jeuFini==2){
		printf("\n Vous êtes mort ! \n");
	}
	printf("Vous avez obtenu un score de : %d ! \n",score);
	return 0;
}


