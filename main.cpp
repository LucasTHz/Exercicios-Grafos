#include "matrizadj/Grafo.h" 
#include <fstream>
using namespace std;

int main (int argc, char **argv) 
{
    ifstream in ("input.txt");
    Grafo *grafo = new Grafo (in);
    grafo->imprime ();  

    //Ex1: criar e imprimir grafo transposto.

    //Ex2: Criar e imprimir um grafo não orientado.
    
    //Ex3: Ler e inserir nova aresta, verificando se ela já existe.
   
    //Ex4: Ler e remover uma aresta, verificando se ela já existe.

    //Ex5: Imprimir uma lista adjacente de um determinado vértices.
}
