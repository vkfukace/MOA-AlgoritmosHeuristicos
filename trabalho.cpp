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

    // Gera um caminho inicial, começando no vértice 1 e passando sequencialmente
    // pelos vértices 2, 3, 4, ..., até o último.
    // Retorna a distância total do caminho.
    vector<int> gerarCaminhoInicialSequencial()
    {
        vector<int> caminho;
        int i;
        for (i = 1; i <= numVertices; i++)
        {
            caminho.push_back(i);
        }
        return caminho;
    }

    // Imprime as coordenadas de todos os vértices na tela
    void printVertices()
    {
        for (int j = 1; j < numVertices + 1; j++)
        {
            cout << j << " => x: " << vertices[j].x << " y: " << vertices[j].y << "\n";
        }
    }

    // Retorna o índice do vizinho mais próximo de verticeAtual
    // Retorna também o índice deste vizinho na lista de vértices disponíveis e o tamanho da aresta formada;
    int vizinhoMaisProximo(int verticeAtual, deque<int> idxVerticesDisponiveis, float &tamanhoMenorAresta, int &idxListaDisponiveis){
        float distanciaCalc;
        int idxMaisProximo;

        // tamanhoMenorAresta inicialmente possui o maior valor de float possível
        tamanhoMenorAresta = INF_POS;
        for (int i = 0; i < (int)idxVerticesDisponiveis.size(); i++)
        {
            distanciaCalc = distancia(vertices[verticeAtual], vertices[idxVerticesDisponiveis.at(i)]);
            if (distanciaCalc < tamanhoMenorAresta)
            {
                tamanhoMenorAresta = distanciaCalc;
                idxMaisProximo = idxVerticesDisponiveis.at(i);
                idxListaDisponiveis = i;
            }
        }
        return idxMaisProximo;
    }

    // Aplica o algoritmo do vizinho mais próximo para o PCV.
    float solveVizinhoMaisProximo(int verticeInicial)
    {
        // Inicializa a lista de vértices disponíveis
        deque<int> idxVerticesDisponiveis;
        for (int i = 1; i <= numVertices; i++)
        {
            idxVerticesDisponiveis.push_back(i);
        }

        float tamanhoMenorAresta;
        int idxMaisProximo, idxRemover;
        melhorDistancia = 0;

        int idxVerticeAtual = verticeInicial;
        remover(idxVerticeAtual - 1, idxVerticesDisponiveis);
        while (idxVerticesDisponiveis.size() > 0)
        {
            melhorCaminho.push_back(idxVerticeAtual);
            // Seleciona o vizinho mais próximo
            idxMaisProximo = vizinhoMaisProximo(idxVerticeAtual, idxVerticesDisponiveis, tamanhoMenorAresta, idxRemover);
            melhorDistancia += tamanhoMenorAresta;
            // cout << "# melhorDistancia:" << melhorDistancia << " # valor:" << tamanhoMenorAresta << " # idx1:" << idxVerticeAtual << " # idx2:" << idxMaisProximo << "\n";
            // cout << "# vertices sobrando:" << idxVerticesDisponiveis.size() << "\n\n";

            idxVerticeAtual = idxMaisProximo;
            remover(idxRemover, idxVerticesDisponiveis);
        }
        melhorCaminho.push_back(idxVerticeAtual);
        // cout << "final x1:" << vertices[idxVerticeAtual].x << " y1:" << vertices[idxVerticeAtual].y << endl;
        // cout << "final x2:" << vertices[melhorCaminho[0]].x << " y2:" << vertices[melhorCaminho[0]].y << endl;
        melhorDistancia += distancia(vertices[idxVerticeAtual], vertices[melhorCaminho[0]]);
        // cout << "# melhorDistancia:" << melhorDistancia << " # valor:" << tamanhoMenorAresta << " # idx1:" << idxVerticeAtual << " # idx2:" << melhorCaminho[0] << "\n";

        return melhorDistancia;
    }

    // Realiza a troca 2-opt no caminho dado.
    vector<int> trocarDoisOpt(vector<int> caminho, int i, int j)
    {
        vector<int> novoCaminho = caminho;
        while (i < j)
        {
            int aux = novoCaminho[i];
            novoCaminho[i] = novoCaminho[j];
            novoCaminho[j] = aux;
            i++;
            j--;
        }
        return novoCaminho;
    }

    // Aplica o algoritmo 2-opt para o PCV.
    float solve2Opt()
    {
        melhorCaminho = gerarCaminhoInicialSequencial();
        vector<int> novoCaminho;
        float novaDistancia;
        int contadorSemMelhora = 0, maximoSemMelhora = 3;

        // Cada vez que o 2-opt for executado sem melhorar a distância do caminho, 
        // contadorSemMelhora é incrementado.
        // maximoSemMelhora indica o número máximo de vezes seguidas que o 2-opt 
        // pode ser executado sem melhorar o resultado.
        while (contadorSemMelhora < maximoSemMelhora)
        {
            melhorDistancia = distanciaCaminho(melhorCaminho);
            contadorSemMelhora++;
            for (int i = 0; i < numVertices; i++)
            {
                for (int j = i + 1; j < numVertices; j++)
                {
                    novoCaminho = trocarDoisOpt(melhorCaminho, i, j);
                    novaDistancia = distanciaCaminho(novoCaminho);
                    if (novaDistancia < melhorDistancia)
                    {
                        contadorSemMelhora = 0;
                        melhorCaminho = novoCaminho;
                        melhorDistancia = novaDistancia;
                    }
                }
            }
        }
        return melhorDistancia;
    }

    float calculoCustoInsercao(vector<int> ciclo, int i, int j, int k){
        float d_ik = distancia(vertices[i], vertices[k]);
        float d_kj = distancia(vertices[k], vertices[j]);
        float d_ij = distancia(vertices[i], vertices[j]);
        return d_ik + d_kj - d_ij;
    }

    // Aplica o algoritmo nearest insertion para o PCV.
    // Inicia a execução no vértice dado por verticeInicial.
    float solveRandomInsertion()
    {
        // Inicializa a lista de vértices disponíveis
        deque<int> idxVerticesDisponiveis;
        for (int i = 1; i <= numVertices; i++)
            idxVerticesDisponiveis.push_back(i);
        // Inicializa o ciclo com todos os elementos contendo -1
        vector<int> proxCiclo(numVertices + 1, -1);

        // Escolhe vértice aleatório
        srand(time(NULL));
        int numVerticesDisponiveis = (int)idxVerticesDisponiveis.size();
        int idxRandom = rand() % numVerticesDisponiveis;
        int verticeEscolhido = idxVerticesDisponiveis[idxRandom];
        remover(idxRandom, idxVerticesDisponiveis);
        numVerticesDisponiveis--;
        proxCiclo[verticeEscolhido] = verticeEscolhido;
        int verticeInicial = verticeEscolhido, tamanhoCiclo = 1;

        while(numVerticesDisponiveis > 0){
            idxRandom = rand() % numVerticesDisponiveis;
            verticeEscolhido = idxVerticesDisponiveis[idxRandom];
            numVerticesDisponiveis--;
            remover(idxRandom, idxVerticesDisponiveis);
            int i = verticeInicial, j, k = verticeEscolhido, iInsercao, jInsercao;
            float melhorCustoInsercao = INF_POS;
            // Busca o melhor custo de insercao de k em um vértice i, j do ciclo
            for(int cont = 0; cont < tamanhoCiclo; cont++){
                j = proxCiclo[i];
                float custoInsercao = calculoCustoInsercao(proxCiclo, i, j, k);
                if(custoInsercao < melhorCustoInsercao){
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

        int i = verticeInicial;
        for(int cont = 0; cont < tamanhoCiclo; cont++){
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

// Retorna o número de vértices declarado no campo DIMENSION.
// Assume que a string está no formato "DIMENSION : <numVertices>".
int extrairNumVertices(string stringNumVertices)
{
    int i = stringNumVertices.find(":");
    stringNumVertices = stringNumVertices.substr(i + 1);
    return stoi(stringNumVertices);
}


// Inicializa a lista de vértices utilizando linhas de entrada
// da linha de comando.
// Assume que a entrada está formatada de acordo com os exemplos.
// Inicializa a posição 0 da linha de vértices com um vértice vazio.
void inicializar(vector<Vertice> &listaVertices, int &tamanhoLista)
{
    string linhaEntrada;
    vector<string> listaTokens;

    getline(cin, linhaEntrada); // NAME
    getline(cin, linhaEntrada); // TYPE
    getline(cin, linhaEntrada); // COMMENT

    getline(cin, linhaEntrada); // DIMENSION
    // tamanhoLista = extrairNumVertices(linhaEntrada);
    // listaVertices.resize(tamanhoLista + 1);

    getline(cin, linhaEntrada); // EDGE_WEIGHT_TYPE
    getline(cin, linhaEntrada); // NODE_COORD_SECTION

    // Declaração dos vértices
    // Primeiro vértice é vazio
    listaVertices.push_back(Vertice());
    tamanhoLista = 0;
    getline(cin, linhaEntrada);
    while (linhaEntrada.find("EOF") == string::npos)
    {
        listaTokens = tokenizar(linhaEntrada);
        listaVertices.push_back(Vertice(stof(listaTokens[1]), stof(listaTokens[2])));
        tamanhoLista++;
        getline(cin, linhaEntrada);
    }
}

int main()
{
    vector<Vertice> listaVertices;
    int tamanhoLista;
    inicializar(listaVertices, tamanhoLista);
    PCVSolver pcvSolver(listaVertices, tamanhoLista);

    float resultado;
    // resultado = pcvSolver.solveVizinhoMaisProximo(1);
    // resultado = pcvSolver.solve2Opt();
    resultado = pcvSolver.solveRandomInsertion();
    cout << resultado << "\n";

    return 0;
}