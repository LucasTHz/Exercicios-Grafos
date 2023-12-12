#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
#include <cstring>
#include <float.h>
using namespace std;
class Grafo
{
public:
  class Aresta
  {
  private:
    int v1, v2, peso;

  public:
    Aresta(int v1, int v2, int peso)
    {
      this->v1 = v1;
      this->v2 = v2;
      this->peso = peso;
    }

    int _peso() { return this->peso; }
    int _v1() { return this->v1; }
    int _v2() { return this->v2; }
    ~Aresta() {}
  };

private:
  int **mat; // @{\it pesos do tipo inteiro}@
  int numVertices;
  int *pos; // @{\it posi\c{c}\~ao atual ao se percorrer os adjs de um v\'ertice v}@

public:
  Grafo(int numVertices);
  Grafo(istream &in);
  Grafo(int numVertices, int numArestas);
  void insereAresta(int v1, int v2, int peso);
  bool existeAresta(int v1, int v2) const;
  bool listaAdjVazia(int v) const;
  Aresta *lerAresta();
  Aresta *primeiroListaAdj(int v);
  Aresta *proxAdj(int v);
  Aresta *retiraAresta(int v1, int v2);
  void imprime() const;
  int _numVertices() const;
  Grafo *grafoTransposto();
  Grafo *grafoNaoDirecionado();
  void listaAdj(int v);
  int grauVertice(int v);
  bool completo();
  bool regular();
  bool euleriano();
  bool subeuleriano();
  void buscaProfundidade(int v, bool visitados[]);
  void floydWarshall(int origem, int destino);
  void cutAresta();
  ~Grafo();
};

Grafo::Grafo(istream &in)
{
  int v1, v2, peso;
  in >> this->numVertices;
  this->mat = new int *[numVertices];
  for (int i = 0; i < numVertices; i++)
    this->mat[i] = new int[numVertices];
  this->pos = new int[numVertices];

  while (in >> v1 >> v2 >> peso)
  {
    Grafo::Aresta *a = new Grafo::Aresta(v1, v2, peso);
    this->insereAresta(a->_v1(), a->_v2(), a->_peso());
    delete a;
  }
}

Grafo::Grafo(int numVertices)
{
  this->mat = new int *[numVertices];
  for (int i = 0; i < numVertices; i++)
    this->mat[i] = new int[numVertices];
  this->pos = new int[numVertices];
  this->numVertices = numVertices;
  for (int i = 0; i < this->numVertices; i++)
  {
    for (int j = 0; j < this->numVertices; j++)
      this->mat[i][j] = 0;
    this->pos[i] = -1;
  }
}

Grafo::Grafo(int numVertices, int numArestas)
{
  this->mat = new int *[numVertices];
  for (int i = 0; i < numVertices; i++)
    this->mat[i] = new int[numVertices];
  this->pos = new int[numVertices];
  this->numVertices = numVertices;
  for (int i = 0; i < this->numVertices; i++)
  {
    for (int j = 0; j < this->numVertices; j++)
      this->mat[i][j] = 0;
    this->pos[i] = -1;
  }
}

void Grafo::insereAresta(int v1, int v2, int peso)
{
  this->mat[v1][v2] = peso;
}
bool Grafo::existeAresta(int v1, int v2) const
{
  return (this->mat[v1][v2] > 0);
}
bool Grafo::listaAdjVazia(int v) const
{
  for (int i = 0; i < this->numVertices; i++)
    if (this->mat[v][i] > 0)
      return false;
  return true;
}

Grafo::Aresta *Grafo::lerAresta()
{
  cout << "Aresta:" << endl;
  cout << "  V1:";
  int v1 = 0;
  cin >> v1;
  cout << "  V2:";
  int v2 = 0;
  cin >> v2;
  cout << "  Peso:";
  int peso = 0;
  cin >> peso;
  return new Aresta(v1, v2, peso);
}

