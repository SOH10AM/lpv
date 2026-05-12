#include <iostream>
#include <queue>
#include <stack>
#include <omp.h>
#include <vector>

using namespace std;

class Graph {

    int v;
    vector<vector<int>> adj;

public:

    Graph(int v) {
        this->v = v;
        adj.resize(v);
    }

    void addEdge(int a, int b) {
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // ================= SEQUENTIAL BFS =================

    void sequentialBfs(int start) {

        queue<int> q;
        vector<bool> visited(v, false);

        double startTime = omp_get_wtime();

        visited[start] = true;
        q.push(start);

        cout << "\nSequential BFS : ";

        while (!q.empty()) {

            int node = q.front();
            q.pop();

            cout << node << " -> ";

            for (int i = 0; i < adj[node].size(); i++) {

                int neighbour = adj[node][i];

                if (!visited[neighbour]) {

                    visited[neighbour] = true;
                    q.push(neighbour);
                }
            }
        }

        double endTime = omp_get_wtime();

        cout << "\nTime Required : "
             << endTime - startTime << " seconds\n";
    }

    // ================= PARALLEL BFS =================

    void parallelBfs(int start) {

        queue<int> q;
        vector<bool> visited(v, false);

        double startTime = omp_get_wtime();

        visited[start] = true;
        q.push(start);

        cout << "\nParallel BFS using OpenMP : ";

        while (!q.empty()) {

            int node = q.front();
            q.pop();

            cout << node << " -> ";

            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++) {

                int neighbour = adj[node][i];

                if (!visited[neighbour]) {

                    #pragma omp critical
                    {
                        if (!visited[neighbour]) {

                            visited[neighbour] = true;
                            q.push(neighbour);
                        }
                    }
                }
            }
        }

        double endTime = omp_get_wtime();

        cout << "\nTime Required : "
             << endTime - startTime << " seconds\n";
    }

    // ================= SEQUENTIAL DFS =================

    void sequentialDfs(int start) {

        stack<int> st;
        vector<bool> visited(v, false);

        double startTime = omp_get_wtime();

        visited[start] = true;
        st.push(start);

        cout << "\nSequential DFS : ";

        while (!st.empty()) {

            int node = st.top();
            st.pop();

            cout << node << " -> ";

            for (int i = 0; i < adj[node].size(); i++) {

                int neighbour = adj[node][i];

                if (!visited[neighbour]) {

                    visited[neighbour] = true;
                    st.push(neighbour);
                }
            }
        }

        double endTime = omp_get_wtime();

        cout << "\nTime Required : "
             << endTime - startTime << " seconds\n";
    }

    // ================= PARALLEL DFS =================

    void parallelDfs(int start) {

        stack<int> st;
        vector<bool> visited(v, false);

        double startTime = omp_get_wtime();

        visited[start] = true;
        st.push(start);

        cout << "\nParallel DFS using OpenMP : ";

        while (!st.empty()) {

            int node = st.top();
            st.pop();

            cout << node << " -> ";

            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++) {

                int neighbour = adj[node][i];

                if (!visited[neighbour]) {

                    #pragma omp critical
                    {
                        if (!visited[neighbour]) {

                            visited[neighbour] = true;
                            st.push(neighbour);
                        }
                    }
                }
            }
        }

        double endTime = omp_get_wtime();

        cout << "\nTime Required : "
             << endTime - startTime << " seconds\n";
    }
};

int main() {

    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    // BFS
    g.sequentialBfs(0);
    g.parallelBfs(0);

    // DFS
    g.sequentialDfs(0);
    g.parallelDfs(0);

    return 0;
}