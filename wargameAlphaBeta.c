

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define NB_LIGNES 10
#define NB_COLONNES 10
#define INFINI 10000
#define MAX_COUPS_EGAUX 112

/*#define DEBUG*/

typedef struct pion_s
{
	int couleur;
	int valeur;
}Pion;

Pion *plateauDeJeu;

typedef struct coup{
    int valeur;
    int l1, c1, l2, c2;
} Coup;

void f_affiche_plateau(Pion *plateau);
int f_convert_char2int(char c);
char f_convert_int2char(int i);

Coup f_min(Pion *plateau, int joueur, int profondeur_limite, int joueur_racine, int mode_de_jeux, int alpha, int beta);
Coup f_max(Pion *plateau, int joueur, int profondeur_limite, int joueur_racine, int mode_de_jeux, int alpha, int beta);

int nb_noeuds_visites_joueurX = 0;
int nb_noeuds_visites_joueurO = 0;

int total_noeuds_partie_X = 0;
int total_noeuds_partie_O = 0;

int f_convert_char2int(char c){
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
	if(c>='A' && c<='Z')
		return (int)(c-'A');
	if(c>='a' && c<='z')
		return (int)(c-'a');
	return -1;
}

char f_convert_int2char(int i){
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	return (char)i+'A';
}

