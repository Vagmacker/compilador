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
void analisarToken(Lista* lista, TabelaSimbolo* tabelaSimbolos) {
	int 
		i,
		count = 0,
		aspas = 0,
		valorAscii,
		numeroLinha,
		countValor = 0,
		countVirgulas = 0,
		countVariaveis = 0,
		hasDeclaracaoPrincipal = 0;

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
	
	Elem* no = *lista;
	
	if (lista == NULL) {
        exit(1);
    }
    
    //char nome[] = {"       falso escrever($nome);"};
    //printf("%s\n", nome);
    //tratarLerEscrever(nome);
    //printf("%s\n", nome);
    //exit(0);

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
			if ((valorAscii != 10) && (valorAscii != 32) && (valorAscii != 40) && (valorAscii != 41) && (valorAscii != 44) && (valorAscii != 59) && (valorAscii != 36) && (valorAscii != 9) && (valorAscii != 60) && (valorAscii != 61) && (valorAscii != 62) && (valorAscii != 123) && (valorAscii != 125)) {
				if (hasPossuiAspas == false) {
					palavraAux[count] = (char) valorAscii;
					count++;
				}
				
				atualizarValorVariavel(tabelaSimbolos, nomeVariavelAux, palavraAux);
			} else if (hasPossuiAspas == false) {

				// Verifica se é declaração de variavel e atribuição
//				if (isSimboloAtribuicao == true) {
//					hasAtribuicao = true;
//				}

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
					} else {
						if (hasAtribuicao == true) {
							strcpy(valorAtribuicao, palavraAux);
							//printf("variavel: %s possui o valoor: %s\n", nomeVariavelAux, palavraAux);
							//atualizarValorVariavel(tabelaSimbolos, nomeVariavelAux, palavraAux);
							limparConteudoVetor(palavraAux);
							count = 0;
							//hasAtribuicao = false;
							continue;
						}
					}
				} else {
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
	    				strcpy(novoSimbolo.variavel, palavraAux);
	    				
	    				insereFinalTabelaSimbolo(tabelaSimbolos, novoSimbolo);
	    				
	    				// apenas para tipos de caracter
					    if (strcmp(tipoVariavel, tiposVariaveis[2]) == 0) {
					    	atualizarValorVariavel(novoSimbolo, nomeVariavelAux, palavraAux);
						}
					}

					// limparConteudoVetor(auxTamanhoPalavra);
					
					// validar se a variavel ja foi declarada.
					if (validarVariavelDeclarada(palavraAux, tabelaSimbolos) == 0) {
						throwNewExeception(numeroLinha, 11, palavraAux);
					}
				}
				
				// verifica se a string em '"' está valida
				if (strlen(palavraAux) > 0) {
					if (hasVariavel == false && hasPalavraReservada == false && aspas > 0) {
						hasString = validaPalavraString(palavraAux, numeroLinha);
					}
				}
				
				if (strlen(palavraAux) > 0) {
					// compara se é uma variavel e se é uma palavra reservada
					//if (hasVariavel == false && hasPalavraReservada == false && aspas == 0) {
					//	throwNewExeception(numeroLinha, 13, palavraAux);
					//}
				}
				
				count = 0;
				hasString = 0;
				hasVariavel = 0;
				hasPalavraReservada = 0;
				limparConteudoVetor(palavraAux);
				
				// como $ faz parte da variavel e tambem é uma condição de parada, entao preciso incrementar aqui
				if (valorAscii == 36) {
					palavraAux[count] = (char) valorAscii;
					count++;
				}
			} // fim do else para valores que são condições de parada.

			// condição de parada quando para limpar a variavel 'palavraValorAux'
			// espaco, $, virgula, ponto e virgula
			if ((aspas == 2) && ((valorAscii == 32) || (valorAscii == 36) || (valorAscii == 44) || (valorAscii == 59))) {
				atualizarValorVariavel(tabelaSimbolos, nomeVariavelAux, palavraValorAux);
				limparConteudoVetor(palavraValorAux);
				limparConteudoVetor(nomeVariavelAux);
				countValor = 0;
				hasPossuiAspas = false;
				
				// como $ faz parte da variavel e tambem é uma condição de parada, entao preciso incrementar aqui
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
			if (valorAscii == 44 && hasPossuiAspas == false) {
				countVirgulas ++;	
			}

		} // fim do for que percorre as colunas da linha
		
		// verifica o duplo balanceamento de '"'
		if (aspas != 0) {
			throwNewExeception(numeroLinha, 14, conteudoLinha);
		}

		// Essa validação de ; no final esta invalida criar uma nova
		hasPossuiPontoVirgula = validaDeclaracaoComPontoVirgula(conteudoLinha, numeroLinha);
		
		// verifica se a linha possui declaração de variavel e tem ; nessa linha
		if (hasLinhaComVariavel == true && hasPossuiPontoVirgula == false) {
			throwNewExeception(numeroLinha, 15, conteudoLinha);
		}
		
		// caso tenha mais de 1 variavel a quantidade de virgulas será 'countVirgulas' - 'countVariaveis' - 1)
		//if (countVariaveis > 1 && hasEscreva == false) {
		//	if (countVirgulas != (countVariaveis - 1)) {
		//		throwNewExeception(numeroLinha, 20, conteudoLinha);	
		//	}
		//}

		aspas = 0;
		no = no->prox;
		countValor = 0;
		hasLeia = false;
		countVirgulas = 0;
		countVariaveis = 0;
		hasEscreva = false;
		hasRepetir = false;
		hasAtribuicao = false;
		hasPossuiAspas = false;
		isSimboloAtribuicao = false;
		hasLinhaComVariavel = false;
		hasPossuiPontoVirgula = false;
		limparConteudoVetor(tipoVariavel);
		limparConteudoVetor(palavraValorAux);
		limparConteudoVetor(nomeVariavelAux);
	} // fim while
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
	int isValido = 0, i;
	
	for (i = 0; i < 3; i++) {
		if (strcmp(palavra, tiposVariaveis[i]) == 0) {
			isValido = 1;
			break;
		}		
	}

	return isValido;
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
 * Verifica se a linha possui a instrução 'leia' e verifica seus criterios.
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
		hasLeiaValido = 0, // verifica se a palavra leia esta no padrão válido.
		hasPossuiPontoVirgula = 0;
	
	char palavraAux[UCHAR_MAX];
	limparConteudoVetor(palavraAux);

	// verifica se a palavra reservada é 'ler'
	if (strcmp(palavra, palavrasReservadas[0]) == 0) {

		// identifica se a palavra reservada é 'ler'
		isValido = 1;
		
		// valida se apos a palavra 'ler' possui obrigatoriamente um '(' e uma variavel.
		for (i = 0; i < strlen(linha); i++) {
			valorAscii = (int) linha[i];
			
			// Verifica se a caracter ascii informado e uma condição de parada, para ser feita uma determinada analise.
			// As condições de parada sao os caracterers : \0, espaco, ), virgula, ponto virgula, $, tabs
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
		
		// não pode haver declarações dentro da estrutura
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
				
				// verifica se o caracter é ';'
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
 * Remove palavras reservadas na linha onde possui a instrução 'ler' e 'escrever'.
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
		// condições de parada: espaço
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
 * Verifica se a linha possui a instrução 'escreva' e verifica seus criterios.
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
	
	// verifica se a palavra reservada é 'escrever'
	if (strcmp(palavra, palavrasReservadas[8]) == 0) {
		
		// identifica se a palavra reservada é 'escrever'
		isValido = 1;

		// valida se apos a palavra 'escrever' possui obrigatoriamente um '(' e uma variavel.
		for (i = 0; i < strlen(linhaToProcessamento); i++) {
			valorAscii = (int) linhaToProcessamento[i];
			
			//printf("%d - %c - %s\n", valorAscii, linhaToProcessamento[i], palavraAux);
			// Verifica se a caracter ascii informado e uma condição de parada, para ser feita uma determinada analise.
			// As condições de parada sao os caracterers : \0, espaco, ), virgula, ponto virgula, $, tabs
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
		
		// nao pode haver declarações dentro da estrutura
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
				
				// verifica se o caracter é ';'
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

			// se na declaração possui [ ou ] 
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
		// verifica se o ultimo caracter é um ], caso seja vai percorrendo para salvar o tamanho
		if (i == tamanhoPalavra && valorAscii == 93) {
			hasFecharColchete++;
			continue;
			// condição que irá acumular enquanto for numero e não encontrar [.
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
		printf("\n[TIPO CARACTERE]\n");
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

		// verifica se o ultimo caracter é um ], caso seja vai percorrendo para salvar o tamanho
		if (i == tamanhoPalavra && valorAscii == 93) {
			hasFecharColchete++;
			continue;
			// condição que ira acumular enquanto for 0-9 e que o mesmo tenha .
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
		printf("\n[TIPO REAL]\n");
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

		// verifica se o ultimo caracter é um ], caso seja vai percorrendo para salvar o tamanho
		if (i == tamanhoPalavra && valorAscii == 93) {
			hasPossuiConchete++;
			continue;
			// condição que ira acumular enquanto for numero e nao encontrar
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

		// verifica se o ultimo caracter é um ], caso seja vai percorrendo para salvar o tamanho
		if (i == tamanhoPalavra && valorAscii == 93) {
			palavra[i] = '\0';
			continue;
			// condição que ira acumular enquanto for numero e nao encontrar [
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
	
	// verifica se é impar, caso seja o duplo balanceamento esta invalido.
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
 * Verifica se a gramatica possui uma função principal válida.
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
 * Verifica se a declaração da função está válida.
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
