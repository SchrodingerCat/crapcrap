/**
* \file ia.c
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
#include <math.h>
#include "couleurs.h"
#include "functions.h"
#include "ia.h"

/**
 * \fn int ia1_init(int** laby, int** freq, int w, int h, struct Coordonnees* ia1)
 * \brief Fonction qui insère une ia pour la première fois dans le labyrinthe
 * \param laby
 * \param freq
 * \param w
 * \param h
 * \param ia1 
 * \return 1 si l'ia a bien été insérée
 */
int ia1_init(int** laby, int** freq, int w, int h, struct Coordonnees* ia1)
{
  int position_vide = cherche_vide(laby,w,h);                       
  ia1->y = position_vide%100;                                   
  ia1->x = (position_vide-(position_vide%100))/100;             
  laby = ia1_insere_dans_case(laby,w,h,ia1->y,ia1->x);           
  freq = remplir_mat_frequence(freq,ia1->y,ia1->x);             
  return 1;
} 

/**
 * \fn int ia1_premier_deplacement(int** laby, int** freq, int w, int h, struct Coordonnees* ia1_old, struct Coordonnees* ia1, int* direction)
 * \brief Fonction qui va effectuer le premier déplacement de l'ia
 * \param laby adresse
 * \param freq adresse
 * \param w
 * \param h
 * \param ia1_old adresse 
 * \param ia1 adresse
 * \param direction adresse
 * \return
 */
int ia1_premier_deplacement(int** laby, int** freq, int w, int h, struct Coordonnees* ia1_old, struct Coordonnees* ia1, int* direction)
{
  ia1_old->x = ia1->x;                                                    
  ia1_old->y = ia1->y;                                                    
  *direction = ia_cherche_deplacement(laby,ia1->y,ia1->x);                
  laby = ia1_deplace(laby,w,h,ia1->y,ia1->x, *direction );                 
  ia1->x = next_col(ia1_old->x, *direction);                              
  ia1->y = next_line(ia1_old->y, *direction);                             
  freq = remplir_mat_frequence(freq,ia1->y,ia1->x);                       
  return 1;
}

/**
 * \fn  int** ia1_insere_dans_case(int** laby, int w, int h, int pos_y, int pos_x)
 * \brief Fonction qui insère une ia dans le labyrinthe
 * \param laby adresse du labyrinthe
 * \param w taille en largeur de la matrice
 * \param h taille en hauteur de la matrice
 * \param pos_y ancien numéro de ligne
 * \param pos_x ancien numéro de colonne
 * \return l'adresse du labyrinthe contenant l'ia
 */
int** ia1_insere_dans_case(int** laby, int w, int h, int pos_y, int pos_x)
{
  laby[pos_y][pos_x] = 4;
  return laby;
}

/**
 * \fn int** ia1_deplace(int** laby, int w, int h, int old_y, int old_x, int direction)
 * \brief Fonction qui déplace une fois une ia dans le labyrinthe
 * \param laby adresse du labyrinthe
 * \param w taille en largeur de la matrice
 * \param h taille en hauteur de la matrice
 * \param old_y ancien numéro de ligne
 * \param old_x ancien numéro de colonne
 * \param direction direction que doit prendre l'ia 
 * \return l'adresse du labyrinthe une fois l'ia déplacée une fois
 *
 * La direction est notée telle que:
 * 0 : haut
 * 1 : droite
 * 2 : bas
 * 3 : gauche
 */
int** ia1_deplace(int** laby, int w, int h, int old_y, int old_x, int direction)
{
  laby[old_y][old_x] = 0;
  if(direction == 0) {
    //nord
    laby = ia1_insere_dans_case(laby,w,h,old_y-1,old_x);
  }
  else if(direction == 1) {
    //est
    laby = ia1_insere_dans_case(laby,w,h,old_y,old_x+1);
  }
  else if(direction == 2) {
    //sud
    laby = ia1_insere_dans_case(laby,w,h,old_y+1,old_x);
  }
  else if(direction == 3) {
    //ouest
    laby = ia1_insere_dans_case(laby,w,h,old_y,old_x-1);
  }
  else{
    printf("Mauvaise direction\n");
  }
  return laby;
}

