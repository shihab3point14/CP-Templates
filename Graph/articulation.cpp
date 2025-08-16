// Articulation points in a graph
// O(V + E)
int n;
vector<vector<int>> adj;

vector<bool> visited;
vector<int> tin, low;
int timer;
vector<int> cutpoints;

void dfs(int v, int p = -1) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children=0;
    for (int to : adj[v]) {
        if (to == p) continue;
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs(to, v);
            low[v] = min(low[v], low[to]);
            if (low[to] >= tin[v] && p!=-1)
                cutpoints.push_back(v);
            ++children;
        }
    }
    if(p == -1 && children > 1)
        cutpoints.push_back(v);
}

void find_articulation_point() {
    timer = 0;
    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);
    cutpoints.clear();
    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            dfs (i);
    }
}