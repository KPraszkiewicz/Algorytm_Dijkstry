#include<iostream>
#include<vector>
#include <fstream>
#include<queue>
#include<limits>
#include<climits>
#include<algorithm>
#include<queue>
#include <stack>
using namespace std;

struct Wierzcholek
{
    int index;
    int waga;
    int pop = -1;
    int d = INT_MAX;
};

bool operator>(const Wierzcholek& a, const Wierzcholek& b) { return a.waga > b.waga; }
bool operator<(const Wierzcholek& a, const Wierzcholek& b) { return a.waga < b.waga; }

struct Graf
{
    vector<vector<Wierzcholek>> adj; // lista sąsiedztwa
    int N = 0; // ilosc wierzcholkow 0, ..., n - 1
};

Graf wczytaj_z_pliku(const char* nazwa)
{
    ifstream os(nazwa);
    Graf G;
    int liczba_E, liczba_V, v;
    Wierzcholek w;

    if(!os.good())
    {
        os.close();
        throw "XXX";
    }

    os >> liczba_V >> liczba_E;

    G.N = liczba_V;
    G.adj.resize(liczba_V);

    while(!os.eof() && liczba_E--)
    {
        os >> v >> w.index >> w.waga;
        G.adj[v].push_back(w);
    }
    os.close();
    return G;
}

vector<Wierzcholek> dijkstra(const Graf& G, int start)
{
    const int N = G.N;
    std::vector<Wierzcholek> V(N);
    int *D, *P, *O;

    if(start >= N)
    {
        cout << "Wierzcholek {" << start << "} nie nalezy do grafu!" << endl;
        return V;
    }

    // inicjalizacja
    D = new int[N];
    P = new int[N];
    O = new int[N];

    for(int i = 0; i < N; ++i)
    {
        D[i] = INT_MAX / 2;
        P[i] = -1;
        O[i] = 0;
    }
    D[start] = 0;

    for(int i = 0; i < N; ++i)
    {
        int min = -1;

        while(O[++min]);
        for(int j = min + 1; j < N; ++j)
            if(!O[j] && D[j] < D[min])
                min = j;

        O[min] = 1;

        for(auto& b: G.adj[min])
        {
            int u = b.index;
            if(!O[u] && D[u] > D[min] + b.waga)
            {
                D[u] = D[min] + b.waga;
                P[u] = min;
            }
        }
    }

    for(int i = 0; i < N; ++i)
    {
        V[i] = {i,0,P[i],D[i]};
    }

    delete [] P;
    delete [] D;
    delete [] O;
    return V;
}

void wypisz(const vector<Wierzcholek>& V, size_t start, size_t koniec)
{
    std::stack<int> S;
    cout << "droga dla {" << start << "}->{" << koniec << "}: ";
    if(start >= V.size() || koniec >= V.size())
        return;

    auto a = V[koniec];
    while(a.pop >= 0)
    {
        S.push(a.index);
        a = V[a.pop];
    }
    cout << a.index;

    while(!S.empty())
    {
        cout << " -> " << S.top();
        S.pop();
    }
    cout << endl;
}

int main()
{
    Graf G;
    int start;
    try
    {
        G = wczytaj_z_pliku("dijkstra_dane.txt");
    }
    catch (...) {
        cout << "Nie udalo sie wczytac pliku!" << endl;
        return 0;
    }

    for(int i=0; i < G.N; ++i)
    {
        cout << i << ": ";
        for(auto v: G.adj[i])
        {
            cout << v.index << "(" << v.waga << ") ";
        }
        cout << endl;
    }


    do
    {
        do
        {
            cout << "Podaj wierzcholek startowy [0-" << G.N - 1 << "]: ";
            cin >> start;
            cin.clear();
            cin.ignore();
        }while(start < 0 || start >= G.N);

        for(int i = 0; i < G.N; ++i)
             wypisz( dijkstra(G,start), start, i);

    }while(true);

    //auto wynik = dijkstra(G,start);
}
