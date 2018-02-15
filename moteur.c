#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define TAILLE_X 60
#define TAILLE_Y 10

void creerZone(int t[TAILLE_X][TAILLE_Y]){
	int i,j=0;
	for (i=0;i<TAILLE_X;i++){
		for(j=0;j<TAILLE_Y;j++){
			if(i==0 || j==0 || i == TAILLE_X -1 || j == TAILLE_Y -1 || (i==5 && (j==10 || j==11 || j==12))){	//Insérer les murs ici
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
	if (t[y-1]!=0 && t[y+1]!=0 && t[x+1]!=0 && t[x-1]!=0){	//si le monstre est entouré de 4 murs, on le tue
		t[x][y] = 0;
	}
	int monstre=2;
	int n = (rand()%4);


	if (n==0){
		//Avancer en avant
		t[x][y] = 0;
		t[x][y-1] = monstre;
	}
	else if(n==1 && t[x+1]==0){
		//Avancer à droite
		t[x][y] = 0;
		t[x+1][y] = monstre;
	}
	else if (n==2 && t[){
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
	int i,j = 0;
	int x,y = 2;		//Le monstre se situe 
	int compteur = 0;

	creerZone(t);	//Initialisation de la zone


	while(jeu_fini == 0){
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		affichage(t[TAILLE_X][TAILLE_Y]);	//affichage de la zone
		monstre(t[TAILLE_X][TAILLE_Y],x,y);	//Déplacement du monstre

		for (i=0;i<TAILLE_X;i++){		//recherche de la nouvelle position du monstre
			for(j=0;j<TAILLE_Y;j++){
				if(t[i][j]==2){
					x=i;
					y=j;
				}
			}
		}

		sleep(1);	//Temps d'affichage
		compteur++;
		if(compteur=10){
			jeu_fini = 1;
		}
	}
	printf("\n Bien joué ! \n");

	return 0;
}
