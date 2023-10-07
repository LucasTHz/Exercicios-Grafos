#include "Lista.h"
#include "FPHeapMinIndireto.h"
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
    bool operator<(const Aresta &p) const
    {
      return peso < p.peso;
    }
    ~Aresta() {}
  };

private:
  class Celula
  {
    friend class Grafo;
    friend ostream &operator<<(ostream &out, const Celula &celula)
    {
      out << "vertice:" << celula.vertice << endl;
      out << "peso:" << celula.peso << endl;
      return out;
    }

  private:
    int vertice, peso;

  public:
    Celula(int v, int p)
    {
      this->vertice = v;
      this->peso = p;
    }
    Celula(const Celula &cel) { *this = cel; }
    bool operator==(const Celula &celula) const
    {
      return this->vertice == celula.vertice;
    }
    bool operator!=(const Celula &celula) const
    {
      return this->vertice != celula.vertice;
    }
    const Celula &operator=(const Celula &cel)
    {
      this->vertice = cel.vertice;
      this->peso = cel.peso;
      return *this; // @{\it permite atribui\c{c}\~oes encadeadas}@
    }
    ~Celula() {}
  };
  Lista<Celula> *adj;
  int numVertices;

public:
  Grafo(istream &in);
  Grafo(int numVertices);
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
  void buscaProfundidade();
  void visitaDfs(int u, int *cor, int *antecessor);
  void buscaProfundidadeAciclico();
  bool aciclico(int u, int *cor, int *antecessor);
  int numComponentes();
  vector<int> ordemTopologica();
  void visitaDfsComOrdem(int u, int *cor, int *antecessor, vector<int> &ordem);
  void visitaBfs(int u, int *cor, int *dist, int *antecessor);
  void buscaProLargura();
  void buscaMenorCaminho(int u, int v);
  void imprimeCaminho(int u, int v, int *antecessor);
  void kruskal();
  int encontreConjunto(int *conjunto, int x);
  void unirConjunto(int *conjunto, int x, int y);

  ~Grafo();
};

Grafo::Grafo(istream &in)
{
  int v1, v2, peso, numVertices;
  in >> numVertices;

  this->adj = new Lista<Celula>[numVertices];
  this->numVertices = numVertices;

  while (in >> v1 >> v2 >> peso)
  {
    Grafo::Aresta *a = new Grafo::Aresta(v1, v2, peso);
    this->insereAresta(a->_v1(), a->_v2(), a->_peso());
    delete a;
  }
}

Grafo::Grafo(int numVertices)
{
  this->adj = new Lista<Celula>[numVertices];
  this->numVertices = numVertices;
}
Grafo::Grafo(int numVertices, int numArestas)
{
  this->adj = new Lista<Celula>[numVertices];
  this->numVertices = numVertices;
}

Grafo::Aresta *lerAresta()
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
  return new Grafo::Aresta(v1, v2, peso);
}

