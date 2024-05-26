#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int exposant(int e) {
  int a = 1;
  for (int i = 0; i < e; i++) {
    a = a * 10;
  }
  return a;
}

void sleep_chrono(unsigned int temps) {
  clock_t debut_temps = clock();
  clock_t fin_temps = temps * CLOCKS_PER_SEC / 1000 + debut_temps;

  while (clock() < fin_temps)
    ;
}

// Fonction vérifiant l'exactitude des valeurs saisie tout au long du jeu
int verification_saisie(int minimum_saisie, int maximum_saisie) {
  int ret_var = 0;
  int valeur = 1;
  int verif = 0;
  while (ret_var != 1 || valeur < 0) {
    do {
      if (verif != 0) {
        printf("Saisie incorrecte, veuillez recommencer en respectant les "
               "consignes\n");
      }
      ret_var = scanf("%d", &valeur);
      while (getchar() != '\n') {
      }
      verif++;

    } while (valeur<minimum_saisie | valeur> maximum_saisie);
  }
  return valeur;
}

// procédure qui créer la matrice qui est utilisée pour l'intégralité du code
void matrice(int tab[TAILLE][TAILLE], int taille) {
  int ecart = 0;
  for (int i = 0; i < taille; i++) {
    for (int j = 0; j < taille; j++) {
      int g = (i == 0 | j == 0 | i == taille - 1 |
               j == taille - 1); 
      if (g) {
        tab[i][j] = 1; 
      } else if (g == 0 &&
                 (i % 2 == 0 |
                  j % 2 ==
                      0)) {
        tab[i][j] = 9;
      }
    }
  }
  int a, b;
  if (taille % 2 == 0) {
    ecart = (taille - 8) /
            2;
  } else {
    ecart = (taille - 8) / 2 + 1;
  }
  do {
    a = (rand() % (ecart)) * 2 + 4;
    b = (rand() % (ecart)) * 2 + 4;
  } while (a == b);
  tab[a][1] = 1;
  tab[b][1] = 1;
  do {
    a = (rand() % (ecart)) * 2 + 4;
    b = (rand() % (ecart)) * 2 + 4;
  } while (a == b);
  tab[1][a] = 1;
  tab[1][b] = 1;
  do {
    a = (rand() % (ecart)) * 2 + 4;
    b = (rand() % (ecart)) * 2 + 4;
  } while (a == b);
  tab[a][taille - 2] = 1;
  tab[b][taille - 2] = 1;
  do {
    a = (rand() % (ecart)) * 2 + 4;
    b = (rand() % (ecart)) * 2 + 4;
  } while (a == b);
  tab[taille - 2][a] = 1;
  tab[taille - 2][b] = 1;
}

// procédure affichant les bords entourants les cibles
void bords(int tab1[TAILLE][TAILLE], int x, int y) {
  int angle = (rand() % 4) + 1;
  if (angle == 1) {
    tab1[x - 1][y] = 1;
    tab1[x - 1][y - 1] = 1;
    tab1[x][y - 1] = 1;
  }
  if (angle == 2) {
    tab1[x - 1][y] = 1;
    tab1[x - 1][y + 1] = 1;
    tab1[x][y + 1] = 1;
  }
  if (angle == 3) {
    tab1[x + 1][y] = 1;
    tab1[x + 1][y + 1] = 1;
    tab1[x][y + 1] = 1;
  }
  if (angle == 4) {
    tab1[x + 1][y] = 1;
    tab1[x + 1][y - 1] = 1;
    tab1[x][y - 1] = 1;
  }
}

//procédure verifiant que les cibles sont à des coordonnées distinctes
int elementCibles(Cible tab2[TAILLE], int x, int y) {
  for (int i = 0; i < 18; i++) {
    if (tab2[i].x == x && tab2[i].y == y)
      return 1;
  }
  return 0;
}

// procédure qui permet d'initialiser et deplacer les cibles sur la matrice
void init_Cibles(int tab1[TAILLE][TAILLE], Cible tab2[TAILLE], int taille) {
  Cible cible;
  int valeur =
      taille / 2.6;
  for (int i = 0; i < 18; i++) {
    do {
      cible.y = (rand() % valeur) * 2 + 5;
      cible.x = (rand() % valeur) * 2 + 5;
    } while (tab1[cible.x][cible.y] != 0 |
             elementCibles(tab2, cible.x, cible.y) == 1);

    tab1[cible.x][cible.y + 2] = 8;
    tab1[cible.x][cible.y - 2] = 8;
    tab1[cible.x + 2][cible.y] = 8;
    tab1[cible.x - 2][cible.y] = 8;
    tab1[cible.x - 2][cible.y + 2] = 8;
    tab1[cible.x - 2][cible.y - 2] = 8;
    tab1[cible.x + 2][cible.y - 2] = 8;
    tab1[cible.x + 2][cible.y + 2] = 8;
    tab1[cible.x][cible.y] = 2;
    cible.numero = i + 1; 
    tab2[i] = cible;    
  }
}

