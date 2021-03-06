#include <iostream>
#include <string>
#include <vector>
#include <vector>
#include <cmath>
#include <ctime>
#include <limits>
#include <fstream>

#define INF_POS std::numeric_limits<float>::max();

using namespace std;

class Vertice
{
public:
    float x, y; // coordenadas do vertice

    Vertice() {}

    Vertice(float i, float j)
    {
        x = i;
        y = j;
    }
};

// Remove o elemento de índice i da listaVertices em tempo constante.
// Não preserva a ordem original dos elementos
void remover(int i, vector<int> &listaVertices)
{
    int ultimoIdx = listaVertices.size() - 1;
    int aux = listaVertices[i];
    listaVertices[i] = listaVertices[ultimoIdx];
    listaVertices[ultimoIdx] = aux;
    listaVertices.pop_back();
}

class PCVSolver
{
private:
    vector<Vertice> vertices;
    int numVertices;
    vector<int> melhorCaminho;
    float melhorDistancia;

public:
    // Inicializa a classe com a lista e a quantidade de vértices
    PCVSolver(vector<Vertice> listaVertices, int n)
    {
        vertices = listaVertices;
        numVertices = n;
    }

    // Retorna a distância entre os vértices v1 e v2.
    float distancia(Vertice v1, Vertice v2)
    {
        float deltaX = v2.x - v1.x;
        float deltaY = v2.y - v1.y;

        return sqrt((deltaX * deltaX) + (deltaY * deltaY));
    }

    // Retorna a distância percorrida pelo caminho inteiro.
    float distanciaCaminho(vector<int> caminho)
    {
        int i;
        float distanciaTotal = 0;
        for (i = 0; i < (int)caminho.size() - 1; i++)
        {
            distanciaTotal += distancia(vertices[caminho[i]], vertices[caminho[i + 1]]);
        }
        distanciaTotal += distancia(vertices[caminho[i]], vertices[caminho[0]]);
        return distanciaTotal;
    }

    // Imprime todos os vértices do melhorCaminho
    void printCaminho()
    {
        for (int j = 0; j < melhorCaminho.size(); j++)
        {
            cout << melhorCaminho[j] << " => ";
        }
        cout << melhorCaminho[0] << endl;
    }

    // Imprime as coordenadas de todos os vértices na tela
    void printVertices()
    {
        for (int j = 1; j < numVertices + 1; j++)
        {
            cout << j << " => x: " << vertices[j].x << " y: " << vertices[j].y << "\n";
        }
    }

    float calculoCustoInsercao(vector<int> &ciclo, int i, int j, int k)
    {
        float d_ik = distancia(vertices[i], vertices[k]);
        float d_kj = distancia(vertices[k], vertices[j]);
        float d_ij = distancia(vertices[i], vertices[j]);
        return d_ik + d_kj - d_ij;
    }

    // Aplica o algoritmo random insertion para o PCV.
    float solveRandomInsertion()
    {
        // Inicializa a lista de vértices disponíveis.
        vector<int> idxVerticesDisponiveis;
        for (int i = 1; i <= numVertices; i++)
            idxVerticesDisponiveis.push_back(i);
        int numVerticesDisponiveis = (int)idxVerticesDisponiveis.size();

        // Inicializa o ciclo vazio.
        // proxCiclo funciona de forma diferente de melhorCaminho.
        // No melhorCaminho, o elemento após melhorCaminho[i] é melhorCaminho[i+1],
        // no proxCiclo, o elemento após proxCiclo[i] é proxCiclo[proxCiclo[i]].
        vector<int> proxCiclo(numVertices + 1, -1);

        // Escolhe vértice inicial aleatório.
        srand(time(NULL));
        int idxRandom = rand() % numVerticesDisponiveis;
        int verticeInicial = idxVerticesDisponiveis[idxRandom];
        remover(idxRandom, idxVerticesDisponiveis);
        numVerticesDisponiveis--;

        // Define um ciclo inicial com um único vértice.
        proxCiclo[verticeInicial] = verticeInicial;
        int tamanhoCiclo = 1;

        while (numVerticesDisponiveis > 0)
        {
            // Escolhe outro vértice aleatório, chamado k.
            idxRandom = rand() % numVerticesDisponiveis;
            int k = idxVerticesDisponiveis[idxRandom];
            numVerticesDisponiveis--;
            remover(idxRandom, idxVerticesDisponiveis);

            int i = verticeInicial, j, iInsercao, jInsercao;
            float melhorCustoInsercao = INF_POS;
            // Busca todas as arestas (i, j) do ciclo, e escolhe aquela com o menor custo de inserção de k.
            for (int cont = 0; cont < tamanhoCiclo; cont++)
            {
                j = proxCiclo[i];
                float custoInsercao = calculoCustoInsercao(proxCiclo, i, j, k);
                if (custoInsercao < melhorCustoInsercao)
                {
                    iInsercao = i;
                    jInsercao = j;
                    melhorCustoInsercao = custoInsercao;
                }
                i = j;
            }
            proxCiclo[k] = jInsercao;
            proxCiclo[iInsercao] = k;
            tamanhoCiclo++;
        }

        // Transfere o ciclo para o melhorCaminho, e calcula a distância
        int i = verticeInicial;
        for (int cont = 0; cont < tamanhoCiclo; cont++)
        {
            melhorCaminho.push_back(i);
            i = proxCiclo[i];
        }
        melhorDistancia = distanciaCaminho(melhorCaminho);
        return melhorDistancia;
    }
};

