#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <strings.h>
#include <locale.h>

/**
 * Armazena o total de memoria utilizada no compilador.
 */
int TOTAL_CONSUMO_MEMORIA = 0;

/**
 * 1024000 bytes => 1,024 megabytes => 1024 kilobyte
 */
int MAX_TOTAL_CONSUMO_MEMORIA = 1024000;

char palavrasReservadas[12][12] = {
	"ler",
	"real",
	"falso",
	"funcao",
	"testar",
	"repetir",
	"retorno",
	"inteiro",
	"escrever",
	"caracter",
	"principal",
	"verdadeiro",
};

char tiposVariaveis[3][10] = {
	"real",
	"inteiro",
	"caracter",
};

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

typedef struct linha {
    char conteudo[200];
    int linha;
} Linha;

typedef struct elemento {
    struct elemento *ant;
    Linha dados;
    struct elemento *prox;
} Elem;

typedef struct elemento* Lista;

FILE* carregarArquivo(char *nome);

Lista* criaLista();

void liberaLista(Lista* lista);

int insereListaFinal(Lista* lista, Linha lin);

int insereListaInicio(Lista* lista, Linha lin);

int removeLista(Lista* lista, int nuLinha);

int removeListaInicio(Lista* lista);

int removeListaFinal(Lista* lista);

int tamanhoLista(Lista* lista);

int listaVazia(Lista* lista);

void imprimeLista(Lista* lista);

Lista* getConteudo(FILE* arquivo);

void memoriaConsumida(int memoria, int situacao);

TabelaSimbolo* criaListaTabelaSimbolo();

void analisarToken(Lista* lista, TabelaSimbolo* tabelaSimbolos);

void removerEspaco(char *palavra);

void removerTabulacao(char* palavra);

void removerQuebraLinha(char* palavra);

void atualizarValorVariavel(TabelaSimbolo* lista, char * noVariavel, char * valor);

void validarTipoCaractere(char * palavra, int numeroLinha);

void verificarPresencaColchetes(char* palavra, char* tipoVariavel, int numeroLinha);

void validarTipoCaractere(char * palavra, int numeroLinha);

void validarTipoReal(char * palavra, int numeroLinha);

void getTamanhoVariavel(char palavra[], char retorno[], int numeroLinha);

void removeTamanhoVariavel(char palavra[]);

int validaPalavraString(char* palavra, int numeroLinha);

int hasParOrImpar(int valor);

int isPrincipalValido(char *palavra, int nuLinha);

void throwNewExeception(int nuLinha, int tipoErro, char *palavra);

int validarDeclaracaoFuncao(char *palavra, int nuLinha);

void liberaListaTabelaSimbolo(TabelaSimbolo* lista);

void imprimeTabelaSimbolo(TabelaSimbolo* lista);

main() {
	setlocale(LC_ALL, "PORTUGUESE");
	
	printf("####################################################################\n");
	printf("# Aluno: Joao Felipe Vagmacker Ribeiro e Rafael Ferreira Barros\n");
	printf("# Compilador 2019/2\n");
	printf("####################################################################\n\n");
	
	memoriaConsumida(sizeof(tiposVariaveis), 1);
	memoriaConsumida(sizeof(palavrasReservadas), 1);

	// Ler arquivo.
	FILE *arquivo;

	arquivo = carregarArquivo("teste2.txt");

	memoriaConsumida(sizeof(arquivo), 1);

	// Guardar conteudo da estrutura.
	Lista *linhasArquivo;
	memoriaConsumida(sizeof(linhasArquivo), 1);
	
	TabelaSimbolo* tabelaSimbolos = criaListaTabelaSimbolo();
	memoriaConsumida(sizeof(tabelaSimbolos), 1);

	linhasArquivo = getConteudo(arquivo);
	analisarToken(linhasArquivo, tabelaSimbolos);
	
	liberaLista(linhasArquivo);
	
	// TODO imprimindo palavras reservadas.
	// imprimePalavrasReservadas();
	// imprimeTiposVariaveis();
	
	// TODO desconsiderar TAB, SPACE

	// TODO Processar arquivo.
	
	// TODO calcular consumo de mem�ria.
	// TODO Criar tabela de simbolos
	
	imprimeTabelaSimbolo(tabelaSimbolos);
	liberaListaTabelaSimbolo(tabelaSimbolos);

	printf("\n\n");
	system("pause");
}

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

    // n� encontrado
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

FILE* carregarArquivo(char *nome) {

    FILE *arquivo;
    
    arquivo = fopen(nome, "r");
    
    if (arquivo == NULL) {
		printf("\nErro, nao foi possivel abrir o arquivo\n");
	}

	return arquivo;
}

/**
 * Imprime o vetor com todas as palavras reservadas.
 */
void imprimePalavrasReservadas() {
	int i;
	for (i = 0; i < 12; i++) {
		printf("%i => %s \n", i, palavrasReservadas[i]);
	}
}

/**
 * Imprime o vetor com todas as palavras reservadas de tipos de variaveis.
 */
void imprimeTiposVariaveis() {
	int i;
	for (i = 0; i < 3; i++) {
		printf("%i => %s \n", i, tiposVariaveis[i]);
	}
}

/**
 * Limpa o lixo contido no vetor.
 *
 * @param char vetor
 */
void limparConteudoVetor(char vetor[]) {
	int i = 0;

	while (i < UCHAR_MAX) {
		vetor[i] = '\0';
		i ++;
	}
}

/**
 * Aplica as validacoes
 */
