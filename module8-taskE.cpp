// Поиск отрицательного цикла

#include <algorithm>
#include <iostream>
#include <vector>

const long long kInf = 1e10;
const int kMaxWeigt = 100000;
struct Edge {
  size_t to;
  long long weight;
  Edge(size_t to, long long weight) : to(to), weight(weight) {}
};

class Graph {
 public:
  explicit Graph(size_t vertex_number) : graph_(vertex_number) {}
  size_t GetVertexNumber() const { return graph_.size(); }
  void AddEdge(size_t from, size_t to, long long weight) {
    graph_[from].push_back(Edge(to, weight));
  }
  std::vector<Edge> GetAdjacentVertices(size_t vertex) const {
    return graph_[vertex];
  }

 private:
  std::vector<std::vector<Edge>> graph_;
};

std::vector<Edge> edges;

std::vector<size_t> FindNegativeCycle(const Graph& graph) {
  size_t vertex_num = graph.GetVertexNumber();
  std::vector<long long> distance(vertex_num, kInf);
  std::vector<long long> parent(vertex_num, -1);
  distance[0] = 0;
  size_t start = vertex_num;
  for (size_t _ = 0; _ <= vertex_num; ++_) {
    start = vertex_num;
    for (size_t from = 0; from < vertex_num; ++from) {
      for (Edge edge : graph.GetAdjacentVertices(from)) {
        size_t to = edge.to;
        int weight = edge.weight;
        if (distance[from] == kInf) {
          continue;
        }
        if ((distance[from] + weight) < distance[to]) {
          start = to;
          distance[to] = distance[from] + weight;
          parent[to] = from;
        }
      }
    }
  }
  if (start == vertex_num) {
    return {};
  }
  for (size_t i = 0; i < vertex_num; ++i) {
    start = parent[start];
  }
  size_t vertex = parent[start];
  std::vector<size_t> negative_cycle;
  while (start != vertex) {
    negative_cycle.push_back(vertex + 1);
    vertex = parent[vertex];
  }
  negative_cycle.push_back(vertex + 1);
  negative_cycle.push_back(parent[vertex] + 1);
  return negative_cycle;
}

int main() {
  size_t vertex_number;
  std::cin >> vertex_number;
  Graph graph = Graph(vertex_number);
  for (size_t i = 0; i < vertex_number; ++i) {
    for (size_t j = 0; j < vertex_number; ++j) {
      long long weight;
      std::cin >> weight;
      if (weight == kMaxWeigt) {
        weight = kInf;
      }
      graph.AddEdge(i, j, weight);
    }
  }
  std::vector<size_t> negative_cycle = FindNegativeCycle(graph);
  if (negative_cycle.empty()) {
    std::cout << "NO";
    return 0;
  }
  std::cout << "YES\n";
  std::cout << negative_cycle.size() << "\n";
  reverse(negative_cycle.begin(), negative_cycle.end());
  for (size_t i = 0; i < negative_cycle.size(); ++i) {
    std::cout << negative_cycle[i] << " ";
  }
}
