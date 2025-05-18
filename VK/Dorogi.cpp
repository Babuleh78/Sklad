#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {

    int v, n;
    cin >> v >> n;

    vector<vector<int>> adj(v);
    for (int i = 0; i < n; ++i) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    int u, w;
    cin >> u >> w;

    vector<int> dist(v, -1);
    vector<int> count(v, 0);
    queue<int> q;

    dist[u] = 0;
    count[u] = 1;
    q.push(u);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int neighbor : adj[current]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[current] + 1;
                count[neighbor] = count[current];
                q.push(neighbor);
            }
            else if (dist[neighbor] == dist[current] + 1) {
                count[neighbor] += count[current];
            }
        }
    }

    cout << count[w] << endl;

    return 0;
}
