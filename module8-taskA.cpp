// Подлец взломал навигационную систему корабля и создал K
// графов, представляющих различные карты расположения комнат
// на корабле. Ваша задача состоит в том, чтобы найти кратчайший
// путь от вашего текущего местоположения до каждой из других комнат.

#include <iostream>
#include <queue>
#include <vector>

const int kInf = 2009000999;

struct Edge {
  size_t to;
  int weight;
  Edge(size_t to, int weight) : to(to), weight(weight) {}
};

class Graph {
 public:
  Graph() : graph_({}) {}
  Graph(size_t vertex_number) : graph_(vertex_number) {}
  size_t GetVertexNumber() const { return graph_.size(); }
  void AddEdge(size_t from, size_t to, int weight) {
    graph_[from].push_back(Edge(to, weight));
  }
  std::vector<Edge> GetAdjacentVertices(size_t vertex) const {
    return graph_[vertex];
  }

 private:
  std::vector<std::vector<Edge>> graph_;
};

std::vector<int> SearchShortestPaths(Graph& graph, size_t start) {
  std::vector<int> dist(graph.GetVertexNumber(), kInf);
  std::vector<int> is_visited(graph.GetVertexNumber(), 0);
  dist[start] = 0;
  std::priority_queue<std::pair<int, int>> heap;
  heap.push({0, start});
  while (!heap.empty()) {
    int vertex = heap.top().second;
    heap.pop();
    while (!heap.empty() && is_visited[vertex] == 1) {
      vertex = heap.top().second;
      heap.pop();
    }
    if (heap.empty() && is_visited[vertex] == 1) {
      break;
    }
    is_visited[vertex] = 1;
    for (Edge edge : graph.GetAdjacentVertices(vertex)) {
      size_t to = edge.to;
      int weight = edge.weight;
      if (dist[to] > dist[vertex] + weight) {
        dist[to] = dist[vertex] + weight;
        heap.push({-1 * dist[to], to});
      }
    }
  }
  return dist;
}

int main() {
  int map_number;
  std::cin >> map_number;
  for (int _ = 0; _ < map_number; ++_) {
    int vertex_number;
    int edge_number;
    std::cin >> vertex_number >> edge_number;
    Graph graph = Graph(vertex_number);
    for (int i = 0; i < edge_number; ++i) {
      int from;
      int to;
      int weight;
      std::cin >> from >> to >> weight;
      graph.AddEdge(from, to, weight);
      graph.AddEdge(to, from, weight);
    }
    size_t start;
    std::cin >> start;
    std::vector<int> shortest_dist = SearchShortestPaths(graph, start);
    for (int distance : shortest_dist) {
      std::cout << distance << " ";
    }
    std::cout << "\n";
  }
}