/**
 * \fn int ia_cherche_deplacement(int** laby, int old_y, int old_x) 
 * \brief Fonction qui recherche une direction indiquant une case vide
 * \param laby adresse du labyrinthe
 * \param old_y ancien numéro de ligne
 * \param old_x ancien numéro de colonne
 * \return Direction menant à une case vide
 *
 * L'ordre de test des directions est aléatoire.
 *
 * La direction est notée telle que:
 * 0 : haut
 * 1 : droite
 * 2 : bas
 * 3 : gauche
 */
int ia_cherche_deplacement(int** laby, int old_y, int old_x)
{
  int deplacement_possible = 0;
  int test_direction;
  int count = 0;

  while( !(deplacement_possible == 1 || count >25) ) {
    test_direction = rand()%4;
    if(test_direction == 0) {
      if(laby[old_y-1][old_x]==0){
        deplacement_possible = 1;
      }
    }
    else if(test_direction == 1) {
      if(laby[old_y][old_x+1]==0){
        deplacement_possible = 1;
      }
    }
    else if(test_direction == 2) {
      if(laby[old_y+1][old_x]==0){
        deplacement_possible = 1;
      }
    }
    else if(test_direction == 3) {
      if(laby[old_y][old_x-1]==0){
        deplacement_possible = 1;
      }
    }
    count++;
  }
  return test_direction;
}

/**
 * \fn int ia_dir_relative(int** laby , int old_y , int old_x , int old_dir)
 * \brief Fonction qui recherche une direction relative pour l'ia1 en fonction de son précédent déplacement
 * \param laby adresse du labyrinthe
 * \param old_y ancien numéro de ligne
 * \param old_x ancien numéro de colonne
 * \param old_dir précédente direction 
 * \return Direction du prochain déplacement
 *
 * La nouvelle direction possible, avec une notation relative telle que :
 * 3 : tourne a gauche 3 fois (270°)
 * 2 : tourne a gauche 2 fois (180°)
 * 1 : tourne a gauche 1 fois ( 90°)
 * 0 : garde la précédente direction
 */
int ia_dir_relative(int** laby , int old_y , int old_x , int old_dir)
{
  int tirage = rand()%100;
  int new_direction_relative;
  // tout droit
  if ( tirage < 90 && est_case_vide_avec_direction(laby,old_dir,old_y,old_x) ) {
    new_direction_relative = 0;
  }
  // tourner à gauche
  else if ( (tirage >= 90 && tirage < 94) && est_case_vide_avec_direction(laby, dir_relative_to_absolue(laby,old_dir,1) ,old_y,old_x) ) {
    new_direction_relative = 1;
  }
  // tourner à droite
  else if ( (tirage >= 94 && tirage < 98) && est_case_vide_avec_direction(laby, dir_relative_to_absolue(laby,old_dir,3) ,old_y,old_x) ) {
    new_direction_relative = 3;
  }
  // reculer
  else {
    new_direction_relative = 2;
  }
  return new_direction_relative;
}

/**
 * \fn void ia1_play(int** laby, int** freq, int w, int h,int* direction, struct Coordonnees* ia1)
 * \brief Fonction qui permet à l'ia de proceder à un déplacement 
 * \param laby adresse du labyrinthe
 * \param freq adresse de la matrice des frequences
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \param direction adresse de la direction de l'ia
 * \param ia1 adresse de la position de l'ia1
 */
