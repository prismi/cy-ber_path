#ifndef JEU_H
#define JEU_H

#include <stdbool.h>

#define LIMITE_PRENOM 25
#define MAX_MOUVEMENTS 21
#define TAILLE 100

// structure définissant une cible
typedef struct {
    int numero;
    int x;
    int y;
} Cible;

// structure définissant un robot
typedef struct {
    int numero;
    int x;
    int y;
} Robot;

// structure définissant un joueur
typedef struct {
    int numero;
    char prenom[LIMITE_PRENOM];
    int nb_deplacement;
    int pts;
} Joueur;

int exposant(int e);
void sleep_chrono(unsigned int temps);
int verification_saisie(int minimum_saisie, int maximum_saisie);
void matrice(int tab[TAILLE][TAILLE], int taille);
void bords(int tab1[TAILLE][TAILLE], int x, int y);
int elementCibles(Cible tab2[TAILLE], int x, int y);
void init_Cibles(int tab1[TAILLE][TAILLE], Cible tab2[TAILLE], int taille);
int elementRobot(Robot tab3[TAILLE], int x, int y);
void robot(int tab1[TAILLE][TAILLE], Robot tab3[TAILLE], int taille);
void mouvementsRobot(int tab1[TAILLE][TAILLE], Cible tab2[TAILLE], Robot rob[TAILLE], int coordonnees, int sens, int taille);
int verification_sens(int liste[TAILLE], int nb);
void init_Joueur(Joueur tab4[TAILLE], int nb);
int chercherMin(Joueur joueur[TAILLE], int nb);
int ptsMax(Joueur joueur[TAILLE], int nb);
int ptsMaxegalite(Joueur joueurs[TAILLE], int nb, int ptsMax);
void emoji(Robot robot[TAILLE], int i, int j);
void affichageGrille(int tab[TAILLE][TAILLE], Cible tab1[TAILLE], Robot robot[TAILLE], int taille);
void initManche(int tab[TAILLE][TAILLE], Cible tab1[TAILLE], Robot robot1[TAILLE], Joueur joueurs[TAILLE], int nv, int manche, int nb_manche, int nb, int taille);

#endif