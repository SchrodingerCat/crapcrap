int ia1_init(int** laby, int** freq, int w, int h, struct Coordonnees* ia1);
int ia1_premier_deplacement(int** laby, int** freq, int w, int h, struct Coordonnees* ia1_old, struct Coordonnees* ia1, int* direction);
int** ia1_insere_dans_case(int** laby, int w, int h, int pos_y, int pos_x);
int** ia1_deplace(int** laby, int w, int h, int old_y, int old_x, int direction);
int ia_cherche_deplacement(int** laby, int old_y, int old_x); 
int ia_dir_relative(int** laby , int old_y , int old_x , int old_dir);
void ia1_play(int** laby, int** freq, int w, int h,int* direction, struct Coordonnees* ia1);
int ia2_init(int** laby, int** freq, int w, int h, struct Coordonnees* ia2);
int ia2_premier_deplacement(int** laby, int** freq, int w, int h, struct Coordonnees* ia2_old, struct Coordonnees* ia2, int* direction);
int** ia2_insere_dans_case(int** laby, int w, int h, int pos_y, int pos_x);
int** ia2_deplace(int** laby, int w, int h, int old_y, int old_x, int direction);
void ia2_play(int** laby, int** freq, int w, int h,int* direction, struct Coordonnees* ia2);

