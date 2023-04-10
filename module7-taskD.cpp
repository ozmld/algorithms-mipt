// Вам задан ориентированный граф с N вершинами и M ребрами.
// Найдите компоненты сильной связности заданного графа и
// топологически отсортируйте его конденсацию.

#include <iostream>
#include <vector>

class Graph {
 public:
  Graph() : graph_({}) {}
  Graph(size_t vertex_number) : graph_(vertex_number) {}
  size_t GetVertexNumber() const { return graph_.size(); }
  void AddEdge(size_t v, size_t u) { graph_[v].push_back(u); }
  std::vector<size_t> GetAdjacentVertices(size_t v) const { return graph_[v]; }
  Graph GetReversed() const {
    Graph g = Graph(GetVertexNumber());
    for (size_t i = 0; i < GetVertexNumber(); ++i) {
      for (size_t u : GetAdjacentVertices(i)) {
        g.AddEdge(u, i);
      }
    }
    return g;
  }

 private:
  std::vector<std::vector<size_t>> graph_;
};

void SortByOutTime(const Graph& g, size_t v, std::vector<size_t>& used,
                   std::vector<size_t>& out_order) {
  used[v] = 1;
  for (size_t u : g.GetAdjacentVertices(v)) {
    if (used[u] == 0) {
      SortByOutTime(g, u, used, out_order);
    }
  }
  out_order.push_back(v);
}

void FillSameComponent(const Graph& g, size_t v, std::vector<size_t>& used,
                       std::vector<size_t>& component, size_t component_num) {
  used[v] = 1;
  component[v] = component_num;
  for (size_t u : g.GetAdjacentVertices(v)) {
    if (used[u] == 0) {
      FillSameComponent(g, u, used, component, component_num);
    }
  }
}

std::pair<std::vector<size_t>, size_t> FindComponents(const Graph& graph) {
  size_t vertex_num = graph.GetVertexNumber();
  std::vector<size_t> used(vertex_num, 0), out_order;
  for (size_t v = 0; v < vertex_num; ++v) {
    if (used[v] == 0) {
      SortByOutTime(graph, v, used, out_order);
    }
  }
  Graph revered_graph = graph.GetReversed();
  used.assign(vertex_num, 0);
  std::vector<size_t> component(vertex_num, 0);
  size_t component_num = 1;
  for (size_t i = 0; i < vertex_num; ++i) {
    size_t v = out_order[vertex_num - 1 - i];
    if (used[v] == 0) {
      FillSameComponent(revered_graph, v, used, component, component_num++);
    }
  }
  return {component, component_num};
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  Graph graph = Graph(n);
  for (size_t i = 0; i < m; ++i) {
    size_t v, u;
    std::cin >> v >> u;
    graph.AddEdge(--v, --u);
  }
  std::vector<size_t> component;
  size_t component_num = 1;
  std::pair<std::vector<size_t>, size_t> component_info = FindComponents(graph);
  component = component_info.first;
  component_num = component_info.second;
  std::cout << component_num - 1 << "\n";
  for (size_t i = 0; i < n; ++i) {
    std::cout << component[i] << " ";
  }
}