void analisarToken(Lista* lista, TabelaSimbolo* tabelaSimbolos) { // TODO 2
	int 
		i,
		count = 0,
		aspas = 0,
		valorAscii,
		numeroLinha,
		countValor = 0,
		countAspas = 0,
		countChaves = 0,
		countVirgulas = 0,
		countVariaveis = 0,
		isFuncaoValida = 0,
		countColchetes = 0,
		countParenteses = 0,
		isFuncaoExistente = 0,
		hasDeclaracaoPrincipal = 0,
		isLinhaPossuiDeclaracaoFuncao = 0;
		
		
		memoriaConsumida(sizeof(i), 1);
		memoriaConsumida(sizeof(count), 1);
		memoriaConsumida(sizeof(aspas), 1);
		memoriaConsumida(sizeof(valorAscii), 1);
		memoriaConsumida(sizeof(countValor), 1);
		memoriaConsumida(sizeof(countAspas), 1);
		memoriaConsumida(sizeof(countChaves), 1);
		memoriaConsumida(sizeof(numeroLinha), 1);
		memoriaConsumida(sizeof(countVirgulas), 1);
		memoriaConsumida(sizeof(countVariaveis), 1);
		memoriaConsumida(sizeof(isFuncaoValida), 1);
		memoriaConsumida(sizeof(countColchetes), 1);
		memoriaConsumida(sizeof(countParenteses), 1);
		memoriaConsumida(sizeof(isFuncaoExistente), 1);
		memoriaConsumida(sizeof(hasDeclaracaoPrincipal), 1);
		memoriaConsumida(sizeof(isLinhaPossuiDeclaracaoFuncao), 1);
		

	char
		palavraAux[UCHAR_MAX],
		nomeEscopo[UCHAR_MAX],
		tipoVariavel[UCHAR_MAX],
		conteudoLinha[UCHAR_MAX],
		tamanhoPalavra[UCHAR_MAX],
		valorAtribuicao[UCHAR_MAX],
		palavraValorAux[UCHAR_MAX],
		nomeVariavelAux[UCHAR_MAX],
		auxTamanhoPalavra[UCHAR_MAX];
		
		memoriaConsumida(sizeof(palavraAux), 1);
		memoriaConsumida(sizeof(nomeEscopo), 1);
		memoriaConsumida(sizeof(tipoVariavel), 1);
		memoriaConsumida(sizeof(conteudoLinha), 1);
		memoriaConsumida(sizeof(tamanhoPalavra), 1);
		memoriaConsumida(sizeof(valorAtribuicao), 1);
		memoriaConsumida(sizeof(palavraValorAux), 1);
		memoriaConsumida(sizeof(nomeVariavelAux), 1);
		memoriaConsumida(sizeof(auxTamanhoPalavra), 1);

	bool
		hasLeia = false,
		hasString = false,
		hasEscreva = false,
		hasRepetir = false,
		hasVariavel = false,
		hasAtribuicao = false,
		hasPossuiAspas = false,
		isSimboloAtribuicao = false,
		hasLinhaComVariavel = false,
		hasPalavraReservada = false,
		hasPossuiPontoVirgula = false;
		
		memoriaConsumida(sizeof(hasLeia), 1);
		memoriaConsumida(sizeof(hasString), 1);
		memoriaConsumida(sizeof(hasEscreva), 1);
		memoriaConsumida(sizeof(hasRepetir), 1);
		memoriaConsumida(sizeof(hasVariavel), 1);
		memoriaConsumida(sizeof(hasAtribuicao), 1);
		memoriaConsumida(sizeof(hasPossuiAspas), 1);
		memoriaConsumida(sizeof(isSimboloAtribuicao), 1);
		memoriaConsumida(sizeof(hasLinhaComVariavel), 1);
		memoriaConsumida(sizeof(hasPalavraReservada), 1);
		memoriaConsumida(sizeof(hasPossuiPontoVirgula), 1);
	
	Elem* no = *lista;
	
	if (lista == NULL) {
        exit(1);
    }
    
	limparConteudoVetor(palavraAux);
	limparConteudoVetor(nomeEscopo);
	limparConteudoVetor(tipoVariavel);
	limparConteudoVetor(tamanhoPalavra);
	limparConteudoVetor(valorAtribuicao);
	limparConteudoVetor(palavraValorAux);
	limparConteudoVetor(nomeVariavelAux);
	limparConteudoVetor(auxTamanhoPalavra);

	while (no != NULL) {
		strcpy(conteudoLinha, no->dados.conteudo);
		numeroLinha = no->dados.linha;

		for (i = 0; i < strlen(conteudoLinha); i++) {
			valorAscii = (int) conteudoLinha[i];
			
			// se encontrar (
			if (valorAscii == 40) {
				countParenteses++;
			}
			
			// se encontrar )
			if (valorAscii == 41) {
				countParenteses--;
			}
			
			// se encontrar {
			if (valorAscii == 123) {
				countChaves++;
			}
			
			// se encontrar }
			if (valorAscii == 125) {
				countChaves--;
			}
			
			// se encontrar [
			if (valorAscii == 91) {
				countColchetes++;
			}
			
			// se encontrar ]
			if (valorAscii == 93) {
				countColchetes--;
			}
			
			// verifica se existe ';'
			if (valorAscii == 59) {
				hasPossuiPontoVirgula = true;
			}
			
			// verifica se e '"'
			if (valorAscii == 34) {
				aspas++;
				hasPossuiAspas = true;
			}
			
			if (valorAscii == 58) {
				if (((int) conteudoLinha[i + 1]) == 61) {
					hasAtribuicao = true;
					continue;
				}
			}

			// Verifica se a 'valosAscii' informado eh uma condicao de parada
			// As condicoes de parada sao os caracterers : \0, espaco, (, ), virgula, ponto virgula, $, tabs, <, =, >, {, }
			if ((valorAscii != 10) && (valorAscii != 32) && (valorAscii != 40) && (valorAscii != 41) && (valorAscii != 44) && (valorAscii != 59) && (valorAscii != 36) && (valorAscii != 9) && (valorAscii != 60) && (valorAscii != 61) && (valorAscii != 62) && (valorAscii != 123) && (valorAscii != 125) && (valorAscii != 91) && (valorAscii != 93)) {
				if (hasPossuiAspas == false) {
					palavraAux[count] = (char) valorAscii;
					count++;
				}
				
				// printf("Nome variavel => %d - %d - (%s) - (%s)\n", valorAscii, numeroLinha, nomeVariavelAux, palavraAux);
				// TODO mudar 'atualizarValorVariavel' de lugar
				// atualizarValorVariavel(tabelaSimbolos, nomeVariavelAux, palavraAux);
			} else {

				printf("%d - %d - %c - %s\n", numeroLinha, valorAscii, (char) valorAscii, palavraAux);

				hasVariavel = validarDeclaracaoVariaveis(palavraAux, numeroLinha);

				if (!hasVariavel) {
					hasPalavraReservada = validarPalavrasReservadas(palavraAux);

					if (hasPalavraReservada == true) {
						// caso seja inteiro, real ou caractere
						if (validarTipoVariavel(palavraAux) == 1) {
							strcpy(tipoVariavel, palavraAux);
							hasLinhaComVariavel = true;
						}

						hasLeia = validarInstrucaoLeia(palavraAux, numeroLinha, conteudoLinha);
						hasEscreva = validarInstrucaoEscreva(palavraAux, numeroLinha, conteudoLinha);
						
						if (strcmp(palavraAux, palavrasReservadas[5]) == 0) {
							hasRepetir = true;
						}
						
						// Verifica principal
						if (strcmp(palavraAux, palavrasReservadas[10]) == 0) {
							hasDeclaracaoPrincipal++;
							
							// Verifica se a declaracao de principal esta valida
							isPrincipalValido(conteudoLinha, numeroLinha);								
							strcpy(nomeEscopo, palavraAux);
							
							// Verifica se existe mais de uma declaracao do modulo principal
							if (hasDeclaracaoPrincipal > 1) {
								throwNewExeception(numeroLinha, 19, palavraAux);
							}
						}
						
						// TODO FUNCAO						
						if (strcmp(palavraAux, palavrasReservadas[3]) == 0) {
							isLinhaPossuiDeclaracaoFuncao++;
						}
						
						// TODO LER
						
						// TODO ESCREVER
						
						// TODO REPETIR
						
						// TODO TESTAR
						
						// TODO FALSO
						
						// TODO VERDADEIRO
						
						// TODO RETORNO
						
					} else {
						if (hasAtribuicao == true) {
							strcpy(valorAtribuicao, palavraAux);
							limparConteudoVetor(palavraAux);
							count = 0;
							//hasAtribuicao = false;
							continue;
						}
						
						isFuncaoValida = validarDeclaracaoFuncao(palavraAux, numeroLinha);
						
						if (isLinhaPossuiDeclaracaoFuncao && isFuncaoValida) {
							strcpy(nomeEscopo, palavraAux);
						}

						if (strlen(palavraAux) > 0) {
							if (!hasVariavel && !hasPalavraReservada && !isFuncaoValida) {
								throwNewExeception(numeroLinha, 21, palavraAux);
							}
						}
					}
				} else {
					printf("===> %s\n", palavraAux);
					countVariaveis++;
					
					if (hasLinhaComVariavel == true && hasLeia == false) {
						if (validarVariavelDeclarada(palavraAux, tabelaSimbolos) == 1) {
							throwNewExeception(numeroLinha, 7, palavraAux);
						}
					}

					if (strcmp(tipoVariavel, tiposVariaveis[1])) {
						verificarPresencaColchetes(palavraAux, tipoVariavel, numeroLinha);	
					}
					
					if (hasLinhaComVariavel && hasVariavel) {
						Simbolo novoSimbolo;
	
	    				strcpy(nomeVariavelAux, palavraAux);
	
						strcpy(novoSimbolo.escopo, nomeEscopo);
	    				strcpy(novoSimbolo.tipo, tipoVariavel);
	    				strcpy(novoSimbolo.variavel, nomeVariavelAux);
	    				
	    				insereFinalTabelaSimbolo(tabelaSimbolos, novoSimbolo);
	    				
	    				// apenas para tipos de caracter
					    if (strcmp(tipoVariavel, tiposVariaveis[2]) == 0) {
					    	printf("Encontrou tipo caracter\n");
					    	atualizarValorVariavel(tabelaSimbolos, nomeVariavelAux, palavraAux);
						}
					}

					// limparConteudoVetor(auxTamanhoPalavra);
					
					// validar se a variavel ja foi declarada.
					if (validarVariavelDeclarada(palavraAux, tabelaSimbolos) == 0) {
						throwNewExeception(numeroLinha, 11, palavraAux);
					}
				}
				
				// verifica se a string em '"' esta valida
				if (strlen(palavraAux) > 0) {
					if (hasVariavel == false && hasPalavraReservada == false && aspas > 0) {
						hasString = validaPalavraString(palavraAux, numeroLinha);
					}
				}
				
				if (strlen(palavraAux) > 0) {
					// compara se e uma variavel e se e uma palavra reservada
					//if (hasVariavel == false && hasPalavraReservada == false && aspas == 0) {
					//	throwNewExeception(numeroLinha, 13, palavraAux);
					//}
				}
				
				count = 0;
				hasString = 0;
				hasVariavel = 0;
				hasPalavraReservada = 0;
				limparConteudoVetor(palavraAux);
				
				// como $ faz parte da variavel e tambem e uma condicao de parada, entao preciso incrementar aqui
				if (valorAscii == 36) {
					palavraAux[count] = (char) valorAscii;
					count++;
				}
			} // fim do else para valores que sa condcoes de parada.

			// condicao de parada quando para limpar a variavel 'palavraValorAux'
			// espaco, $, virgula, ponto e virgula
			if ((aspas == 2) && ((valorAscii == 32) || (valorAscii == 36) || (valorAscii == 44) || (valorAscii == 59))) {
				atualizarValorVariavel(tabelaSimbolos, nomeVariavelAux, palavraValorAux);
				limparConteudoVetor(palavraValorAux);
				limparConteudoVetor(nomeVariavelAux);
				countValor = 0;
				hasPossuiAspas = false;
				
				// como $ faz parte da variavel e tambem � uma condicao de parada, entao preciso incrementar aqui
				if (valorAscii == 36) {
					palavraAux[count] = (char) valorAscii;
					count++;
				}
			}

			// caso seja encontrada 2 aspas, e em seguida seja um caracter de parada tais como:
			// virgula, ')', ';'
			// deve ser zerado a contagem de 'countAspas'
			if ((aspas == 2) && ((valorAscii == 44) || (valorAscii == 41) || (valorAscii == 59))) {
				aspas = 0;
			}
			
			if (hasAtribuicao == true && aspas > 0) {
				palavraValorAux[countValor] = (char) valorAscii;
				countValor++;
			}
			
			// conta a quantidade de ','
			if (valorAscii == 44) {
				countVirgulas ++;	
			}

		} // fim do for que percorre as colunas da linha
		
		if (countParenteses != 0) {
			throwNewExeception(numeroLinha, 26, conteudoLinha);
		}
		
		if (countColchetes != 0) {
			throwNewExeception(numeroLinha, 25, conteudoLinha);
		}
		
		// verifica o duplo balanceamento de '"'
		if (aspas != 0) {
			throwNewExeception(numeroLinha, 14, conteudoLinha);
		}

		// Essa validacao de ; no final esta invalida criar uma nova
		hasPossuiPontoVirgula = validaDeclaracaoComPontoVirgula(conteudoLinha, numeroLinha);
		
		printf("+++++++>>> %d = %d - %d - %d\n", numeroLinha, hasLinhaComVariavel, hasPossuiPontoVirgula, isLinhaPossuiDeclaracaoFuncao);
		
		// verifica se a linha possui declaracao de variavel e tem ; nessa linha
		if ((hasLinhaComVariavel == true && hasPossuiPontoVirgula == false) && isLinhaPossuiDeclaracaoFuncao == 0) {
			throwNewExeception(numeroLinha, 15, conteudoLinha);
		}
		
		// caso tenha mais de 1 variavel a quantidade de virgulas sera 'countVirgulas' - 'countVariaveis' - 1)
		if (countVariaveis > 1) {
			if (countVirgulas != (countVariaveis - 1)) {
				throwNewExeception(numeroLinha, 16, conteudoLinha);	
			}
		}

		aspas = 0;
		no = no->prox;
		countValor = 0;
		hasLeia = false;
		countVirgulas = 0;
		countVariaveis = 0;
		hasEscreva = false;
		hasRepetir = false;
		isFuncaoExistente = 0;
		hasAtribuicao = false;
		hasPossuiAspas = false;
		isSimboloAtribuicao = false;
		hasLinhaComVariavel = false;
		hasPossuiPontoVirgula = false;
		isLinhaPossuiDeclaracaoFuncao = 0;
		limparConteudoVetor(tipoVariavel);
		limparConteudoVetor(palavraValorAux);
		limparConteudoVetor(nomeVariavelAux);
	} // fim while
	
	if (hasDeclaracaoPrincipal == 0) {
		char valorTmp[] = {""};
		throwNewExeception(0, 22, valorTmp);
	}
	
	if (countChaves != 0) {
		throwNewExeception(0, 23, conteudoLinha);
	}
}

