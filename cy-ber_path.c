#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // Pour la fonction sleep

// Définition des constantes
#define TAILLE_MIN 25 // Taille minimale de la grille
#define TAILLE_MAX 30 // Taille maximale de la grille
#define NB_JOUEURS 4  // Nombre de joueurs
#define NB_CIBLES 18  // Nombre de cibles

// Structure pour représenter un robot (position x et y)
typedef struct {
  int x; // Coordonnée x du robot
  int y; // Coordonnée y du robot
} Robot;

// Structure pour représenter un joueur
typedef struct {
  Robot pos;      // Position du joueur sur la grille
  char cible;     // Cible assignée au joueur
  int mvtSaisi;   // Nombre de mouvements prédits par le joueur
  int mvtFait;    // Nombre de mouvements effectués par le joueur
  int mvtRestant; // Nombre de mouvements restants
  char nom[25];   // Nom du joueur (max 25 caractères)
} Joueur;

// Déclaration des variables globales
char grille[TAILLE_MAX][TAILLE_MAX]; // Grille de jeu
char cibles[NB_CIBLES];              // Tableau des cibles
const char *joueurEmojis[NB_JOUEURS] = {"\U0001F916", "\U0001F47D",
                                        "\U0001F47E", "\U0001F63E"};
// Emojis pour chaque joueur

// Procédure pour créer la grille de jeu
void creationGrille(int grilleTaille, int numJoueurs, Joueur *joueurs) {
  // Initialiser la grille avec des espaces vides
  for (int i = 0; i < grilleTaille; i++) {
    for (int j = 0; j < grilleTaille; j++) {
      grille[i][j] = ' ';
    }
  }

  // Placer les murs autour de la grille
  for (int i = 0; i < grilleTaille; i++) {
    grille[0][i] = '_';                // Mur supérieur
    grille[i][0] = '|';                // Mur gauche
    grille[grilleTaille - 1][i] = '_'; // Mur inférieur
    grille[i][grilleTaille - 1] = '|'; // Mur droit
  }

  // Initialiser les cibles (lettres de A à R)
  for (int i = 0; i < NB_CIBLES; i++) {
    cibles[i] = 'A' + i;
  }

  // Placer les cibles sur la grille
  for (int i = 0; i < NB_CIBLES; i++) {
    int x, y;
    int validPosition;
    int random;
    do {
      validPosition = 1;
      x = rand() % (grilleTaille - 4) + 2; // Éviter les bords
      y = rand() % (grilleTaille - 4) + 2; // Éviter les bords

      // Vérifier que la cible n'est pas à côté d'une autre cible
      for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
          if (grille[x + dx][y + dy] >= 'A' && grille[x + dx][y + dy] <= 'R') {
            validPosition = 0;
            break;
          }
        }
        if (!validPosition)
          break;
      }
    } while (!validPosition);

    grille[x][y] = cibles[i]; // Marquer la position de la cible avec une lettre

    // Placer des murs formant un angle autour de la cible
    random = rand() % 4;
    switch (random) {
    case 0:
      grille[x][y - 1] = '+';
      grille[x + 1][y] = '+';
      break;
    case 1:
      grille[x][y - 1] = '+';
      grille[x - 1][y] = '+';
      break;
    case 2:
      grille[x][y + 1] = '+';
      grille[x + 1][y] = '+';
      break;
    case 3:
      grille[x][y + 1] = '+';
      grille[x - 1][y] = '+';
      break;
    default:
      break;
    }
  }

  // Placer les murs perpendiculaires aux bordures
  for (int i = 0; i < 2; i++) {
    int r;
    r = rand() % (grilleTaille - 2) +
        1; // Position aléatoire pour chaque mur de chaque bordure
    while (grille[1][r] ==
           '+') { // Si un mur est déjà présent, choisir une autre position
      r = rand() % (grilleTaille - 2) + 1;
    }
    grille[1][r] = '+'; // Mur supérieur

    r = rand() % (grilleTaille - 2) + 1;
    while (grille[r][1] == '+') {
      r = rand() % (grilleTaille - 2) + 1;
    }
    grille[r][1] = '+'; // Mur gauche

    r = rand() % (grilleTaille - 2) + 1;
    while (grille[grilleTaille - 2][r] == '+') {
      r = rand() % (grilleTaille - 2) + 1;
    }
    grille[grilleTaille - 2][r] = '+'; // Mur inférieur

    r = rand() % (grilleTaille - 2) + 1;
    while (grille[r][grilleTaille - 2] == '+') {
      r = rand() % (grilleTaille - 2) + 1;
    }
    grille[r][grilleTaille - 2] = '+'; // Mur droit
  }

  // Placer les joueurs sur des positions aléatoires distinctes
  for (int i = 0; i < numJoueurs; i++) {
    int x, y;
    do {
      x = rand() % (grilleTaille - 2) + 1;
      y = rand() % (grilleTaille - 2) + 1;
    } while (grille[x][y] != ' '); // Assurer que la position est vide
    joueurs[i].pos.x = x;
    joueurs[i].pos.y = y;
    grille[x][y] = '1' + i; // Marquer la position du joueur avec son numéro
  }
}

