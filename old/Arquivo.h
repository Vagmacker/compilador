// Tratamento das regras de manipula��o referente ao arquivo.

FILE* carregarArquivo(char *nome) {

    FILE *arquivo;
    
    arquivo = fopen(nome, "r");
    
    if (arquivo == NULL) {
		printf("\nErro, n�o foi possivel abrir o arquivo\n");
	}

	return arquivo;
}
