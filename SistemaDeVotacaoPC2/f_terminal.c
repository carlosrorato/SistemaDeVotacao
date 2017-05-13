/* PARTE REFERENTE AO TERMINAL
 AUTORES: CARLOS HENRIQUE RORATO, FELIPE ALMEIDA GEMMAL
 DATA: NOVEMBRO/2016     PROGRAMAÇÃO DE COMPUTADORES 2 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "urna.h"


void menu_eleicao(){
    FILE *arquivo,*arq_cand,*arq_para_cargo;
    char municipio[C], uf[3], z_eleitoral[C], secao[C],cargo_nome[C];
    int turno;



    system("clear");

    //cadastro dos candidatos abrindo em "w"
    arquivo = fopen("eleicao.txt", "w");
    if(arquivo == NULL){
        printf("Erro na criação/abertura do arquivo!\n");
        exit(1);
    }
    arq_para_cargo = fopen("cargo.txt", "w");
    if(arq_para_cargo == NULL){
        printf("Erro na criação/abertura do arquivo!\n");
        exit(1);
    }

    //procedimento de cadastro
    printf("Informe o cargo a ser disputado:\n");
    scanf(" %[^\n]s", cargo_nome);
    printf("Informe o município de realização da eleição:\n");
    scanf(" %[^\n]s", municipio);
    printf("Informe a unidade federativa:\n");
    scanf(" %s", uf);
    printf("Informe a Zona Eleitoral:\n");
    scanf(" %s", z_eleitoral);
    printf("Informe a Seção:\n");
    scanf(" %s", secao);
    printf("Informe o Turno da Eleição:\n");
    scanf("%d", &turno);
    system("clear");
    printf("Cadastramento efetuado com sucesso!\n");

    //gravação dos arquivos
    fprintf(arquivo,"Dados da Eleição:\n Cargo: %s\n Município: %s\n UF: %s\n Zona Eleitoral: %s\n Seção: %s\n Turno: %d\n", cargo_nome, municipio, uf, z_eleitoral,secao,turno);
    fprintf(arq_para_cargo,"%s",cargo_nome);
    //limpando arquivo dos candidatos
    arq_cand= fopen("candidatos.txt","w");

    fclose(arq_cand);
    fclose(arq_para_cargo);
    fclose(arquivo);
}

void menu_candidato(){
    int op;
    do{
        printf("1-Adicionar candidato\n");
        printf("2-Remover candidato por numero\n");
        printf("3-Limpar lista\n");
        printf("4-Listar candidatos\n");
        printf("0-Sair\n");
        scanf("%d",&op);
        system("clear");
        switch(op){
            case 1:
                add_cand();
                break;
            case 2:
                rem_cand();
                break;
            case 3:
                limpa_cand();
                break;
            case 4:
                mostra_cand();
                break;
        }

    }while(op!=0);
}

void add_cand(){
    system("clear");
    FILE *pf;
    Candidato candidato;
    int teste=0;

    pf=fopen("candidatos.txt","r+");

    if(pf == NULL){
        puts("Erro ao acessar o arquivo, programa abortado\n");
        exit(1);
    }
		//Testa por numeros repitidos , para nao atrapalhar na votacao
    do{
        printf("Digite o numero do candidato\n");
        scanf("%d",&candidato.numero);
        teste=validar(candidato.numero,pf);
    }while(teste == 0);

    printf("Digite o partido do candidato\n");
    scanf(" %s",candidato.partido);
    printf("Digite o nome e sobrenome do candidato\n");
    scanf(" %[^\n]s",candidato.nome);
    printf("Digite o nome e sobrenome do vice\n");
    scanf(" %[^\n]s",candidato.vice);
    candidato.votos=0;
    candidato.percent=0;

    //fprintf(pf,"%d\n%s\n%s\n%s\n\n",num,partido,nome,nome_vice);
    fwrite(&candidato,sizeof(Candidato),1,pf);

    system("clear");
    puts("Adicionado com sucesso");
fclose(pf);
}

void rem_cand(){
    FILE *pf;
    int num,close=0,tam_file;
    Candidato candidato;

    pf=fopen("candidatos.txt","r+b");
    if(pf == NULL){
        puts("Erro ao acessar o arquivo, programa abortado\n");
        exit(1);
    }
    system("clear");
    printf("Digite o numero do candidato a ser removido\n");
    scanf("%d",&num);

    while(!feof(pf)){

        fread(&candidato,sizeof(Candidato),1,pf);

        if(num == candidato.numero){
            close=1;
             while((fread(&candidato,sizeof(Candidato),1,pf) == 1)){          
                fseek(pf,-2*sizeof(Candidato),SEEK_CUR);
                fwrite(&candidato,sizeof(Candidato),1,pf);
                fseek(pf,sizeof(Candidato),SEEK_CUR);

            }
            tam_file=ftell(pf);
            fclose(pf);
            truncate("candidatos.txt",tam_file-sizeof(Candidato));
            system("clear");
            puts("Exclusao sucedida");
            break;
        }

    }

    //Caso não encontre nada
    if(close == 0){
    system("clear");
    puts("Numero nao encontrado\n");
    fclose(pf);
    }

}

void limpa_cand(){
    FILE *pf;
    system("clear");
    pf=fopen("candidatos.txt","wb");
    if(pf == NULL){
        printf("Erro ao acessar o arquivo, programa abortado\n");
        exit(1);
    }

    fclose(pf);
    printf("Limpeza concluida \n");

}

void mostra_cand(){
     FILE *pf;
     Candidato candidato;

     pf = fopen("candidatos.txt","rb");
     if(pf == NULL){
        puts("Erro ao acessar o arquivo, programa abortado");
        exit(1);
     }

     system("clear");
     while((fread(&candidato,sizeof(Candidato),1,pf))==1){
        printf("%d \n%s \n%s \n%s \n\n",candidato.numero,candidato.partido,candidato.nome,candidato.vice);

     }

     fclose(pf);
}

void resultados(int *brancos){
    FILE *pf, *eleicao_resultado, *dados_eleicao;
    Candidato *candidato;
    char linha[C];
    int n_candidatos,i,total_votos=0;

    pf= fopen("resultados.txt","w");
    eleicao_resultado= fopen("candidatos.txt","rb");
    dados_eleicao = fopen("eleicao.txt","r");

    if(pf == NULL || eleicao_resultado == NULL || dados_eleicao == NULL){
        puts("Erro ao acessar o arquivo, programa abortado\n");
        exit(1);
    }
	//Pegando quantidade de candidatos
    fseek(eleicao_resultado,0,SEEK_END);
    n_candidatos=ftell(eleicao_resultado)/sizeof(Candidato);
    rewind(eleicao_resultado);
    candidato=(Candidato*)malloc(n_candidatos*sizeof(Candidato));

    //Pegando candidatos
    for(i=0;i<n_candidatos;i++){
        fread(&candidato[i],sizeof(Candidato),1,eleicao_resultado);

        total_votos+=candidato[i].votos;
    }

    //Escrevendo no arquivo:

    fprintf(pf,"Instituto de Informática \nUniversidade Federal de Goiás (UFG)\n\n");

    while(  fscanf(dados_eleicao," %[^\n]",linha) ==1){
        fprintf(pf,"%s\n",linha);
    }
        fprintf(pf,"\n\n\n");

    for(i=0;i<n_candidatos;i++){
        candidato[i].percent= 100*(float)candidato[i].votos/total_votos;
         fprintf(pf,"NÚMERO DO CANDIDATO(A): %d \nPARTIDO: %s \nNOME DO CANDIDATO(A): %s \nVICE-CANDIDATO(A):%s \nQUANTIDADE DE VOTOS: %d \nPORCENTAGEM: %.2%%f \n\n",candidato[i].numero,candidato[i].partido,candidato[i].nome,candidato[i].vice,candidato[i].votos,candidato[i].percent);
    }

    fprintf(pf,"Final: \nBrancos/Nulos:%d \nVotos totais: %d\nVotos válidos: %d",*brancos,total_votos+*(brancos),total_votos);

    system("clear");
    printf("Arquivo gerado com sucesso\n");

	free(candidato);
	fclose(pf);
	fclose(eleicao_resultado);
	fclose(dados_eleicao);
}

int validar(int numero,FILE *pf){
    rewind(pf);
    Candidato candidato;
    int retorno=1;

    while(fread(&candidato,sizeof(Candidato),1,pf)==1){
            if(numero == candidato.numero){
                retorno = 0;
                system("clear");
                printf("Numero ja existe\n");
                break;
            }
    }


    fseek(pf,0,SEEK_END);
    return retorno;
}
