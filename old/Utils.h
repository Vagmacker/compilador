/**
 * Classe utilitaria do projeto
 *
 * @author j.ribeiro
 */

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
		printf("Sua memoria está entre 90% a 99% do total disponível, memoria atual: %.2f%\n\n", percentMemoriaConsumida);
	}
	
	if (TOTAL_CONSUMO_MEMORIA > MAX_TOTAL_CONSUMO_MEMORIA) {
		char a[1] = "a";
		throwNewExeception(0, 2, a);
	}
}
