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
 private:
  std::vector<long long> dataset_;

 protected:
  int F(int i) const;
  int G(int i) const;

 public:
  std::vector<Point> tree;
  size_t tree_size;
  long long GetPrefixAnswer(int pos) const;
  void Update(int pos, long long val);
  FenwickTree() {
    tree_size = 0;
    tree = {};
    dataset_ = {};
  }
  FenwickTree(std::vector<Point> data) {
    tree_size = data.size();
    dataset_ = {};
    tree = {};
    for (size_t i = 0; i < tree_size; ++i) {
      dataset_.push_back(0);
      tree.push_back(data[i]);
      tree[i].weight = 0;
    }
    for (size_t i = 0; i < tree_size; ++i) {
      Update(i, data[i].weight);
    }
  }
};

int FenwickTree::F(int i) const { return i & (i + 1); }

int FenwickTree::G(int i) const { return i | (i + 1); }

long long FenwickTree::GetPrefixAnswer(int pos) const {
  long long ans = 0;
  for (int i = pos; i >= 0; i = F(i) - 1) {
    ans += tree[i].weight;
  }
  return ans;
}

void FenwickTree::Update(int pos, long long val) {
  long long delta = val - dataset_[pos];
  dataset_[pos] = val;
  for (size_t i = pos; i < tree_size; i = G(i)) {
    tree[i].weight += delta;
  }
}

bool CompX(const Point& p, const Point& q) { return p.x < q.x; }

bool CompY(const Point& p, const Point& q) { return p.y < q.y; }

int LowerBound(int l_x, int r_x, int rx, const std::vector<Point>& r) {
  while (r_x - l_x > 1) {
    int m_x = (l_x + r_x) / 2;
    if (r[m_x].y > rx) {
      r_x = m_x;
    } else {
      l_x = m_x;
    }
  }
  return l_x;
}

class FenwickOfFenwicks : public FenwickTree {
 private:
  std::vector<int> real_coordinate_x_;
  std::vector<int> new_index_by_old_x_;
  std::vector<std::vector<int>> new_index_by_old_y_;
  int FindXPos(int l, int r, int pivot) const;
  std::vector<int> FindYPos(int pos, int ry) const;
  std::vector<FenwickTree> tree_of_trees_;
  int tree_of_trees_size_;

 public:
  FenwickOfFenwicks(std::vector<Point> points) {
    size_t n = points.size();
    real_coordinate_x_.resize(n);
    new_index_by_old_x_.resize(n);
    new_index_by_old_y_.resize(n);
    tree_of_trees_size_ = n;
    sort(points.begin(), points.end(), CompX);
    std::vector<Point> temp_l;
    for (size_t i = 0; i < n; ++i) {
      real_coordinate_x_[i] = points[i].x;
      points[i].x = i;
      new_index_by_old_x_[points[i].index] = i;

      temp_l = {};
      for (size_t j = F(i); j <= i; ++j) {
        temp_l.push_back(points[j]);
      }
      sort(temp_l.begin(), temp_l.end(), CompY);
      tree_of_trees_.push_back(FenwickTree(temp_l));
      for (size_t j = 0; j < temp_l.size(); ++j) {
        new_index_by_old_y_[temp_l[j].index].push_back(j);
      }
    }
  }
  void Update(int index, long long val);
  long long GetPrefixAnswer(int rx, int ry) const;
};

int FenwickOfFenwicks::FindXPos(int l, int r, int pivot) const {
  while (r - l > 1) {
    int m = (l + r) / 2;
    if (real_coordinate_x_[m] > pivot) {
      r = m;
    } else {
      l = m;
    }
  }
  return l;
}
void FenwickOfFenwicks::Update(int index, long long val) {
  int cur = 0;
  int pos = new_index_by_old_x_[index - 1];
  for (int i = pos; i < tree_of_trees_size_; i = G(i)) {
    int iy = new_index_by_old_y_[index - 1][cur++];
    tree_of_trees_[i].Update(iy, val);
  }
}

long long FenwickOfFenwicks::GetPrefixAnswer(int rx, int ry) const {
  if (real_coordinate_x_[0] > rx) {
    return 0;
  }
  int pos = FindXPos(0, tree_of_trees_size_, rx);
  long long ans1 = 0;
  int cur = 0;
  std::vector<int> y_pos = FindYPos(pos, ry);
  for (int i = pos; i >= 0; i = F(i) - 1) {
    ans1 += tree_of_trees_[i].GetPrefixAnswer(y_pos[cur++]);
  }
  return ans1;
}

std::vector<int> FenwickOfFenwicks::FindYPos(int pos, int ry) const {
  std::vector<int> y_pos = {};
  for (int i = pos; i >= 0; i = F(i) - 1) {
    if (tree_of_trees_[i].tree[0].y > ry) {
      y_pos.push_back(-1);
      continue;
    }
    int l = 0, r = (int)tree_of_trees_[i].tree_size;
    while (r - l > 1) {
      int m = (l + r) / 2;
      if (tree_of_trees_[i].tree[m].y > ry) {
        r = m;
      } else {
        l = m;
      }
    }
    y_pos.push_back(l);
  }
  return y_pos;
}

int main() {
  int n;
  std::cin >> n;
  std::vector<Point> points;
  for (int i = 0; i < n; ++i) {
    int x, y, w;
    std::cin >> x >> y >> w;
    points.push_back(Point(x, y, w, i));
  }
  FenwickOfFenwicks tree_of_trees = FenwickOfFenwicks(points);
  int m;
  std::cin >> m;
  for (int _ = 0; _ < m; ++_) {
    std::string operation;
    std::cin >> operation;
    if (operation == "get") {
      int rx, ry;
      std::cin >> rx >> ry;
      long long ans = tree_of_trees.GetPrefixAnswer(rx, ry);
      std::cout << ans << "\n";
    } else {
      int i, z;
      std::cin >> i >> z;
      tree_of_trees.Update(i, z);
    }
  }
}
