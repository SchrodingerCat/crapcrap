#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "couleurs.h"
#include "functions.h"

#define w 32
#define h 32

int main() 
{
  srand(time(NULL));
  int** laby = init_mat(w,h);
  int** freq = init_mat(w,h); 
  int position_vide;
  int direction;
  int compteur_deplacement;
  struct Coordonnees* ia1;
  struct Coordonnees* joueur; 
  struct Coordonnees* ia1_old;

  ia1_old = init_struct_coord();
  ia1 = init_struct_coord();
  joueur = init_struct_coord();
  laby = init_mur(laby,w,h);
  laby = init_bord(laby,w,h);
  laby = trace_obstacles(laby,w,h); 
  freq = fabrique_mat_frequence(laby,freq,w,h);

  //insere une ia
  position_vide = cherche_vide(laby,w,h);
  ia1->y = position_vide%100;
  ia1->x = (position_vide-(position_vide%100))/100;
  laby = insere_ia(laby,w,h,ia1->y,ia1->x);   
  freq = remplir_mat_frequence(freq,ia1->y,ia1->x); 

  //premier deplacement de l'ia
  ia1_old->x = ia1->x;
  ia1_old->y = ia1->y;
  direction = ia_cherche_deplacement(laby,ia1->y,ia1->x); 
  laby = deplace_ia(laby,w,h,ia1->y,ia1->x, direction );
  ia1->x = next_col(ia1_old->x, direction);
  ia1->y = next_line(ia1_old->y, direction); 
  freq = remplir_mat_frequence(freq,ia1->y,ia1->x);

  //insert le joueur
  laby = joueur_insertion(laby, w, h, joueur);

  //autres déplacements
  compteur_deplacement = -1;
  while(compteur_deplacement<10000) {
    direction = dir_relative_to_absolue(laby , direction , ia_dir_relative(laby , ia1->y , ia1->x , direction) );
    laby = deplace_ia(laby,w,h,ia1->y,ia1->x, direction );
    ia1->x = next_col(ia1->x, direction);
    ia1->y = next_line(ia1->y, direction);
    freq = remplir_mat_frequence(freq,ia1->y,ia1->x);
    show_laby(laby,w,h);
    compteur_deplacement++;
    //show_freq(freq , w , h , compteur_deplacement);
    printf("déplacements : %d\n", compteur_deplacement);
    usleep(50000);
  }
  show_freq(freq , w , h , compteur_deplacement);
  return 0;
}