// Procédure pour afficher la grille de jeu
void afficherGrille(int grilleTaille) {
  system("clear"); // Effacer le terminal
  for (int i = 0; i < grilleTaille; i++) {
    for (int j = 0; j < grilleTaille; j++) {
      if (grille[i][j] == '+') {
        printf(
            "\033[31m%c \033[0m",
            grille[i][j]); // Affiche les murs entourant les cibles '+' en rouge
      } else if (grille[i][j] >= 'A' && grille[i][j] <= 'R') {
        printf("\033[32m%c \033[0m",
               grille[i][j]); // Affiche les cibles en vert
      } else if (grille[i][j] >= '1' && grille[i][j] <= '4') {
        printf("%s  ",
               joueurEmojis[grille[i][j] - '1']); // Affiche l'emoji du joueur
      } else {
        printf("%c ", grille[i][j]); // Affiche tous les autres caractères
      }
    }
    printf("\n");
  }
}

// Procédure pour déplacer un joueur
void deplacementJoueur(Joueur *joueur, char sens, int grilleTaille,
                       Joueur *joueurs, int cibleAtteinte) {
  int mvX = joueur->pos.x; // Coordonnée x actuelle du joueur
  int mvY = joueur->pos.y; // Coordonnée y actuelle du joueur
  int dx = 0, dy = 0; // Variables pour déterminer la direction du déplacement
  static char previousValue =
      ' '; // Variable statique pour mémoriser la valeur précédente de la case

  // Déterminer la direction du déplacement
  switch (sens) {
  case 'h':
  case 'H':
    dx = -1;
    break; // Haut
  case 'b':
  case 'B':
    dx = 1;
    break; // Bas
  case 'g':
  case 'G':
    dy = -1;
    break; // Gauche
  case 'd':
  case 'D':
    dy = 1;
    break; // Droite
  default:
    break;
  }

  // Boucle pour déplacer le joueur en ligne droite jusqu'à rencontrer un
  // obstacle
  while (1) {
    int nvX = mvX + dx; // Nouvelle coordonnée x après déplacement
    int nvY = mvY + dy; // Nouvelle coordonnée y après déplacement

    // Vérifier si le déplacement est valide
    if (nvX < 0 || nvX >= grilleTaille || nvY < 0 || nvY >= grilleTaille ||
        grille[nvX][nvY] == '+' ||
        (grille[nvX][nvY] >= '1' && grille[nvX][nvY] <= '4')) {
      break; // Sortir de la boucle si un obstacle est rencontré
    }

    mvX = nvX; // Stocker les nouvelles coordonnées calculées
    mvY = nvY;
  }

  // Si la cible est atteinte
  if (grille[mvX][mvY] == joueur->cible) {
    grille[joueur->pos.x][joueur->pos.y] =
        previousValue; // Restaurer la valeur précédente de la case
    previousValue =
        grille[mvX][mvY]; // Sauvegarder la nouvelle valeur de la case
    joueur->pos.x = mvX;  // Déplacer le joueur aux nouvelles coordonnées
    joueur->pos.y = mvY;  // Déplacer le joueur aux nouvelles coordonnées
    grille[mvX][mvY] = joueur->cible;
  }

  // Si le joueur déclare forfait
  else if (sens == 'q' || sens == 'Q') {
    grille[joueur->pos.x][joueur->pos.y] =
        previousValue;   // Restaurer la valeur précédente de la case
    previousValue = ' '; // Garder un espace vide en mémoire pour le premier
                         // déplacement du prochain joueur
  }

  // Sinon, mettre à jour la grille
  else {
    grille[joueur->pos.x][joueur->pos.y] =
        previousValue; // Restaurer la valeur précédente de la case
    previousValue =
        grille[mvX][mvY]; // Sauvegarder la nouvelle valeur de la case
    joueur->pos.x = mvX;  // Déplacer le joueur aux nouvelles coordonnées
    joueur->pos.y = mvY;  // Déplacer le joueur aux nouvelles coordonnées
    grille[mvX][mvY] =
        '1' +
        (joueur - joueurs); // Mettre à jour la position du joueur sur la grille
  }
}

