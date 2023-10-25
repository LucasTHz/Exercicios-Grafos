#include "listaadj/Grafo.h"
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
    ifstream in("input.txt");
    Grafo *grafo = new Grafo(in);
    // vector<int> v;
    // grafo->imprime();
    // grafo->buscaProfundidade();
    // grafo->buscaProLargura();
    // grafo->imprime();
    // grafo->buscaMenorCaminho(6, 0);
    // grafo->kruskal();
    grafo->prim(0);

    // printf("Grafo original \n");
    grafo->imprime();

    // Ex1: criar e imprimir grafo transposto.
    // printf("Grafo transposto \n");
    // grafo = grafo->grafoTransposto();
    // grafo->imprime();

    // // Ex2: Criar e imprimir um grafo não orientado.
    // printf("Grafo não orientado \n");
    // grafo = grafo->grafoNaoDirecionado();

    // // Ex3: Ler e inserir nova aresta, verificando se ela já existe.
    // Grafo::Aresta *aresta = grafo->lerAresta();
    // if (grafo->existeAresta(aresta->_v1(), aresta->_v2()))
    // {
    //     cout << "Aresta já existe" << endl;
    // }
    // else
    // {
    //     grafo->insereAresta(aresta->_v1(), aresta->_v2(), aresta->_peso());
    // }

    // Ex4: Ler e remover uma aresta, verificando se ela já existe.
    // Grafo::Aresta *aresta = grafo->lerAresta();
    // if (grafo->existeAresta(aresta->_v1(), aresta->_v2()))
    // {
    //     cout << "Aresta já existe" << endl;
    // }
    // else
    // {
    //     grafo->retiraAresta(aresta->_v1(), aresta->_v2());
    // }
    // grafo->imprime();

    // Ex5: Imprimir uma lista adjacente de um determinado vértices.
    // Grafo::Aresta *aresta = grafo->lerAresta();

    // grafo->listaAdj(2);
    // grafo->imprime();

    // if (grafo->subeuleriano())
    //     cout << "Subeuleriano" << endl;
    // if (grafo->euleriano())
    //     cout << "Euleriano" << endl;
    // if (grafo->regular())
    //     cout << "Regular" << endl;
    // if (grafo->completo())
    //     cout << "Completo" << endl;
}
