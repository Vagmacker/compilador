/**
 * Class referente as opera��es pertinentes a Tabela de S�mbolos.
 */

/**
 * Struct que armazena a informacoes do simbolo da tabela de simbolos.
 */
typedef struct simbolo {
	char
		tipo[UCHAR_MAX],
		valor[UCHAR_MAX],
		escopo[UCHAR_MAX],
		variavel[UCHAR_MAX];
} Simbolo;

/**
 * Struct que armazena os elementos da lista, de acordo com o tipo do dado.
 */
typedef struct elemSimbolo {
    struct elemSimbolo *ant;
    Simbolo dados;
    struct elemSimbolo *prox;
} ElemSimbolo;

typedef struct elemSimbolo* TabelaSimbolo;

/**
 * Cria a lista, que armazena a tabela de simbolos.
 *
 * @return TabelaSimbolo* lista
 */
TabelaSimbolo* criaListaTabelaSimbolo() {
    TabelaSimbolo* lista = (TabelaSimbolo*) malloc(sizeof(TabelaSimbolo));

    if (lista != NULL) {
        *lista = NULL;
    }

    return lista;
}

/**
 * Remove todos os elementos da lista.
 *
 * @param TabelaSimbolo* lista
 */
void liberaListaTabelaSimbolo(TabelaSimbolo* lista) {
    if (lista != NULL) {
        ElemSimbolo* no;

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
 * @param TabelaSimbolo* lista
 * @param Simbolo simbolo
 * @return int
 */
int insereFinalTabelaSimbolo(TabelaSimbolo* lista, Simbolo simbolo) {
    if (lista == NULL) {
        return 0;
    }

    ElemSimbolo *no;
    no = (ElemSimbolo*) malloc(sizeof(ElemSimbolo));
    if (no == NULL) {
        return 0;
    }

    no->dados = simbolo;
    no->prox = NULL;

    // lista vazia: insere in�cio
    if ((*lista) == NULL) {
        no->ant = NULL;
        *lista = no;
    } else {
        ElemSimbolo *aux;
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
int insereInicioTabelaSimbolo(TabelaSimbolo* lista, Simbolo simbolo) {
    if (lista == NULL) {
        return 0;
    }

    ElemSimbolo* no;
    no = (ElemSimbolo*) malloc(sizeof(ElemSimbolo));

    if (no == NULL) {
        return 0;
    }

    no->dados = simbolo;
    no->prox = (*lista);
    no->ant = NULL;

    //lista n�o vazia: apontar para o anterior!
    if (*lista != NULL) {
        (*lista)->ant = no;
    }

    *lista = no;

    return 1;
}

/**
 * Retorna o total de itens que compoen a lista.
 *
 * @param TabelaSimbolo* lista
 * @return int cont
 */
int tamanhoTabelaSimbolo(TabelaSimbolo* lista) {
    if (lista == NULL) {
        return 0;
    }

    int cont = 0;
    ElemSimbolo* no = *lista;

    while (no != NULL) {
        cont++;
        no = no->prox;
    }

    return cont;
}

/**
 * Verifica se a lista esta vazia.
 *
 * @param TabelaSimbolo* lista
 * @return int
 */
int isVazioTabelaSimbolo(TabelaSimbolo* lista) {
	int isVazio = 0;

    if (lista == NULL) {
        isVazio = 1;
    }

    if (*lista == NULL) {
        isVazio = 1;
    }

    return isVazio;
}

/**
 * Imprime os itens da lista.
 *
 * @param TabelaSimbolo* lista
 */
void imprimeTabelaSimbolo(TabelaSimbolo* lista) {
    if (lista == NULL) {
        return;
    }

    ElemSimbolo* no = *lista;
    bool isListaVazia = isVazioTabelaSimbolo(lista);
    printf("\n================================================\n");
    printf("============== TABELA DE SIMBOLOS ==============\n");
    printf("================================================\n");
    
    if (isListaVazia == 1) {
    	printf("\tA TABELA DE SIMBOLOS EST� VAZIA.\n");	
    	exit(1);
	}

    while (no != NULL) {
        printf("Escopo: %s \n", no->dados.escopo);
        printf("Tipo: %s \n", no->dados.tipo);
        printf("Variavel: %s \n", no->dados.variavel);
        printf("Valor: %s \n", no->dados.valor);
        printf("------------------------------------------------\n");
        no = no->prox;
    }
    
    printf("\nCONSUMO DE MEMORIA: %d bytes\n\n", TOTAL_CONSUMO_MEMORIA);
    
    float porcentagem = 0;

	if (MAX_TOTAL_CONSUMO_MEMORIA > 0) {
		porcentagem = (TOTAL_CONSUMO_MEMORIA * 100) / MAX_TOTAL_CONSUMO_MEMORIA;
	}	 

	printf("Porcentagem consumida => %.2f %% de %i bytes\n\n" , porcentagem, MAX_TOTAL_CONSUMO_MEMORIA);
}

