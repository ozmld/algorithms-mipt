#include <iostream>

const long long kInf = 1e10;

struct Node {
  long long key;
  size_t h;
  Node *l, *r;
  Node() : key(0), h(0), l(nullptr), r(nullptr) {}
  Node(long long k) : key(k), h(1), l(nullptr), r(nullptr) {}
};
class AVL {
 public:
  AVL() : root_(nullptr) {}
  AVL(Node* t) : root_(t) {}
  Node* Insert(long long x);
  bool Find(long long x) const;
  long long Next(long long x) const;
  void Delete();

 private:
  Node* root_;
  size_t Depth() const;
  int DepthDiffer() const;
  size_t UpdateDepth();
  Node* LeftRotate();
  Node* RightRotate();
  void BigLeftRotate();
  void BigRightRotate();
  void FixAVL();
};
Node* AVL::Insert(long long x) {
  if (root_ == nullptr) {
    return new Node(x);
  }
  if (x <= root_->key) {
    root_->l = AVL(root_->l).Insert(x);
  } else {
    root_->r = AVL(root_->r).Insert(x);
  }
  FixAVL();
  return root_;
}
bool AVL::Find(long long x) const {
  if (root_ == nullptr) {
    return false;
  }
  if (x == root_->key) {
    return true;
  }
  if (x > root_->key) {
    return AVL(root_->r).Find(x);
  }
  return AVL(root_->l).Find(x);
}
long long AVL::Next(long long x) const {
  if (root_ == nullptr) {
    return kInf;
  }
  if (x == root_->key) {
    return root_->key;
  }
  if (x > root_->key) {
    return AVL(root_->r).Next(x);
  }
  return std::min(AVL(root_->l).Next(x), root_->key);
}
void AVL::Delete() {
  if (root_ == nullptr) {
    return;
  }
  AVL(root_->l).Delete();
  AVL(root_->r).Delete();
  delete root_;
}
size_t AVL::Depth() const {
  if (root_ == nullptr) {
    return 0;
  }
  return root_->h;
}
int AVL::DepthDiffer() const {
  if (root_ == nullptr) {
    return 0;
  }
  return AVL(root_->r).Depth() - AVL(root_->l).Depth();
}
size_t AVL::UpdateDepth() {
  return std::max(AVL(root_->l).Depth(), AVL(root_->r).Depth()) + 1;
}
Node* AVL::LeftRotate() {
  Node* temp = root_->r;
  root_->r = temp->l;
  temp->l = root_;
  root_->h = UpdateDepth();
  temp->h = AVL(temp).UpdateDepth();
  root_ = temp;
  return root_;
}
Node* AVL::RightRotate() {
  Node* temp = root_->l;
  root_->l = temp->r;
  temp->r = root_;
  root_->h = UpdateDepth();
  temp->h = AVL(temp).UpdateDepth();
  root_ = temp;
  return root_;
}
void AVL::BigLeftRotate() {
  root_->l = AVL(root_->l).LeftRotate();
  root_ = RightRotate();
}
void AVL::BigRightRotate() {
  root_->r = AVL(root_->r).RightRotate();
  root_ = LeftRotate();
}
void AVL::FixAVL() {
  root_->h = UpdateDepth();
  if (DepthDiffer() == -2) {
    if (AVL(root_->l).DepthDiffer() == 1) {
      BigLeftRotate();
    } else {
      root_ = RightRotate();
    }
  }
  if (DepthDiffer() == 2) {
    if (AVL(root_->r).DepthDiffer() == -1) {
      BigRightRotate();
    } else {
      root_ = LeftRotate();
    }
  }
}

int main() {
  size_t n;
  std::cin >> n;
  AVL tree;
  long long y = 0;
  long long mod = 1e9;
  for (size_t i = 0; i < n; ++i) {
    char operation;
    std::cin >> operation;
    if (operation == '+') {
      long long x;
      std::cin >> x;
      x = (x + y + mod) % mod;
      if (!tree.Find(x)) {
        tree = tree.Insert(x);
      }
      y = 0;
    } else {
      long long x;
      std::cin >> x;
      y = tree.Next(x);
      if (y == kInf) {
        y = -1;
      }
      std::cout << y << "\n";
    }
  }
  tree.Delete();
}