//procédure verifiant que les cibles sont à des coordonnées distinctes
int elementRobot(Robot tab3[TAILLE], int x, int y) {
  for (int i = 0; i < 4; i++) {
    if (tab3[i].x == x && tab3[i].y == y)
      return 1;
  }
  return 0;
}

// procédure permettant la creation et le placement des robots
void robot(int tab1[TAILLE][TAILLE], Robot tab3[TAILLE], int taille) {
  Robot robot;
  int valeur = taille / 2.6;
  for (int i = 0; i < 4; i++) {
    do {
      robot.y = (rand() % valeur) * 2 + 5;
      robot.x = (rand() % valeur) * 2 + 5;
    } while (tab1[robot.x][robot.y] != 0 |
             elementRobot(tab3, robot.x, robot.y) == 1);
    tab1[robot.x][robot.y] = 3;
    robot.numero = i + 1; 
    tab3[i] = robot;           
  }
}

// procédure permettant les deplacements des robots
void mouvementsRobot(int tab1[TAILLE][TAILLE], Cible tab2[TAILLE],
                     Robot rob[TAILLE], int coordonnees, int sens, int taille) {
  int x = rob[coordonnees].x;
  int y = rob[coordonnees].y;
  int mur = 0;
  if (elementCibles(tab2, x, y) == 1) {
    tab1[x][y] = 2;
  } else {
    tab1[x][y] = 0;
  }
  if (sens == 1) {
    mur = 1;
    for (int i = 0; i < x; i++) {
      if (tab1[i][y] == 1) {
        mur = i + 1;
      }
    }
    tab1[mur][y] = 3;
    rob[coordonnees].x = mur;
  }

  if (sens == 2) { 
    mur = taille - 1;
    for (int i = taille - 1; i > x; i--) {
      if (tab1[i][y] == 1) {
        mur = i - 1;
      }
    }
    tab1[mur][y] = 3;
    rob[coordonnees].x = mur;
  }

  if (sens == 3) {
    mur = 1;
    for (int i = 0; i < y; i++) {
      if (tab1[x][i] == 1) {
        mur = i + 1;
      }
    }
    tab1[x][mur] = 3;
    rob[coordonnees].y = mur;
  }

  if (sens == 4) {
    mur = taille - 1;
    for (int i = taille - 1; i > y; i--) {
      if (tab1[x][i] == 1) {
        mur = i - 1;
      }
    }
    tab1[x][mur] = 3;
    rob[coordonnees].y = mur;
  }
}

// fonction permettant de verifier que les robots respectant bien le parcours effectué par les joueurs  
int verification_sens(int liste[TAILLE], int nb) {
  for (int i = 0; i < nb; i++) {
    if (liste[i] <= 0 | liste[i] > 4) {
      return 1;
    }
  }
  return 0;
}

// procédure qui permet d'initialiser et deplacer les cibles sur la matrice
void init_Joueur(Joueur tab4[TAILLE], int nb) {
  Joueur joueur;
  char prenom[LIMITE_PRENOM];

  for (int i = 0; i < nb; i++) {
    joueur.numero = i + 1;
    joueur.pts = 0;

    do {
      printf("Entrez le prénom du joueur %d (max %d caractères) : ", i + 1,
             LIMITE_PRENOM - 1);
      if (fgets(prenom, LIMITE_PRENOM, stdin) == NULL) {
        fprintf(stderr, "Erreur lors de la lecture du prénom.\n");
        exit(EXIT_FAILURE);
      }

      prenom[strcspn(prenom, "\n")] = '\0';
    } while (strlen(prenom) >= LIMITE_PRENOM - 1);

    strncpy(joueur.prenom, prenom, LIMITE_PRENOM - 1);
    joueur.prenom[LIMITE_PRENOM - 1] =
        '\0';

    tab4[i] = joueur;
  }
}

// Fonction 
int chercherMin(Joueur joueur[TAILLE], int nb) {
  int ch_min = 0;
  for (int i = 0; i < nb; i++) {
    if (joueur[i].nb_deplacement < joueur[ch_min].nb_deplacement) {
      ch_min = i;
    }
  }
  return ch_min;
}