/**
 * Verifica se a linha possui ; no final
 *
 * @param char* conteudoLinha
 * @param int numeroLinha
 */
int validaDeclaracaoComPontoVirgula(char* conteudoLinha, int numeroLinha) {
	int isValido = 0, i, valorAscii, countTotalPontoVirgula = 0;
	
	for (i = 0; i < strlen(conteudoLinha); i++) {
		valorAscii = (int) conteudoLinha[i]; 
		
		if (valorAscii == 59) {
			countTotalPontoVirgula++;
		}
	}
	
	if (countTotalPontoVirgula == 1) {
		isValido = 1;
	}
	
	return isValido;
}

/**
 * Atualiza o valor da variavel existente.
 */
void atualizarValorVariavel(TabelaSimbolo* lista, char * noVariavel, char * valor) {
	if (lista == NULL) {
        return;
    }
    
	ElemSimbolo* no = *lista;
	
	while (no != NULL) {
		if (strcmp(noVariavel, no->dados.variavel) == 0) {
			strcpy(no->dados.valor, valor);
		}
		no = no->prox;
    }
}


/**
 * Valida palavras reservadas, utilizar como boleano, apos passar a palavra. validar separadamente.
 *
 * @param char* palavra
 * @return int isValido
 */
int validarPalavrasReservadas(char* palavra) {
	int isValido = 0, i;

	for (i = 0; i < 12; i++) {
		if (strcmp(palavra, palavrasReservadas[i]) == 0) {
			isValido = 1;
			break;
		}		
	}
	
	return isValido;
}

