
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <climits>

using namespace std;

const int INF = 999999999;

// Structure to store network connections
struct Edge {
    int src;
    int dest;
    int weight;
};

// Sort edges according to their cost
bool compareEdges(Edge a, Edge b) {
    return a.weight < b.weight;
}

// ==========================================
// NETWORK DESIGN CLASS
// ==========================================

class NetworkDesign {

private:

    vector<string> cities;
    vector<Edge> edges;

    // Find city index using its name
    int findCity(string name) {
        for (int i = 0; i < (int)cities.size(); i++) {
            if (cities[i] == name) {
                return i;
            }
        }
        return -1;
    }

    // Find representative of a set
    int findSet(int parent[], int x) {
        if (parent[x] != x) {
            parent[x] = findSet(parent, parent[x]);
        }
        return parent[x];
    }

    // Join two disjoint sets
    void unionSets(int parent[], int rank[], int x, int y) {

        int rootX = findSet(parent, x);
        int rootY = findSet(parent, y);

        if (rootX == rootY) {
            return;
        }

        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        }
        else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        }
        else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }

public:

    // ======================================
    // ADD CITY
    // ======================================

    void addCity(string name) {

        if (name.empty()) {
            cout << "City name cannot be empty!\n";
            return;
        }

        if (findCity(name) != -1) {
            cout << "City already exists!\n";
            return;
        }

        cities.push_back(name);

        cout << "City added successfully: "
             << name << "\n";
    }

    // ======================================
    // ADD CONNECTION
    // ======================================

    void addConnection(string city1, string city2,
                       int cost) {

        int u = findCity(city1);
        int v = findCity(city2);

        if (u == -1 || v == -1) {
            cout << "One or both cities do not exist!\n";
            return;
        }

        if (u == v) {
            cout << "A city cannot connect to itself!\n";
            return;
        }

        if (cost <= 0) {
            cout << "Connection cost must be positive!\n";
            return;
        }

        // Check duplicate connections
        for (int i = 0; i < (int)edges.size(); i++) {

            if ((edges[i].src == u && edges[i].dest == v) ||
                (edges[i].src == v && edges[i].dest == u)) {

                cout << "Connection already exists!\n";
                return;
            }
        }

        Edge e;
        e.src = u;
        e.dest = v;
        e.weight = cost;

        edges.push_back(e);

        cout << "Connection added successfully!\n";
    }

    // ======================================
    // DISPLAY NETWORK
    // ======================================

    void displayNetwork() {

        cout << "\n========== NETWORK DETAILS ==========\n";

        if (cities.empty()) {
            cout << "No cities available.\n";
            return;
        }

        cout << "\nCities in the network:\n";

        for (int i = 0; i < (int)cities.size(); i++) {
            cout << i + 1 << ". " << cities[i] << "\n";
        }

        cout << "\nNetwork Connections:\n";

        if (edges.empty()) {
            cout << "No connections available.\n";
            return;
        }

        cout << "\n";
        cout << "CITY 1\t\tCITY 2\t\tCOST\n";
        cout << "------------------------------------------\n";

        for (int i = 0; i < (int)edges.size(); i++) {

            cout << cities[edges[i].src] << "\t\t"
                 << cities[edges[i].dest] << "\t\t"
                 << edges[i].weight << "\n";
        }

        cout << "==========================================\n";
    }

    // ======================================
    // PRIM'S ALGORITHM
    // ======================================

    int primAlgorithm(bool show) {

        int n = (int)cities.size();

        if (n == 0 || edges.empty()) {
            if (show) {
                cout << "\nNetwork is empty!\n";
            }
            return -1;
        }

        // Create adjacency matrix
        vector<vector<int> > graph(
            n, vector<int>(n, INF)
        );

        for (int i = 0; i < (int)edges.size(); i++) {

            int u = edges[i].src;
            int v = edges[i].dest;
            int w = edges[i].weight;

            graph[u][v] = w;
            graph[v][u] = w;
        }

        vector<int> key(n, INF);
        vector<int> parent(n, -1);
        vector<bool> inMST(n, false);

        key[0] = 0;

        int totalCost = 0;

        vector<Edge> mst;

        // Repeat for every vertex
        for (int count = 0; count < n; count++) {

            int minKey = INF;
            int u = -1;

            // Find minimum key vertex
            for (int i = 0; i < n; i++) {

                if (!inMST[i] && key[i] < minKey) {
                    minKey = key[i];
                    u = i;
                }
            }

            // Graph is disconnected
            if (u == -1) {

                if (show) {
                    cout << "\nMST cannot be formed!\n";
                    cout << "The network is disconnected.\n";
                }

                return -1;
            }

            inMST[u] = true;

            // Store selected MST edge
            if (parent[u] != -1) {

                Edge e;
                e.src = parent[u];
                e.dest = u;
                e.weight = graph[parent[u]][u];

                mst.push_back(e);

                totalCost += e.weight;
            }

            // Update adjacent vertices
            for (int v = 0; v < n; v++) {

                if (graph[u][v] != INF &&
                    !inMST[v] &&
                    graph[u][v] < key[v]) {

                    key[v] = graph[u][v];
                    parent[v] = u;
                }
            }
        }

        if (show) {

            cout << "\n========== PRIM'S ALGORITHM ==========\n";

            cout << "\nMinimum Spanning Tree Connections:\n";

            for (int i = 0; i < (int)mst.size(); i++) {

                cout << cities[mst[i].src]
                     << " -- "
                     << cities[mst[i].dest]
                     << " : "
                     << mst[i].weight << "\n";
            }

            cout << "\nTotal Minimum Cost = "
                 << totalCost << "\n";

            cout << "Time Complexity: O(V^2)\n";
            cout << "======================================\n";
        }

        return totalCost;
    }

    // ======================================
    // KRUSKAL'S ALGORITHM
    // ======================================

    int kruskalAlgorithm(bool show) {

        int n = (int)cities.size();

        if (n == 0 || edges.empty()) {

            if (show) {
                cout << "\nNetwork is empty!\n";
            }

            return -1;
        }

        // Copy edges and sort by weight
        vector<Edge> sortedEdges = edges;

        sort(sortedEdges.begin(),
             sortedEdges.end(),
             compareEdges);

        // Initialize disjoint sets
        vector<int> parent(n);
        vector<int> rank(n, 0);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }

        vector<Edge> mst;

        int totalCost = 0;

        // Process edges in increasing order
        for (int i = 0;
             i < (int)sortedEdges.size();
             i++) {

            Edge e = sortedEdges[i];

            int rootU = findSet(&parent[0], e.src);
            int rootV = findSet(&parent[0], e.dest);

            // Check whether edge forms a cycle
            if (rootU != rootV) {

                mst.push_back(e);

                totalCost += e.weight;

                unionSets(
                    &parent[0],
                    &rank[0],
                    rootU,
                    rootV
                );

                if ((int)mst.size() == n - 1) {
                    break;
                }
            }
        }

        // MST must contain exactly n-1 edges
        if ((int)mst.size() != n - 1) {

            if (show) {
                cout << "\nMST cannot be formed!\n";
                cout << "The network is disconnected.\n";
            }

            return -1;
        }

        if (show) {

            cout << "\n========== KRUSKAL'S ALGORITHM ==========\n";

            cout << "\nMinimum Spanning Tree Connections:\n";

            for (int i = 0; i < (int)mst.size(); i++) {

                cout << cities[mst[i].src]
                     << " -- "
                     << cities[mst[i].dest]
                     << " : "
                     << mst[i].weight << "\n";
            }

            cout << "\nTotal Minimum Cost = "
                 << totalCost << "\n";

            cout << "Time Complexity: O(E log E)\n";
            cout << "==========================================\n";
        }

        return totalCost;
    }

    // ======================================
    // COMPARE BOTH ALGORITHMS
    // ======================================

    void compareAlgorithms() {

        cout << "\n========== ALGORITHM COMPARISON ==========\n";

        clock_t start, end;

        int primCost, kruskalCost;

        // Measure Prim execution time
        start = clock();

        primCost = primAlgorithm(false);

        end = clock();

        double primTime =
            (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

        // Measure Kruskal execution time
        start = clock();

        kruskalCost = kruskalAlgorithm(false);

        end = clock();

        double kruskalTime =
            (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

        if (primCost == -1 || kruskalCost == -1) {

            cout << "Cannot compare MST costs.\n";
            cout << "Please check network connectivity.\n";

            return;
        }

        cout << "\nPrim's Algorithm:\n";
        cout << "Minimum Cost: " << primCost << "\n";
        cout << "Execution Time: " << primTime << " ms\n";

        cout << "\nKruskal's Algorithm:\n";
        cout << "Minimum Cost: " << kruskalCost << "\n";
        cout << "Execution Time: " << kruskalTime << " ms\n";

        cout << "\nComparison Result:\n";

        if (primCost == kruskalCost) {
            cout << "Both algorithms produced the same MST cost.\n";
        }
        else {
            cout << "The MST costs are different.\n";
        }

        cout << "\nPrim Complexity: O(V^2)\n";
        cout << "Kruskal Complexity: O(E log E)\n";

        cout << "\n==========================================\n";
    }

    // ======================================
    // LOAD SAMPLE NETWORK
    // ======================================

    void loadSampleData() {

        if (!cities.empty() || !edges.empty()) {
            cout << "\nSample data can only be loaded into an empty network.\n";
            cout << "Start a fresh run to load the sample network.\n";
            return;
        }

        cout << "\nLoading Sample Network...\n";

        addCity("Delhi");
        addCity("Mumbai");
        addCity("Chandigarh");
        addCity("Shimla");
        addCity("Jaipur");
        addCity("Amritsar");

        addConnection("Delhi", "Mumbai", 110);
        addConnection("Delhi", "Chandigarh", 60);
        addConnection("Delhi", "Jaipur", 70);
        addConnection("Mumbai", "Jaipur", 100);
        addConnection("Chandigarh", "Shimla", 40);
        addConnection("Chandigarh", "Amritsar", 50);
        addConnection("Shimla", "Delhi", 90);
        addConnection("Jaipur", "Amritsar", 120);
        addConnection("Mumbai", "Amritsar", 150);

        cout << "\nSample network loaded successfully!\n";
    }

    // ======================================
    // MAIN MENU
    // ======================================

    void menu() {

        int choice;
        string city1, city2, name;
        int cost;

        do {

            cout << "\n\n";
            cout << "========================================\n";
            cout << "      NETWORK DESIGN OPTIMIZATION\n";
            cout << "========================================\n";

            cout << "1. Add City\n";
            cout << "2. Add Connection\n";
            cout << "3. Display Network\n";
            cout << "4. Find MST using Prim's Algorithm\n";
            cout << "5. Find MST using Kruskal's Algorithm\n";
            cout << "6. Compare Both Algorithms\n";
            cout << "7. Load Sample Network\n";
            cout << "8. Exit\n";

            cout << "========================================\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {

                cin.clear();
                cin.ignore(10000, '\n');

                cout << "Invalid input! Enter a number from 1 to 8.\n";

                continue;
            }

            switch (choice) {

                case 1:

                    cout << "Enter city name: ";

                    cin >> ws;
                    getline(cin, name);

                    addCity(name);

                    break;

                case 2:

                    cout << "Enter first city: ";

                    cin >> ws;
                    getline(cin, city1);

                    cout << "Enter second city: ";

                    getline(cin, city2);

                    cout << "Enter connection cost: ";

                    if (!(cin >> cost)) {

                        cin.clear();
                        cin.ignore(10000, '\n');

                        cout << "Invalid cost input!\n";

                        break;
                    }

                    addConnection(city1, city2, cost);

                    break;

                case 3:

                    displayNetwork();

                    break;

                case 4:

                    primAlgorithm(true);

                    break;

                case 5:

                    kruskalAlgorithm(true);

                    break;

                case 6:

                    compareAlgorithms();

                    break;

                case 7:

                    loadSampleData();

                    break;

                case 8:

                    cout << "\nThank you for using the system!\n";

                    break;

                default:

                    cout << "Invalid choice! Please try again.\n";
            }

        } while (choice != 8);
    }

};

// ==========================================
// MAIN FUNCTION
// ==========================================

int main() {

    NetworkDesign system;

    cout << "\n";
    cout << "========================================\n";
    cout << "   WELCOME TO NETWORK DESIGN SYSTEM\n";
    cout << "========================================\n";

    system.menu();

    return 0;
}

