#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "couleurs.h"
#include "functions.h"

/* fun init_mat
 *
 */
int** init_mat(int w, int h) {
  int i;
  int** laby;
  laby = malloc(sizeof(int) * w);
  for(i=0;i<h;i++) {
    laby[i] = malloc(sizeof(int) * h);
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
void show_laby(int** laby, int w, int h) {
  int i,j;
  for(i=0 ; i<h ; i++) {
    for(j=0 ; j<w ; j++) {
      if(laby[i][j]==0) {
        couleur("40");
        printf(" ");
      }
      else if(laby[i][j]==1) {
        couleur("40");
        printf(" ");
      }
      else if(laby[i][j]==2) {
        couleur("40");
        printf(" ");
      }
      else if(laby[i][j]==3) {
        //couleur("32;40");
        printf("C");
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
void show_freq(int** freq, int w, int h) {
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
      else if(freq[i][j]<=15) {
        couleur("46");
        printf(" ");
      }
      else if(freq[i][j]<=30) {
        couleur("42");
        printf(" ");
      }
      else if(freq[i][j]<=50) {
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

/* fun next_dir : indique la nouvelle direction
 * arg actual_dir : direction actuelle
 *      Nord : 0
 * Ouest : 3 | Est : 1
 *     Sud : 2
 */
int next_dir(int actual_dir) {
  int alea = rand()%10;
  int new_dir;

  if(alea==0) {
    if(actual_dir==0){
      new_dir = 3;
    }
    else{
      new_dir = actual_dir - 1;
    }
  }
  else if(alea==1) {
    if(actual_dir==3){
      new_dir = 0;
    }
    else{
      new_dir = actual_dir + 1;
    }
  }
  else{
    new_dir = actual_dir;
  }
  return new_dir;
}

/* fun next_col
*/ 
int next_col(int pos_x, int dir) {
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
int next_line(int pos_y, int dir) {
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

/* fun init_mur
 * arg w : largeur
 * arg h : hauteur
 *
 */
int** init_mur(int** laby, int w, int h) {
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
int** init_bord(int** laby, int w, int h) {
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

/* fun cherche_mur : recherche un mur
 * arg w : largeur
 * arg h : hauteur
 * si (x,y) est un mur alors 
 * retourne c = (100*x) + y
 */
int cherche_mur(int** laby, int w, int h) {
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

/* fun cherche_mur_ecart : recherche d'un mur entouré de murs au N,S,E,O
 * arg w : largeur
 * arg h : hauteur
 *
 */
int cherche_mur_ecart(int** laby, int w, int h) {
  int there_is_mur_autour = 0;
  int c, coord_y, coord_x;

  while( !(there_is_mur_autour == 1)){
    c = cherche_mur(laby,w,h);
    //if(c==0) {
    //  return 0;
    //}
    coord_y = c%100;
    coord_x = (c-(c%100))/100;
    if((laby[coord_y-1][coord_x] == 1) && (laby[coord_y+1][coord_x] == 1) && (laby[coord_y][coord_x-1] == 1) && (laby[coord_y][coord_x+1] == 1) ) {
      there_is_mur_autour = 1;
    }
  }
  return c;
}

/* fun dig_a_track : creuse un chemin a partir d'un mur
 * arg w : largeur
 * arg h : hauteur
 * arg c_x : coordonnée du mur largeur
 * arg c_y : coordonnée du mur hauteur
 * ret : le nombre de mur creusé
 */
int dig_a_track(int** laby, int w, int h, int c_y, int c_x) {
  int direction = rand()%4;
  int touche_bord = 0;
  int touche_vide = 0;
  int prochain_x;
  int prochain_y;
  int dig_count = 0;
  laby[c_y][c_x] = 0;

  while( !((touche_bord==1) || (touche_vide==1)) ) {
    prochain_x = next_col(c_x,direction);
    prochain_y = next_line(c_y,direction);

    if(laby[prochain_y][prochain_x] == 2) {
      touche_bord = 1;
    } 
    else if(laby[prochain_y][prochain_x] == 0) {
      touche_vide = 1;
    } 
    else {
      laby[prochain_y][prochain_x] = 0;
      c_x = prochain_x;
      c_y = prochain_y;
      direction = next_dir(direction);  
      dig_count++;
    }
  }
  return dig_count;
}

/* fun creuse_laby : 
 *
 */
int** creuse_laby(int** laby, int w, int h) {
  int count_digged = 0;
  int coord, coord_x, coord_y;
  float max_creux;

  //on creuse l'entrée
  count_digged = count_digged + dig_a_track(laby, w, h, 1 , 1);
  printf("\n");
  show_laby(laby,w,h); 
  //on creuse a la sortie
  count_digged = count_digged + dig_a_track(laby, w, h, w-2 , h-2);
  printf("\n");
  show_laby(laby,w,h); 
  //améliorer
  max_creux = (((w-1)*(h-1))/2.5);
  printf("%f", max_creux);
  while(count_digged <= max_creux ) {
    coord = cherche_mur_ecart(laby, w, h);
    //if(coord == 0) {
    //  printf("Impossible de creuser autant\n");
    //  return laby;
    //}
    coord_y = coord%100;
    coord_x = (coord-(coord%100))/100;
    count_digged = count_digged + dig_a_track( laby, w, h , coord_y, coord_x);
  }
  ///if(count>=1000) {
  // return laby;
  //}
  printf("vide : %d", count_digged);
  printf("\n");
  return laby;
}
/* fun insere_joueur : insere le joueur dans le laby
 * arg laby : le laby
 * arg w
 * arg h 
 * pos_x : position joueur en x
 * pos_y : position joueur en y
 */
int** insere_joueur(int** laby, int w, int h, int pos_y, int pos_x) {
  laby[pos_y][pos_x] = 3;
  return laby;
}

/* fun cherche_vide : cherche une case vide
 * arg laby :
 * arg w : largeur
 * arg h : hauteur
 * renvoit une coordonée sous la forme (100*x) +y
 */
int cherche_vide(int** laby, int w, int h) {
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
int** insere_ia(int** laby, int w, int h, int pos_y, int pos_x) {
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
int** deplace_ia(int** laby, int w, int h, int old_y, int old_x, int direction) {
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
int est_case_vide(int** laby, int direction, int old_y, int old_x) {
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
int ia_cherche_deplacement(int** laby, int old_y, int old_x) {
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


/* fun ia_nouveau_deplacement_rand
 * arg laby
 * arg old_y
 * arg old_x
 * arg old_dir
 * retourne une direction
 */
int ia_nouveau_deplacement_rand(int** laby, int old_y, int old_x, int old_dir) {
  int maxliberte = 10;
  int liberte = rand()%(100)+1;
  if(est_case_vide(laby,old_dir,old_y,old_x) && liberte > maxliberte) {
    return old_dir;
  }
  else {
    int deplacement_possible = 0;
    int test_direction;
    int count = 0;
    while( !(deplacement_possible == 1 || count > 10000) ) {
      test_direction = rand()%2;
      if(test_direction == 0) {
        //tourne gauche
        if((old_dir !=0) && (est_case_vide(laby,old_dir-1,old_y,old_x))){
          test_direction = old_dir - 1;
          deplacement_possible = 1;
        }
        else if( est_case_vide(laby,3,old_y,old_x) ){
          test_direction = 3;
          deplacement_possible = 1; 
        }
      }
      else if(test_direction == 1) {
        //tourne droite
        if((old_dir !=3) && (est_case_vide(laby,old_dir+1,old_y,old_x))){
          test_direction = old_dir + 1;
          deplacement_possible = 1;
        }
        else if( est_case_vide(laby,0,old_y,old_x) ){
          test_direction = 0;
          deplacement_possible = 1; 
        }
      }
    count++;
    }
    if(deplacement_possible==0 && liberte > maxliberte) {
      if((old_dir == 0) || (old_dir == 1)) {
        test_direction = old_dir + 2;
      } 
      else {
        test_direction = old_dir - 2;
      }
    }
    else if(deplacement_possible==0) {
      test_direction = ia_nouveau_deplacement_rand(laby,old_y,old_x,old_dir);
    }
    return test_direction;
  }
}


/* fun fabrique_mat_frequence : renvoit une matrice contenant
 * -1 : mur
 * -2 : bords 
 * 0  : cases vides  
 * arg laby : la matrice du jeu
 */
int** fabrique_mat_frequence(int** laby, int** freq, int w, int h) {
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
int** remplir_mat_frequence(int** freq, int pos_y, int pos_x) {
  freq[pos_y][pos_x] = freq[pos_y][pos_x] + 1;
  return freq;
}

/* fun creuse_rand
 * arg laby
 * arg creuse_max
 */

int** creuse_rand(int** laby, int creuse_max, int w, int h) {
  int compteur = 0;
  int coord_y;
  int coord_x;
  int mur;
  while( compteur < creuse_max ) {
    mur = cherche_mur(laby,w,h); 
    coord_y = mur%100;
    coord_x = (mur-(mur%100))/100;
    laby[coord_y][coord_x] = 0;
    compteur++;
  }
  return laby;
}

/* fun trace_obstacles
 *
 * w : largeur
 * h : hauteur
 */
int** trace_obstacles(int** laby, int w, int h) {
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
