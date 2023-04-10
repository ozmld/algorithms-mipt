// Дан неориентированный (быть может несвязный) граф. Требуется найти все мосты в нем.

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>


struct VertexInfo {
  unsigned short tin, ret, parent;
  unsigned short used;
  VertexInfo() : tin(0), ret(0), parent(0), used(0) {}
};

class Graph {
 public:
  explicit Graph() : graph_({}) {}
  explicit Graph(size_t vertex_number) : graph_(vertex_number) {}
  unsigned short GetVertexNumber() const { return graph_.size(); }
  void AddEdge(unsigned short v, unsigned short u, size_t num) {
    graph_[v].push_back(u);
    if (edges_.count({v, u}) == 0) {
      edges_[{v, u}] = num;
    } else {
      edges_[{v, u}] = 0;
    }
  }
  const std::vector<unsigned short>& GetAdjacentVertices(unsigned short v) const {
    return graph_[v];
    
  }
  size_t GetEdgeNumber(unsigned short v, unsigned short u) const {
    if (edges_.count({v, u}) != 0) {
      return edges_.at({v, u});
    }
    return 0;
  }
 private:
  std::vector<std::vector<unsigned short>> graph_;
  std::map<std::pair<unsigned short, unsigned short>, size_t> edges_;
};

void FindBridgesInComponent(const Graph& graph, unsigned short v, unsigned short timer,
         std::vector<VertexInfo>& vertex_info,
         std::vector<size_t>& bridges) {
  vertex_info[v].tin = timer++;
  vertex_info[v].ret = vertex_info[v].tin;
  vertex_info[v].used = 1;
  for (size_t u : graph.GetAdjacentVertices(v)) {
    if (u == vertex_info[v].parent) {
      continue;
    }
    if (vertex_info[u].used == 1) {
      vertex_info[v].ret = std::min(vertex_info[v].ret, vertex_info[u].tin);
      continue;
    }
    vertex_info[u].parent = v;
    FindBridgesInComponent(graph, u, timer, vertex_info, bridges);
    vertex_info[v].ret = std::min(vertex_info[v].ret, vertex_info[u].ret);
    if (vertex_info[u].ret == vertex_info[u].tin &&
        graph.GetEdgeNumber(v, u) != 0) {
      bridges.push_back(graph.GetEdgeNumber(v, u));
    }
  }
}

void GetAllBridges(const Graph& graph, std::vector<size_t>& bridges) {
  size_t vertex_num = graph.GetVertexNumber();
  std::vector<VertexInfo> vertex_info(vertex_num);
  for (size_t i = 0; i < vertex_num; ++i) {
    if (vertex_info[i].used == 0) {
      vertex_info[i].parent = -1;
      FindBridgesInComponent(graph, i, 0, vertex_info, bridges);
    }
  }
  std::sort(bridges.begin(), bridges.end());
}

int main() {
  unsigned short n;
  size_t m;
  std::cin >> n >> m;
  Graph graph = Graph(n);
  for (size_t i = 0; i < m; ++i) {
    unsigned short u, v;
    std::cin >> u >> v;
    if (u == v) {
      continue;
    }
    --u;
    --v;
    graph.AddEdge(v, u, i + 1);
    graph.AddEdge(u, v, i + 1);
  }
  std::vector<size_t> bridges = {};
  GetAllBridges(graph, bridges);
  std::cout << bridges.size() << "\n";
  for (size_t bridge : bridges) {
    std::cout << bridge << " ";
  }
}
