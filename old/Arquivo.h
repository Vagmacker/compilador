// Tratamento das regras de manipulação referente ao arquivo.

FILE* carregarArquivo(char *nome) {

    FILE *arquivo;
    
    arquivo = fopen(nome, "r");
    
    if (arquivo == NULL) {
		printf("\nErro, não foi possivel abrir o arquivo\n");
	}

	return arquivo;
}