/**
 * Verifica se a palavra reservada eh um tipo de variavel.
 *
 * @param char *palavra
 */
int validarTipoVariavel(char *palavra) {
	if (strcmp(palavra, palavrasReservadas[1]) == 0) {
		return 1;
	}

	if (strcmp(palavra, palavrasReservadas[7]) == 0) {
		return 1;
	}

	if (strcmp(palavra, palavrasReservadas[9]) == 0) {
		return 1;
	}

	return 0;
}

/**
 * Valida declaracoes de variaveis.
 *
 * @param char *palavra
 * @param int numeroLinha
 */
int validarDeclaracaoVariaveis(char *palavra, int numeroLinha) {
	int isValido = 0, i, valorAscii;

	if ((int) palavra[0] == 36) {
		// apenas a-z
		if ((int) palavra[1] >= 97 && (int) palavra[1] <= 122) {
			isValido = 1;
		} else {
			isValido = 0;
			throwNewExeception(numeroLinha, 8, palavra);
		}
		
		for (i = 2; i < strlen(palavra); i++) {
			valorAscii = (int) palavra[i];
			// permiter apenas a-z, 0-9, A-Z, [, ], .
			if (! ((valorAscii >= 97 && valorAscii <= 122) || (valorAscii >= 48 && valorAscii <= 57) || (valorAscii >= 65 && valorAscii <= 90) || valorAscii == 91 || valorAscii == 93 || valorAscii == 46)) {
				isValido = 0;
				throwNewExeception(numeroLinha, 8, palavra);
			}
		}
	}

	return isValido;
}

/**
 * Verifica se a linha possui a instrucao 'leia' e verifica seus criterios.
 *
 * @param char* palavra
 * @param int nuLinha
 * @param int linha
 */
int validarInstrucaoLeia(char * palavra, int nuLinha, char * linha) {
	int 
		i , 
		count = 0, 
		valorAscii, 
		isValido = 0, 
		parenteses = 0, 
		hasLeiaValido = 0, // verifica se a palavra leia esta no padrao valido.
		hasPossuiPontoVirgula = 0;
	
	char palavraAux[UCHAR_MAX];
	limparConteudoVetor(palavraAux);

	// verifica se a palavra reservada � 'ler'
	if (strcmp(palavra, palavrasReservadas[0]) == 0) {

		// identifica se a palavra reservada � 'ler'
		isValido = 1;
		
		// valida se apos a palavra 'ler' possui obrigatoriamente um '(' e uma variavel.
		for (i = 0; i < strlen(linha); i++) {
			valorAscii = (int) linha[i];
			
			// Verifica se a caracter ascii informado e uma condicao de parada, para ser feita uma determinada analise.
			// As condicoes de parada sao os caracterers : \0, espaco, ), virgula, ponto virgula, $, tabs
			if ((valorAscii != 10) && (valorAscii != 32) && (valorAscii != 41) && (valorAscii != 44) && (valorAscii != 59) && (valorAscii != 9)) {
				palavraAux[count] = (char) valorAscii;
				count++;
				
				if (strcmp(palavraAux, "ler($") == 0) {
					hasLeiaValido++;
					break;
				}
			}
		}
		
		if (hasLeiaValido == 0) {
			throwNewExeception(nuLinha, 3, linha);
		}
		
		// nao pode haver declaracoes dentro da estrutura
		for (i = 0; i < strlen(linha); i++) {
			valorAscii = (int) linha[i]; 
			
			// cada caractere tem que ser diferente de \0, espaco e tab
			if ((valorAscii != 10) && (valorAscii != 32) && (valorAscii != 9)) {
				// balanceamento de parenteses '('
				if (valorAscii == 40) {
					parenteses++;
				}
				
				// balanceamento de parenteses ')'
				if (valorAscii == 41) {
					parenteses--;
				}
			}
		}
		
		// verifica se a linha do leia possui ';'
		for (i = strlen(linha); i > 0; i--) {
			valorAscii = (int) linha[i]; 
			
			// cada caractere tem que ser diferente de \0, espaco e tab
			if ((valorAscii != 10) && (valorAscii != 32) && (valorAscii != 9)) {
				
				// verifica se o caracter e ';'
				if (valorAscii == 59 && hasPossuiPontoVirgula == 0) {
					hasPossuiPontoVirgula++;
					break;
				}
			}
		}
		
		// verifica se existe duplo balanceamento de parentes
		if (parenteses != 0) {
			throwNewExeception(nuLinha, 4, linha);
		}
		
		// verificar o ; no final da linha do leia
		if (hasPossuiPontoVirgula != 1) {
			throwNewExeception(nuLinha, 5, linha);
		}
	}
		
	return isValido;
}