// Procédure pour afficher un compte à rebours
void countdown(int secondes) {
  for (int i = secondes; i > 0; i--) {
    printf("\rTemps restant: %d secondes", i); // Afficher le temps restant
    fflush(stdout);                            // Forcer l'affichage immédiat
    sleep(1);                                  // Attendre une seconde
  }
  printf("\rTemps écoulé!                    \n"); // Afficher le message de fin
                                                   // de compte à rebours
}

// Procédure  principale pour lancer le jeu à chaque manche
void lancerJeu(int grilleTaille, int numJoueurs, int nbManche,
               Joueur *joueurs) {
  int pointsManche[NB_JOUEURS] = {
      0}; // Tableau pour stocker les points de chaque manche
  int difficulte, tempsLimite;

  // Sélection du niveau de difficulté
  printf("Choisissez un niveau de difficulté (1: Facile, 2: Intermédiaire, 3: "
         "Difficile): ");
  scanf("%d", &difficulte);

  // Définir le temps limite en fonction de la difficulté
  switch (difficulte) {
  case 1:
    tempsLimite = 60;
    break;
  case 2:
    tempsLimite = 30;
    break;
  case 3:
    tempsLimite = 15;
    break;
  default:
    printf(
        "Niveau de difficulté invalide. Mode facile sélectionné par défaut.\n");
    tempsLimite = 60;
    break;
  }

  // Boucle de jeu pour chaque manche
  for (int manche = 1; manche <= nbManche; manche++) {
    grilleTaille = rand() % 6 +
                   25; // Choisir une taille de grille aléatoire entre 25 et 30
    creationGrille(grilleTaille, numJoueurs, joueurs); // Créer la grille
    afficherGrille(grilleTaille);                      // Afficher la grille
    printf("\nManche %d / %d\n", manche, nbManche);

    // Assigner une cible à chaque joueur
    for (int i = 0; i < numJoueurs; i++) {
      int cibleRand = rand() % NB_CIBLES;
      joueurs[i].cible = cibles[cibleRand];
      printf("%s %s - Votre devez atteindre la cible %c\n", joueurEmojis[i],
             joueurs[i].nom, joueurs[i].cible);
    }
    printf("\n");

    // Démarrer le compte à rebours
    countdown(tempsLimite);

    // Effacer la grille et les instructions
    system("clear");

    // Demander les prédictions de mouvements
    for (int i = 0; i < numJoueurs; i++) {
      int validInput = 0;
      while (!validInput) {
        printf("%s %s - Combien de mouvements pour atteindre la cible %c ?: ",
               joueurEmojis[i], joueurs[i].nom, joueurs[i].cible);
        if (scanf("%d", &joueurs[i].mvtSaisi) == 1 &&
            joueurs[i].mvtSaisi >= 1 && joueurs[i].mvtSaisi <= 20) {
          validInput = 1;
        } else {
          printf("Saisie incorrecte, veuillez recommencer \n");
          while (getchar() != '\n')
            ; // Vider le buffer d'entrée
        }
      }
      joueurs[i].mvtRestant =
          joueurs[i].mvtSaisi; // Initialiser les mouvements restants
    }

    // Réafficher la grille
    afficherGrille(grilleTaille);

    // Déplacements des joueurs et calcul des résultats
    for (int i = 0; i < numJoueurs; i++) {
      int mvtFait = 0;
      int cibleAtteinte = 0;

      // Boucle pour les déplacements des joueurs
      while (cibleAtteinte == 0) {
        afficherGrille(grilleTaille);
        printf("Manche %d\n\nà vous de jouer !\n\n", manche);

        // Vérifier si le joueur a encore des mouvements restants
        if (joueurs[i].mvtRestant == 0) {
          printf("%s %s - Vous avez effectué tous vos mouvements!\n",
                 joueurEmojis[i], joueurs[i].nom);
          break;
        }

        // Demander le déplacement au joueur
        char sens;
        printf("%s %s - Déplacement (haut: h, bas: b, gauche: g, droite: d, "
               "quitter: q) : \n",
               joueurEmojis[i], joueurs[i].nom);
        scanf(" %c", &sens);

        // Déplacer le joueur
        deplacementJoueur(&joueurs[i], sens, grilleTaille, joueurs,
                          cibleAtteinte);
        mvtFait++;
        joueurs[i].mvtRestant--;

        // Si le joueur déclare forfait
        if (sens == 'q' || sens == 'Q') {
          cibleAtteinte = 2;
          printf("Tu passes ton tour.\n\n");
          break;
        }

        // Vérifier si le mouvement est valide
        if (sens != 'h' && sens != 'H' && sens != 'g' && sens != 'G' &&
            sens != 'd' && sens != 'D' && sens != 'q' && sens != 'Q' &&
            sens != 'b' && sens != 'B') {
          printf("Mouvement incorrecte, veuillez recommencer\n\n");
          struct timespec req;
          req.tv_sec = 2;
          req.tv_nsec = 0;
          nanosleep(&req, NULL);
          mvtFait--;
          joueurs[i].mvtRestant++;
        }

        // Vérifier si le joueur a atteint la cible
        if (grille[joueurs[i].pos.x][joueurs[i].pos.y] == joueurs[i].cible) {
          cibleAtteinte = 1;
          printf("Félicitations ! vous avez atteint la cible !\n\n");
          struct timespec req;
          req.tv_sec = 2;
          req.tv_nsec = 0;
          nanosleep(&req, NULL);
          break;
        }
      }

      joueurs[i].mvtFait =
          mvtFait; // Enregistrer le nombre de mouvements effectués

      // Calcul des points
      if (cibleAtteinte == 1 && mvtFait == joueurs[i].mvtSaisi) {
        pointsManche[i] += 2; // 2 points si le joueur atteint la cible avec le
                              // nombre exact de mouvements prédits
      } else if (cibleAtteinte == 1 && mvtFait < joueurs[i].mvtSaisi) {
        pointsManche[i] -= 1; // Si un joueur atteint sa cible avec - de mvt que
                              // prévu, il perd 1 point.
      } else if (cibleAtteinte == 1 && mvtFait > joueurs[i].mvtSaisi ||
                 cibleAtteinte == 2) { // Si un joueur atteint sa cible avec +
                                       // de mvt que prévu, ou s'il passe, tous
                                       // les autres joueurs gagnent 1 point.
        for (int j = 0; j < numJoueurs; j++) {
          pointsManche[j] += 1;
        }
      }
    }
  }

  printf("\nScore Final :\n"); // affichage du score final
  int pointsMax = pointsManche[0];
  for (int i = 1; i < numJoueurs; i++) {
    if (pointsManche[i] > pointsMax) {
      pointsMax = pointsManche[i];
    }
  }

  int gagnant[NB_JOUEURS]; // détermination du gagnant
  int nbGagnant = 0;
  for (int i = 0; i < numJoueurs; i++) {
    if (pointsManche[i] == pointsMax) {
      gagnant[nbGagnant++] = i;
    }
  }

  if (nbGagnant > 1) { // gestion d'une égalité
    printf("Egalité! ");
    for (int i = 0; i < nbGagnant; i++) {
      printf("%s", joueurs[gagnant[i]].nom);
      if (i < nbGagnant - 1) {
        printf(" et ");
      }
    }
    printf(" remportent la partie!\n");
  } else {
    printf("%s remporte la partie !\n", joueurs[gagnant[0]].nom);
  }
}

