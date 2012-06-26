#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "couleurs.h"
#include "functions.h"

/* fun init_mat
 */
int** init_mat(int w, int h) 
{
  int i;
  int** laby;
  laby = malloc(sizeof(int) * w);
  for(i=0;i<h;i++) {
    laby[i] = malloc(sizeof(int) * h);
  }
  return laby;
}

/* fun init_mur
 * arg w : largeur
 * arg h : hauteur
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

/* fun init_bord
 * arg w : largeur
 * arg h : hauteur
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

/* fun show_laby
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
        //couleur("32;40");
        printf("J");
      }
      else if(laby[i][j]==4) {
        couleur("45");
        printf("i");
      }
      couleur("0");
    }
    printf("\n");
  }
}

/* fun show_freq
 * affiche les frequences d'appartition de l'ia par case
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

/* fun next_col
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
/* fun next_line
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


/* fun cherche_mur : recherche un mur
 * arg w : largeur
 * arg h : hauteur
 * si (x,y) est un mur alors 
 * retourne c = (100*x) + y
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
    //printf("%d\n", limite);
  }
  //if(limite>=10000000) {
  //  return 0;
  //}
  return ((100*coord_x) + coord_y);
}

/* fun insere_joueur : insere le joueur dans le laby
 * arg laby : le laby
 * arg w
 * arg h 
 * pos_x : position joueur en x
 * pos_y : position joueur en y
 */
int** insere_joueur(int** laby, int w, int h, int pos_y, int pos_x)
{
  laby[pos_y][pos_x] = 3;
  return laby;
}

/* fun cherche_vide : cherche une case vide
 * arg laby :
 * arg w : largeur
 * arg h : hauteur
 * renvoit une coordonée sous la forme (100*x) +y
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

/* fun insere_ia : insere une ia dans le laby
 * arg laby : le laby
 * arg w
 * arg h 
 * pos_x : position joueur en x
 * pos_y : position joueur en y
 */
int** insere_ia(int** laby, int w, int h, int pos_y, int pos_x)
{
  laby[pos_y][pos_x] = 4;
  return laby;
}

/* fun deplace_ia : 
 * arg laby
 * arg w
 * arg h
 * arg old_y
 * arg old_x
 * arg direction
 */
int** deplace_ia(int** laby, int w, int h, int old_y, int old_x, int direction)
{
  laby[old_y][old_x] = 0;
  if(direction == 0) {
    //nord
    laby = insere_ia(laby,w,h,old_y-1,old_x);
  }
  else if(direction == 1) {
    //est
    laby = insere_ia(laby,w,h,old_y,old_x+1);
  }
  else if(direction == 2) {
    //sud
    laby = insere_ia(laby,w,h,old_y+1,old_x);
  }
  else if(direction == 3) {
    //ouest
    laby = insere_ia(laby,w,h,old_y,old_x-1);
  }
  else{
    printf("Mauvaise direction\n");
  }
  return laby;
}

/* fun est_case_vide
 * arg laby
 * arg dir
 * arg old_y
 * arg old_x
 * renvoit 
 * 1 si vide
 * 0 si non-vide
 */
int est_case_vide(int** laby, int direction, int old_y, int old_x)
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

/* ia_cherche_deplacement
 * arg laby
 * arg w : largeur
 * arg h : hauteur
 * arg old_x
 * arg old_y
 */
int ia_cherche_deplacement(int** laby, int old_y, int old_x)
{
  int deplacement_possible = 0;
  int test_direction;
  int count = 0;

  while( !(deplacement_possible == 1 || count >25) ) {
    test_direction = rand()%4;
    if(test_direction == 0) {
      //nord
      if(laby[old_y-1][old_x]==0){
        deplacement_possible = 1;
      }
    }
    else if(test_direction == 1) {
      //est
      if(laby[old_y][old_x+1]==0){
        deplacement_possible = 1;
      }
    }
    else if(test_direction == 2) {
      //sud
      if(laby[old_y+1][old_x]==0){
        deplacement_possible = 1;
      }
    }
    else if(test_direction == 3) {
      //ouest
      if(laby[old_y][old_x-1]==0){
        deplacement_possible = 1;
      }
    }
    count++;
  }
  return test_direction;
}


/** fun ia_dir_relative
 * arg laby : le labyrinthe
 * arg old_y : position sur y
 * arg old_x : position sur x
 * arg old_dir : ancienne direction
 ** renvoit une nouvelle direction possible, avec une notation relative telle que :
 * 3 : tourne a gauche 3 fois (270°)
 * 2 : tourne a gauche 2 fois (180°)
 * 1 : tourne a gauche 1 fois ( 90°)
 * 0 : garde la précédente direction
 */
int ia_dir_relative(int** laby , int old_y , int old_x , int old_dir)
{
  int tirage = rand()%100;
  int new_direction_relative;
  if ( tirage < 70 && est_case_vide(laby,old_dir,old_y,old_x) ) {
    new_direction_relative = 0;
  }
  //gauche
  else if ( (tirage >= 70 && tirage < 80) && est_case_vide(laby, dir_relative_to_absolue(laby,old_dir,1) ,old_y,old_x) ) {
    new_direction_relative = 1;
  }
  //droite
  else if ( (tirage >= 80 && tirage < 90) && est_case_vide(laby, dir_relative_to_absolue(laby,old_dir,3) ,old_y,old_x) ) {
    new_direction_relative = 3;
  }
  //sinon on recule
  else {
    new_direction_relative = 2;
  }
  return new_direction_relative;
}

/** fun dir_relative_to_absolue
 * arg laby : le labyrinthe
 * arg old_dir : ancienne direction
 * arg dir_relative : direction relative ( (0;1;2;3) où le chiffre est le nombre de rotation de 90° dans le sens anti-horaire
 ** renvoit la direction avec la notation absolue telle que :
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

/* fun fabrique_mat_frequence : renvoit une matrice contenant
 * -1 : mur
 * -2 : bords 
 * 0  : cases vides  
 * arg laby : la matrice du jeu
 */
int** fabrique_mat_frequence(int** laby, int** freq, int w, int h)
{
  int i,j;

  for(i=0;i<h;i++) {
    for(j=0;j<w;j++) {
      //si c'est un mur
      if(laby[i][j] == 1) {
        freq[i][j] = -1;
      }
      //si c'est un bord
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

/* fun remplir_mat_frequence : ajoute 1 a une position indiquée
 * arg pos_y :
 * arg pos_x :  
 */
int** remplir_mat_frequence(int** freq, int pos_y, int pos_x)
{
  freq[pos_y][pos_x] = freq[pos_y][pos_x] + 1;
  return freq;
}

/* fun trace_obstacles
 *
 * w : largeur
 * h : hauteur
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
/* fun joueur_insertion : insert un joueur dans une case vide aléatoire
 * arg laby : laby
 * arg w: largeur du labyrinthe
 * arg h: hauteur du labyrinthe
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

/* fun initialiser_coordonnees
 * arg point : structure contenant des coordonnées
 */
struct Coordonnees* init_struct_coord()
{
  struct Coordonnees* position = NULL;
  position = malloc(sizeof(int)* 2);
  position->x = 0;
  position->y = 0;
  return position;
}

/* fun demande_direction_relative
 * arg laby
 * arg joueur
 * renvoit la direction relative du joueur
 *
int demande_direction_relative(int** laby, struct Coordonnees* joueur)
{
  int deplacement possible = 0;

}
*/

