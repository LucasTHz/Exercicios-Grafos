#include "listaadj/Grafo.h"
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
    ifstream in("input.txt");
    Grafo *grafo = new Grafo(in);
    // grafo->imprime();
    // grafo->buscaProfundidade();
    int numero_componentes = grafo->numComponentes();
    cout << "Numero de componentes: " << numero_componentes << endl;

    grafo->buscaProfundidadeAciclico();
    vector<int> ordem = grafo->ordemTopologica();
    cout << "Ordem topologica: ";
    // inverte a ordem
    for (int i = ordem.size() - 1; i >= 0; i--)
        cout << ordem[i] << " ";
    cout << endl;

    // grafo->buscaProLargura();
}