// procédure pour afficher les règles du jeu
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
  printf("CY-BER PATH se joue avec 4 joueurs!\n");
  printf("Le jeu se joue en 10 manches max!\n");
  printf("Le nombre de mouvements par joueur lors d'une manche ne peut pas "
         "dépasser 20!\n");
  printf("Vous devez choisir un niveau de difficulté, plus la difficulté est "
         "élevée, plus la grille disparaît vite!\n");
  printf("[Facile = la grille s'affiche durant 60 secondes]\n");
  printf("[Moyen = la grille s'affiche durant 30 secondes]\n");
  printf("[Difficile = la grille s'affiche durant 15 secondes]\n\n");
  printf("==================================================\n\n\n");
}

int main() {
  int grilleTaille, numJoueurs = NB_JOUEURS, nbManche;
  srand(time(NULL));

  menu();

  Joueur joueurs[NB_JOUEURS];
  for (int i = 0; i < NB_JOUEURS; i++) {
    int validName = 0;
    while (!validName) {
      printf("Entrez le prénom du joueur %d (max 25 caractères): ",
             i + 1); // saisie du prénom des 4 joueurs
      scanf("%s", joueurs[i].nom);
      if (strlen(joueurs[i].nom) <= 25) {
        validName = 1;
      } else {
        printf(
            "Le prénom dépasse 25 caractères. Veuillez recommencer.\n"); // robustesse
                                                                         // pour
                                                                         // ne
                                                                         // pas
                                                                         // dépasser
                                                                         // la
                                                                         // taille
                                                                         // du
                                                                         // tableau
      }
    }
  }

  printf("Saissisez le nombre de manches: ");
  scanf("%d", &nbManche);

  while (getchar() != '\n' && (nbManche < 1 || nbManche > 10)) {
    printf("\n\nSaisie incorrecte, veuillez recommencer \n\n\n");
    printf("Saississez le nombre de manches que vous souhaitez jouer: ");
    scanf("%d", &nbManche);
  }

  lancerJeu(grilleTaille, numJoueurs, nbManche, joueurs);

  return 0;
}