/**
 * Remove palavras reservadas na linha onde possui a instrucao 'ler' e 'escrever'.
 *
 * @param char * palavra
 */
void tratarLerEscrever(char *palavra) {
	int 
		i, 
		count = 0,
		valorAscii;
		
	char palavraAux[UCHAR_MAX];

	limparConteudoVetor(palavraAux);

	for (i = 0; i < strlen(palavra); i++) {
		valorAscii = (int) palavra[i];
		
		//printf("%d --- %c ---- %s\n", valorAscii, palavra[i], palavraAux);
		// condicoes de parada: espaco
		if (valorAscii != 32 && valorAscii != 9) {
			palavraAux[count] = palavra[i];
			count++;
		} else {
			//printf("entrou aqui1\n");
			//printf("==== (%s)--- (%s)--- (%s)\n", palavraAux, palavrasReservadas[2], palavrasReservadas[11]);
			if ((strcmp(palavraAux, palavrasReservadas[2]) == 0) || (strcmp(palavraAux, palavrasReservadas[11]) == 0)) {
				//printf("entrou aqui2\n");
				limparConteudoVetor(palavraAux);
				count = 0;
			}
		}
	}

	strcpy(palavra, palavraAux);
}

/**
 * Verifica se a linha possui a instrucao 'escreva' e verifica seus criterios.
 *
 * @param char* palavra
 * @param int nuLinha
 * @param int linha
 */
int validarInstrucaoEscreva(char * palavra, int nuLinha, char * linha) {
	int 
		i, 
		count = 0, 
		valorAscii, 
		isValido = 0, 
		parenteses = 0, 
		hasEscrevaValido = 0, // verifica se a palavra escreva esta no padrao 'escrever($' ou 'escrever("'
		hasPossuiPontoVirgula = 0; 

	char palavraAux[UCHAR_MAX];
	char linhaToProcessamento[UCHAR_MAX];
	
	limparConteudoVetor(palavraAux);
	limparConteudoVetor(linhaToProcessamento);

	strcpy(linhaToProcessamento, linha);
	tratarLerEscrever(linhaToProcessamento);
	
	// verifica se a palavra reservada e 'escrever'
	if (strcmp(palavra, palavrasReservadas[8]) == 0) {
		
		// identifica se a palavra reservada � 'escrever'
		isValido = 1;

		// valida se apos a palavra 'escrever' possui obrigatoriamente um '(' e uma variavel.
		for (i = 0; i < strlen(linhaToProcessamento); i++) {
			valorAscii = (int) linhaToProcessamento[i];
			
			//printf("%d - %c - %s\n", valorAscii, linhaToProcessamento[i], palavraAux);
			// Verifica se a caracter ascii informado e uma condicao de parada, para ser feita uma determinada analise.
			// As condicoes de parada sao os caracterers : \0, espaco, ), virgula, ponto virgula, $, tabs
			if ((valorAscii != 10) && (valorAscii != 32) && (valorAscii != 41) && (valorAscii != 44) && (valorAscii != 59) && (valorAscii != 9)) {
				palavraAux[count] = (char) valorAscii;
				count++;
				
				if ((strcmp(palavraAux, "escrever(") == 0)) {
					hasEscrevaValido++;
					break;
				}
			}
		}

		if (hasEscrevaValido == 0) {
			throwNewExeception(nuLinha, 6, linha);
		}
		
		// nao pode haver declaracoes dentro da estrutura
		for (i = 0; i < strlen(linhaToProcessamento); i++) {
			valorAscii = (int) linhaToProcessamento[i]; 
			
			// cada caractere tem que ser diferente de \0, espaco e tab
			if ((valorAscii != 10) && (valorAscii != 32) && (valorAscii != 9)) {
				// balanceamento de parenteses '('
				if (valorAscii == 40) {
					parenteses++;
				}
				
				// balanceamento de parenteses ')'
				if (valorAscii == 41) {
					parenteses--;
				}
			}
		}
		
		// verifica se a linha do esrever possui ';'
		for (i = strlen(linhaToProcessamento); i > 0; i--) {
			valorAscii = (int) linhaToProcessamento[i];
			
//			printf("%d ---- %c --- %s\n", valorAscii, linha[i], linhaToProcessamento);
			
			// cada caractere tem que ser diferente de \0, espaco e tab
			if ((valorAscii != 10) && (valorAscii != 32) && (valorAscii != 9)) {
				
				// verifica se o caracter � ';'
				if (valorAscii == 59 && hasPossuiPontoVirgula == 0) {
					hasPossuiPontoVirgula++;
					break;
				}
			}
		}
		
		// verifica se existe duplo balanceamento de parentes
		if (parenteses != 0) {
			throwNewExeception(nuLinha, 4, linha);
		}

		// verificar o ; no final da linha do escreva
		if (hasPossuiPontoVirgula != 1) {
			throwNewExeception(nuLinha, 5, linha);
		}
	}
		
	return isValido;
}

/**
 * Valida se a variavel informada ja esta declarada na tabela de simbolos
 *
 * @param char * palavra
 */
int validarVariavelDeclarada(char* palavra, TabelaSimbolo* tabelaSimbolos) {
	int isValido = 0;
    
	if (tabelaSimbolos == NULL) {
        exit(1);
    }
	
	ElemSimbolo* no = *tabelaSimbolos;

    while (no != NULL) {
    	if (strcmp(palavra, no->dados.variavel) == 0) {
    		isValido = 1;
    		break;
		}
        no = no->prox;
    }
	
	return isValido;
}