Grafo::Aresta *Grafo::primeiroListaAdj(int v)
{
  // @{\it Retorna a primeira aresta que o v\'ertice v participa ou}@
  // @{\it {\bf NULL} se a lista de adjac\^encia de v for vazia}@
  this->pos[v] = -1;
  return this->proxAdj(v);
}

Grafo::Aresta *Grafo::proxAdj(int v)
{
  // @{\it Retorna a pr\'oxima aresta que o v\'ertice v participa ou}@
  // @{\it {\bf NULL} se a lista de adjac\^encia de v estiver no fim}@
  this->pos[v]++;
  while ((this->pos[v] < this->numVertices) &&
         (this->mat[v][this->pos[v]] == 0))
    this->pos[v]++;
  if (this->pos[v] == this->numVertices)
    return NULL;
  else
    return new Aresta(v, this->pos[v], this->mat[v][this->pos[v]]);
}

Grafo::Aresta *Grafo::retiraAresta(int v1, int v2)
{
  if (this->mat[v1][v2] == 0)
    return NULL; // @{\it Aresta n\~ao existe}@
  else
  {
    Aresta *aresta = new Aresta(v1, v2, this->mat[v1][v2]);
    this->mat[v1][v2] = 0;
    return aresta;
  }
}

void Grafo::imprime() const
{
  cout << "   ";
  for (int i = 0; i < this->numVertices; i++)
    cout << i << "   ";
  cout << endl;
  for (int i = 0; i < this->numVertices; i++)
  {
    cout << i << "  ";
    for (int j = 0; j < this->numVertices; j++)
      cout << this->mat[i][j] << "   ";
    cout << endl;
  }
}

int Grafo::_numVertices() const { return this->numVertices; }

Grafo *Grafo::grafoTransposto()
{
  Grafo *grafoT = new Grafo(this->numVertices);
  for (int v = 0; v < this->numVertices; v++)
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);
      while (adj != NULL)
      {
        grafoT->insereAresta(adj->_v2(), adj->_v1(), adj->_peso());
        delete adj;
        adj = this->proxAdj(v);
      }
    }
  return grafoT;
}

Grafo *Grafo::grafoNaoDirecionado()
{
  Grafo *grafoND = new Grafo(this->numVertices);
  for (int v = 0; v < this->numVertices; v++)
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);
      while (adj != NULL)
      {
        grafoND->insereAresta(adj->_v1(), adj->_v2(), adj->_peso());
        grafoND->insereAresta(adj->_v2(), adj->_v1(), adj->_peso());
        delete adj;
        adj = this->proxAdj(v);
      }
    }
  return grafoND;
}

void Grafo::listaAdj(int v)
{

  for (int j = 0; j < this->numVertices; j++)
    if (this->mat[v][j] > 0)
      cout << j << "   ";
  cout << endl;
}

int Grafo::grauVertice(int v)
{
  int grau = 0;
  for (int j = 0; j < this->numVertices; j++)
    if (existeAresta(v, j))
    {
      // if (this->mat[v][j] > 0)
      grau++;
    }
  return grau;
}

bool Grafo::completo()
{
  for (int i = 0; i < this->numVertices; i++)
  {
    if (this->grauVertice(i) < this->numVertices - 1)
    {
      return false;
    }
  }
  return true;
}

bool Grafo::regular()
{
  int grauAtual = this->grauVertice(0);
  for (int i = 0; i < this->numVertices; i++)
  {
    if (this->grauVertice(i) != grauAtual)
    {
      return false;
    }
  }
  return true;
}

// todos os vértices de grau par
bool Grafo::euleriano()
{
  for (int i = 0; i < this->numVertices; i++)
  {
    if (this->grauVertice(i) % 2 != 0)
    {
      return false;
    }
  }
  return true;
}

