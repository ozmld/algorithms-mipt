#include <fstream>
#include <random>

const long long kInf = 1e10;
struct Node {
  long long key, prior, size;
  Node* l;
  Node* r;
  Node() {}
  Node(long long k) {
    /// For random priorities
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<long long> uni(1, 1e6);

    prior = uni(rng);
    key = k;
    size = 1;
    l = nullptr;
    r = nullptr;
  }
  Node(std::pair<long long, long long> node)
      : key(node.first), prior(node.second), size(0), l(nullptr), r(nullptr) {}
};
class Treap;
class Treap {
 public:
  Treap() : root_(nullptr) {}
  Treap(Node* t) : root_(t) {}
  void Insert(Node* element) {
    std::pair<Treap, Treap> splitted = Split(element->key);
    root_ =
        Merge(splitted.first.Merge(splitted.first, element), splitted.second)
            .root_;
  }
  long long Kth(long long k) const {
    if (root_ == nullptr) {
      return -1;
    }
    if (LeftChild().Size() == (k - 1)) {
      return root_->key;
    }
    if (LeftChild().Size() >= k) {
      return LeftChild().Kth(k);
    }
    return RightChild().Kth(k - LeftChild().Size() - 1);
  }
  long long Next(long long x) const {
    if (root_ == nullptr) {
      return kInf;
    }
    if ((x + 1) == root_->key) {
      return root_->key;
    }
    if ((x + 1) > root_->key) {
      return RightChild().Next(x);
    }
    return std::min(LeftChild().Next(x), root_->key);
  }
  long long Prev(long long x) const {
    if (root_ == nullptr) {
      return -1;
    }
    if ((x - 1) == root_->key) {
      return root_->key;
    }
    if ((x - 1) < root_->key) {
      return LeftChild().Prev(x);
    }
    return std::max(RightChild().Prev(x), root_->key);
  }
  bool Find(long long x) const {
    if (root_ == nullptr) {
      return false;
    }
    if (root_->key == x) {
      return true;
    }
    if (x > root_->key) {
      return RightChild().Find(x);
    }
    return LeftChild().Find(x);
  }
  void Delete(int x) {
    std::pair<Treap, Treap> splitted1 = Split(x);
    std::pair<Treap, Treap> splitted2 = splitted1.second.Split(x + 1);
    splitted2.first.Clear();
    *this = Merge(splitted1.first, splitted2.second);
  }
  void Clear() {
    if (root_ == nullptr) {
      return;
    }
    LeftChild().Clear();
    RightChild().Clear();
    delete root_;
  }

 private:
  Node* root_;
  long long Size() const {
    if (root_ == nullptr) {
      return 0;
    }
    return root_->size;
  }
  void Update() {
    if (root_ != nullptr) {
      root_->size = 1 + LeftChild().Size() + RightChild().Size();
    }
  }
  Treap Merge(Treap t1, Treap t2) {
    if (t1.root_ == nullptr || t2.root_ == nullptr) {
      return (t1.root_ == nullptr) ? t2 : t1;
    }
    if (t1.root_->prior > t2.root_->prior) {
      t1.root_->r = Merge(Treap(t1.root_->r), t2).root_;
      root_ = t1.root_;
      Update();
      return (Treap(root_));
    }
    t2.root_->l = Merge(t1, Treap(t2.root_->l)).root_;
    root_ = t2.root_;
    Update();
    return (Treap(root_));
  }
  std::pair<Treap, Treap> Split(long long k) {
    if (root_ == nullptr) {
      return std::make_pair(Treap(), Treap());
    }
    if (k > root_->key) {
      std::pair<Treap, Treap> splitted = RightChild().Split(k);
      root_->r = splitted.first.root_;
      Update();
      return std::make_pair(*this, splitted.second);
    }
    std::pair<Treap, Treap> splitted = LeftChild().Split(k);
    root_->l = splitted.second.root_;
    Update();
    return std::make_pair(splitted.first, *this);
  }
  Treap LeftChild() const { return Treap(root_->l); }
  Treap RightChild() const { return Treap(root_->r); }
};

std::string ParseOperation(Treap& treap, std::string operation, long long x) {
  if (operation == "insert") {
    if (!treap.Find(x)) {
      Node* elem = new Node(x);
      treap.Insert(elem);
    }
    return "";
  }
  if (operation == "exists") {
    if (treap.Find(x)) {
      return "true\n";
    }
    return "false\n";
  }
  if (operation == "next") {
    long long y = treap.Next(x);
    if (y == kInf) {
      return "none\n";
    }
    return std::to_string(y) + "\n";
  }
  if (operation == "prev") {
    long long y = treap.Prev(x);
    if (y == -1) {
      return "none\n";
    }
    return std::to_string(y) + "\n";
  }
  if (operation == "kth") {
    long long y = treap.Kth(x + 1);
    if (y == -1) {
      return "none\n";
    }
    return std::to_string(y) + "\n";
  }
  treap.Delete(x);
  return "";
}
int main() {
  std::ifstream is("input.txt");
  std::ofstream os("output.txt");
  std::string operation;
  long long x;
  Treap treap;
  while (is >> operation >> x) {
    os << ParseOperation(treap, operation, x);
  }
  os.close();
  treap.Clear();
}
