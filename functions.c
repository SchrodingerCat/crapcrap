/**
* \file functions.c
* \brief Programme contenant les fonctions du cracrap
* \author Raphael.C
* \version 0.0.0.0.9
* \date 12 juillet 2012
*
* Programme de crapcraping.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "couleurs.h"
#include "functions.h"
#include "ia.h"

/**
 * \fn int** init_mat(int w, int h)
 * \brief Fonction d'allocation en mémoire d'une matrice bidimentionnelle
 * \param w taille en largeur de la matrice
 * \param h taille en hauteur de la matrice
 * \return l'adresse de la matrice
 */
/**
 * \fn
 * \brief
 * \param
 * \return
 */
int** init_mat(int w, int h)
{
  int i;
  int** laby;
  laby = (int **) malloc(sizeof(int *) * w);
  for(i=0;i<w;i++) {
    laby[i] = (int *) malloc(sizeof(int) * h);
  }
  return laby;
}

/**
 * \fn int** init_mur(int** laby, int w, int h)
 * \brief Fonction de création de murs dans toute la matrice
 * \param laby adresse du labyrinthe
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \return l'adresse du labyrinthe avec des murs
 */
int** init_mur(int** laby, int w, int h)
{
  int i,j;
  for(i=0 ; i<h ; i++) {
    for(j=0 ; j<w ; j++) {
      laby[i][j] = 1;
    }
  }
  return laby;
}

/**
 * \fn int** init_bord(int** laby, int w, int h)
 * \brief Fonction de création de bords délimitant le labyrinthe
 * \param laby adresse du labyrinthe
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \return l'adresse du labyrinthe avec des bords
 */
int** init_bord(int** laby, int w, int h)
{
  int i,j;
  for(i=0 ; i<h ; i++) {
    for(j=0 ; j<w ; j++) {
      if((i==0) || (j==0) || (i==h-1) || (j==w-1)) {
        laby[i][j] = 2;
      }
    }
  }
  return laby;
}

/**
 * \fn void show_laby(int** laby, int w, int h)
 * \brief Fonction d'affichage du labyrinthe
 * \param laby adresse du labyrinthe
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \return rien
 * rappel du contenu possible du labyrinthe:
 * 0 : vide
 * 1 : mur
 * 2 : bord
 * 3 : joueur
 * 4 : ia
 */
void show_laby(int** laby, int w, int h)
{
  int i,j;
  for(i=0 ; i<h ; i++) {
    for(j=0 ; j<w ; j++) {
      if(laby[i][j]==0) {
        //couleur("40");
        printf(" ");
      }
      else if(laby[i][j]==1) {
        couleur("47");
        printf(" ");
      }
      else if(laby[i][j]==2) {
        couleur("47");
        printf(" ");
      }
      else if(laby[i][j]==3) {
        couleur("42");
        printf(" ");
      }
      else if(laby[i][j]==4) {
        couleur("41");
        printf("1");
      }
      else if(laby[i][j]==5) {
        couleur("43");
        printf("2");
      }
      couleur("0");
    }
    printf("\n");
  }
}

/**
 * \fn void show_freq(int** freq, int w, int h, int total_deplacement)
 * \brief Fonction d'affichage des frequences d'apparition de l'ia par case
 * \param freq adresse de la matrice des frequences
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \param total_deplacement c'est le nombre de déplacement déjà effectué par l'ia
 * \return rien
 */
void show_freq(int** freq, int w, int h, int total_deplacement)
{
  double seuil_bleu = 0.15;
  double seuil_vert = 0.30;
  double seuil_orange = 0.50;
  int i,j;
  for(i=0 ; i<h ; i++) {
    for(j=0 ; j<w ; j++) {
      if((freq[i][j]==-2)) {
        couleur("40");
        printf(" ");
      }
      else if(freq[i][j]==-1) {
        couleur("40");
        printf(" ");
      }
      else if(freq[i][j]==0) {
        printf(" ");
      }
      else if( (( (double)freq[i][j])/total_deplacement) <= seuil_bleu/100) {
        couleur("46");
        printf(" ");
      }
      else if( (( (double)freq[i][j])/total_deplacement) <= seuil_vert/100) {
        couleur("42");
        printf(" ");
      }
      else if( (( (double)freq[i][j])/total_deplacement) <= seuil_orange/100) {
        couleur("43");
        printf(" ");
      }
      else {
        couleur("41");
        printf(" ");
      }
      couleur("0");
    }
    printf("\n");
  }
}