void ia1_play(int** laby, int** freq, int w, int h,int* direction, struct Coordonnees* ia1)
{
  *direction = dir_relative_to_absolue(laby , *direction , ia_dir_relative(laby , ia1->y , ia1->x , *direction) );
  laby = ia1_deplace(laby,w,h,ia1->y,ia1->x, *direction );
  ia1->x = next_col(ia1->x, *direction);
  ia1->y = next_line(ia1->y, *direction);
  freq = remplir_mat_frequence(freq,ia1->y,ia1->x);
}

/**
 * \fn int ia2_init(int** laby, int** freq, int w, int h, struct Coordonnees* ia2)
 * \brief Fonction qui insère une ia pour la première fois dans le labyrinthe
 * \param laby
 * \param freq
 * \param w
 * \param h
 * \param ia2 
 * \return 1 si l'ia a bien été insérée
 */
int ia2_init(int** laby, int** freq, int w, int h, struct Coordonnees* ia2)
{
  int position_vide = cherche_vide(laby,w,h);                       
  ia2->y = position_vide%100;                                   
  ia2->x = (position_vide-(position_vide%100))/100;             
  laby = ia2_insere_dans_case(laby,w,h,ia2->y,ia2->x);           
  freq = remplir_mat_frequence(freq,ia2->y,ia2->x);             
  return 1;
} 

/**
 * \fn int ia2_premier_deplacement(int** laby, int** freq, int w, int h, struct Coordonnees* ia2_old, struct Coordonnees* ia2, int* direction)
 * \brief Fonction qui va effectuer le premier déplacement de l'ia
 * \param laby adresse
 * \param freq adresse
 * \param w
 * \param h
 * \param ia2_old adresse 
 * \param ia2 adresse
 * \param direction adresse
 * \return
 */
int ia2_premier_deplacement(int** laby, int** freq, int w, int h, struct Coordonnees* ia2_old, struct Coordonnees* ia2, int* direction)
{
  ia2_old->x = ia2->x;                                                    
  ia2_old->y = ia2->y;                                                    
  *direction = ia_cherche_deplacement(laby,ia2->y,ia2->x);                
  laby = ia2_deplace(laby,w,h,ia2->y,ia2->x, *direction );                 
  ia2->x = next_col(ia2_old->x, *direction);                              
  ia2->y = next_line(ia2_old->y, *direction);                             
  freq = remplir_mat_frequence(freq,ia2->y,ia2->x);                       
  return 1;
}

/**
 * \fn  int** ia2_insere_dans_case(int** laby, int w, int h, int pos_y, int pos_x)
 * \brief Fonction qui insère une ia dans le labyrinthe
 * \param laby adresse du labyrinthe
 * \param w taille en largeur de la matrice
 * \param h taille en hauteur de la matrice
 * \param pos_y ancien numéro de ligne
 * \param pos_x ancien numéro de colonne
 * \return l'adresse du labyrinthe contenant l'ia
 */
int** ia2_insere_dans_case(int** laby, int w, int h, int pos_y, int pos_x)
{
  laby[pos_y][pos_x] = 5;
  return laby;
}

/**
 * \fn int** ia2_deplace(int** laby, int w, int h, int old_y, int old_x, int direction)
 * \brief Fonction qui déplace une fois une ia dans le labyrinthe
 * \param laby adresse du labyrinthe
 * \param w taille en largeur de la matrice
 * \param h taille en hauteur de la matrice
 * \param old_y ancien numéro de ligne
 * \param old_x ancien numéro de colonne
 * \param direction direction que doit prendre l'ia 
 * \return l'adresse du labyrinthe une fois l'ia déplacée une fois
 *
 * La direction est notée telle que:
 * 0 : haut
 * 1 : droite
 * 2 : bas
 * 3 : gauche
 */
