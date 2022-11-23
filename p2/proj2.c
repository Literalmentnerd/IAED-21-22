#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"

/* Funcoes Reservas */

Reserva* sort(Reserva *reservas, int numreser){
	/* Da sort as reservas pela ordem lexicografica */
	int i,j;
	Reserva temp;

	for( i=0 ; i<numreser-1 ; i++){
		for( j=0 ; j<numreser-1-i; j++){
			if(strcmp(reservas[j].id,reservas[j+1].id) > 0){
				temp.id = malloc(sizeof(char)*(strlen(reservas[j].id)+1));
				strcpy(temp.id,reservas[j].id);
				temp.data = reservas[j].data;
				temp.numpassa = reservas[j].numpassa;
				strcpy(temp.vooId,reservas[j].vooId);
				free(reservas[j].id);
				reservas[j].id = malloc(sizeof(char)*(strlen(reservas[j+1].id)+1));
				strcpy(reservas[j].id,reservas[j+1].id);
				reservas[j].data = reservas[j+1].data;
				reservas[j].numpassa = reservas[j+1].numpassa;
				strcpy(reservas[j].vooId,reservas[j+1].vooId);
				free(reservas[j+1].id);
				reservas[j+1].id = malloc(sizeof(char)*(strlen(temp.id)+1));
				strcpy(reservas[j+1].id,temp.id);
				reservas[j+1].data = temp.data;
				reservas[j+1].numpassa = temp.numpassa;
				strcpy(reservas[j+1].vooId,temp.vooId);
				free(temp.id);
			}
		}
	}
	return reservas;
}


void listaReservas(Reserva *reservas, Reserva r, int numreser){
	int i;
	reservas = sort(reservas,numreser);
    for(i = 0; i<numreser; i++){
		/* for que descobre todas as reservas com o codvoo e data inserida
		 pelo user e vai imprimindo o seu codreserva e numero de passageiros */
        if(strcmp(reservas[i].vooId,r.vooId) == 0 &&
		 reservas[i].data.dia == r.data.dia && 
		 reservas[i].data.mes == r.data.mes && 
		 reservas[i].data.ano == r.data.ano){
			printf("%s %d\n",reservas[i].id,reservas[i].numpassa);
        }
    }
}


int validaCodReserva(char *id){
	int i = 0, l = strlen(id);
    if(l < 10){
		/* Se o tamanho do codigo for menor que 10 devolve falso */
        return FALSE;
	}
	for (i = 0; i < l; i++){
		/* for que verifica se todos os caracteres no codreserva sao ou maiusculas ou numeros */
        if (!(id[i] >= 'A' && id[i] <= 'Z') && !(id[i] >= '0' && id[i] <= '9')){
            return FALSE;
		}
	}
	return TRUE;
}


int encontraCodReserva(Reserva *reservas,char id[],int *numreser){
	int i;
	for( i=0 ; i<*numreser ; i++ ){
		if(strcmp(reservas[i].id,id) == 0){
			/* se encontra um id igual devolve o index desseid no array */
			return i;
		}
	}
	return NAO_EXISTE;
}


int validaReserva(Reserva *reservas,Reserva r,int *numreser){
	int index = encontraVoo(r.vooId,r.data);
    if(validaCodReserva(r.id) == FALSE){
        printf("invalid reservation code\n");
	}
    else if(index == NAO_EXISTE){
        printf("%s: flight does not exist\n",r.vooId);
	}
	else if(encontraCodReserva(reservas,r.id,numreser) != NAO_EXISTE){
		printf("%s: flight reservation already used\n",r.id);
	}
	else if(_voos[index].ocupacao+r.numpassa > _voos[index].capacidade){
		printf("too many reservations\n");
	}
    else if(validaData(r.data) == FALSE){
        printf("invalid date\n");
	}
    else if (r.numpassa <= 0){
        printf("invalid passenger number\n");
	}
    else{
        return TRUE;
	}
	return FALSE;
}


Reserva* adicionaReserva(Reserva *reservas, Reserva r, int *numreser){
	/* adiciona a reserva ao array com todas as reservas do sistema */
    int vooindex = encontraVoo(r.vooId,r.data);
    _voos[vooindex].ocupacao += r.numpassa;
    if(*numreser%REALLOC == 0){
		/* Da realloc do espaco do array de 10 em 10 */
		reservas = realloc(reservas,sizeof(Reserva)*(*numreser+REALLOC));
	}
    reservas[*numreser].id = malloc(sizeof(char)*(strlen(r.id)+1));
    reservas[*numreser].data = r.data;
    strcpy(reservas[*numreser].id,r.id);
    reservas[*numreser].numpassa = r.numpassa;
    strcpy(reservas[*numreser].vooId,r.vooId);
    *numreser += 1; /* aumenta o numero de reservas */
    return reservas;
}