/**
 * \fn int next_col(int pos_x, int dir)
 * \brief Fonction qui détermine un numéro de colonne à partir d'un numéro de colonne et d'un direction
 * \param pos_x ancien numéro de colonne
 * \param dir direction
 * \return nouveau numéro de colonne
 */
int next_col(int pos_x, int dir)
{
  if((dir==0) || (dir==2)) {
    return pos_x;
  }
  else if(dir==3) {
    return pos_x-1;
  }
  else{
    return pos_x+1;
  }
}

/**
 * \fn int next_line(int pos_y, int dir)
 * \brief Fonction qui détermine un numéro de ligne à partir d'un numéro de ligne et d'un direction
 * \param pos_y ancien numéro de ligne
 * \param dir direction
 * \return nouveau numéro de ligne
 */
int next_line(int pos_y, int dir)
{
  if((dir==3) || (dir==1)) {
    return pos_y;
  }
  else if(dir==0) {
    return pos_y-1;
  }
  else{
    return pos_y+1;
  }
}

/**
 * \fn int cherche_mur(int** laby, int w, int h)
 * \brief Fonction de recherche d'une position dans le labyrinthe correspondant a un mur
 * \param laby adresse du labyrinthe
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \return une position dans le labyrinthe sous la forme { (100*x) + y }
 */
int cherche_mur(int** laby, int w, int h)
{
  int coord_x = 0;
  int coord_y = 0;
  int is_a_mur = 0;
  int limite = 0;
  while( !(is_a_mur == 1) ) {
    coord_x = rand()%(w);
    coord_y = rand()%(h);
    if(laby[coord_y][coord_x]==1) {
      is_a_mur = 1;
    }
    limite = limite +1;
  }
  return ((100*coord_x) + coord_y);
}

/**
 * \fn int** insere_joueur(int** laby, int w, int h, int pos_y, int pos_x)
 * \brief Fonction qui insère un joueur dans le labyrinthe
 * \param laby adresse du labyrinthe
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \param pos_y position en y
 * \param pos_x position en x
 * \return adresse du labyrinthe avec le joueur
 */
int** insere_joueur(int** laby, int w, int h, int pos_y, int pos_x)
{
  if(pos_x<=w && pos_y<=h) {
    laby[pos_y][pos_x] = 3;
  }
  else {
    printf("ERREUR:impossible d'inserer le joueur en dehors du labyrinthe\n");
    return laby;
  }
  return laby;
}

/**
 * \fn int cherche_vide(int** laby, int w, int h)
 * \brief Fonction de recherche d'une case vide
 * \param laby adresse du labyrinthe
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \return une position dans le labyrinthe sous la forme { (100*x) + y }
 */
int cherche_vide(int** laby, int w, int h)
{
  int trouve_vide = 0;
  int x,y,c;
  while( !(trouve_vide == 1) ) {
    x = rand()%(w);
    y = rand()%(h);
    if(laby[y][x]==0) {
      trouve_vide = 1;
      c = 100*x + y;
    }
  }
  return c;
}

/**
 * \fn int est_case_vide_avec_direction(int** laby, int direction, int old_y, int old_x)
 * \brief Fonction qui teste si une coordonnée indiqué par une position (x,y) ainsi que une direction, est vide
 * \param laby adresse du labyrinthe
 * \param old_y ancien numéro de ligne
 * \param old_x ancien numéro de colonne
 * \param direction direction
 * \return 1 si vrai, 0 sinon
 *
 * La direction est notée telle que:
 * 0 : haut
 * 1 : droite
 * 2 : bas
 * 3 : gauche
 */
