/* BIBLIOTECA COM AS FUNÇÕES DO f_terminal.c E f_interface.c
 AUTORES: CARLOS HENRIQUE RORATO, FELIPE ALMEIDA GEMMAL
 DATA: NOVEMBRO/2016     PROGRAMAÇÃO DE COMPUTADORES 2 */

#include <gtk/gtk.h>

#define C 129 // quantidade de caracteres padrão do programa

typedef struct{
    char partido[10];
    char nome[C];
    char vice[C];
    int numero;
    int votos;
    float percent;
}Candidato;

//Menu principal
void menu();

//funções referentes ao terminal
void menu_eleicao();
void menu_candidato();
void add_cand();
void rem_cand();
void limpa_cand();
void mostra_cand();
void resultados(int *brancos);
int validar(int numero, FILE *pf);

//funções de interface
int playSound( char *filename );
void visualizar_candidato();
void corrigir_voto();
void voto_branco();
void confirmar_voto(GtkButton *button,gpointer user_data);
void desbloqueio();
void votacao(int *brancos);
