#include <fstream>
#include <random>

const long long kInf = 1e10;
struct Node {
  long long key, prior, size;
  Node* l;
  Node* r;
  Node() {}
  Node(long long k, long long p)
      : key(k), prior(p), size(1), l(nullptr), r(nullptr) {}
  Node(std::pair<long long, long long> node)
      : key(node.first), prior(node.second), size(0), l(nullptr), r(nullptr) {}
};

long long Size(Node* t) {
  if (t == nullptr) {
    return 0;
  }
  return t->size;
}
void Update(Node* t) {
  if (t != nullptr) {
    t->size = 1 + Size(t->l) + Size(t->r);
  }
}

void Merge(Node*& t, Node* t1, Node* t2) {
  if (t1 == nullptr || t2 == nullptr) {
    t = (t1 == nullptr) ? t2 : t1;
    return;
  }
  if (t1->prior > t2->prior) {
    Merge(t1->r, t1->r, t2);
    t = t1;
    Update(t);
    return;
  }
  Merge(t2->l, t1, t2->l);
  t = t2;
  Update(t);
}

void Split(Node* t, long long k, Node*& t1, Node*& t2) {
  if (t == nullptr) {
    t1 = nullptr;
    t2 = nullptr;
    return;
  }
  if (k > t->key) {
    Split(t->r, k, t->r, t2);
    t1 = t;
    Update(t1);
    return;
  }
  Split(t->l, k, t1, t->l);
  t2 = t;
  Update(t2);
}
void Insert(Node*& t, Node* element) {
  Node *t1 = nullptr, *t2 = nullptr;
  Split(t, element->key, t1, t2);
  Merge(t1, t1, element);
  Merge(t, t1, t2);
}

long long Kth(Node* t, long long k) {
  if (t == nullptr) {
    return -1;
  }
  if (Size(t->l) == (k - 1)) {
    return t->key;
  }
  if (Size(t->l) >= k) {
    return Kth(t->l, k);
  }
  return Kth(t->r, k - Size(t->l) - 1);
}
long long Next(Node* t, long long x) {
  if (t == nullptr) {
    return kInf;
  }
  if ((x + 1) == t->key) {
    return t->key;
  }
  if ((x + 1) > t->key) {
    return Next(t->r, x);
  }
  return std::min(Next(t->l, x), t->key);
}
long long Prev(Node* t, long long x) {
  if (t == nullptr) {
    return -1;
  }
  if ((x - 1) == t->key) {
    return t->key;
  }
  if ((x - 1) < t->key) {
    return Prev(t->l, x);
  }
  return std::max(Prev(t->r, x), t->key);
}

bool Find(Node* t, long long x) {
  if (t == nullptr) {
    return false;
  }
  if (t->key == x) {
    return true;
  }
  if (x > t->key) {
    return Find(t->r, x);
  }
  return Find(t->l, x);
}
void Delete(Node*& t) {
  if (t == nullptr) {
    return;
  }
  Delete(t->l);
  Delete(t->r);
  delete t;
}
void Delete(Node*& t, int x) {
  Node *t1 = nullptr, *t2 = nullptr, *t3 = nullptr;
  Split(t, x, t1, t2);
  Split(t2, x + 1, t2, t3);
  Delete(t2);
  Merge(t, t1, t3);
}
std::string ParseOperation(Node*& t, std::string operation, long long x) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<long long> uni(1, 1e6);
  if (operation == "insert") {
    if (!Find(t, x)) {
      long long r = uni(rng);
      Node* elem = new Node(x, r);
      Insert(t, elem);
    }
    return "";
  }
  if (operation == "exists") {
    if (Find(t, x)) {
      return "true\n";
    }
    return "false\n";
  }
  if (operation == "next") {
    long long y = Next(t, x);
    if (y == kInf) {
      return "none\n";
    }
    return std::to_string(y) + "\n";
  }
  if (operation == "prev") {
    long long y = Prev(t, x);
    if (y == -1) {
      return "none\n";
    }
    return std::to_string(y) + "\n";
  }
  if (operation == "kth") {
    long long y = Kth(t, x + 1);
    if (y == -1) {
      return "none\n";
    }
    return std::to_string(y) + "\n";
  }
  Delete(t, x);
  return "";
}
int main() {
  std::ifstream is("input.txt");
  std::ofstream os("output.txt");
  std::string operation;
  long long x;
  Node* t = nullptr;
  while (is >> operation >> x) {
    os << ParseOperation(t, operation, x);
  }
  os.close();
  Delete(t);
}