int est_case_vide_avec_direction(int** laby, int direction, int old_y, int old_x)
{
  int colonne = next_col(old_x, direction);
  int ligne = next_line(old_y, direction);
  if(laby[ligne][colonne] == 0) {
    return 1;
  }
  else {
    return 0;
  }
}

/**
 * \fn int est_case_vide(int** laby, struct Coordonnees* position)
 * \brief Fonction qui teste si une coordonnée indiquée, est vide
 * \param laby adresse du labyrinthe
 * \param position adresse de la structure des coordonnées
 * \return 1 si vrai, 0 sinon
 */
int est_case_vide(int** laby, struct Coordonnees* position)
{
  if(laby[position->y][position->x] == 0) {
    return 1;
  }
  else {
    return 0;
  }
}

/**
 * \fn int dir_relative_to_absolue (int** laby , int old_dir , int dir_relative)
 * \brief Fonction qui determine une direction absolue en fonction d'une ancienne direction et d'une direction relative
 * \param laby adresse du labyrinthe
 * \param old_dir ancienne direction
 * \param dir_relative direction relative
 * \return Direction absolue
 *
 * La direction relative telle que :
 * 3 : tourne a gauche 3 fois (270°)
 * 2 : tourne a gauche 2 fois (180°)
 * 1 : tourne a gauche 1 fois ( 90°)
 * 0 : garde la précédente direction
 *
 * La direction absolue telle que:
 * 0 : haut
 * 1 : droite
 * 2 : bas
 * 3 : gauche
 */
int dir_relative_to_absolue (int** laby , int old_dir , int dir_relative)
{
  //tout droit
  if (dir_relative == 0) {
    return old_dir;
  }
  //tourne anti-horaire 1 fois
  else if (dir_relative == 1) {
    if (old_dir == 0) {
      return 3;
    }
    else {
      return (old_dir - 1);
    }
  }
  //tourne anti-horaire 2 fois
  else if (dir_relative == 2) {
    if ( (old_dir == 0) || (old_dir == 1) ) {
      return (old_dir + 2);
    }
    else {
      return (old_dir - 2);
    }
  }
  //tourne anti-horaire 3 fois
  else if (dir_relative == 3) {
      if (old_dir == 3) {
        return 0;
      }
      else {
        return (old_dir + 1);
      }
  }
  else {
    printf("Conversion impossible >< \n");
    return 42;
  }
}

/**
 * \fn int** fabrique_mat_frequence(int** laby, int** freq, int w, int h)
 * \brief Fonction qui construit une proto-matrice de frequence, en fonction de la matrice du jeu
 * \param laby adresse du labyrinthe
 * \param freq adresse de la matrice des frequences
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \return adresse de la matrice de frequence
 *
 * la matrice des frequences contenant:
 * -1 si le labyrinthe contient un mur
 * -2 si le labyrinthe contient un bord
 * 0 si le labyrinthe contient une case vide
*/
int** fabrique_mat_frequence(int** laby, int** freq, int w, int h)
{
  int i,j;

  for(i=0;i<h;i++) {
    for(j=0;j<w;j++) {
      if(laby[i][j] == 1) {
        freq[i][j] = -1;
      }
      else if(laby[i][j] == 2) {
        freq[i][j] = -2;
      }
      else if(laby[i][j] == 0) {
        freq[i][j] = 0;
      }
    }
  }
  return freq;
}

/**
 * \fn int** remplir_mat_frequence(int** freq, int pos_y, int pos_x)
 * \brief
 * \param freq adresse de la matrice des frequences
 * \param pos_y position en y où il faut incrémenter la matrice des fréquences
 * \param pos_x position en x où il faut incrémenter la matrice des fréquences
 * \return l'adresse de la matrice des frequences incrémentée
 */
int** remplir_mat_frequence(int** freq, int pos_y, int pos_x)
{
  freq[pos_y][pos_x] = freq[pos_y][pos_x] + 1;
  return freq;
}

/**
 * \fn int** trace_obstacles(int** laby, int w, int h)
 * \brief Fonction qui trace des murs dans le labyrinthe selon un modèle
 * \param laby adresse du labyrinthe
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \return l'adresse du labyrinthe avec les murs
 */
