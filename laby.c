/**
* \file laby.c
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
#include <unistd.h>
#include <pthread.h>
#include "couleurs.h"
#include "functions.h"

#define w 32
#define h 32

int main() 
{
  srand(time(NULL));
  int** laby = init_mat(w,h);
  int** freq = init_mat(w,h); 
  //int position_vide;
  int* direction = malloc(sizeof(int));
  int compteur_deplacement;
  struct Coordonnees* ia1;
  struct Coordonnees* joueur; 
  struct Coordonnees* ia1_old;
  struct Datas_ddr* datas1;

  ia1_old = init_struct_coord();
  ia1 = init_struct_coord();
  joueur = init_struct_coord();
  laby = init_mur(laby,w,h);
  laby = init_bord(laby,w,h);
  laby = trace_obstacles(laby,w,h); 
  freq = fabrique_mat_frequence(laby,freq,w,h);

  insere_ia_init(laby, freq, w, h, ia1);

  ia_premier_deplacement(laby, freq, w, h, ia1_old, ia1, direction);

  //insertion du joueur
  laby = joueur_insertion(laby, w, h, joueur);

  //autres déplacements 
  compteur_deplacement = -1;
  
  // on déclare le thread
  pthread_t thread1;
  
  // on fabrique la structure pour le passage dans le thread
  datas1 = init_struct_datas_ddr(laby,joueur);
  
  // on lance le thread
  pthread_create(&thread1, NULL, demande_direction_relative, datas1);
  
  while(compteur_deplacement<10000) {
   
    // si le déplacement a changé alors:
    deplace_joueur(laby, w, h, datas1, datas1->direction);

    ia1_play(laby,freq,w,h,direction,ia1);
    system("clear");
    printf("\tz : haut\n"); 
    printf("\tq : gauche\n"); 
    printf("\ts : bas\n"); 
    printf("\td : droite\n");
    show_laby(laby,w,h);
    compteur_deplacement++;
    //show_freq(freq, w, h, compteur_deplacement);
    printf("déplacements : %d\n", compteur_deplacement);
    usleep(250000);
  }

  // on termine le thread
  int pthread_cancel(pthread_t thread1);
  
  show_freq(freq , w , h , compteur_deplacement);
  return 0;
}