/**
 * Verifica se a linha possui [ ou ]
 *
 * @param char* palavra
 * @param char* tipoVariavel
 * @param int numeroLinha
 */
void verificarPresencaColchetes(char* palavra, char* tipoVariavel, int numeroLinha) {
	int i, valorAscii;
		
	if (strcmp(tipoVariavel, tiposVariaveis[0]) == 0) {
		for (i = 0; i < strlen(palavra); i++) {
			valorAscii = (int) palavra[i];

			// se na declaracao possui [ ou ] 
			if (!valorAscii == 91 || !valorAscii == 93) {
				throwNewExeception(numeroLinha, 14, palavra);
			}
		}
	}
	
	// TODO tratar quando for caractere
	if (strcmp(tipoVariavel, tiposVariaveis[1]) == 0) {
		validarTipoCaractere(palavra, numeroLinha);
	}
	
	// TODO tratar quando for real
	if (strcmp(tipoVariavel, tiposVariaveis[2]) == 0) {
		validarTipoReal(palavra, numeroLinha);
	}
}

/**
 * Valida o tipo de dados do caractere
 *
 * @param palavra
 * @param numeroLinha
 */
void validarTipoCaractere(char * palavra, int numeroLinha) {
	int 
		i,
		count = 0,
		valorAscii,
		countValorEntreConchete = 0,
		tamanhoPalavra = strlen(palavra) - 1;

	int hasFecharColchete = 0;
	int hasAberturaColchete = 0;

	char auxValorTamanho[UCHAR_MAX];

	limparConteudoVetor(auxValorTamanho);
	
	// percorre a palavra de traz para frente
	for (i = tamanhoPalavra; i >= 0; i--) {
		valorAscii = palavra[i];

		// printf("valor ascii: %d\n", valorAscii);
		// verifica se o ultimo caracter e um ], caso seja vai percorrendo para salvar o tamanho
		if (i == tamanhoPalavra && valorAscii == 93) {
			hasFecharColchete++;
			continue;
			// condicao que ira acumular enquanto for numero e nao encontrar [.
		} else if(valorAscii >= 48 && valorAscii <= 57) { 
			auxValorTamanho[count] = palavra[i];
			countValorEntreConchete++;
			
			count++;
		} else if (valorAscii == 91) {
			hasAberturaColchete++;
		}
	}
	
	// TODO Validar valores acima ou iguais a zero.
	
	// printf("%d =>>>>> %d =>>> %d", hasAberturaColchete, hasFecharColchete, countValorEntreConchete);
	if (hasFecharColchete == 0 || hasAberturaColchete == 0 || countValorEntreConchete == 0) {
		throwNewExeception(numeroLinha, 8, palavra);
	}
}

/**
 * Valida o tipo de dados real
 *
 * @param palavra
 * @param numeroLinha
 */
void validarTipoReal(char * palavra, int numeroLinha) {
	int 
		i,
		valorAscii,
		count = 0,
		countValorEntreConchete = 0,
		tamanhoPalavra = strlen(palavra) - 1;

	int countTotalPonto = 0;
	int hasFecharColchete = 0;
	int hasAberturaColchete = 0;

	char auxValorTamanho[UCHAR_MAX];

	limparConteudoVetor(auxValorTamanho);
	// percorre a palavra de traz para frente
	for (i = tamanhoPalavra; i >= 0; i--) {
		valorAscii = palavra[i];

		// verifica se o ultimo caracter e um ], caso seja vai percorrendo para salvar o tamanho
		if (i == tamanhoPalavra && valorAscii == 93) {
			hasFecharColchete++;
			continue;
			// condicao que ira acumular enquanto for 0-9 e que o mesmo tenha .
		} else if((valorAscii >= 48 && valorAscii <= 57) || valorAscii == 46) { 
			auxValorTamanho[count] = palavra[i];
			countValorEntreConchete++;
			
			count++;
		} else if (valorAscii == 91) {
			hasAberturaColchete++;
		}
	}
	
	// TODO Validar valores acima ou iguais a zero, tratar depois
	
	if (hasFecharColchete == 0 || hasAberturaColchete == 0 || countValorEntreConchete == 0) {
		throwNewExeception(numeroLinha, 8, palavra);
	}
	
	for (i = 0; i < strlen(auxValorTamanho); i++) {
		valorAscii = auxValorTamanho[i];
		
		if (valorAscii == 46) {
			countTotalPonto++;
		}
	}
	
	if (countTotalPonto != 1) {
		printf("\n[TIPO REAL]\n");
		throwNewExeception(numeroLinha, 8, palavra);
	}
}

/**
 * Verifica se a variavel informada possui tamanho, caso sim, retorna o tamanho.
 *
 * @param char palavra[]
 * @param char retorno[]
 * @param int numeroLinha
 */
void getTamanhoVariavel(char palavra[], char retorno[], int numeroLinha) {
	int 
		i,
		hasPossuiConchete = 0,
		valorAscii, count = 0,
		countValorEntreConchete = 0,
		tamanhoPalavra = strlen(palavra) - 1;

	char valorTamanho[UCHAR_MAX], auxValorTamanho[UCHAR_MAX];
	
	limparConteudoVetor(valorTamanho);
	limparConteudoVetor(auxValorTamanho);
	
	// percorre a palavra de traz para frente
	for (i = tamanhoPalavra; i >= 0; i--) {
		valorAscii = palavra[i];

		// verifica se o ultimo caracter � um ], caso seja vai percorrendo para salvar o tamanho
		if (i == tamanhoPalavra && valorAscii == 93) {
			hasPossuiConchete++;
			continue;
			// condi��o que ira acumular enquanto for numero e nao encontrar
		} else if((valorAscii >= 48 && valorAscii <= 57) && valorAscii != 91) {
			auxValorTamanho[count] = palavra[i];
			countValorEntreConchete++;
			count++;
		} else if (valorAscii == 91) {
			hasPossuiConchete++;
			break;
		}
	}
	
	// se existir apenas um abertura ou fechamento de conchete
	if (hasPossuiConchete == 1) {
		throwNewExeception(numeroLinha, 9, palavra);
	} else if (hasPossuiConchete == 2) {
		if (countValorEntreConchete == 0) {
			throwNewExeception(numeroLinha, 10, palavra);
		}
	}
	
	count = 0;
	
	for (i = strlen(auxValorTamanho) - 1; i >= 0; i--) {
		valorTamanho[count] = auxValorTamanho[i];
		count++;
	}
	
	// por motivos que nao sei, criar uma funcao que retorna string esta dando erro
	// entao uma maneira de contornar tive que devolver o valor por referencia, infelizmente.
	strcpy(retorno, valorTamanho);
}