int** trace_obstacles(int** laby, int w, int h)
{
  int i,j;
  int test_ligne = 1;
  int test_colonne = 1;
  for(i=1 ; i<(h-1) ; i++) {
    test_colonne = 1;
    if(test_ligne == 0 || test_ligne == 1) {
      for(j=1 ; j<(w-1) ; j++) {
       if(test_colonne==0 || test_colonne==1) {
          laby[i][j] = 1;
          test_colonne++;
        }
        else {
          test_colonne = 0;
          laby[i][j] = 0;
        }
      }
    test_ligne++;
    }
    else {
      for(j=1 ; j<(w-1) ; j++) {
        laby[i][j] = 0;
      }
      test_ligne = 0;
    }
  }
  return laby;
}

/**
 * \fn int** joueur_insertion(int** laby, int w, int h, struct Coordonnees* joueur)
 * \brief Fonction qui insère un joueur dans le labyrinthe à une position en argument
 * \param laby adresse du labyrinthe
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \param joueur coordonnées du joueur
 * \return l'adresse du labyrinthe une fois le joueur inséré
 */
int** joueur_insertion(int** laby, int w, int h, struct Coordonnees* joueur)
{
  int position_vide = cherche_vide(laby,w,h);
  int position_vide_y = position_vide%100;
  int position_vide_x = (position_vide - (position_vide%100)) / 100;
  laby[position_vide_y][position_vide_x] = 3;
  (*joueur).x = position_vide_x;
  (*joueur).y = position_vide_y;
  return laby;
}

/**
 * \fnstruct Coordonnees* init_struct_coord()
 * \brief Fonction d'allocation en mémoire d'une structure contenant un couple de coordonnées (x,y)
 * \return l'adresse de la structure des coordonnées (x,y)
 */
struct Coordonnees* init_struct_coord()
{
  struct Coordonnees* position = NULL;
  position = malloc(sizeof(int)* 2);
  position->x = 0;
  position->y = 0;
  return position;
}

/**
 * \fn struct Datas_ddr* init_struct_datas_ddr(int** laby, struct Coordonnees* position)
 * \brief Fonction d'allocation en mémoire d'une structure contenant l'adresse de la structure de la position du joueur, un déplacement possible, et l'adresse du labyrinthe
 * \param laby adresse du labyrinthe
 * \param joueur coordonnées du joueur
 * \return l'adresse de la structure des données destinées a la fonction "demande_direction_relative(void* data)" qui sera lancée dans un thread
 */
struct Datas_ddr* init_struct_datas_ddr(int** laby, struct Coordonnees* position)
{
  struct Datas_ddr* data1 = NULL;
  data1 = malloc((sizeof(int**)*1) + (sizeof(int)*3));
  data1->laby1 = laby;
  data1->y_joueur = position->y;
  data1->x_joueur = position->x;
  data1->direction = ia_cherche_deplacement(laby,position->y,position->x);
  return data1;
}

/**
 * \fn void* demande_direction_relative(void* data)
 * \brief Fonction servant à capturer le déplacements que souhaite le joueur, en capturant les touches du clavier, cette fonction sera placée dans un thread
 * \param data données brutes qui contiennent une "struct Datas_ddr*"
 */
void* demande_direction_relative(void* data)
{
  //decast:
  struct Datas_ddr* data1 = (struct Datas_ddr*) data;
  //boucle
  while(1) {
    int deplacement_possible = 0;
    char touche_joueur = '\n';
    while(!(deplacement_possible==1)) {
      touche_joueur = getchar();
      if(touche_joueur == 'z' && est_case_vide_avec_direction(data1->laby1, 0, data1->y_joueur -1, data1->x_joueur)) {
        deplacement_possible = 1;
      }
      else if(touche_joueur == 's' && est_case_vide_avec_direction(data1->laby1, 2, data1->y_joueur +1, data1->x_joueur)) {
        deplacement_possible = 1;
      }
      else if(touche_joueur == 'q' && est_case_vide_avec_direction(data1->laby1, 3, data1->y_joueur, data1->x_joueur -1)) {
        deplacement_possible = 1;
      }
      else if(touche_joueur == 'd' && est_case_vide_avec_direction(data1->laby1, 1, data1->y_joueur, data1->x_joueur +1)) {
        deplacement_possible = 1;
      }
    }
    if (touche_joueur == 'z') {
      data1->direction = 0;
    }
    else if(touche_joueur == 'q') {
      data1->direction = 3;
    }
    else if(touche_joueur == 's') {
      data1->direction = 2;
    }
    else if(touche_joueur == 'd') {
      data1->direction = 1;
    }
  }
}