// conter apenas dois vértices de grau ímpar
bool Grafo::subeuleriano()
{
  int cont = 0;
  for (int i = 0; i < this->numVertices; i++)
  {
    if (this->grauVertice(i) % 2 != 0)
    {
      cont++;
    }
  }

  return cont == 2 ? true : false;
}

// void Grafo::buscaProfundidade()
// {
//   int *cor = new int[this->numVertices];
// }

void Grafo::floydWarshall(int origem, int destino)
{
  int **distancia = new int *[this->numVertices];
  int **antecessor = new int *[this->numVertices];
  for (int i = 0; i < this->numVertices; i++)
  {
    distancia[i] = new int[this->numVertices];
    antecessor[i] = new int[this->numVertices];
  }

  for (int i = 0; i < this->numVertices; i++)
    for (int j = 0; j < this->numVertices; j++)
    {
      if (i == j) // a distancia entre o vertice e ele mesmo é 0
      {
        distancia[i][j] = 0;
        antecessor[i][j] = -1;
      }
      else if (this->mat[i][j] != NULL) // verifica se uma unica aresta conecta os dois vertices
      {
        distancia[i][j] = this->mat[i][j];
        antecessor[i][j] = i;
      }
      else
      {
        distancia[i][j] = __INT_MAX__;
        antecessor[i][j] = -1;
      }
    }

  for (int k = 0; k < this->numVertices; k++)
    for (int i = 0; i < this->numVertices; i++)
      for (int j = 0; j < this->numVertices; j++)
        if (distancia[i][k] != __INT_MAX__ && distancia[k][j] != __INT_MAX__)
        {

          if (distancia[i][k] + distancia[k][j] < distancia[i][j])
          {
            distancia[i][j] = distancia[i][k] + distancia[k][j];
            antecessor[i][j] = antecessor[k][j];
          }
        }

  cout << "Matriz final" << endl;
  for (int i = 0; i < this->numVertices; i++)
  {
    for (int j = 0; j < this->numVertices; j++)
    {
      if (distancia[i][j] == __INT_MAX__)
        cout << "INF"
             << "   ";
      else
        cout << distancia[i][j] << "   ";
    }
    cout << endl;
  }

  cout << endl
       << "Matriz de antecessores" << endl;
  for (int i = 0; i < this->numVertices; i++)
  {
    for (int j = 0; j < this->numVertices; j++)
    {
      if (antecessor[i][j] == __INT_MAX__)
        cout << "INF"
             << "   ";
      else
        cout << antecessor[i][j] << "   ";
    }
    cout << endl;
  }

  if (distancia[origem][destino] == __INT_MAX__)
  {
    cout << "Não existe caminho entre os vertices" << endl;
    return;
  }

  cout << endl
       << "Menor caminho entre os verticies fornecidos" << endl;
  int i = origem;
  int j = destino;
  cout << "Caminho: " << j << " ";
  while (antecessor[i][j] != i)
  {
    if (antecessor[i][j] == __INT_MAX__)
    {
      cout << "Não existe caminho entre os vertices" << endl;
      return;
    }
    cout << antecessor[i][j] << " ";
    j = antecessor[i][j];
  }
  cout << i << endl;
  cout << "Distancia: " << distancia[origem][destino] << endl;
  for (int i = 0; i < this->numVertices; i++)
  {
    delete[] distancia[i];
    delete[] antecessor[i];
  }
  delete[] distancia;
  delete[] antecessor;
}

/*
fazer um algoritmo que utiliza a ideia de conectividade de grafos que resolva o
 o seguinte problem: Considere uma rede de distribuição de energia onde a conectividade torna-se o menor número
 de linha de transmissão entre as torres cuja falha causaria um apagão.Quantas linhas precisam
falhar(no mínimo) para termos um apagão?

*/

Grafo::~Grafo()
{
  for (int i = 0; i < numVertices; i++)
    delete[] this->mat[i];
  delete[] this->mat;
  delete[] this->pos;
}
