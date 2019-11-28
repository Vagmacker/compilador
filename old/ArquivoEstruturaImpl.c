/**
 * Classe de implementação dos métodos definidos em 'IArquivoEstrutura.h'.
 *
 * @author j.ribeiro
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "IArquivoEstrutura.h"

/**
 * Cria a lista.
 *
 * @return Lista* lista
 */
Lista* criaLista() {
    Lista* lista = (Lista*) malloc(sizeof(Lista));

    if (lista != NULL) {
        *lista = NULL;
    }

    return lista;
}

/**
 * Remove todos os elementos da lista.
 *
 * @param Lista* lista
 */
void liberaLista(Lista* lista) {
    if (lista != NULL) {
        Elem* no;

        while ((*lista) != NULL) {
            no = *lista;
            *lista = (*lista)->prox;
            free(no);
        }

        free(lista);
    }
}

/**
 * Inseri no final da lista de acordo com parametros informados.
 *
 * @param Lista* lista
 * @param Linha lin
 * @return int
 */
int insereListaFinal(Lista* lista, Linha lin) {
    if (lista == NULL) {
        return 0;
    }

    Elem *no;
    no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) {
        return 0;
    }

    no->dados = lin;
    no->prox = NULL;

    // lista vazia: insere inicio
    if ((*lista) == NULL) {
        no->ant = NULL;
        *lista = no;
    } else {
        Elem *aux;
        aux = *lista;

        while (aux->prox != NULL) {
            aux = aux->prox;
        }

        aux->prox = no;
        no->ant = aux;
    }
    return 1;
}

/**
 * Inseri no inicio da lista de acordo com parametros informados.
 *
 * @param Lista* lista
 * @param Linha lin
 * @return int
 */
int insereListaInicio(Lista* lista, Linha lin) {
    if (lista == NULL) {
        return 0;
    }

    Elem* no;
    no = (Elem*) malloc(sizeof(Elem));

    if (no == NULL) {
        return 0;
    }

    no->dados = lin;
    no->prox = (*lista);
    no->ant = NULL;

    //lista nao vazia: apontar para o anterior!
    if (*lista != NULL) {
        (*lista)->ant = no;
    }

    *lista = no;

    return 1;
}

/**
 * Remove o item da lista de acordo com o numero da linha informado.
 *
 * @param Lista* lista
 * @param int nuLinha
 * @return int
 */
int removeLista(Lista* lista, int nuLinha) {
    if (lista == NULL) {
        return 0;
    }

    // lista vazia
    if ((*lista) == NULL) {
        return 0;
    }

    Elem *no = *lista;
    while (no != NULL && no->dados.linha != nuLinha) {
        no = no->prox;
    }

    // nó encontrado
    if(no == NULL) {
        return 0;
    }

    // remover o primeiro?
    if (no->ant == NULL) {
        *lista = no->prox;
    } else {
        no->ant->prox = no->prox;
    }

    // nao eh o ultimo?
    if (no->prox != NULL) {
        no->prox->ant = no->ant;
    }

    free(no);
    return 1;
}

/**
 * Remove o item no inicio da lista.
 *
 * @param Lista* lista
 * @return int
 */
int removeListaInicio(Lista* lista) {
    if (lista == NULL) {
        return 0;
    }

    // lista vazia
    if ((*lista) == NULL) {
        return 0;
    }

    Elem *no = *lista;
    *lista = no->prox;

    if(no->prox != NULL) {
        no->prox->ant = NULL;
    }

    free(no);
    return 1;
}

/**
 * Remove o item no final da lista.
 *
 * @param Lista* lista
 * @return int
 */
int removeListaFinal(Lista* lista) {
    if(lista == NULL) {
        return 0;
    }

    // lista vazia
    if ((*lista) == NULL) {
        return 0;
    }

    Elem *no = *lista;

    while(no->prox != NULL) {
        no = no->prox;
    }

    // remover o primeiro e unico
    if(no->ant == NULL) {
        *lista = no->prox;
    } else {
        no->ant->prox = NULL;
    }

    free(no);
    return 1;
}

/**
 * Retorna o total de itens que compoen a lista.
 *
 * @param Lista* lista
 * @return int cont
 */
int tamanhoLista(Lista* lista) {
    if (lista == NULL) {
        return 0;
    }

    int cont = 0;
    Elem* no = *lista;

    while (no != NULL) {
        cont++;
        no = no->prox;
    }

    return cont;
}

/**
 * Verifica se a lista esta vazia.
 *
 * @param Lista* lista
 * @return int
 */
int listaVazia(Lista* lista) {
    if (lista == NULL) {
        return 1;
    }

    if (*lista == NULL) {
        return 1;
    }

    return 0;
}

/**
 * Imprime os itens da lista.
 *
 * @param Lista* lista
 */
void imprimeLista(Lista* lista) {
    if (lista == NULL) {
        return;
    }

    Elem* no = *lista;

    while (no != NULL) {
        printf("Linha: %d \n", no->dados.linha);
        printf("Conteudo: %s \n", no->dados.conteudo);
        printf("Total de caracter na linha: %d \n", strlen(no->dados.conteudo));
        printf("------------------------------------------\n");
        no = no->prox;
    }
}

/**
 * Retorna o conteudo da lista.
 *
 * @param FILE* arquivo
 * @return Lista* linhas
 */
Lista* getConteudo(FILE* arquivo) {
	char conteudo[200];
	int nuLinha = 1;
	Lista* linhas = criaLista();

	while ((fgets(conteudo, sizeof(conteudo), arquivo)) != NULL) {

		if (strlen(conteudo) != 1) {
			Linha item;
			item.linha = nuLinha;
			strcpy(item.conteudo, conteudo);
			insereListaFinal(linhas, item);
		}
					
		nuLinha ++;
	}

	fclose(arquivo);
	return linhas;
}
