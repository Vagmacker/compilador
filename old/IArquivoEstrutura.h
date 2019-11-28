/**
 * Definição das estrutura e assinaturas referentes ao 'Arquivo'.
 *
 * @author j.ribeiro
 */

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
