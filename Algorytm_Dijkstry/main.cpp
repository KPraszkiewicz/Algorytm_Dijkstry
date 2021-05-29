#include<iostream>
#include<vector>
#include <fstream>
#include<queue>
#include<limits>
#include<algorithm>
#include<queue>
using namespace std;

struct K
{
    int a, b, waga;


    bool operator>(const K& k) const {return a > k.a;}
    bool operator<(const K& k) const {return a < k.a;}
};

struct Graf
{
    vector<K> E; // teblica jest posortowana wzgl�dem 1 wierzcho�ka
    vector<int> V;
    bool wczytaj_z_pliku(const char* nazwa)
    {
        ifstream os(nazwa);
        if(!os.good())
        {
            os.close();
            return false;
        }
        int liczba_E, liczba_V;
        K k;
        os >> liczba_V >> liczba_E;
        V.resize(liczba_V);
        E.reserve(liczba_E);
        while(!os.eof())
        {
            os >> k.a >> k.b >> k.waga;
            E.push_back(k);

        }
        sort(E.begin(),E.end());
        os.close();
        return true;
    }
};

struct V_dijkstra
{
    int d = INT_MAX;
    int pop = -1;
    int index;

    bool operator>(const V_dijkstra& v) const {return d > v.d;}
    bool operator<(const V_dijkstra& v) const {return d < v.d;}
};

vector<V_dijkstra> dijkstra(const Graf& G, int start)
{
    const int N = G.V.size();
    std::vector<V_dijkstra> V(N);
    for(int i = 0; i < N; ++i)
    {
        V[i].index = i;
    }
    V[start].d = 0;
    std::priority_queue<V_dijkstra*, std::vector<V_dijkstra*>,std::greater<V_dijkstra*>> Q;
    Q.push(&V[start]);
    while(!Q.empty())
    {
        auto* v = Q.top();
        for(const auto& e: G.E)
        {
            if(e.a == v->index)
            {
                V_dijkstra& a{V[e.a]};
                V_dijkstra& b{V[e.b]};
                if(b.d > a.d + e.waga)
                {
                    b.d = a.d + e.waga;
                    b.pop = a.index;
                    Q.push(&b);
                }
            }
        }
        Q.pop();
    }
    return V;
}

int main()
{
    Graf G;
    G.wczytaj_z_pliku("dijkstra_dane.txt");
    for(auto e : G.E)
        cout << "(" << e.a << "," << e.b << "): " << e.waga << endl;
    int start = 0;
    int koniec = 4;
    vector<V_dijkstra> wynik = dijkstra(G,start);
    cout << "droga dla {" << start << "}->{" << koniec << "}: ";
    V_dijkstra a = wynik[koniec];
    while(a.pop >= 0)
    {
        cout << a.index<< " <- ";
        a = wynik[a.pop];
    }
    cout << a.index << endl;

}