// fonction qui cherche le nombre de points le plus élevé parmi les joueurs
int ptsMax(Joueur joueur[TAILLE], int nb) {
  int max = 0;
  for (int i = 0; i < nb; i++) {
    if (joueur[i].pts > joueur[max].pts) {
      max = i;
    }
  }
  return max;
}

// fonction qui gère une eventuelle égalité de points
int ptsMaxegalite(Joueur joueurs[TAILLE], int nb, int ptsMax) {
  int count = 0;
  for (int i = 0; i < nb; i++) {
    if (joueurs[i].pts == ptsMax) {
      count++;
    }
  }
  return count;
}

// procédure permettant l'affichage des emojis propre à chaque robot

void emoji(Robot robot[TAILLE], int i, int j) {
  if (robot[0].x == i && robot[0].y == j) {
    printf("%s", "\U0001F921");
  }
  if (robot[1].x == i && robot[1].y == j) {
    printf("%s", "\U0001F63C");
  }
  if (robot[2].x == i && robot[2].y == j) {
    printf("%s", "\U0001F608");
  }
  if (robot[3].x == i && robot[3].y == j) { 
    printf("%s", "\U0001F47D");
  }
}

// procédure permettant l'affichage de la grille
void affichageGrille(int tab[TAILLE][TAILLE], Cible tab1[TAILLE],
                     Robot robot[TAILLE], int taille) {
  for (int i = 0; i < taille; i++) {
    for (int j = 0; j < taille; j++) {
      if (tab[i][j] == 1) {
        printf("🧱");
      } else if (tab[i][j] ==
                 2) {
        for (int k = 0; k < 18; k++) {
          if (tab1[k].x == i && tab1[k].y == j) {
            if (tab1[k].numero <=
                9) {
              printf("%d ", tab1[k].numero);
            } else {
              printf("%d", tab1[k].numero);
            }
          }
        }
      } else if (tab[i][j] ==
                 3) {
        emoji(robot, i, j);
      } else {
        printf("  ");
      }
    }
    printf("\n");
  }
}

// procédure lançant un jeu
void initManche(int tab[TAILLE][TAILLE], Cible tab1[TAILLE],
                Robot robot1[TAILLE], Joueur joueurs[TAILLE], int nv,
                int manche, int nb_manche, int nb, int taille) {
  srand(time(NULL));
  int parcours = 0;   
  int alea_robot = 0;
  int alea_cible = 0;
  do {
    alea_robot = rand() % 4;
    alea_cible = rand() % 18;
  } while (robot1[alea_robot].x == tab1[alea_cible].x &&
           robot1[alea_robot].y == tab1[alea_cible].y);
  int temps = 0;
  Joueur joueurMouvementMinimum;

  if (nv == 1) {

    temps = 60;
  }
  if (nv == 2) {
    temps = 30;
  }
  if (nv == 3) {
    temps = 15;
  }

  for (int i = 0; i < temps; i++) {
    printf("%d\n", i); // afichage du compte à rebours
    printf("Manche numéro %d/%d\n", manche, nb_manche);
    printf("Robot ");
    emoji(robot1, robot1[alea_robot].x, robot1[alea_robot].y);
    printf(" Cible numéro %d\n",
           alea_cible + 1);
    affichageGrille(tab, tab1, robot1, taille);
    sleep_chrono(1000);
    system("clear"); 
  }

  int a = 0; 
  for (int i = 0; i < nb; i++) {
    printf("%s, saississez le nombre de déplacements que vous souhaitez effectuer (max 20) : ",
           joueurs[i].prenom);
    a = verification_saisie(1, 20);
    joueurs[i].nb_deplacement = a; 
  }
  joueurMouvementMinimum = joueurs[chercherMin(joueurs, nb)];

  int liste[MAX_MOUVEMENTS] = {0};
  do {
    printf("%s, saissisez le parcours que vous souhaitez effectuer\n 1 = haut, 2 = bas, 3 = gauche, 4 = droite: ",
           joueurMouvementMinimum.prenom);
    parcours =
        verification_saisie(1, exposant(joueurMouvementMinimum.nb_deplacement));
    for (int i = joueurMouvementMinimum.nb_deplacement; i > 0; i--) {
      liste[joueurMouvementMinimum.nb_deplacement - i] =
          parcours / exposant(i - 1);
      parcours -=
          liste[joueurMouvementMinimum.nb_deplacement - i] * exposant(i - 1);
    }
  } while (verification_sens(liste, joueurMouvementMinimum.nb_deplacement) == 1);

  int mouv_realise = 0;
  for (int i = 0; i < joueurMouvementMinimum.nb_deplacement; i++) {
    system("clear");
    affichageGrille(tab, tab1, robot1, taille);
    mouvementsRobot(tab, tab1, robot1, alea_robot, liste[i], taille);
    if (tab1[alea_robot].x != robot1[alea_robot].x &&
        tab1[alea_robot].y != robot1[alea_robot].y) {
      mouv_realise++;
    }
    sleep_chrono(3000);
  }
  int mm_coord = (tab1[alea_robot].x == robot1[alea_robot].x &&
                  tab1[alea_robot].y == robot1[alea_robot].y);
  if (mm_coord && mouv_realise == joueurMouvementMinimum.nb_deplacement) {
    joueurMouvementMinimum.pts += 2;
  } else if (mm_coord &&
             mouv_realise != joueurMouvementMinimum.nb_deplacement) {
    joueurMouvementMinimum.pts -= 1; 
  } else {
    for (int i = 0; i < nb; i++) {
      if (i != chercherMin(joueurs, nb)) {
        joueurs[i].pts++;
      }
    }
  }
}

