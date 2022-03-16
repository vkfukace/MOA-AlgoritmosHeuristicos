#include <iostream>
#include <string>
#include <vector>
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

// Retorna o número de vértices declarado no campo DIMENSION.
// Assume que a string está no formato "DIMENSION : <numVertices>".
int extrairNumVertices(string stringNumVertices)
{
    int i = stringNumVertices.find(":");
    stringNumVertices = stringNumVertices.substr(i + 1);
    return stoi(stringNumVertices);
}

// Separa a string em uma lista de tokens
// Tokens são separados por espaço na string original
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

// Inicializa a lista de vértices utilizando entradas
// da linha de comando.
void inicializar(vector<Vertice> &listaVert)
{
    string linhaEntrada;
    vector<string> listaTokens;
    int n;

    getline(cin, linhaEntrada); // NAME
    getline(cin, linhaEntrada); // TYPE
    getline(cin, linhaEntrada); // COMMENT

    getline(cin, linhaEntrada); // DIMENSION
    n = extrairNumVertices(linhaEntrada);
    cout << "\n############ " << n << " ############\n";
    listaVert.resize(n + 1);
    cout << "\n############ " << listaVert.size() << " ############\n";

    getline(cin, linhaEntrada); // EDGE_WEIGHT_TYPE
    getline(cin, linhaEntrada); // NODE_COORD_SECTION

    // Declaração dos vértices
    int i = 1;
    getline(cin, linhaEntrada);
    while (linhaEntrada != "EOF")
    {
        listaTokens = tokenizar(linhaEntrada);

        // for (int j = 0; j < listaTokens.size(); j++)
        // {
        //     cout << listaTokens[j] << " a ";
        // }
        // cout << "\n";
        // cout << linhaEntrada << " num " << i << "\n";
        // cout << "X:" << listaVert[i].x << " Y:" << listaVert[i].y << "\n";

        listaVert[i] = Vertice(stoi(listaTokens[1]), stoi(listaTokens[2]));

        i++;
        getline(cin, linhaEntrada);
        // TODO: ta tendo q apertar enter dps, talvez tem que arrumar
    }
}

int main()
{
    vector<Vertice> listaVert;
    inicializar(listaVert);

    for (int j = 0; j < listaVert.size(); j++)
    {
        cout << j << " => x: " << listaVert[j].x << " y: " << listaVert[j].y << "\n";
    }

    cout << "Inicialização dos vértices completa\n";

    return 0;
}