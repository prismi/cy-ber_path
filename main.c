#include "cy-ber_path.h"
#include <stdio.h>

int main(void) {
  srand(time(NULL));
  int taille = (rand() % 6) * 2 + 31;
  
  Cible cibl_pos[TAILLE];
  Robot rob_pos[TAILLE];
  Joueur joueurs[TAILLE];
  int tab[TAILLE][TAILLE] = {0};
  int nb_joueurs = 0;
  int manche = 1;
  int nb_manche = 0;
  int niveau = 0;

  matrice(tab, taille);
  init_Cibles(tab, cibl_pos, taille);
  for (int i = 0; i < 18; i++) {
    bords(tab, cibl_pos[i].x, cibl_pos[i].y);
  }
  robot(tab, rob_pos, taille);

  menu();

  printf("Combien de joueurs pour cette partie ?");
  nb_joueurs = verification_saisie(2, 8);

  init_Joueur(joueurs, nb_joueurs);

  printf("Combien de manches souhaitez vous jouez ?");
  nb_manche = verification_saisie(1, 5);

  printf("niveau de difficulté\n1 = facile, 2 = moyen, 3 = difficile ");
  niveau = verification_saisie(1, 3);

  for (int i = 0; i < nb_manche; i++) {
    initManche(tab, cibl_pos, rob_pos, joueurs, niveau, manche, nb_manche,
               nb_joueurs, taille);
    manche++;
  }

  if (manche > nb_manche) {
    int maxpts = joueurs[ptsMax(joueurs, nb_joueurs)].pts;
    int equalCount = ptsMaxegalite(joueurs, nb_joueurs, maxpts);
    if (equalCount > 1) {
      printf("Égalité de points ! Les joueurs suivants ont gagné la partie :\n");
      for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].pts == maxpts) {
          printf("%s\n", joueurs[i].prenom);
        }
      }
    } else {
      printf("%s, vous avez gagné la partie !\n",
             joueurs[ptsMax(joueurs, nb_joueurs)].prenom);
    }
    for (int i = 0; i < nb_joueurs; i++) {
      printf("\n %s : %d pts\n", joueurs[i].prenom, joueurs[i].pts);
    }
  }

  return 0;
}