// procédure permettant l'affichage d'un ménu
void menu() {
  printf(" _____                                                               "
         "              _____ \n");
  printf("( ___ "
         ")--------------------------------------------------------------------"
         "-------( ___ )\n");
  printf(" |   |                                                               "
         "              |   | \n");
  printf(" |   |                  ____  _                                      "
         "              |   | \n");
  printf(" |   |                 | __ )(_) ___ _ ____   _____ _ __  _   _ ___  "
         "              |   | \n");
  printf(" |   |                 |  _ \\| |/ _ \\ '_ \\ \\ / / _ \\ '_ \\| | | "
         "/ __|               |   | \n");
  printf(" |   |                 | |_) | |  __/ | | \\ V /  __/ | | | |_| \\__ "
         "\\               |   | \n");
  printf(" |   |                 |____/|_|\\___|_|_|\\_/\\_/\\___|_| "
         "|_|\\__,_|___/               |   | \n");
  printf(" |   |                               / __| | | | '__|                "
         "              |   | \n");
  printf(" |   |                               \\__ \\ |_| | |                 "
         "                |   | \n");
  printf(" |   |             ______   __    ___|___/\\__,_|_|    ____   _  "
         "_____ _   _        |   | \n");
  printf(" |   |            / ___\\ \\ / /   | __ )| ____|  _ \\  |  _ \\ / "
         "\\|_   _| | | |       |   | \n");
  printf(" |   |           | |    \\ V /____|  _ \\|  _| | |_) | | |_) / _ \\ "
         "| | | |_| |       |   | \n");
  printf(" |   |           | |___  | |_____| |_) | |___|  _ <  |  __/ ___ \\| "
         "| |  _  |       |   | \n");
  printf(" |   |            \\____| |_|     |____/|_____|_| \\_\\ |_| /_/   "
         "\\_\\_| |_| |_|       |   | \n");
  printf(" |___|                                                               "
         "              |___| \n");
  printf("(_____)--------------------------------------------------------------"
         "-------------(_____) \n\n\n");
  printf("RÈGLES DU JEU\n\n");
  printf("CY-BER PATH se joue avec un nombre de participants allant de 2 à 8!\n");
  printf("Le nombre de mouvements par manche et par joueur ne peut pas dépasser 20!\n");
  printf("Le jeu se joue en 5 manches max!\n");
  printf("Vous devez choisir un niveau de difficulté, plus la difficulté est élevée, plus la grille disparaît vite!\n");
  printf("[Facile = la grille s'affiche durant 60 secondes]\n");
  printf("[Moyen = la grille s'affiche durant 30 secondes]\n");
  printf("[Difficile = la grille s'affiche durant 15 secondes]\n\n");
  printf("==================================================\n\n\n");
}

int main(void) {
  srand(time(NULL));
  int taille =
      (rand() % 6) * 2 +
      31;

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

  printf("Combien de joueurs?(entre 2 et max 8 joueurs)");
  nb_joueurs = verification_saisie(2, 8);

  init_Joueur(joueurs, nb_joueurs);

  printf("Combien de manches voulez-vous ?(max 5 )");
  nb_manche = verification_saisie(1, 5);

  printf("niveau de difficulté\n1=facile, 2=moyen, 3=difficile ");
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
      printf("Égalité de pt ! Les joueurs suivants ont gagné :\n");
      for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].pts == maxpts) {
          printf("%s\n", joueurs[i].prenom);
        }
      }
    } else {
      printf("%s, vous avez gagné !\n",
             joueurs[ptsMax(joueurs, nb_joueurs)].prenom);
    }
    for (int i = 0; i < nb_joueurs; i++) {
      printf(" %s : %d pts\n", joueurs[i].prenom, joueurs[i].pts);
    }
  }

  return 0;
}
