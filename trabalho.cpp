#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>

#define INF_POS std::numeric_limits<float>::max();

using namespace std;

class Vertice
{
public:
    int x, y; // coordenadas do vertice

    Vertice() {}

    Vertice(int i, int j)
    {
        x = i;
        y = j;
    }
};

// Separa a string em uma lista de tokens
// Tokens são trechos da string original separados por um espaço
vector<string> tokenizar(string str)
{
    vector<string> listaTokens;
    int i;
    string token = "";
    for (i = 0; i < str.length(); i++)
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

// Retorna o número de vértices declarado no campo DIMENSION.
// Assume que a string está no formato "DIMENSION : <numVertices>".
int extrairNumVertices(string stringNumVertices)
{
    int i = stringNumVertices.find(":");
    stringNumVertices = stringNumVertices.substr(i + 1);
    return stoi(stringNumVertices);
}

// Remove o elemento de índice i da listaVertices em tempo constante.
// Não preserva a ordem original dos elementos
void remover(int i, deque<int> &listaVertices)
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
    vector<int> caminho;
    float resultado;

public:
    // Inicializa a classe com a lista e a quantidade de vértices
    PCVSolver(vector<Vertice> listaVertices, int n)
    {
        vertices = listaVertices;
        numVertices = n;
    }

    // Retorna a distância entre os vértices v1 e v2.
    float distancia(int idxV1, int idxV2)
    {
        int deltaX = vertices[idxV2].x - vertices[idxV1].x;
        int deltaY = vertices[idxV2].y - vertices[idxV1].y;

        return sqrt((deltaX * deltaX) + (deltaY * deltaY));
    }

    // Retorna a distância percorrida pelo caminho inteiro.
    float distanciaCaminho(vector<int> caminho)
    {
        int i;
        float distanciaTotal = 0;
        for (i = 0; i < caminho.size() - 1; i++)
        {
            distanciaTotal += distancia(caminho[i], caminho[i + 1]);
        }
        distanciaTotal += distancia(caminho[i], caminho[0]);
        return distanciaTotal;
    }

    // Imprime as coordenadas de todos os vértices na tela
    void printVertices()
    {
        for (int j = 1; j < numVertices + 1; j++)
        {
            cout << j << " => x: " << vertices[j].x << " y: " << vertices[j].y << "\n";
        }
    }

    // Aplica o algoritmo do vizinho mais próximo para o PCV.
    // Inicia a execução no vértice dado por verticeInicial.
    float solveVizinhoMaisProximo(int verticeInicial)
    {
        resultado = 0;
        // Inicializa a lista de vértices disponíveis
        deque<int> idxVerticesDisponiveis;
        for (int i = 1; i <= numVertices; i++)
        {
            idxVerticesDisponiveis.push_back(i);
        }

        int idxVerticeAtual = verticeInicial;
        remover(idxVerticeAtual - 1, idxVerticesDisponiveis);
        while (idxVerticesDisponiveis.size() > 0)
        {
            caminho.push_back(idxVerticeAtual);
            // valorMenorCaminho inicialmente possui o maior valor de float possível
            float valorMenorCaminho = INF_POS;
            int idxMenorCaminho, idxRemover;
            float distanciaCalc;

            // Passa por todos os vértices disponíveis e seleciona o mais próximo do verticeAtual
            for (int j = 0; j < idxVerticesDisponiveis.size(); j++)
            {
                distanciaCalc = distancia(idxVerticeAtual, idxVerticesDisponiveis.at(j));
                if (distanciaCalc < valorMenorCaminho)
                {
                    valorMenorCaminho = distanciaCalc;
                    idxMenorCaminho = idxVerticesDisponiveis.at(j);
                    idxRemover = j;
                }
            }
            resultado += valorMenorCaminho;
            // cout << "# resultado:" << resultado << " # valor:" << valorMenorCaminho << " # idx1:" << idxVerticeAtual << " # idx2:" << idxMenorCaminho << "\n";
            // cout << "# vertices sobrando:" << idxVerticesDisponiveis.size() << "\n\n";

            idxVerticeAtual = idxMenorCaminho;
            remover(idxRemover, idxVerticesDisponiveis);
        }
        caminho.push_back(idxVerticeAtual);
        resultado += distancia(idxVerticeAtual, caminho[0]);

        return resultado;
    }

    // Aplica o algoritmo 2-opt para o PCV.
    float solve2Opt(int verticeInicial)
    {
        // https://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems/
    }
};

// Inicializa a lista de vértices utilizando linhas de entrada
// da linha de comando.
// Assume que a entrada está formatada de acordo com os exemplos.
void inicializar(vector<Vertice> &listaVertices, int &tamanhoLista)
{
    string linhaEntrada;
    vector<string> listaTokens;

    getline(cin, linhaEntrada); // NAME
    getline(cin, linhaEntrada); // TYPE
    getline(cin, linhaEntrada); // COMMENT

    getline(cin, linhaEntrada); // DIMENSION
    tamanhoLista = extrairNumVertices(linhaEntrada);
    cout << "\n############ " << tamanhoLista << " ############\n";
    listaVertices.resize(tamanhoLista + 1);
    cout << "\n############ " << listaVertices.size() << " ############\n";

    getline(cin, linhaEntrada); // EDGE_WEIGHT_TYPE
    getline(cin, linhaEntrada); // NODE_COORD_SECTION

    // Declaração dos vértices
    int i = 1;
    getline(cin, linhaEntrada);
    while (linhaEntrada != "EOF")
    {
        listaTokens = tokenizar(linhaEntrada);
        listaVertices[i] = Vertice(stoi(listaTokens[1]), stoi(listaTokens[2]));
        i++;
        getline(cin, linhaEntrada);
    }
}

int main()
{
    vector<Vertice> listaVertices;
    int tamanhoLista;
    inicializar(listaVertices, tamanhoLista);
    cout << "\n############ inicio ############\n";
    PCVSolver pcvSolver(listaVertices, tamanhoLista);

    float resultado = pcvSolver.solveVizinhoMaisProximo(1);
    cout << "\n# resultado:" << resultado << "\n";

    return 0;
}