// Separa a string em uma lista de tokens
// Tokens são trechos da string original separados por um espaço
vector<string> tokenizar(string str)
{
    vector<string> listaTokens;
    int i;
    string token = "";
    for (i = 0; i < (int)str.length(); i++)
    {
        if (str[i] == ' ')
        {
            if (!token.empty())
            {
                listaTokens.push_back(token);
                token = "";
            }
        }
        else
        {
            token = token + str[i];
        }
    }
    if (!token.empty())
    {
        listaTokens.push_back(token);
    }

    return listaTokens;
}

// Inicializa a lista de vértices utilizando linhas de entrada
// da linha de comando.
// Assume que a entrada está formatada de acordo com os exemplos.
// Inicializa a posição 0 da linha de vértices com um vértice vazio.
void inicializarPorTerminal(vector<Vertice> &listaVertices, int &tamanhoLista)
{
    string linhaEntrada;
    vector<string> listaTokens;

    for (int i = 0; i < 6; i++)
    {
        getline(cin, linhaEntrada);
    }

    // Declaração dos vértices
    // Primeiro vértice é vazio
    listaVertices.push_back(Vertice());
    tamanhoLista = 0;
    getline(cin, linhaEntrada);
    // Enquanto não achar um "EOF"
    while (linhaEntrada.find("EOF") == string::npos)
    {
        listaTokens = tokenizar(linhaEntrada);
        listaVertices.push_back(Vertice(stof(listaTokens[1]), stof(listaTokens[2])));
        tamanhoLista++;
        getline(cin, linhaEntrada);
    }
}

// Inicializa a lista de vértices utilizando um arquivo
// Assume que a entrada está formatada de acordo com os exemplos.
// Inicializa a posição 0 da linha de vértices com um vértice vazio.
// Retorna true caso o arquivo exista, false caso contrário
bool inicializarPorArquivo(string nomeArquivo, vector<Vertice> &listaVertices, int &tamanhoLista)
{
    ifstream arq(nomeArquivo);

    if (!arq.good())
        return false;

    string linhaEntrada;
    vector<string> listaTokens;

    for (int i = 0; i < 6; i++)
    {
        getline(arq, linhaEntrada);
    }

    // Declaração dos vértices
    // Primeiro vértice é vazio
    listaVertices.push_back(Vertice());
    tamanhoLista = 0;
    // Enquanto não for o fim do arquivo e não achar um "EOF"
    while (getline(arq, linhaEntrada) && linhaEntrada.find("EOF") == string::npos)
    {
        listaTokens = tokenizar(linhaEntrada);
        listaVertices.push_back(Vertice(stof(listaTokens[1]), stof(listaTokens[2])));
        tamanhoLista++;
    }

    return true;
}

int main(int argc, char **argv)
{
    vector<Vertice> listaVertices;
    int tamanhoLista;

    if (argc > 1)
    {
        if (inicializarPorArquivo(argv[1], listaVertices, tamanhoLista))
        {
            cout << "Inicialização completa" << endl;
        }
        else
        {
            cout << "Erro na leitura do arquivo" << endl;
            return 0;
        }
    }
    else
    {
        cout << "Insira o caso de teste" << endl;
        inicializarPorTerminal(listaVertices, tamanhoLista);
    }

    PCVSolver pcvSolver(listaVertices, tamanhoLista);

    float resultado;
    clock_t tempoInicial = clock();
    cout << "Executando..." << endl;

    resultado = pcvSolver.solveRandomInsertion();

    clock_t tempoFinal = clock();
    float tempoTotal = (tempoFinal - tempoInicial) / (float)CLOCKS_PER_SEC;
    cout << "Execução finalizada" << endl;
    cout << "Tempo: " << tempoTotal << "s" << endl;
    cout << "Resultado: ";
    cout << resultado << "\n";

    return 0;
}