Pion *f_init_plateau(){
	int i, j;
	Pion *plateau=NULL;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	plateau = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
	if(plateau == NULL)
	{
		printf("error: unable to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<NB_LIGNES; i++)
	{
		for(j=0; j<NB_COLONNES; j++)
		{
			plateau[i*NB_COLONNES+j].couleur = 0;
			plateau[i*NB_COLONNES+j].valeur = 0;
		}
	}
	
	/*Les pions du joueur 1*/
	plateau[9*NB_COLONNES+5].couleur = 1;
	plateau[9*NB_COLONNES+5].valeur = 1;

	plateau[9*NB_COLONNES+6].couleur = 1;
	plateau[9*NB_COLONNES+6].valeur = 2;

	plateau[9*NB_COLONNES+7].couleur = 1;
	plateau[9*NB_COLONNES+7].valeur = 3;

	plateau[9*NB_COLONNES+8].couleur = 1;
	plateau[9*NB_COLONNES+8].valeur = 2;

	plateau[9*NB_COLONNES+9].couleur = 1;
	plateau[9*NB_COLONNES+9].valeur = 1;

	plateau[8*NB_COLONNES+0].couleur = 1;
	plateau[8*NB_COLONNES+0].valeur = 1;

	plateau[8*NB_COLONNES+1].couleur = 1;
	plateau[8*NB_COLONNES+1].valeur = 3;

	plateau[8*NB_COLONNES+2].couleur = 1;
	plateau[8*NB_COLONNES+2].valeur = 3;

	plateau[8*NB_COLONNES+3].couleur = 1;
	plateau[8*NB_COLONNES+3].valeur = 1;

	plateau[8*NB_COLONNES+6].couleur = 1;
	plateau[8*NB_COLONNES+6].valeur = 1;

	plateau[8*NB_COLONNES+7].couleur = 1;
	plateau[8*NB_COLONNES+7].valeur = 1;

	plateau[8*NB_COLONNES+8].couleur = 1;
	plateau[8*NB_COLONNES+8].valeur = 1;

	plateau[7*NB_COLONNES+1].couleur = 1;
	plateau[7*NB_COLONNES+1].valeur = 1;

	plateau[7*NB_COLONNES+2].couleur = 1;
	plateau[7*NB_COLONNES+2].valeur = 1;

	/*Les pions du joueur 2*/
	plateau[2*NB_COLONNES+7].couleur = -1;
	plateau[2*NB_COLONNES+7].valeur = 1;

	plateau[2*NB_COLONNES+8].couleur = -1;
	plateau[2*NB_COLONNES+8].valeur = 1;

	plateau[1*NB_COLONNES+1].couleur = -1;
	plateau[1*NB_COLONNES+1].valeur = 1;

	plateau[1*NB_COLONNES+2].couleur = -1;
	plateau[1*NB_COLONNES+2].valeur = 1;

	plateau[1*NB_COLONNES+3].couleur = -1;
	plateau[1*NB_COLONNES+3].valeur = 1;

	plateau[1*NB_COLONNES+6].couleur = -1;
	plateau[1*NB_COLONNES+6].valeur = 1;

	plateau[1*NB_COLONNES+7].couleur = -1;
	plateau[1*NB_COLONNES+7].valeur = 3;

	plateau[1*NB_COLONNES+8].couleur = -1;
	plateau[1*NB_COLONNES+8].valeur = 3;

	plateau[1*NB_COLONNES+9].couleur = -1;
	plateau[1*NB_COLONNES+9].valeur = 1;

	plateau[0*NB_COLONNES+0].couleur = -1;
	plateau[0*NB_COLONNES+0].valeur = 1;

	plateau[0*NB_COLONNES+1].couleur = -1;
	plateau[0*NB_COLONNES+1].valeur = 2;

	plateau[0*NB_COLONNES+2].couleur = -1;
	plateau[0*NB_COLONNES+2].valeur = 3;

	plateau[0*NB_COLONNES+3].couleur = -1;
	plateau[0*NB_COLONNES+3].valeur = 2;

	plateau[0*NB_COLONNES+4].couleur = -1;
	plateau[0*NB_COLONNES+4].valeur = 1;

#ifdef DEBUG
printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

return plateau;
}

void f_affiche_plateau(Pion *plateau){
	int i,j,k;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	printf("\n    ");
	for(k=0; k<NB_COLONNES; k++)
		printf("%2c ",f_convert_int2char(k));
	printf("\n    ");
	for(k=0; k<NB_COLONNES; k++)
		printf("-- ");
	printf("\n");
	for(i=NB_LIGNES-1; i>=0; i--)
	{
		printf("%2d ", i);
		for(j=0; j<NB_COLONNES; j++)
		{
			printf("|");
			switch(plateau[i*NB_COLONNES+j].couleur)
			{
			case -1:
				printf("%do",plateau[i*NB_COLONNES+j].valeur);
				break;
			case 1:
				printf("%dx",plateau[i*NB_COLONNES+j].valeur);
				break;
			default:
				printf("  ");
			}
		}
		printf("|\n    ");
		for(k=0; k<NB_COLONNES; k++)
			printf("-- ");
		printf("\n");
	}
	printf("    ");

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

/**
* Après chaque coup regarde si un joueur a gagné ou non la partie
* Retourne la couleur du gagnant (-1, 0 ou 1)
**/
int f_gagnant(Pion* plateau){
	int i, j, somme1=0, somme2=0;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	//Quelqu'un est-il arrive sur la ligne de l'autre
	for(i=0; i<NB_COLONNES; i++)
	{
		if(plateau[i].couleur == 1)
			return 1;
		if(plateau[(NB_LIGNES-1)*NB_COLONNES+i].couleur == -1)
			return -1;
	}

	//taille des armees
	for(i=0; i<NB_LIGNES; i++)
	{
		for(j=0; j<NB_COLONNES; j++)
		{
			if(plateau[i*NB_COLONNES+j].couleur == 1)
				somme1++;
			if(plateau[i*NB_COLONNES+j].couleur == -1)
				somme2++;
		}
	}
	if(somme1==0)
		return -1;
	if(somme2==0)
		return 1;

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}


/**
 * Prend comme argument le plateau, la ligne et la colonne de la case
 * pour laquelle la bataille a lieu
 * Renvoie la couleur gagnante du combat
 * */
int f_bataille(Pion *plateau, int l, int c)
{
	int i, j, mini, maxi, minj, maxj;
	int somme=0;

#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
	mini = l-1<0?0:l-1;
	maxi = l+1>NB_LIGNES-1?NB_LIGNES-1:l+1;
	minj = c-1<0?0:c-1;
	maxj = c+1>NB_COLONNES-1?NB_COLONNES-1:c+1;

	for(i=mini; i<=maxi; i++)
	{
		for(j=minj; j<=maxj; j++)
		{
			somme += plateau[i*NB_COLONNES+j].couleur*plateau[i*NB_COLONNES+j].valeur;
		}
	}
	somme -= plateau[l*NB_COLONNES+c].couleur*plateau[l*NB_COLONNES+c].valeur;

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	if(somme < 0)
		return -1;
	if(somme > 0)
		return 1;

	return plateau[l*NB_COLONNES+c].couleur;
}


/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_test_mouvement(Pion *plateau, int l1, int c1, int l2, int c2, int couleur){
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
	printf("de (%d,%d) vers (%d,%d)\n", l1, c1, l2, c2);
#endif
	/* Erreur, hors du plateau */
	if(l1 < 0 || l1 >= NB_LIGNES || l2 < 0 || l2 >= NB_LIGNES ||
			c1 < 0 || c1 >= NB_COLONNES || c2 < 0 || c2 >= NB_COLONNES)
		return 1;
	/* Erreur, il n'y a pas de pion a deplacer ou le pion n'appartient pas au joueur*/
	if(plateau[l1*NB_COLONNES+c1].valeur == 0 || plateau[l1*NB_COLONNES+c1].couleur != couleur)
		return 1;
	/* Erreur, tentative de tir fratricide */
	if(plateau[l2*NB_COLONNES+c2].couleur == plateau[l1*NB_COLONNES+c1].couleur)
		return 1;

	if(l1-l2 >1 || l2-l1 >1 || c1-c2 >1 || c2-c1 >1 || (l1==l2 && c1==c2))
		return 1;
#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}


/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 *  et effectue le trantement de l'operation demandée
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_bouge_piece(Pion *plateau, int l1, int c1, int l2, int c2, int couleur){
	int gagnant=0;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	if(f_test_mouvement(plateau, l1, c1, l2, c2, couleur) != 0)
		return 1;


	/* Cas ou il n'y a personne a l'arrivee */
	if(plateau[l2*NB_COLONNES+c2].valeur == 0)
	{
		plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
		plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
		plateau[l1*NB_COLONNES+c1].couleur = 0;
		plateau[l1*NB_COLONNES+c1].valeur = 0;
	}
	else
	{
		gagnant=f_bataille(plateau, l2, c2);
		/* victoire */
		if(gagnant == couleur)
		{
			plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
			plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
			plateau[l1*NB_COLONNES+c1].couleur = 0;
			plateau[l1*NB_COLONNES+c1].valeur = 0;
		}
		/* defaite */
		else if(gagnant != 0)
		{
			plateau[l1*NB_COLONNES+c1].couleur = 0;
			plateau[l1*NB_COLONNES+c1].valeur = 0;
		}
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
	return 0;
}

//Calcul du nombre de pions sur le plateau du joueur
int f_nbPions(Pion* jeu, int joueur){
	int nbPion=0;
	int i, j;
	for (i = 0; i < NB_COLONNES; ++i)
	{
		for (j = 0; j < NB_LIGNES; ++j)
		{
			if (jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				++nbPion;
			}
		}
	}
	return nbPion;
}

//Calcul de la valeur de tous les pions du joueur
int f_valeur(Pion* jeu, int joueur){
	int i, j;
	int valeur=0;
	for (i = 0; i < NB_COLONNES; ++i)
	{
		for (j = 0; j < NB_LIGNES; ++j)
		{
			if (jeu[i * NB_COLONNES + j].couleur == joueur)
			{
				valeur += jeu[i * NB_COLONNES + j].valeur;
			}
		}
	}
	return valeur;
}

// Retourne le nombre de coup pour arriver à la ligne de l'adversaire
int f_distance_arrivee(int ligne, int joueur){
	if(joueur == 1) {
		return ligne;
	} 
	else {
		return NB_LIGNES - 1 - ligne;
	}
}

// Retourne la somme, pour tous les pions du joueur, les points selon la distance à l'arrivée
// (NB_LIGNES - 1) - f_distance_arrivee(i, joueur) : plus le pion est proche, plus le score est élevé
int f_calcul_avance(Pion* jeu, int joueur){
	int point_total = 0;
	int point_avance = 0;
	for(int i = 0; i < NB_LIGNES; i++){
		for(int j = 0; j < NB_COLONNES; j++){
			Pion pion = jeu[i*NB_COLONNES+j];
			if(pion.couleur == joueur){
				point_avance = ((NB_LIGNES - 1) - f_distance_arrivee(i, joueur));
				point_total += (point_avance + 1) * pion.valeur;
			}
		}
	}
	return point_total;
}

// Calcule la somme des voisins du pion
int f_force_voisins(Pion *jeu, int joueur) {
    int somme_forces = 0;

    for (int l = 0; l < NB_LIGNES; l++) {
        for (int c = 0; c < NB_COLONNES; c++) {
            // Ignore les pions du joueur adverse
            if (jeu[l * NB_COLONNES + c].couleur != joueur) {
                continue;
            }
            // Parcourir le voisinage 3x3 autour du pion
            for (int i = l - 1; i <= l + 1; i++) {
                for (int j = c - 1; j <= c + 1; j++) {
                    //  Ignore le pion lui-même
                    if (i == l && j == c) {
                        continue;
                    }
                    // Vérifie que la case est dans le plateau
                    if (i < 0 || i >= NB_LIGNES || j < 0 || j >= NB_COLONNES) {
                        continue;
                    }
                    Pion voisin = jeu[i * NB_COLONNES + j];
                    if (voisin.couleur == joueur) {
                        somme_forces += voisin.valeur;
                    }
                }
            }
        }
    }
    return somme_forces;
}

//fonction d'évaluation
int f_eval(Pion* jeu,int joueur_racine, int profondeur){
	/** A vous de trouver une fonction efficace **/
	int score = 0;
	int gagnant = f_gagnant(jeu);

    if (gagnant == joueur_racine) {
        return INFINI + profondeur;
    }
    if (gagnant == -joueur_racine) {
        return -INFINI - profondeur;
    }
    //int diff_nbPions = f_nbPions(jeu, joueur_racine) - f_nbPions(jeu, -joueur_racine);
    int diff_valeur =  f_valeur(jeu, joueur_racine) - f_valeur(jeu, -joueur_racine);
    int diff_avance = f_calcul_avance(jeu, joueur_racine) - f_calcul_avance(jeu, -joueur_racine);
	int diff_force_voisins = f_force_voisins(jeu, joueur_racine) - f_force_voisins(jeu, -joueur_racine);

   	//score += 10 * diff_nbPions; // Si uniquement ce critère, tourne à l'infini
    score += 50 * diff_valeur;
    score += 25 * diff_avance;
	score += 5 * diff_force_voisins;

    return score;
	
}

int f_eval2(Pion* jeu, int joueur_racine, int profondeur){
	/** A vous de trouver une fonction efficace **/
	int score = 0;
	int gagnant = f_gagnant(jeu);

    if (gagnant == joueur_racine) {
        return INFINI + profondeur;
    }
    if (gagnant == -joueur_racine) {
        return -INFINI - profondeur;
    }
    //int diff_nbPions = f_nbPions(jeu, joueur_racine) - f_nbPions(jeu, -joueur_racine);
    int diff_valeur =  f_valeur(jeu, joueur_racine) - f_valeur(jeu, -joueur_racine);
    int diff_avance = f_calcul_avance(jeu, joueur_racine) - f_calcul_avance(jeu, -joueur_racine);
	int diff_force_voisins = f_force_voisins(jeu, joueur_racine) - f_force_voisins(jeu, -joueur_racine);
	
   	//score += 10 * diff_nbPions; // Si uniquement ce critère, tourne à l'infini
    score += 50 * diff_valeur;
    score += 25 * diff_avance;
	score += 5 * diff_force_voisins;

    return score;
}

//copie du plateau
void f_copie_plateau(Pion* source, Pion* destination){
	int i, j;
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			destination[i * NB_COLONNES + j].couleur = source[i * NB_COLONNES + j].couleur;
			destination[i * NB_COLONNES + j].valeur = source[i * NB_COLONNES + j].valeur;
		}
	}
}

//mise a zero du plateau
Pion* f_raz_plateau(){
	Pion* jeu = NULL;
	int i, j;
	jeu = (Pion *) malloc(NB_LIGNES * NB_COLONNES * sizeof (Pion));
	for (i = 0; i < NB_LIGNES; i++)
	{
		for (j = 0; j < NB_COLONNES; j++)
		{
			jeu[i * NB_COLONNES + j].couleur = 0;
			jeu[i * NB_COLONNES + j].valeur = 0;
		}
	}
	return jeu;
}

// Incrémente le nombre de noeuds visités
// Remis à zéro chaque chaque tour
void f_nbNoeudsVisites(int joueur){
	if(joueur == 1) nb_noeuds_visites_joueurX += 1;
	if(joueur == -1) nb_noeuds_visites_joueurO += 1;
}

//Fonction min trouve le minimum des noeuds fils
Coup f_min(Pion *plateau, int joueur, int profondeur_limite, int joueur_racine, int mode_de_jeux, int alpha, int beta){
	Coup min = { INFINI+100, -1, -1, -1, -1 };
	Coup liste_meilleurs_coups[MAX_COUPS_EGAUX];
	int nb_coups_egaux = 0; 

	f_nbNoeudsVisites(joueur_racine);
	
	if((profondeur_limite == 0 || f_gagnant(plateau) != 0) && joueur_racine == -1 && mode_de_jeux == 3){
		min.valeur = f_eval2(plateau, joueur_racine, profondeur_limite);
		return min;
	}
	if((profondeur_limite == 0 || f_gagnant(plateau) != 0)){
		min.valeur = f_eval(plateau, joueur_racine, profondeur_limite);
		return min;
	}

	int coupure = 0;

	for(int l1=0; l1<NB_LIGNES; l1++){
		for(int c1=0; c1<NB_COLONNES; c1++){
			if(plateau[l1*NB_COLONNES+c1].couleur==joueur){

				for(int l2=l1-1; l2<=l1+1; l2++){
					for(int c2=c1-1; c2<=c1+1; c2++){
						Pion plateau_fils[NB_LIGNES*NB_COLONNES];
						f_copie_plateau(plateau, plateau_fils);
						if(f_bouge_piece(plateau_fils, l1, c1, l2, c2, joueur) != 1){

							Coup coup = f_max(plateau_fils, -joueur, profondeur_limite-1, joueur_racine, mode_de_jeux, alpha, beta);
							if(coup.valeur < min.valeur){
								min.valeur = coup.valeur;
								nb_coups_egaux = 0;
							}
							if(coup.valeur == min.valeur && nb_coups_egaux < MAX_COUPS_EGAUX){
    							liste_meilleurs_coups[nb_coups_egaux++] = (Coup){coup.valeur, l1, c1, l2, c2};
							}
							if(min.valeur <= beta){
								beta = min.valeur;
							}
							if(beta <= alpha){
								coupure = 1;
							}
						}
						if(coupure == 1) break;	 
					}
					if(coupure == 1) break;	 
				}
			}
			if(coupure == 1) break;	 
		}
		if(coupure == 1) break;	 
	}
	if(nb_coups_egaux > 0)
    	min = liste_meilleurs_coups[rand() % nb_coups_egaux];
	return min;
}

//Fonction max trouve le maximum des noeuds fils
Coup f_max(Pion *plateau, int joueur, int profondeur_limite, int joueur_racine, int mode_de_jeux, int alpha, int beta){
	Coup max = { -INFINI-100, -1, -1, -1, -1 };
	Coup liste_meilleurs_coups[MAX_COUPS_EGAUX];
	int nb_coups_egaux = 0; 

	f_nbNoeudsVisites(joueur_racine);
	
	if((profondeur_limite == 0 || f_gagnant(plateau) != 0) && joueur_racine == -1 && mode_de_jeux == 3){
		max.valeur = f_eval2(plateau, joueur_racine, profondeur_limite);
		return max;
	}
	if(profondeur_limite == 0 || f_gagnant(plateau) != 0){
		max.valeur = f_eval(plateau, joueur_racine, profondeur_limite);
		return max;
	}

	int coupure = 0;

	for(int l1=0; l1<NB_LIGNES; l1++){
		for(int c1=0; c1<NB_COLONNES; c1++){
			if(plateau[l1*NB_COLONNES+c1].couleur==joueur){
				
				for(int l2=l1-1; l2<=l1+1; l2++){
					for(int c2=c1-1; c2<=c1+1; c2++){
						Pion plateau_fils[NB_LIGNES*NB_COLONNES];
						f_copie_plateau(plateau, plateau_fils);
						if(f_bouge_piece(plateau_fils, l1, c1, l2, c2, joueur) != 1){

							Coup coup = f_min(plateau_fils, -joueur, profondeur_limite-1, joueur_racine, mode_de_jeux, alpha, beta);
							if(coup.valeur > max.valeur){
								max.valeur=coup.valeur;
								nb_coups_egaux = 0;
							}
							if(coup.valeur == max.valeur && nb_coups_egaux < MAX_COUPS_EGAUX){
								liste_meilleurs_coups[nb_coups_egaux++] = (Coup){coup.valeur, l1, c1, l2, c2};
							}
							if(max.valeur >= alpha){
								alpha = max.valeur;
							}
							if(alpha >= beta){
								coupure = 1;
							}
						}
						if(coupure == 1) break;	 
					}
					if(coupure == 1) break;	 
				}
			}
			if(coupure == 1) break;	 
		}
		if(coupure == 1) break;	 
	}
	if(nb_coups_egaux > 0)
    	max = liste_meilleurs_coups[rand() % nb_coups_egaux];
	return max;
}

/**
 * Calcule et joue le meilleur cout
 * */
void f_IA(int joueur, int mode, int bloque_printf){
#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
	
	int joueur_racine = joueur;
	int profondeur_limite = 3;
	int alpha = -INFINI-100;
	int beta = INFINI+100;
	nb_noeuds_visites_joueurX = 0;
	nb_noeuds_visites_joueurO = 0;

    Coup meilleur_coup = f_max(plateauDeJeu, joueur, profondeur_limite, joueur_racine, mode, alpha, beta);
    
	total_noeuds_partie_X += nb_noeuds_visites_joueurX;
	total_noeuds_partie_O += nb_noeuds_visites_joueurO;

	if(meilleur_coup.l1 == -1){   // aucun coup légal trouvé
		printf("L'IA ne peut pas jouer\n");
		return;
	}

	int valeur_eval = meilleur_coup.valeur;
	int l1 = meilleur_coup.l1;
	int c1 = meilleur_coup.c1;
	int l2 = meilleur_coup.l2;
	int c2 = meilleur_coup.c2;

	f_bouge_piece(plateauDeJeu, l1, c1, l2, c2, joueur);
	if(!bloque_printf){
		if(joueur == 1) {
			printf("\nNoeuds visités : %i", nb_noeuds_visites_joueurX);
			printf("\nIA X : pion %c%i déplacé en %c%i avec un score : %i", f_convert_int2char(c1), l1, f_convert_int2char(c2), l2, valeur_eval);
		}else{
			printf("\nNoeuds visités : %i", nb_noeuds_visites_joueurO);
			printf("\nIA O : pion %c%i déplacé en %c%i avec un score : %i", f_convert_int2char(c1), l1, f_convert_int2char(c2), l2, valeur_eval);
		}
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}


/**
 * Demande le choix du joueur humain et calcule le coup demande
 * */
void f_humain(int joueur){
	char c1, c2;
	char buffer[32];
	int l1, l2;


#ifdef DEBUG
	printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

	printf("joueur ");
	switch(joueur){
	case -1:
		printf("o ");
		break;
	case 1:
		printf("x ");
		break;
	default:
		printf("inconnu ");
	}
	printf("joue:\n");
	while(1){
		fgets(buffer, 32, stdin);
		if(sscanf(buffer, "%c%i%c%i\n", &c1, &l1, &c2, &l2) == 4){
			if(f_bouge_piece(plateauDeJeu, l1, f_convert_char2int(c1), l2, f_convert_char2int(c2), joueur) == 0)
				break;
		}else {
			fflush(stdin);
			printf("mauvais choix\n");
		}
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

int main(int argv, char *argc[]){
	srand(time(NULL));
	int fin = 0, mode = 0 , ret, joueur = 1; 
	int tour = 1; 
	int nbParties=0; // Utilisé dans le mode 4, pour joueur plusieurs parties
	printf("1 humain vs IA\n2 humain vs humain\n3 IA vs IA\n4 IA vs IA (N fois)\n");
	printf("Choisissez le mode de jeu : "); 
	scanf("%d", &mode);

	if(mode == 1 || mode == 2 || mode == 3){
		total_noeuds_partie_X = 0;
		total_noeuds_partie_O = 0;

		plateauDeJeu = f_init_plateau();
		while (!fin)
		{
			f_affiche_plateau(plateauDeJeu);
			printf("\nTour %i :", tour);
			tour += 1;
			if(mode==1)
			{
				if(joueur>0)
					f_humain(joueur);
				else
					f_IA(joueur, mode, 0);
			}
			else if(mode==2)
			{
				f_humain(joueur);
			}
			else
			{
				f_IA(joueur, mode, 0);
			}

			if ((ret = f_gagnant(plateauDeJeu)) != 0)
			{
				switch (ret)
				{
				case 1:
					f_affiche_plateau(plateauDeJeu);
					printf("\njoueur x gagne!");
					fin = 1;
					break;
				case -1:
					f_affiche_plateau(plateauDeJeu);
					printf("\njoueur o gagne!");
					fin = 1;
					break;
				}
			}
			joueur = -joueur;
		}
		printf("\nNoeuds totales explorés : X = %d, O = %d\n", total_noeuds_partie_X, total_noeuds_partie_O);
	}
	else if(mode == 4){
		
		int nb_gagneX=0; int nb_gagneO=0;
		printf("\nNombre de parties : "); 
		scanf("%d", &nbParties);
		int i=1;

		if(joueur == 1) printf("1er coup joueur X");
		if(joueur == -1) printf("1er coup joueur O");

		long grand_total_X = 0, grand_total_O = 0;
		double grand_temps = 0.0;
		while(i<=nbParties){
			
			int fin = 0, mode = 3, bloque_printf = 1; // bloque_printf une var quelconque passé à f_IA pour bloquer les affichages
			total_noeuds_partie_X = 0;
			total_noeuds_partie_O = 0;
			
			printf("\n=========Partie %d =========", i);
			
			plateauDeJeu = f_init_plateau();
			clock_t debut_partie = clock();

			while (!fin) {
				f_IA(joueur, mode , bloque_printf);

				if ((ret = f_gagnant(plateauDeJeu)) != 0)
				{
					switch (ret)
					{
					case 1:
						//f_affiche_plateau(plateauDeJeu);
						printf("\njoueur x gagne!");
						nb_gagneX += 1;
						fin = 1;
						break;
					case -1:
						//f_affiche_plateau(plateauDeJeu);
						printf("\njoueur o gagne!");
						nb_gagneO += 1;
						fin = 1;
						break;
					}
				}
				joueur = -joueur;
			}
			i+=1;
			double duree_partie = (double)(clock() - debut_partie) / CLOCKS_PER_SEC;
			grand_temps += duree_partie;

			printf("\nTemps de la partie : %.3f s", duree_partie);
			printf("\nNoeuds totales explorés : X = %d, O = %d", total_noeuds_partie_X, total_noeuds_partie_O);
			printf("\n==========================");
			grand_total_X += total_noeuds_partie_X;
			grand_total_O += total_noeuds_partie_O;
		}
		float proba_nb_gagneX = 100.0f * (float)nb_gagneX / (float)nbParties;
		float proba_nb_gagneO = 100.0f * (float)nb_gagneO / (float)nbParties;
		printf("\nLe joueur X a gagné %d parties sur %d : %.3f%s de victoire", nb_gagneX, nbParties, proba_nb_gagneX, "%");
		printf("\nLe joueur O a gagné %d parties sur %d : %.3f%s de victoire", nb_gagneO, nbParties, proba_nb_gagneO, "%");
		printf("\nMoyenne noeuds/partie : X = %.0f, O = %.0f", (double)grand_total_X / nbParties, (double)grand_total_O / nbParties);
		printf("\nTemps moyen par partie : %.3f s (total : %.3f s)\n", grand_temps / nbParties, grand_temps);
	}

#ifdef DEBUG
	printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

	return 0;
}

