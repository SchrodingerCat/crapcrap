#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "couleurs.h"
#include "functions.h"

#define w 32
#define h 32

int main() {
  srand(time(NULL));
  int** laby = init_mat(w,h);

  laby = init_mur(laby,w,h);
  laby = init_bord(laby,w,h);
  //laby = creuse_laby(laby,w,h);
  //laby = creuse_rand(laby, 200,w,h);
  laby = trace_obstacles(laby,w,h);
  show_laby(laby,w,h);
  printf("\n");  

  int** freq = fabrique_mat_frequence(laby,w,h);
  show_freq(freq,w,h);
  printf("\n");
  
  //insere un joueur
  int position_vide = cherche_vide(laby,w,h);
  int y = position_vide%100;
  int x = (position_vide-(position_vide%100))/100;
  //laby = insere_joueur(laby,w,h,x,y);
  //show_laby(laby,w,h);

  //insere une ia
  position_vide = cherche_vide(laby,w,h);
  y = position_vide%100;
  x = (position_vide-(position_vide%100))/100;
  laby = insere_ia(laby,w,h,y,x);   
  show_laby(laby,w,h);
  freq = remplir_mat_frequence(freq,y,x);
  show_freq(freq,w,h);
  usleep(1000000);
  system("clear");


  //premier deplacement de l'ia
  int direction;
  int x_old, y_old;
  x_old = x;
  y_old = y;
  direction = ia_cherche_deplacement(laby,y,x,direction); 
  laby = deplace_ia(laby,w,h,y,x, direction );
  x = next_col(x_old, direction);
  y = next_line(y_old, direction); 
  freq = remplir_mat_frequence(freq,y,x);
  show_laby(laby,w,h); 
  printf("\n");
  show_freq(freq,w,h);
  usleep(1000000);
  system("clear");

  int compteur_deplacement = 0;
  while(compteur_deplacement<10000) {
    direction = ia_nouveau_deplacement_rand(laby,y,x,direction);
    laby = deplace_ia(laby,w,h,y,x, direction );
  
    x = next_col(x, direction);
    y = next_line(y, direction);
    freq = remplir_mat_frequence(freq,y,x);
    //show_laby(laby,w,h); 
    //show_freq(freq,w,h);
    printf("déplacements : %d\n", compteur_deplacement++);
    //usleep(8000);
    //system("clear");
  }
  show_freq(freq,w,h);
  return 0;

}

