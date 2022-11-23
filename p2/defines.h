#ifndef Proj2
#define Proj2

#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */

#define MAX_CODIGO_AEROPORTO 4	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */
#define MAX_DATA 11		/* dimensão da data */
#define MAX_HORA 6		/* dimensão da hora */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TAMANHO_MAX_COMANDO 65535 /* Tamanho maximo de um comando */
#define REALLOC 10 /* intervalo de reallocs */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* Tipos de Dados */

typedef struct {
	char id[MAX_CODIGO_AEROPORTO];
	char pais[MAX_NOME_PAIS];
	char cidade[MAX_NOME_CIDADE];
	int numVoos;
} Aeroporto;

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

typedef struct {
	char id[MAX_CODIGO_VOO];
	char partida[MAX_CODIGO_AEROPORTO];
	char chegada[MAX_CODIGO_AEROPORTO];
	Data data;
	Hora hora;
	Hora duracao;
	int capacidade;
	int horaPartida;
	int horaChegada;
	int ocupacao;
} Voo;

typedef struct{
    char *id;
    int numpassa;
    char vooId[MAX_CODIGO_VOO];
    Data data;
} Reserva;

/* Variaveis globais*/

extern int _numAeroportos;
extern Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];
extern int _numVoos;
extern Voo _voos[MAX_NUM_VOOS];
extern Data _hoje;
extern const int _diasMesAc[];

/* FUncoes leitura */

Hora leHora();
Data leData();
int leProximaPalavra(char str[]);
void lePalavraAteFimDeLinha(char str[]);
void leAteFimDeLinha();

/* Funcoes datas e horas */

void mostraData(Data d);
void mostraHora(Hora h);
int converteDataNum(Data d);
int converteHoraNum(Hora h);
int converteDataHoraNum(Data d, Hora h);
Hora converteNumHora(int num);
Data converteNumData(int num);
int validaData(Data d);
int validaHora(Hora h);

/* Algoritmo de ordenacao bubblesort */

void bubbleSort(int indexes[], int size, int (*cmpFunc)(int a, int b));

/* FUncoes Aeroportos */

int aeroportoInvalido(char id[]);
int encontraAeroporto(char id[]);
void adicionaAeroporto();
void mostraAeroporto(int index);
int cmpAeroportos(int a, int b);
void listaTodosAeroportos();
void listaAeroportos();

/* Funcoes Voos */

void mostraVoo(int index);
void mostraVooPartida(int index);
void mostraVooChegada(int index);
int encontraVoo(char id[], Data d);
int validaIDVoo(char id[]);
int validaVoo(Voo v);
void criaVoo(Voo v);
void adicionaListaVoos();
int cmpVoosPartida(int a, int b);
int cmpVoosChegada(int a, int b);
void listaVoosPartida();
void listaVoosChegada();
void alteraData();

/* Funcoes Reservas */

Reserva* sort(Reserva *reservas, int numreser);
void listaReservas(Reserva *reservas, Reserva r, int numreser);
int validaCodReserva(char *id);
int encontraCodReserva(Reserva *reservas,char id[],int *numreser);
int validaReserva(Reserva *reservas,Reserva r,int *numreser);
Reserva* adicionaReserva(Reserva *reservas, Reserva r, int *numreser);
Reserva* adicionalistaReservas(Reserva *reservas,int *numreser);
Reserva* eliminaVooReserva(Reserva *reservas,int *numreser);
void limpar(Reserva *reservas,int numreser);

#endif