Reserva* adicionalistaReservas(Reserva *reservas,int *numreser){
    Reserva r;
    int c;
	char *temp = malloc(sizeof(char)+1);

    leProximaPalavra(r.vooId);
    r.data = leData();
	leProximaPalavra(temp);
	c = strlen(temp);

    if(c == 0){
		/* Entra no if se so forem inseridos codvoo e data*/
		if(encontraVoo(r.vooId,r.data) == NAO_EXISTE)
			printf("%s: flight does not exist\n",r.vooId);
		else if(validaData(r.data) == FALSE)
			printf("invalid date\n");
		else{
        	listaReservas(reservas,r,*numreser);
		}
	}
    else{
		/* entra se for inserido os 4 parametros */
		r.id = malloc(sizeof(char)*c+1);
		strcpy(r.id,temp);
        scanf("%d", &r.numpassa);
        leAteFimDeLinha();
        if(validaReserva(reservas,r,numreser) == TRUE){
			/* Se o input for valido a reserva e adicionada */
		    reservas = adicionaReserva(reservas,r,numreser);
		}
    }
    free(temp);
	return reservas;
}


Reserva* eliminaVooReserva(Reserva *reservas,int *numreser){
	int c,j,len,i,temp,flagvoo = 0;
	char *cod = malloc(sizeof(char)+1);
	Data d;

	leProximaPalavra(cod);
	c = encontraCodReserva(reservas,cod,numreser);
	len = strlen(cod);

	if(c != NAO_EXISTE){
		/* Se for reserva entra aqui e elimina essa reserva */
		for( j = c ; j<*numreser-1 ; j++){
			/* Desloca todos os elementos com o index superior ao que se quer eliminar um para a esquerda*/
			free(reservas[j].id);
			reservas[j].id = malloc(sizeof(Reserva)*(strlen(reservas[j+1].id)+1));
			strcpy(reservas[j].id,reservas[j+1].id);
			reservas[j].data = reservas[j+1].data;
			reservas[j].numpassa = reservas[j+1].numpassa;
			strcpy(reservas[j].vooId,reservas[j+1].vooId);
		}
		*numreser -= 1; /* numero de reservas e diminuido */
		reservas = realloc(reservas,sizeof(Reserva)*(*numreser)+1); /* Redimensiona o array */
		free(cod);
		return reservas;
	}
	else{
		if(len >= 10){
			printf("not found\n");
			free(cod);
			return reservas;
		}
		/* Se nao entra aqui e elimina o(s) Voo(s) */
		for( i=0 ; i<_numVoos ; i++){
			d = converteNumData(_voos[i].horaChegada);
			if(strcmp(_voos[i].id,cod) == 0 && d.ano == _hoje.ano && d.mes == _hoje.mes && d.dia < _hoje.dia ||
		strcmp(_voos[i].id,cod) == 0 && d.ano == _hoje.ano && d.mes < _hoje.mes ||
		strcmp(_voos[i].id,cod) == 0 && d.ano < _hoje.ano){
				/* Quando encontra um id igual elimina o */
				for( temp=i ; temp<_numVoos-1 ; temp++){
					_voos[temp].capacidade = _voos[temp+1].capacidade;
					strcpy(_voos[temp].chegada,_voos[temp+1].chegada);
					_voos[temp].data = _voos[temp+1].data;
					_voos[temp].duracao = _voos[temp+1].duracao;
					_voos[temp].hora = _voos[temp+1].hora;
					_voos[temp].horaChegada = _voos[temp+1].horaChegada;
					_voos[temp].horaPartida = _voos[temp+1].horaPartida;
					strcpy(_voos[temp].id,_voos[temp+1].id);
					_voos[temp].ocupacao = _voos[temp+1].ocupacao;
					strcpy(_voos[temp].partida,_voos[temp+1].partida);
				}
				_numVoos -= 1;
				flagvoo += 1;
			}
		}
		if(flagvoo == 0){
			/* Se flagvoo = 0 entao quer dizer que esse codigo nao existe */
			printf("not found\n");
		}
		free(cod);
		return reservas;
	}
}


void limpar(Reserva *reservas,int numreser){
	/* limpa a memoria do programa antes de o fechar */
    int i;
    for(i = 0; i<numreser ; i++){
        free(reservas[i].id);
    }
    free(reservas);
}