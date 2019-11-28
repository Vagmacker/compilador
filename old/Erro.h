/**
 * Class de mensageria de erro do projeto.
 *
 * @author j.ribeiro
 */


/**
 * Exibe a mensagem de erro encontrada ao validar as linhas do arquivo.
 * 
 * @param int nuLinha
 * @param int tipoErro
 */
void throwNewExeception(int nuLinha, int tipoErro, char *palavra) {
	switch(tipoErro) {
		case 1:
			printf("1 - Erro L�xico => (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 2:
			printf("Erro => A mem�ria limite de %d foi ultrapassada. Mem�ria consumida: %d", TOTAL_CONSUMO_MEMORIA, MAX_TOTAL_CONSUMO_MEMORIA);
		break;
		case 3:
			printf("Erro => A declara��o da palavra reservada 'leia' esta incorreta, (%s), favor verificar. [linha - %d].\n", palavra, nuLinha);
		break;
		case 4:
			printf("Erro => H� aus�ncia do caracter '(' ou ')', (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 5:
			printf("Erro => A linha n�o finalizou com ';' em (%s), favor verificar. [linha - %d].\n", palavra, nuLinha);
		break;
		case 6:
			printf("Erro => A declara��o da palavra reservada 'escrever' esta incorreta, (%s), favor verificar. [linha - %d].\n", palavra, nuLinha);
		break;
		case 7:
			printf("Erro => Uma variavel com o nome (%s) j� foi declarada. [linha - %d].\n", palavra, nuLinha);
		break;
		case 8:
			printf("Erro => Declara��o incorreta de variavel (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 9:
			printf("Erro => H� aus�ncia do caracter '[' ou ']', (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 10:
			printf("Erro => � obrigatorio um valor de tamanho para variavel, (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 11:
			printf("Erro => A variavel (%s) n�o foi declarada. [linha - %d].\n", palavra, nuLinha);
		break;
		case 12:
			printf("Erro => O '%c' precisa ter duplo balanceamento, (%s). [linha - %d].\n", (char) 34, palavra, nuLinha);
		break;
		case 13:
			printf("Erro => A palavra (%s) n�o � uma palavra reservada e nem variavel, n�o consta no escopo. [linha - %d].\n", palavra, nuLinha);
		break;
		case 14:
			printf("Erro => O duplo balanceamento de (\") est� incorreto, (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 15:
			printf("Erro => A linha � uma declara��o de variavel e a mesma deve conter ';' no final da linha (%s). [linha - %d].\n", palavra, nuLinha);
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
			printf("Erro => O modulo principal j� foi declarado anteriormente: (%s). [linha - %d].\n", palavra, nuLinha);
		break;
		case 20:
			printf("Erro => A palavra (%s) n�o � uma fun��o valida. [linha - %d].\n", palavra, nuLinha);
		break;
		default:
			printf("Foi selecionando um tipo de erro nao definido no case.\n");
		break;
	}

	exit(1);
}