void Grafo::insereAresta(int v1, int v2, int peso)
{
  Celula item(v2, peso);
  this->adj[v1].insere(item);
}
bool Grafo::existeAresta(int v1, int v2) const
{
  Celula item(v2, 0);
  return (this->adj[v1].pesquisa(item) != NULL);
}
bool Grafo::listaAdjVazia(int v) const
{
  return this->adj[v].vazia();
}
Grafo::Aresta *Grafo::primeiroListaAdj(int v)
{
  // @{\it Retorna a primeira aresta que o v\'ertice v participa ou}@
  // @{\it {\bf NULL} se a lista de adjac\^encia de v for vazia}@
  Celula *item = this->adj[v]._primeiro();
  return item != NULL ? new Aresta(v, item->vertice, item->peso) : NULL;
}
Grafo::Aresta *Grafo::proxAdj(int v)
{
  // @{\it Retorna a pr\'oxima aresta que o v\'ertice v participa ou}@
  // @{\it {\bf NULL} se a lista de adjac\^encia de v estiver no fim}@
  Celula *item = this->adj[v].proximo();
  return item != NULL ? new Aresta(v, item->vertice, item->peso) : NULL;
}
Grafo::Aresta *Grafo::retiraAresta(int v1, int v2)
{
  Celula chave(v2, 0);
  Celula *item = this->adj[v1].retira(chave);
  Aresta *aresta = item != NULL ? new Aresta(v1, v2, item->peso) : NULL;
  delete item;
  return aresta;
}
void Grafo::imprime() const
{
  for (int i = 0; i < this->numVertices; i++)
  {
    cout << i << ":";
    Celula *item = this->adj[i]._primeiro();
    while (item != NULL)
    {
      cout << "  " << item->vertice << " (" << item->peso << ")";
      item = this->adj[i].proximo();
    }
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

void Grafo::buscaProfundidade()
{
  int *cor = new int[this->numVertices];
  int *antecessor = new int[this->numVertices];

  for (int u = 0; u < this->numVertices; u++)
  {
    cor[u] = 0;
    antecessor[u] = -1;
  }
  for (int u = 0; u < this->numVertices; u++)
  {
    if (cor[u] == 0) // o 0 é branco
    {
      this->visitaDfs(u, cor, antecessor);
    }
  }
}

int Grafo::numComponentes()
{
  int *cor = new int[this->numVertices];
  int *antecessor = new int[this->numVertices];
  int componentes = 0;

  for (int u = 0; u < this->numVertices; u++)
  {
    cor[u] = 0;
    antecessor[u] = -1;
  }
  for (int u = 0; u < this->numVertices; u++)
  {
    if (cor[u] == 0) // o 0 é branco
    {
      componentes++;
      this->visitaDfs(u, cor, antecessor);
    }
  }
  return componentes;
}

void Grafo::visitaDfs(int u, int *cor, int *antecessor)
{
  cor[u] = 1; // o 1 é cinza
  cout << "cinza:" << u << endl;
  if (!this->listaAdjVazia(u))
  {
    Aresta *adj = this->primeiroListaAdj(u);
    while (adj != NULL)
    {
      int v = adj->_v2();
      if (cor[v] == 0)
      {
        antecessor[v] = u;
        this->visitaDfs(v, cor, antecessor);
      }
      delete adj;
      adj = this->proxAdj(u);
    }
    cout << "preto:" << u << endl;

    cor[u] = 2; // o 2 é preto
  }
}

bool Grafo::aciclico(int u, int *cor, int *antecessor)
{
  cor[u] = 1; // o 1 é cinza
  cout << "cinza:" << u << endl;
  if (!this->listaAdjVazia(u))
  {
    Aresta *adj = this->primeiroListaAdj(u);
    while (adj != NULL)
    {
      int v = adj->_v2();
      if (cor[v] == 1)
        return false;
      if (cor[v] == 0)
      {
        antecessor[v] = u;
        this->visitaDfs(v, cor, antecessor);
      }
      delete adj;
      adj = this->proxAdj(u);
    }
    cout << "preto:" << u << endl;

    cor[u] = 2; // o 2 é preto
  }
}

void Grafo::buscaProfundidadeAciclico()
{
  int *cor = new int[this->numVertices];
  int *antecessor = new int[this->numVertices];

  for (int u = 0; u < this->numVertices; u++)
  {
    cor[u] = 0;
    antecessor[u] = -1;
  }
  for (int u = 0; u < this->numVertices; u++)
  {
    if (cor[u] == 0) // o 0 é branco
    {
      this->aciclico(u, cor, antecessor);
    }
  }
}

vector<int> Grafo::ordemTopologica()
{
  int *cor = new int[this->numVertices];
  int *antecessor = new int[this->numVertices];
  vector<int> ordem;

  for (int u = 0; u < this->numVertices; u++)
  {
    cor[u] = 0;
    antecessor[u] = -1;
  }
  for (int u = 0; u < this->numVertices; u++)
  {
    if (cor[u] == 0) // o 0 é branco
    {
      this->visitaDfsComOrdem(u, cor, antecessor, ordem);
    }
  }
  return ordem;
}

void Grafo::visitaDfsComOrdem(int u, int *cor, int *antecessor, vector<int> &ordem)
{
  cor[u] = 1; // o 1 é cinza
  cout << "cinza:" << u << endl;
  if (!this->listaAdjVazia(u))
  {
    Aresta *adj = this->primeiroListaAdj(u);
    while (adj != NULL)
    {
      int v = adj->_v2();
      if (cor[v] == 0)
      {
        antecessor[v] = u;
        this->visitaDfsComOrdem(v, cor, antecessor, ordem);
      }
      delete adj;
      adj = this->proxAdj(u);
    }
    cout << "preto:" << u << endl;

    cor[u] = 2; // o 2 é preto
    ordem.push_back(u);
  }
}

void Grafo::visitaBfs(int u, int *cor, int *dist, int *antecessor)
{
  queue<int> fila;
  fila.push(u);

  while (!fila.empty())
  {
    int u = fila.front();
    fila.pop();
    if (!this->listaAdjVazia(u))
    {
      Aresta *adj = this->primeiroListaAdj(u);
      while (adj != NULL) // para cada v adjacente a u
      {
        int v = adj->_v2();
        if (cor[v] == 0)
        {
          cout << "Visitou: " << v << endl;
          cout << "Mudou para cinza: " << endl;
          cor[v] = 1;
          dist[v] = dist[u] + 1;
          antecessor[v] = u;
          fila.push(v);
        }
        delete adj;
        adj = this->proxAdj(u);
      }

      cor[u] = 2; // o 2 é preto
      cout << "Mudou para preto: " << endl;
    }
  }
}

void Grafo::buscaProLargura()
{
  int *cor = new int[this->numVertices];
  int *antecessor = new int[this->numVertices];
  int *dist = new int[this->numVertices];

  for (int u = 0; u < this->numVertices; u++)
  {
    cor[u] = 0;
    antecessor[u] = -1;
    dist[u] = __INT_MAX__;
  }
  for (int u = 0; u < this->numVertices; u++)
  {
    if (cor[u] == 0) // o 0 é branco
    {
      this->visitaBfs(u, cor, dist, antecessor);
    }
  }
}

void Grafo::imprimeCaminho(int u, int v, int *antecessor)
{

  if (u == v)
    cout << v << endl;
  else if (antecessor[v] == -1)
  {

    cout << "Não exite caminho" << endl;
  }
  else
  {
    imprimeCaminho(u, antecessor[v], antecessor);
    cout << v << endl;
  }
}
void Grafo::buscaMenorCaminho(int u, int v)
{
  int *cor = new int[this->numVertices];
  int *antecessor = new int[this->numVertices];
  int *dist = new int[this->numVertices];

  for (int u = 0; u < this->numVertices; u++)
  {
    cor[u] = 0;
    antecessor[u] = -1;
    dist[u] = __INT_MAX__;
  }

  this->visitaBfs(u, cor, dist, antecessor);
  this->imprimeCaminho(u, v, antecessor);
}

void Grafo::kruskal()
{
  vector<Aresta> S;
  int *conjunto = new int[this->_numVertices()];

  // criar conjunto
  memset(conjunto, -1, sizeof(int) * this->_numVertices());

  vector<Aresta> A;

  for (int v = 0; v < this->numVertices; v++)
  {
    if (!this->listaAdjVazia(v))
    {
      Aresta *adj = this->primeiroListaAdj(v);
      while (adj != NULL)
      {
        A.push_back(*adj);
        delete adj;
        adj = this->proxAdj(v);
      }
    }
  }
  // printf("Conjunto: %d\n", this->_numVertices());
  sort(A.begin(), A.end());

  for (int i = 0; i < this->numVertices; i++)
  {
    if (encontreConjunto(conjunto, A[i]._v1()) != encontreConjunto(conjunto, A[i]._v2()))
    {

      S.push_back(A[i]);
      unirConjunto(conjunto, A[i]._v1(), A[i]._v2());
    }
  }

  for (int i = 0; i < S.size(); i++)
  {
    cout << S[i]._v1() << " " << S[i]._v2() << " " << S[i]._peso() << endl;
  }
}

int Grafo::encontreConjunto(int *conjunto, int x)
{
  if (conjunto[x] == -1)
    return x;
  return encontreConjunto(conjunto, conjunto[x]);
}

void Grafo::unirConjunto(int *conjunto, int x, int y)
{
  int conjuntoX = encontreConjunto(conjunto, x);
  int conjuntoY = encontreConjunto(conjunto, y);
  conjunto[conjuntoX] = conjuntoY;
}

Grafo::~Grafo()
{
  delete[] this->adj;
}