/**
 * \fn int** deplace_joueur(int** laby, int w, int h, struct Datas_ddr* datas1, int direction)
 * \brief Fonction permettant un déplacement du joueur dans le labyrinthe en fonction des informations obtenues par la fonction "void* demande_direction_relative(void* data)"
 * \param laby adresse du labyrinthe
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \param datas1 adresse de la structure de données contenant la position du joueur, et la direction que souhaite l'humain qui dirige le joueur
 * \param direction adresse de la direction de l'ia
 * \return l'adresse du labyrinthe une fois le joueur déplacé
 *
 * REMARQUE: l'argument direction est inutile car la direction est dans datas1 --> [à MODIFIER]
 *
 */
int** deplace_joueur(int** laby, int w, int h, struct Datas_ddr* datas1, int direction)
{
  if( direction == 0 && (est_case_vide_avec_direction(laby, direction, datas1->y_joueur -1, datas1->x_joueur)) ) {
    //nord
    laby[datas1->y_joueur][datas1->x_joueur] = 0;
    laby[datas1->y_joueur - 1][datas1->x_joueur] = 3;
    datas1->y_joueur = datas1->y_joueur - 1;
  }
  else if(direction == 1 && (est_case_vide_avec_direction(laby, direction, datas1->y_joueur, datas1->x_joueur +1)) ) {
    //est
    laby[datas1->y_joueur][datas1->x_joueur] = 0;
    laby[datas1->y_joueur][datas1->x_joueur + 1] = 3;
    datas1->x_joueur = datas1->x_joueur + 1;
  }
  else if(direction == 2 && (est_case_vide_avec_direction(laby, direction, datas1->y_joueur +1, datas1->x_joueur)) ) {
    //sud
    laby[datas1->y_joueur][datas1->x_joueur] = 0;
    laby[datas1->y_joueur + 1][datas1->x_joueur] = 3;
    datas1->y_joueur = datas1->y_joueur + 1;
  }
  else if(direction == 3 && (est_case_vide_avec_direction(laby, direction, datas1->y_joueur, datas1->x_joueur -1)) ) {
    //ouest
    laby[datas1->y_joueur][datas1->x_joueur] = 0;
    laby[datas1->y_joueur][datas1->x_joueur - 1] = 3;
    datas1->x_joueur = datas1->x_joueur - 1;
  }
  else{
    printf("Mauvaise direction\n");
  }
  return laby;
}

/**
 * \fn int verif_distances_joueur_vs_ia (int** laby, struct Datas_ddr* position_joueur, struct Coordonnees* ia)
 * \brief Fonction qui vérifie si la distance entre une ia et un joueur n'est pas proche de zéro
 * \param laby adresse du labyrinthe
 * \param joueur adresse de la structure contenant la position du joueur
 * \param ia adresse de la structure contenant la position de l'ia
 * \return 1 si l'ia est a moins de 2 cases du joueur
 */
int verif_distances_joueur_vs_ia (int** laby, struct Datas_ddr* position_joueur, struct Coordonnees* ia)
{
  struct Coordonnees* joueur = init_struct_coord();
  struct Coordonnees* distances = init_struct_coord();
  int test = 0;
  joueur->y = position_joueur->y_joueur;
  joueur->x = position_joueur->x_joueur;
  distance_entre_positions(joueur, ia, distances);
  //printf("dst: %d,%d", distances->y, distances->x);
  if ((abs(distances->x) <= 1) && (abs(distances->y) <= 1)) {
    test = 1;
  }
  return test;
}
