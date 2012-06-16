#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
  int y;
  int x;
  int x_old, y_old;
  int direction;
  int compteur_deplacement;
  struct Coordonnees joueur;

  laby = init_mur(laby,w,h);
  laby = init_bord(laby,w,h);
  laby = trace_obstacles(laby,w,h); 
  freq = fabrique_mat_frequence(laby,freq,w,h);
  initialiser_coordonnees(&joueur); 

  //insere une ia
  position_vide = cherche_vide(laby,w,h);
  y = position_vide%100;
  x = (position_vide-(position_vide%100))/100;
  laby = insere_ia(laby,w,h,y,x);   
  freq = remplir_mat_frequence(freq,y,x); 

  //premier deplacement de l'ia
  x_old = x;
  y_old = y;
  direction = ia_cherche_deplacement(laby,y,x); 
  laby = deplace_ia(laby,w,h,y,x, direction );
  x = next_col(x_old, direction);
  y = next_line(y_old, direction); 
  freq = remplir_mat_frequence(freq,y,x);

  //insert le joueur
  laby = joueur_insertion(laby, w, h, &joueur);

  //autres déplacements
  compteur_deplacement = -1;
  while(compteur_deplacement<10000) {
    direction = dir_relative_to_absolue(laby , direction , ia_dir_relative(laby , y , x , direction) );
    laby = deplace_ia(laby,w,h,y,x, direction );
    x = next_col(x, direction);
    y = next_line(y, direction);
    freq = remplir_mat_frequence(freq,y,x);
    show_laby(laby,w,h);
    compteur_deplacement++;
    //show_freq(freq , w , h , compteur_deplacement);
    printf("déplacements : %d\n", compteur_deplacement);
    usleep(50000);
  }
  show_freq(freq , w , h , compteur_deplacement);
  return 0;
}