int** ia2_deplace(int** laby, int w, int h, int old_y, int old_x, int direction)
{
  laby[old_y][old_x] = 0;
  if(direction == 0) {
    //nord
    laby = ia2_insere_dans_case(laby,w,h,old_y-1,old_x);
  }
  else if(direction == 1) {
    //est
    laby = ia2_insere_dans_case(laby,w,h,old_y,old_x+1);
  }
  else if(direction == 2) {
    //sud
    laby = ia2_insere_dans_case(laby,w,h,old_y+1,old_x);
  }
  else if(direction == 3) {
    //ouest
    laby = ia2_insere_dans_case(laby,w,h,old_y,old_x-1);
  }
  else{
    printf("Mauvaise direction\n");
  }
  return laby;
}

/**
 * \fn 
 * \brief Fonction qui permet à l'ia de proceder à un déplacement 
 * \param laby adresse du labyrinthe
 * \param freq2 adresse de la matrice des frequences
 * \param w taille en largeur du labyrinthe
 * \param h taille en hauteur du labyrinthe
 * \param direction adresse de la direction de l'ia
 * \param ia2 adresse de la position de l'ia2
 * \param datas1 adresse...
 */
void ia2_play(int** laby, int** freq, int w, int h,int* direction, struct Coordonnees* ia2, struct Datas_ddr* datas1)
{
  *direction = ia2_direction(laby, datas1, ia2->y, ia2->x, *direction);
  laby = ia2_deplace(laby,w,h,ia2->y,ia2->x, *direction );
  ia2->x = next_col(ia2->x, *direction);
  ia2->y = next_line(ia2->y, *direction);
  freq = remplir_mat_frequence(freq,ia2->y,ia2->x);
}

/**
 * \fn int ia2_direction(int** laby, struct Datas_ddr* datas1,int old_y, int old_x, int old_dir)
 * \brief Fonction qui recherche une direction relative pour l'ia1 en fonction de son précédent déplacement
 * \param laby adresse du labyrinthe
 * \param datas1 adresse des des données sur le joueur
 * \param old_y ancien numéro de ligne
 * \param old_x ancien numéro de colonne
 * \param old_dir précédente direction 
 * \param distances
 * \return Direction du prochain déplacement
 *
 *
 * ATTENTION: il faut que vérifie que l'ia peut se déplacer sans passer dans un mur!!
 *
*/
int ia2_direction(int** laby, struct Datas_ddr* datas1,int old_y, int old_x, int old_dir)
{
  int new_direction;
  struct Coordonnees* position_joueur = init_struct_coord();
  struct Coordonnees* position_ia = init_struct_coord();
  struct Coordonnees* distances = init_struct_coord();
  position_joueur->x = datas1->x_joueur;
  position_joueur->y = datas1->y_joueur;
  position_ia->x = old_x;
  position_ia->y = old_y;

  distance_entre_positions(position_joueur, position_ia, distances);
  if(distances->x < 0) {
    if(distances->y < 0) {
      if(abs(distances->x) > abs(distances->y)) {
        //gauche
        new_direction = 3; 
      }
      else {
        //haut
        new_direction = 0;
      }
    }
    else {
      if(abs(distances->x) > abs(distances->y)) {
        //gauche
        new_direction = 3;
      }
      else {
        //bas
        new_direction = 2;
      } 
    }
  }
  else {
    if(distances->y < 0) {
      if(abs(distances->x) > abs(distances->y)) {
        //droite
        new_direction = 1;
      }
      else {
        //haut
        new_direction = 0;
      } 
    }
    else {
      if(abs(distances->x) > abs(distances->y)) {
        //droite
        new_direction = 1;
      }
      else {
        //bas
        new_direction = 2;
      }
    }
  }
  return new_direction;
}

  /**
   * \fn 
 * \brief Fonction qui calcule la distance (x,y) entre 2 points 
 * \param position1
 * \param position2
 * \param distance
 * \return 
 */
int distance_entre_positions (struct Coordonnees* position1,struct Coordonnees* position2,struct Coordonnees* distances)
{
  distances->x = position1->x - position2->x;
  distances->y = position1->y - position2->y;
  return EXIT_SUCCESS;
}

