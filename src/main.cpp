/*#include <iostream>
#include "binheap.h"
//#include <Windows.h>

int main() {
   

    BinomialHeap<int> heap1;
    heap1.insert(12);
    heap1.insert(7);
    heap1.insert(25);
    heap1.insert(15);

    std::cout << "min in heap1: " << heap1.get_min() << " (expect 7)\n";

    BinomialHeap<int> heap2;
    heap2.insert(3);
    heap2.insert(18);
    heap2.insert(8);

    heap1.merge(heap2);

    std::cout << "post-merge min: " << heap1.get_min() << " (xpct 3)\n";

    std::cout << "take: " << heap1.extract_min() << "\n"; // 3
    std::cout << "take: " << heap1.extract_min() << "\n"; // 7
    std::cout << "new min: " << heap1.get_min() << " (xpct 8)\n";

    return 0;
}*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "triheap.h"      // Наша исправленная d-куча (3-куча)
#include "binheap.h" // Твоя биномиальная куча

using namespace std;

const int INF = 1e9;

struct Edge {
    int to;
    int weight;
};

struct Graph {
    int V;
    vector<vector<Edge>> adj;
};

// Проверка связности через DFS
void DFS(int v, const vector<vector<Edge>>& adj, vector<bool>& visited) {
    visited[v] = true;
    for (const auto& edge : adj[v]) {
        if (!visited[edge.to]) {
            DFS(edge.to, adj, visited);
        }
    }
}

bool check_connectivity(const Graph& graph) {
    vector<bool> visited(graph.V, false);
    DFS(0, graph.adj, visited);
    for (bool v : visited) {
        if (!v) return false;
    }
    return true;
}

// Проверка на отсутствие отрицательных весов
bool check_dijkstra_conditions(const Graph& graph) {
    for (int i = 0; i < graph.V; ++i) {
        for (const auto& edge : graph.adj[i]) {
            if (edge.weight < 0) return false;
        }
    }
    return true;
}

// Автогенерация связного неориентированного графа
Graph generate_random_graph(int V, int E) {
    Graph g;
    g.V = V;
    g.adj.resize(V);

    // Гарантируем связность "цепочкой"
    for (int i = 1; i < V; ++i) {
        int weight = rand() % 20 + 1;
        g.adj[i - 1].push_back({ i, weight });
        g.adj[i].push_back({ i - 1, weight });
    }

    int remaining_edges = E - (V - 1);
    while (remaining_edges > 0) {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v) {
            int weight = rand() % 50 + 1;
            g.adj[u].push_back({ v, weight });
            g.adj[v].push_back({ u, weight });
            remaining_edges--;
        }
    }
    return g;
}

// Вариант 1: Дейкстра на d-куче (3-куче)
void dijkstra_triheap(const Graph& graph, int start, vector<int>& dist, vector<int>& parent) {
    dist.assign(graph.V, INF);
    parent.assign(graph.V, -1);
    dist[start] = 0;

    vector<HeapElem> heap; // Используем вектор из triheap.hpp
    Heap_Insert(heap, { 0, start });

    while (!heap.empty()) {
        HeapElem current = Heap_Extract(heap);
        int d = current.first;
        int u = current.second;

        if (d > dist[u]) continue;

        for (const auto& edge : graph.adj[u]) {
            int v = edge.to;
            int weight = edge.weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                Heap_Insert(heap, { dist[v], v });
            }
        }
    }
}

// Вариант 2: Дейкстра на Биномиальной куче
void dijkstra_binomial(const Graph& graph, int start, vector<int>& dist, vector<int>& parent) {
    dist.assign(graph.V, INF);
    parent.assign(graph.V, -1);
    dist[start] = 0;

    BinomialHeap<pair<int, int>> heap; // Наш класс-шаблон
    heap.insert({ 0, start });

    while (!heap.is_empty()) {
        pair<int, int> current = heap.extract_min();
        int d = current.first;
        int u = current.second;

        if (d > dist[u]) continue;

        for (const auto& edge : graph.adj[u]) {
            int v = edge.to;
            int weight = edge.weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                heap.insert({ dist[v], v });
            }
        }
    }
}

void print_path(int target, const vector<int>& parent) {
    vector<int> path;
    for (int v = target; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    cout << "Путь: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i] << (i + 1 == path.size() ? "" : " -> ");
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    Graph graph;
    int mode_choice;
    cout << "--- Режим ввода графа ---\n1. Ручной ввод\n2. Случайная генерация\nВыбор: ";
    cin >> mode_choice;

    if (mode_choice == 1) {
        int V, E;
        cout << "Введите число вершин и ребер: ";
        cin >> V >> E;
        graph.V = V;
        graph.adj.resize(V);
        cout << "Введите ребра (u v weight):\n";
        for (int i = 0; i < E; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            graph.adj[u].push_back({ v, w });
            graph.adj[v].push_back({ u, w });
        }
    }
    else {
        int V, E;
        cout << "Количество вершин и ребер для автогенерации: ";
        cin >> V >> E;
        graph = generate_random_graph(V, E);
        cout << "Граф успешно сгенерирован!\n";
    }

    // Валидация условий лабораторной
    if (!check_dijkstra_conditions(graph)) {
        cout << "Ошибка: Граф содержит ребра с отрицательным весом. Дейкстра не применима!\n";
        return 0;
    }
    if (!check_connectivity(graph)) {
        cout << "Ошибка: Граф не является односвязным (несвязным)!\n";
        return 0;
    }

    int heap_choice;
    cout << "\n--- Выбор структуры данных для очереди ---\n1. d-куча (3-куча)\n2. Сложная куча (Биномиальная куча)\nВыбор: ";
    cin >> heap_choice;

    int start_node;
    cout << "Введите начальную точку алгоритма (0 - " << graph.V - 1 << "): ";
    cin >> start_node;

    vector<int> dist;
    vector<int> parent;

    // Запуск выбранной реализации
    if (heap_choice == 1) {
        cout << "Запуск Дейкстры на 3-куче...\n";
        dijkstra_triheap(graph, start_node, dist, parent);
    }
    else {
        cout << "Запуск Дейкстры на Биномиальной куче...\n";
        dijkstra_binomial(graph, start_node, dist, parent);
    }

    cout << "Расчет окончен.\n";

    // Интерактивный запрос путей
    while (true) {
        int target;
        cout << "\nВведите целевую вершину для получения стоимости и пути (-1 для выхода): ";
        cin >> target;
        if (target == -1) break;

        if (target < 0 || target >= graph.V) {
            cout << "Индекс вершины вне диапазона!\n";
            continue;
        }

        if (dist[target] == INF) {
            cout << "Путь до вершины " << target << " не достижим.\n";
        }
        else {
            cout << "Стоимость кратчайшего пути: " << dist[target] << endl;
            print_path(target, parent);
        }
    }

    return 0;
}