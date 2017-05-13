/* MENU PRINCIPAL, FUNÇÃO MAIN
 AUTORES: CARLOS HENRIQUE RORATO, FELIPE ALMEIDA GEMMAL
 DATA: NOVEMBRO/2016     PROGRAMAÇÃO DE COMPUTADORES 2 */

#include <gtk/gtk.h>
#include "urna.h"
#include <stdio.h>
#include <stdlib.h>

#define SENHA 123 //senha para abertura da urna

int main(int argc, char **argv){
    int op,Sdigitada,brancos=0;


    printf("Digite a senha\n");
    scanf("%d",&Sdigitada);
    system("clear");



    if(Sdigitada == SENHA){
        do{
			printf("Sistema de Gerenciamento de Eleições:\n");
            printf("1-Cadastramento de Eleição\n");
            printf("2-Cadastramento dos Candidatos\n");
            printf("3-Terminal do Eleitor\n");
            printf("4-Geração de Resultado\n");
            printf("0-Sair\n");
            scanf("%d",&op);
            system("clear");

            switch(op){
                case 1:
                    menu_eleicao();
                    break;
                case 2:
                    menu_candidato();
                    break;
                case 3:
                    gtk_init(&argc,&argv);
                    votacao(&brancos);
                    break;
                case 4:
                    resultados(&brancos);
                    break;
                default :
                    exit(0);
            }

        }while(op!=0);
    } else{
        printf("Senha incorreta!\n");
        exit(1);
    }


 return 0;

}