/**
 * Remove a string referente ao tamanho da variavel, [].
 *
 * @param char palavra[]
 */
void removeTamanhoVariavel(char palavra[]) {
	int i, tamanhoPalavra = strlen(palavra) - 1, valorAscii;
	
	// percorre a palavra de traz para frente
	for (i = tamanhoPalavra; i >= 0; i--) {
		valorAscii = palavra[i];

		// verifica se o ultimo caracter e um ], caso seja vai percorrendo para salvar o tamanho
		if (i == tamanhoPalavra && valorAscii == 93) {
			palavra[i] = '\0';
			continue;
			// condicao que ira acumular enquanto for numero e nao encontrar [
		} else if((valorAscii >= 48 && valorAscii <= 57) && valorAscii != 91) {
			palavra[i] = '\0';
		} else if (valorAscii == 91) {
			palavra[i] = '\0';
			break;
		}
	}
}

/**
 * Verifica se a palavra informada e uma string valida, ou seja se a mesma esta dentro de ", " 
 *
 * @param char* palavra
 * @param int numeroLinha
 */
int validaPalavraString(char* palavra, int numeroLinha) {
	int isValido = 0, i, valorAscii, isDuploPalanceamentoValido = 0;
	
	for (i = 0; i < strlen(palavra); i++) {
		valorAscii = (int) palavra[i];
		
		// verifica se existe um caracter de abertura
		if (valorAscii == 34) {
			isDuploPalanceamentoValido++;
		}
	}
	
	// verifica se � impar, caso seja o duplo balanceamento esta invalido.
	if (hasParOrImpar(isDuploPalanceamentoValido) == 0) {
		throwNewExeception(numeroLinha, 12, palavra);
	} else {
		isValido = 1;
	}

	return isValido;
}


/**
 * Verifica se o valor informado e par ou impar. Retorna 1 - par, 0 - impar.
 *
 * @param int valor
 */
int hasParOrImpar(int valor) {
	int isValido = 0;
	
	if (valor % 2 == 0) {
		isValido = 1;
	}
	
	return isValido;
}

/**
 * Verifica se a gramatica possui uma funcao principal v�lida.
 *
 * @param char palavra
 * @param int nuLinha
 */
int isPrincipalValido(char *palavra, int nuLinha) {	
	int i, ascii, count = 0, isInValido = 0;

	char palavraTmp[UCHAR_MAX];	
	char palavraProcessada[UCHAR_MAX];

	limparConteudoVetor(palavraTmp);
	limparConteudoVetor(palavraProcessada);
	
	strcpy(palavraProcessada, palavra);
	
	removerTabulacao(palavraProcessada);
	removerEspaco(palavraProcessada);
	removerQuebraLinha(palavraProcessada);
		
	// Verifica se a palavra principal possui abertura de parentes sem variaveis => 'principal('
	for (i = 0; i < strlen(palavraProcessada); i++) {
		ascii = (int) palavraProcessada[i];
		palavraTmp[count] = (char) ascii;
		count++;
				
		if (strcmp(palavraTmp, "principal(") == 0) {
			isInValido = 1;
			break;
		}
	}
	// printf("------ > %d -- %d -- %c == %s\n", isInValido, ascii, palavraProcessada[i], palavraTmp);
	if (!isInValido) {
		throwNewExeception(nuLinha, 17, palavra);
	}
	
	isInValido = 0;
	count = 0;
	limparConteudoVetor(palavraTmp);
	
	// Verifica se a palavra principal possui fechamento de parentes com abertura de chaves => 'principal(){'
	for (i = 0; i < strlen(palavraProcessada); i++) {
		ascii = (int) palavraProcessada[i];
		palavraTmp[count] = (char) ascii;
		count++;
				
		if (strcmp(palavraTmp, "principal(){") == 0) {
			isInValido = 1;
			break;
		}
	}
	
	if (!isInValido) {
		throwNewExeception(nuLinha, 18, palavra);
	}
	
	return 1;
}

/**
 * Verifica se a declaracao da funcao esta valida.
 *
 * @param char palavra
 * @param int nuLinha
 */
int validarDeclaracaoFuncao(char *palavra, int nuLinha) {
	int isValido = 0, i, ascii, asciiPrimeiro, asciiSegundo, asciiTerceiro;

	asciiPrimeiro = (int) palavra[0];
	asciiSegundo = (int) palavra[1];
	asciiTerceiro = (int) palavra[2];

	// verifica se a palavra inicializa com dois _ = 95 e em seguida um caracter de a-z, 0-9, A-Z
	if (asciiPrimeiro == 95 && asciiSegundo == 95) {
		// apenas a-z, 0-9, A-Z
		if ((asciiTerceiro >= 97 && asciiTerceiro <= 122) || (asciiTerceiro >= 48 && asciiTerceiro <= 57) || (asciiTerceiro >= 65 && asciiTerceiro <= 90)) {
			isValido = 1;
		} else {
			throwNewExeception(nuLinha, 20, palavra);
		}

		for (i = 3; i < strlen(palavra); i++) {
			ascii = (int) palavra[i];

			// permiter apenas a-z, 0-9, A-Z, [, ], .
			if (! ((ascii >= 97 && ascii <= 122) || (ascii >= 48 && ascii <= 57) || (ascii >= 65 && ascii <= 90) || ascii == 91 || ascii == 93 || ascii == 46)) {
				isValido = 0;
				throwNewExeception(nuLinha, 20, palavra);
			}
		}
	}

	return isValido;
}

/**
 * Exibe a mensagem de erro encontrada ao validar as linhas do arquivo.
 * 
 * @param int nuLinha
 * @param int tipoErro
 */
