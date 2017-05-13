/* INTERFACE VISUAL DO PROGRAMA
 AUTORES: CARLOS HENRIQUE RORATO, FELIPE ALMEIDA GEMMAL
 DATA: NOVEMBRO/2016     PROGRAMAÇÃO DE COMPUTADORES 2 */

#include <gtk/gtk.h>
#include <time.h>
#include <stdlib.h>
#include "urna.h"

#define N 0 //espaçamento
#define X 5 //número de algarismos dos números dos candidatos
#define C 129 // qtd de caracteres padrão do programa


/* DECLARAÇÃO DAS VARIÁVEIS GLOBAIS*/
GtkWidget *numero,*nome,*partido,*vice;
GtkWidget *janela, *seu_voto, *cargo, *num_txt, *nome_txt, *part_txt, *vice_txt, *rodape, *fim, *terminal_mesario, *desbloquear_urna;
GtkWidget *layout;
GtkWidget *confirma, *branco, *corrige, *view;
int controle_confirma=0,i=0; //controle para impedir que, clicando mais de uma vez em Confirma, sejam computados mais de um voto.
FILE *candidatos;


//função para o som, utilizada com autorização do professor
int playSound( char *filename ) {
    char command[256]; int status;
    sprintf( command, "aplay -c 1 -q -t wav %s", filename );
    status = system( command );
    return status;
}

void visualizar_candidato(){
	int num,i=0;
    Candidato compara;

	candidatos = fopen("candidatos.txt", "rb");
    if(candidatos == NULL){
        puts("Erro ao acessar o arquivo, encerrando");
        exit(1);
    }

	sscanf(gtk_entry_get_text(GTK_ENTRY(numero)), "%d", &num); //lê o texto digitado no campo de número


	while(!feof(candidatos)){
        fread(&compara, sizeof(Candidato), 1,candidatos); //pega todo o struct

        if(num == compara.numero){
            gtk_label_set_text(GTK_LABEL(nome), compara.nome);
            gtk_label_set_text(GTK_LABEL(partido), compara.partido);
            gtk_label_set_text(GTK_LABEL(vice), compara.vice);
            i++;
        }

	}
	if(i==0){ // caso não seja encontrado nada
        gtk_entry_set_text(GTK_ENTRY(numero), "00");
        gtk_label_set_text(GTK_LABEL(nome), "VOTO BRANCO");
        gtk_label_set_text(GTK_LABEL(partido), "--");
        gtk_label_set_text(GTK_LABEL(vice), "--");
	}

	fclose(candidatos);

}

void corrigir_voto(){
	gtk_entry_set_text(GTK_ENTRY(numero), "");
	gtk_label_set_text(GTK_LABEL(nome), "");
	gtk_label_set_text(GTK_LABEL(partido), "");
	gtk_label_set_text(GTK_LABEL(vice), "");
}

void voto_branco(){
	gtk_entry_set_text(GTK_ENTRY(numero), "00");
	gtk_label_set_text(GTK_LABEL(nome), "VOTO BRANCO");
	gtk_label_set_text(GTK_LABEL(partido), "--");
	gtk_label_set_text(GTK_LABEL(vice), "--");
}

void confirmar_voto(GtkButton *button, gpointer user_data){ //esconde os elementos e mostra a mensagem "fim"
    Candidato compara;
    int *brancos = user_data;
    candidatos = fopen("candidatos.txt", "r+b");
    if(candidatos == NULL){
        puts("Erro ao acessar o arquivo, encerrando");
        exit(1);
    }

    int num;
    sscanf(gtk_entry_get_text(GTK_ENTRY(numero)), "%d", &num); //lê novamente o numero da tela

    //Ações visíveis na interface
	gtk_widget_hide(seu_voto);
	gtk_widget_hide(cargo);
	gtk_widget_hide(num_txt);
	gtk_widget_hide(numero);
	gtk_widget_hide(nome);
	gtk_widget_hide(nome_txt);
	gtk_widget_hide(part_txt);
	gtk_widget_hide(partido);
	gtk_widget_hide(vice);
	gtk_widget_hide(vice_txt);
	gtk_widget_show(fim);

	controle_confirma = controle_confirma + 1;
	i=0; //indica se foi encontrado o candidato do número digitado
	if(controle_confirma==1){ // se o "confirma" é válido (para não computar mais votos que o necessário)
        while(fread(&compara, sizeof(Candidato), 1,candidatos) ==1){
            if(num == compara.numero){
                compara.votos = compara.votos + 1;
                fseek(candidatos,-sizeof(Candidato),SEEK_CUR);
                fwrite(&compara,sizeof(Candidato),1,candidatos);
                i++;
                break;
            }


        }
        if(i==0){ // caso não seja encontrado nada, o voto é considerado branco
		(*brancos)++;
        }
	}
	playSound("fim.wav");
	fclose(candidatos);
}

void desbloqueio(){
	gtk_widget_show(seu_voto);
	gtk_widget_show(cargo);
	gtk_widget_show(num_txt);
	gtk_widget_show(numero);
	gtk_widget_show(nome);
	gtk_widget_show(nome_txt);
	gtk_widget_show(part_txt);
	gtk_widget_show(partido);
	gtk_widget_show(vice);
	gtk_widget_show(vice_txt);
	gtk_widget_hide(fim);
	controle_confirma = 0;
	corrigir_voto();
}

