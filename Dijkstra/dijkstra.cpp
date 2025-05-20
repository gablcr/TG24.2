#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <fstream>

using namespace std;

int infinite = 1000000;

string dijkstra(vector<vector<pair<int, int>>> &adj, int s) {
    int n = adj.size();
    vector<int> dist(n, infinite); // vetor de distâncias
    dist[s] = 0; // distância da origem é 0
    vector<bool> visited(n, false); // vetor de visitados

    // priority_queue de pares (distância, vértice)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, s}); // inserir o vértice de origem com distância 0

    while (!pq.empty()) {
        int u = pq.top().second; // extrai o vértice com a menor distância
        pq.pop();

        if (visited[u]) continue; // ignora se já foi visitado
        visited[u] = true;

        // relaxar as arestas do vértice u
        for (auto &edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;

            // relaxamento
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v}); // inserir na fila com a nova distância
            }
        }
    }

    // Construir string com as distâncias
    string distances = "";
    for (int i = 1; i < n; ++i) {
        if (dist[i] != infinite) {
            distances += to_string(i) + ":" + to_string(dist[i]) + " ";
        } else {
            distances += to_string(i) + ":-1 ";
        }
    }
    return distances;
}

int main(int argc, char *argv[]) {
    string input_file = "";
    string output_file = "";
    int start_node = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            cout << "Help" << endl;
            cout << "-h: mostra help" << endl;
            cout << "-o <arquivo>: redireciona a saída para o arquivo" << endl;
            cout << "-f <arquivo>: lê o grafo do arquivo" << endl;
            cout << "-i : vértice inicial" << endl;

            return 0;
        } else if (strcmp(argv[i], "-o") == 0) {
            output_file = argv[i + 1];
        } else if (strcmp(argv[i], "-f") == 0) {
            input_file = argv[i + 1];
        } else if (strcmp(argv[i], "-i") == 0) {
            start_node = atoi(argv[i + 1]);
        }
    }

    if (input_file == "") {
        cout << "No input file specified. Use the -f parameter" << endl;
        return 1;
    }

    ifstream fin(input_file);
    if (!fin) {
        cerr << "Could not open input file: " << input_file << endl;
        return 1;
    }

    int n, m; // numero de vertices e arestas
    fin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1); // criando a matriz que emula um grafo

    for (int i = 0; i < m; i++) {
        int u, v, w;
        fin >> u >> v >> w; // recebendo os vertices e peso entre arestas
        adj[u].push_back(make_pair(v, w)); // adiciona v nos vizinhos de u
        adj[v].push_back(make_pair(u, w)); // adiciona u nos vizinhos de v
    }

    fin.close();

    string distances = dijkstra(adj, start_node);

    if (!(output_file == "")) {
        ofstream fout(output_file);
        if (!fout) {
            cerr << "Could not open output file: " << output_file << endl;
            return 1;
        }
        fout << distances;
        fout << endl;

        fout.close();
    }
    cout << distances << endl;

    return 0;

}
