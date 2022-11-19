#include <algorithm>
#include <iostream>
#include <vector>

const int kMaxNumber = 1e5;

struct Point {
  long long x, y, weight, index;
  Point(long long xx, long long yy, long long weightw, long long indexi) {
    x = xx;
    y = yy;
    weight = weightw;
    index = indexi;
  }
};

class FenwickTree {
 public:
  std::vector<Point> tree;
  std::vector<long long> dataset;
  int tree_size;
  int F(int i);
  int G(int i);
  long long GetPrefixAnswer(int pos);
  void Update(int pos, long long val);
  FenwickTree() {
    tree_size = 0;
    tree = {};
    dataset = {};
  }
  FenwickTree(std::vector<Point> data, int data_size) {
    tree_size = data_size;
    dataset = {};
    tree = {};
    for (int i = 0; i < tree_size; ++i) {
      dataset.push_back(0);
      tree.push_back(data[i]);
      tree[i].weight = 0;
    }
    for (int i = 0; i < tree_size; ++i) {
      Update(i, data[i].weight);
    }
  }
};

int FenwickTree::F(int i) { return i & (i + 1); }

int FenwickTree::G(int i) { return i | (i + 1); }

long long FenwickTree::GetPrefixAnswer(int pos) {
  long long ans = 0;
  for (int i = pos; i >= 0; i = F(i) - 1) {
    ans += tree[i].weight;
  }
  return ans;
}

void FenwickTree::Update(int pos, long long val) {
  long long delta = val - dataset[pos];
  dataset[pos] = val;
  for (int i = pos; i < tree_size; i = G(i)) {
    tree[i].weight += delta;
  }
}

int F(int i) { return i & (i + 1); }
int G(int i) { return i | (i + 1); }

bool CompX(const Point& p, const Point& q) { return p.x < q.x; }

bool CompY(const Point& p, const Point& q) { return p.y < q.y; }

int LowerBound(int l_x, int r_x, int rx, const std::vector<int>& r) {
  while (r_x - l_x > 1) {
    int m_x = (l_x + r_x) / 2;
    if (r[m_x] > rx) {
      r_x = m_x;
    } else {
      l_x = m_x;
    }
  }
  return l_x;
}

class FenwickOfFenwicks : public FenwickTree {
 public:
  std::vector<FenwickTree> tree_of_trees;
  int tree_of_trees_size;
  FenwickOfFenwicks(int n, std::vector<Point> points) {
    tree_of_trees_size = n;
    std::vector<std::vector<Point>> temp_l;
    for (int i = 0; i < n; ++i) {
      temp_l.push_back({});
      for (int j = F(i); j <= i; ++j) {
        temp_l[i].push_back(points[j]);
      }
      sort(temp_l[i].begin(), temp_l[i].end(), CompY);
      tree_of_trees.push_back(FenwickTree(temp_l[i], (int)temp_l[i].size()));
    }
  }
  void Update(int pos, long long val, std::vector<int>& indexes);
  long long GetPrefixAnswer(int pos, const std::vector<int>& y_pos);
  std::vector<int> FindYPos(
      int pos, int ry, const std::vector<std::vector<int>>& real_coordinate_y);
};

void FenwickOfFenwicks::Update(int pos, long long val,
                               std::vector<int>& indexes) {
  int cur = 0;
  for (int i = pos; i < tree_of_trees_size; i = G(i)) {
    int iy = indexes[cur++];
    tree_of_trees[i].Update(iy, val);
  }
}

long long FenwickOfFenwicks::GetPrefixAnswer(int pos,
                                             const std::vector<int>& y_pos) {
  long long ans1 = 0;
  int cur = 0;
  for (int i = pos; i >= 0; i = F(i) - 1) {
    ans1 += tree_of_trees[i].GetPrefixAnswer(y_pos[cur++]);
  }
  return ans1;
}

std::vector<int> FenwickOfFenwicks::FindYPos(
    int pos, int ry, const std::vector<std::vector<int>>& real_coordinate_y) {
  std::vector<int> y_pos = {};
  for (int i = pos; i >= 0; i = F(i) - 1) {
    if (real_coordinate_y[i][0] > ry) {
      y_pos.push_back(-1);
      continue;
    }
    int l_y = LowerBound(0, (int)tree_of_trees[i].tree_size, ry,
                         real_coordinate_y[i]);
    y_pos.push_back(l_y);
  }
  return y_pos;
}

struct ShrinkedData {
  std::vector<int> real_coordinate_x;
  std::vector<int> new_index_by_old_x;
  std::vector<std::vector<int>> real_coordinate_y;
  std::vector<std::vector<int>> new_index_by_old_y;
  ShrinkedData(int n, std::vector<Point>& points,
               FenwickOfFenwicks& tree_of_trees) {
    real_coordinate_x.resize(n);
    new_index_by_old_x.resize(n);
    real_coordinate_y.resize(n);
    new_index_by_old_y.resize(n);
    for (int i = 0; i < n; ++i) {
      real_coordinate_x[i] = points[i].x;
      points[i].x = i;
      new_index_by_old_x[points[i].index] = i;
    }
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < (int)tree_of_trees.tree_of_trees[i].tree.size();
           ++j) {
        real_coordinate_y[i].push_back(
            tree_of_trees.tree_of_trees[i].tree[j].y);
        new_index_by_old_y[tree_of_trees.tree_of_trees[i].tree[j].index]
            .push_back(j);
      }
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  int n;
  std::cin >> n;
  std::vector<Point> points;
  for (int i = 0; i < n; ++i) {
    int x, y, w;
    std::cin >> x >> y >> w;
    points.push_back(Point(x, y, w, i));
  }
  sort(points.begin(), points.end(), CompX);
  FenwickOfFenwicks tree_of_trees = FenwickOfFenwicks(n, points);
  ShrinkedData data = ShrinkedData(n, points, tree_of_trees);
  int m;
  std::cin >> m;
  for (int _ = 0; _ < m; ++_) {
    std::string operation;
    std::cin >> operation;
    if (operation == "get") {
      int rx, ry;
      std::cin >> rx >> ry;
      int l_x = LowerBound(0, n, rx, data.real_coordinate_x);
      if (data.real_coordinate_x[0] > rx) {
        std::cout << 0 << "\n";
        continue;
      }
      long long ans = tree_of_trees.GetPrefixAnswer(
          l_x, tree_of_trees.FindYPos(l_x, ry, data.real_coordinate_y));
      std::cout << ans << "\n";
    } else {
      int i, z;
      std::cin >> i >> z;
      int ix = data.new_index_by_old_x[i - 1];
      tree_of_trees.Update(ix, z, data.new_index_by_old_y[i - 1]);
    }
  }
}
