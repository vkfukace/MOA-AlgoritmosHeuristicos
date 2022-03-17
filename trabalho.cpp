#include <iostream>
#include <string>
#include <vector>
#include <cmath>

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
// Tokens são separados por um espaço na string original
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
        // cout << token << " : " << i << "\n";
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

class PCVSolver
{
public:
    vector<Vertice> vertices;
    int numVertices;
    vector<int> caminho;

    // Inicializa a classe com a lista e a quantidade de vértices
    PCVSolver(vector<Vertice> listaVertices, int n) {
        vertices = listaVertices;
        numVertices = n;
        caminho.resize(numVertices);
    }

    // Retorna a distância entre os vértices v1 e v2.
    float distancia(int v1, int v2){
        int deltaX = vertices[v2].x - vertices[v1].x;
        int deltaY = vertices[v2].y - vertices[v1].y;

        return sqrt((deltaX * deltaX) + (deltaY * deltaY));
    }

    // Imprime as coordenadas de todos os vértices na tela
    void printVertices(){
        for (int j = 1; j < numVertices + 1; j++)
        {
            cout << j << " => x: " << vertices[j].x << " y: " << vertices[j].y << "\n";
        }
    }
};

// Inicializa a lista de vértices utilizando linhas de entrada
// da linha de comando.
// Assume que a entrada está formatada de acordo com os exemplos.
void inicializar(vector<Vertice> &listaVertices, int &numVertices)
{
    string linhaEntrada;
    vector<string> listaTokens;

    getline(cin, linhaEntrada); // NAME
    getline(cin, linhaEntrada); // TYPE
    getline(cin, linhaEntrada); // COMMENT

    getline(cin, linhaEntrada); // DIMENSION
    numVertices = extrairNumVertices(linhaEntrada);
    cout << "\n############ " << numVertices << " ############\n";
    listaVertices.resize(numVertices + 1);
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
    int numVertices;
    inicializar(listaVertices, numVertices);
    PCVSolver pcvSolver(listaVertices, numVertices);

    pcvSolver.printVertices();

    return 0;
}