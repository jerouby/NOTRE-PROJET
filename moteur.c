#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define TAILLE_X 60
#define TAILLE_Y 20

void creerZone(int t[TAILLE_X][TAILLE_Y]){
	int i,j=0;
	for (i=0;i<TAILLE_X;i++){
		for(j=0;j<TAILLE_Y;j++){
			if(i==0 || j==0 || i == TAILLE_X -1 || j == TAILLE_Y -1 || rand()%8==6){	//Insérer les murs ici
				t[i][j]=1;
			}		
			else if ((i==2 && j ==2)) {	//Insérer les coordonnées des monstres ici
				t[i][j]=2;		
			}
			else{
				t[i][j]=0;
			}

		}
	}


}

void monstre (int t[TAILLE_X][TAILLE_Y], int x, int y){
	int monstre=2;
	srand(time(NULL));
	int n = (rand()%3);

	if (n==0 && x<30 && y<20){
		//Avancer en avant
		t[x][y] = 0;
		t[x][y-1] = monstre;
	}
	else if(n==1 && x<30 && y<20){
		//Avancer à droite
		t[x][y] = 0;
		t[x+1][y] = monstre;
	}
	else if (n==2 && x<30 && y<20){
		//Avancer à gauche
		t[x][y] = 0;
		t[x-1][y] = monstre;
	}
	else if(n==3 && x<30 && y<20){
		//Avancer en bas
		t[x][y] = 0;
		t[x][y+1] = monstre;
	}

}


void affichage(int tab[TAILLE_X][TAILLE_Y]){
	int i=0;
	int j=0;
	for(i=0;i<TAILLE_X;i++){
		for(j=0;j<TAILLE_Y;j++){
			if(tab[i][j]==0){
				printf(" ");
			}
			else if(tab[i][j]==1){
				printf("#");
			}
			else if(tab[i][j]==2){
				printf("m");
			}
		}
		printf("\n");
	}
}


int main () {
	int t[TAILLE_X][TAILLE_Y];
	int jeu_fini = 0;			//variable =1 si le joueur a gagné ou 2 si il est mort
	int i,j, k = 0;
	int x,y = 2;		//Le monstre se situe 
	int monstres[10][2] = {0};	//un tableau qui contiendra la liste de coordonnées des monstres, on considère qu'il y en aura au plus 10
	srand(time(NULL));
	int nbMonstre = 0;
	creerZone(t);	//Initialisation de la zone

	while(jeu_fini == 0){
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		affichage(t);	//affichage de la zone
		monstre(t,x,y);	//Déplacement du monstre

		for (i=0;i<TAILLE_X;i++){		//recherche de la nouvelle position du monstre
			for(j=0;j<TAILLE_Y;j++){
				if(t[i][j]==2){
					monstres[nbMonstre][0]=i;
					monstres[nbMonstre][1]=j;
					nbMonstre++;		
				}
			}
		}
		while(monstres[k][0] != 0){ //tant qu'il y a des monstres à faire bouger
			monstre(t,monstres[k][0],monstres[k][1]);
			k++;		
		}

		sleep(1);	//Temps d'affichage

	}

printf("\n Bien joué ! \n");

return 0;
}