void throwNewExeception(int nuLinha, int tipoErro, char *palavra) {
	
	removerQuebraLinha(palavra);
	
	switch(tipoErro) {
		case 1:
			printf("1 - Erro Lexico => (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 2:
			printf("Erro => A memoria limite de %d foi ultrapassada. Memoria consumida: %d", TOTAL_CONSUMO_MEMORIA, MAX_TOTAL_CONSUMO_MEMORIA);
		break;
		case 3:
			printf("Erro => A declaracao da palavra reservada 'ler' esta incorreta, (%s), favor verificar a inexistencia de parametros ou parenteses. [linha - %d].\n", palavra, nuLinha);
		break;
		case 4:
			printf("Erro => Ha ausencia do caracter '(' ou ')', em (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 5:
			printf("Erro => A linha nao finalizou com ';' em (%s), favor verificar. [linha - %d].\n", palavra, nuLinha);
		break;
		case 6:
			printf("Erro => A declaracao da palavra reservada 'escrever' esta incorreta, (%s), favor verificar a inexistencia de parametros ou parenteses. [linha - %d].\n", palavra, nuLinha);
		break;
		case 7:
			printf("Erro => Uma variavel com o nome (%s) ja foi declarada. [linha - %d].\n", palavra, nuLinha);
		break;
		case 8:
			printf("Erro => Declaracao incorreta de variavel (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 9:
			printf("Erro => Ha ausencia do caracter '[' ou ']', (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 10:
			printf("Erro => eh obrigatorio um valor de tamanho para variavel, (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 11:
			printf("Erro => A variavel (%s) nao foi declarada. [linha - %d].\n", palavra, nuLinha);
		break;
		case 12:
			printf("Erro => O '%c' precisa ter duplo balanceamento, (%s). [linha - %d].\n", (char) 34, palavra, nuLinha);
		break;
		case 13:
			printf("Erro => A palavra (%s) nao e uma palavra reservada e nem variavel, nao consta no escopo. [linha - %d].\n", palavra, nuLinha);
		break;
		case 14:
			printf("Erro => O duplo balanceamento de (\") esta incorreto, (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 15:
			printf("Erro => A linha nao e uma declaracao de variavel e a mesma deve conter ';' no final da linha (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 16:
			printf("Erro => As variavies devem ser separadas por ',' em (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 17:
			printf("Erro => A declaracao da palavra 'principal' esta incorreta, valor verificar abertura de parentes - (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 18:
			printf("Erro => A declaracao da palavra 'principal' esta incorreta, valor verificar abertura de chaves - (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 19:
			printf("Erro => O modulo principal ja foi declarado anteriormente: (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 20:
			printf("Erro => A palavra (%s) nao e uma funcao valida. [linha - %d].\n", palavra, nuLinha);
		break;
		case 21:
			printf("Erro => valor informado nao identificado (%s). [linha - %d].\n", palavra, nuLinha);
			break;
		case 22:
			printf("Erro => Modulo principal nao existente.\n");
			break;
		case 23:
			printf("Erro => O duplo balanceamento de chaves esta incorreto, favor verificar o arquivo.\n");
			break;
		case 24:
			printf("Erro => O duplo balanceamento de aspas esta incorreto, (%s). [linha - %d].\n", palavra, nuLinha);
			break;
		case 25:
			printf("Erro => O duplo balanceamento de colchetes esta incorreto, (%s). [linha - %d].\n", palavra, nuLinha);
			break;
		case 26:
			printf("Erro => O duplo balanceamento de parenteses esta incorreto, (%s). [linha - %d].\n", palavra, nuLinha);
			break;
		default:
			printf("Foi selecionando um tipo de erro nao definido no case.\n");
		break;
	}

	exit(1);
}

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

    // lista vazia: insere inicio
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
    	printf("\tA TABELA DE SIMBOLOS ESTA VAZIA.\n");	
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

/**
 * Conta a quantidade de memoria consumida, incrementado e subtraindo da constante TOTAL_CONSUMO_MEMORIA
 *
 * http://www.cprogressivo.net/2013/03/A-funcao-sizeof-e-blocos-vizinhos-de-memoria-em-C.html
 *
 * @param int memoria
 * @param int situacao: 1 - incrementa, 2 - subtrai
 */
void memoriaConsumida(int memoria, int situacao) {
	float percentMemoriaConsumida = 0;

	if (situacao == 1) {
		TOTAL_CONSUMO_MEMORIA += memoria;	
	} else {
		TOTAL_CONSUMO_MEMORIA -= memoria;
	}

	if (MAX_TOTAL_CONSUMO_MEMORIA > 0) {
		percentMemoriaConsumida = (TOTAL_CONSUMO_MEMORIA * 100) / MAX_TOTAL_CONSUMO_MEMORIA;
	}

	if (percentMemoriaConsumida > 90 && percentMemoriaConsumida < 99) {
		printf("Sua memoria esta entre 90%% a 99%% do total disponivel, memoria atual: %.2f%\n\n", percentMemoriaConsumida);
	}
	
	if (TOTAL_CONSUMO_MEMORIA > MAX_TOTAL_CONSUMO_MEMORIA) {
		char a[1] = "a";
		throwNewExeception(0, 2, a);
	}
}

void removerQuebraLinha(char* palavra) {
	int i, valorAscii, count = 0;
	char palavraAux[UCHAR_MAX];
	limparConteudoVetor(palavraAux);

	for (i = 0; i < strlen(palavra); i++) {
		valorAscii = (int) palavra[i];

		// Line Feed = LF (Windows) => 10
		// Enter = CR (Unix) => 13
		if ((valorAscii != 13) && (valorAscii != 10)) {
			palavraAux[count] = palavra[i];
			count++;
		}
	}

	strcpy(palavra, palavraAux);

	memoriaConsumida(sizeof(i), 1);
	memoriaConsumida(sizeof(valorAscii), 1);
	memoriaConsumida(sizeof(count), 1);
	memoriaConsumida(sizeof(palavraAux), 1);
}

void removerTabulacao(char* palavra) {
	int i, valorAscii, count = 0;
	char palavraAux[UCHAR_MAX];
	limparConteudoVetor(palavraAux);

	for (i = 0; i < strlen(palavra); i++) {
		valorAscii = (int) palavra[i];
		// Tab => 9
		if (valorAscii != 9) {
			palavraAux[count] = palavra[i];
			count++;
		}
	}

	strcpy(palavra, palavraAux);

	memoriaConsumida(sizeof(i), 1);
	memoriaConsumida(sizeof(valorAscii), 1);
	memoriaConsumida(sizeof(count), 1);
	memoriaConsumida(sizeof(palavraAux), 1);
}

void removerEspaco(char *palavra) {
	int i, ascii, count = 0;
	char palavraTmp[UCHAR_MAX];
	limparConteudoVetor(palavraTmp);

	for (i = 0; i < strlen(palavra); i++) {
		ascii = (int) palavra[i];
		// Espaco => 32
		if (ascii != 32) {
			palavraTmp[count] = palavra[i];
			count++;
		}
	}

	strcpy(palavra, palavraTmp);
	memoriaConsumida(sizeof(i), 1);
	memoriaConsumida(sizeof(ascii), 1);
	memoriaConsumida(sizeof(count), 1);
	memoriaConsumida(sizeof(palavraTmp), 1);
}