void votacao(int *brancos){
    FILE *dados_eleicao, *arq_para_cargo;
    char cargo_nome[C];

    arq_para_cargo = fopen("cargo.txt", "r");
    if(arq_para_cargo == NULL){
        printf("Erro na criação/abertura do arquivo!\n");
        exit(1);
    }
    fscanf(arq_para_cargo, "%[^\n]s",cargo_nome);

    dados_eleicao = fopen("eleicao.txt","r");
    if(dados_eleicao == NULL){
		printf("Erro na abertura do arquivo de dados da eleição. Encerrando!");
		exit(1);
	}


	//Construção da estrutura básica da janela, título e tamanho
	janela = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (janela), "Terminal do Eleitor");
	gtk_window_set_default_size(GTK_WINDOW (janela),800,530);
    gtk_signal_connect(GTK_OBJECT (janela), "destroy", gtk_main_quit, NULL);

	//construção do terminal do mesário
	terminal_mesario = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (terminal_mesario), "Terminal do Mesário");
	gtk_window_set_default_size(GTK_WINDOW (terminal_mesario),100,100);
	desbloquear_urna = gtk_button_new_with_label("DESBLOQUEAR URNA");
	gtk_container_add(GTK_CONTAINER(terminal_mesario), desbloquear_urna);
	gtk_widget_show_all(terminal_mesario);
	g_signal_connect(desbloquear_urna,"clicked", desbloqueio,NULL);
	gtk_signal_connect(GTK_OBJECT (terminal_mesario), "destroy", gtk_main_quit, NULL);

	//Construcao dos elementos basicos de labels
	seu_voto = gtk_label_new("SEU VOTO PARA");
	cargo = gtk_label_new(cargo_nome); //OLHAR AQUI: PODE DAR ERRO
	num_txt = gtk_label_new("NÚMERO:");
	nome_txt = gtk_label_new("NOME:");
	part_txt = gtk_label_new("PARTIDO:");
	vice_txt = gtk_label_new("VICE-CANDIDATO(A):");
	fim = gtk_label_new("FIM");
	rodape = gtk_label_new("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n Pressione CONFIRMA para confirmar o voto \n Pressione CORRIGE para corrigir o voto \n Pressione BRANCO para votar em branco \n");

	//BOTOES
	confirma = gtk_button_new_with_label("CONFIRMA");
	corrige = gtk_button_new_with_label("CORRIGE");
	branco = gtk_button_new_with_label("BRANCO");
	view = gtk_button_new_with_label("VISUALIZAR");


	// CORES

	 GdkColor laranja_cor;
     GdkColor verde_cor;
     GdkColor branco_cor;

     gdk_color_parse("#328537", &verde_cor);
     gdk_color_parse("#ffffff", &branco_cor);
     gdk_color_parse("#ff5418", &laranja_cor);

     gtk_widget_modify_bg(confirma, GTK_STATE_NORMAL, &verde_cor);
     gtk_widget_modify_bg(corrige, GTK_STATE_NORMAL, &laranja_cor);
     gtk_widget_modify_bg(branco, GTK_STATE_NORMAL, &branco_cor);

    //modificando tamanho da fonte para a palavra fim

    PangoFontDescription *fonte;
	fonte = pango_font_description_from_string("Monospace");
	pango_font_description_set_size(fonte,60*PANGO_SCALE);
	gtk_widget_modify_font(fim, fonte);


	//Caixa de texto para o número
	numero = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(numero),X); //MELHORAR AQQ
	gtk_entry_set_width_chars(GTK_ENTRY(numero),X);

	//Criação das labels que mostrarão os dados dos candidatos (inicializados vazios, para serem preenchidos depois)
	nome = gtk_label_new(" ");
	partido = gtk_label_new(" ");
	vice = gtk_label_new(" ");

	//Criação do layout
	layout = gtk_layout_new(NULL,NULL);


	//Colocando os itens no layout
	gtk_layout_put(GTK_LAYOUT(layout),fim,320,200);
	gtk_layout_put(GTK_LAYOUT(layout),seu_voto,10,10);
	gtk_layout_put(GTK_LAYOUT(layout),cargo,80,80);
	gtk_layout_put(GTK_LAYOUT(layout),num_txt,10,180);
	gtk_layout_put(GTK_LAYOUT(layout),numero,90,175);
	gtk_layout_put(GTK_LAYOUT(layout),nome_txt,10,250);
	gtk_layout_put(GTK_LAYOUT(layout),part_txt,10,300);
	gtk_layout_put(GTK_LAYOUT(layout),vice_txt,10,370);
	gtk_layout_put(GTK_LAYOUT(layout),rodape,10,450);
	gtk_layout_put(GTK_LAYOUT(layout),view,680,410);
	gtk_layout_put(GTK_LAYOUT(layout),branco,500,480);
	gtk_layout_put(GTK_LAYOUT(layout),corrige,600,480);
	gtk_layout_put(GTK_LAYOUT(layout),confirma,700,480);
	//mostrador dos dados dos candidatos
	gtk_layout_put(GTK_LAYOUT(layout),nome,70,250);
	gtk_layout_put(GTK_LAYOUT(layout),partido,90,300);
	gtk_layout_put(GTK_LAYOUT(layout),vice,160,370);

	//ações dos botões quando clicados
	g_signal_connect(view,"clicked", visualizar_candidato,NULL);
	g_signal_connect(corrige,"clicked", corrigir_voto,NULL);
	g_signal_connect(branco,"clicked", voto_branco,NULL);
	g_signal_connect(confirma,"clicked", confirmar_voto,brancos);

	//Colocando o layout na janela
	gtk_container_add(GTK_CONTAINER(janela), layout);

	//mostrando a janela
	gtk_widget_show_all(janela);

	//esconder a mensagem fim
	gtk_widget_hide(fim);

	//função de espera
	gtk_main();

	//Fechando os arquivos
	fclose(dados_eleicao);
	fclose(arq_para_cargo);
}
