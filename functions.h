//typedef struct Coordonnees Coordonnees;
struct Coordonnees
{
  int x;
  int y;
};

struct Datas_ddr
{
  int** laby1;
  int y_joueur;
  int x_joueur;
  int direction;
};

int** init_mat(int w, int h);
int** init_mur(int** laby, int w, int h);
int** init_bord(int** laby, int w, int h);
void show_laby(int** laby, int w, int h);
void show_freq(int** freq, int w, int h, int total_deplacement); 
int next_col(int pos_x, int dir);
int next_line(int pos_y, int dir);
int cherche_mur(int** laby, int w, int h);
int** insere_joueur(int** laby, int w, int h, int pos_y, int pos_x);
int cherche_vide(int** laby, int w, int h);
int** insere_ia_dans_case(int** laby, int w, int h, int pos_y, int pos_x);
int** deplace_ia(int** laby, int w, int h, int old_y, int old_x, int direction);
int est_case_vide_avec_direction(int** laby, int direction, int old_y, int old_x); 
int est_case_vide(int** laby, struct Coordonnees* position);
int ia_cherche_deplacement(int** laby, int old_y, int old_x); 
int ia_dir_relative(int** laby , int old_y , int old_x , int old_dir);
int dir_relative_to_absolue (int** laby , int old_dir , int dir_relative);
int** fabrique_mat_frequence(int** laby, int** freq, int w, int h);
int** remplir_mat_frequence(int** freq, int pos_y, int pos_x);
int** trace_obstacles(int** laby, int w, int h); 
int** joueur_insertion(int** laby, int w, int h, struct Coordonnees* joueur);
struct Coordonnees* init_struct_coord();
struct Datas_ddr* init_struct_datas_ddr(int** laby, struct Coordonnees* position);
void ia1_play(int** laby, int** freq, int w, int h,int* direction, struct Coordonnees* ia1);
void* demande_direction_relative(void*);
int** deplace_joueur(int** laby, int w, int h, struct Datas_ddr* datas1, int direction);
int insere_ia_init(int** laby, int** freq, int w, int h, struct Coordonnees* ia1);
int ia_premier_deplacement(int** laby, int** freq, int w, int h, struct Coordonnees* ia1_old, struct Coordonnees* ia1, int* direction);
