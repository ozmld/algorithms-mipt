#include <iostream>

const long long kInf = 1e10;

struct Node {
  long long key;
  size_t h;
  Node *l, *r;
  Node() : key(0), h(0), l(nullptr), r(nullptr) {}
  Node(long long k) : key(k), h(1), l(nullptr), r(nullptr) {}
};

size_t Depth(Node* t) {
  if (t == nullptr) {
    return 0;
  }
  return t->h;
}

int DepthDiffer(Node* t) {
  if (t == nullptr) {
    return 0;
  }
  return Depth(t->r) - Depth(t->l);
}

void UpdateDepth(Node* t) { t->h = std::max(Depth(t->l), Depth(t->r)) + 1; }

void LeftRotate(Node*& t) {
  Node* temp = t->r;
  t->r = temp->l;
  temp->l = t;
  UpdateDepth(t);
  UpdateDepth(temp);
  t = temp;
}

void RightRotate(Node*& t) {
  Node* temp = t->l;
  t->l = temp->r;
  temp->r = t;
  UpdateDepth(t);
  UpdateDepth(temp);
  t = temp;
}

void BigLeftRotate(Node*& t) {
  LeftRotate(t->l);
  RightRotate(t);
}

void BigRightRotate(Node*& t) {
  RightRotate(t->r);
  LeftRotate(t);
}

void FixAVL(Node*& t) {
  UpdateDepth(t);
  if (DepthDiffer(t) == -2) {
    if (DepthDiffer(t->l) == 1) {
      BigLeftRotate(t);
    } else {
      RightRotate(t);
    }
  }
  if (DepthDiffer(t) == 2) {
    if (DepthDiffer(t->r) == -1) {
      BigRightRotate(t);
    } else {
      LeftRotate(t);
    }
  }
}

void Insert(Node*& t, long long x) {
  if (t == nullptr) {
    t = new Node(x);
    return;
  }
  if (x <= t->key) {
    Insert(t->l, x);
  } else {
    Insert(t->r, x);
  }
  FixAVL(t);
}

bool Find(Node* t, long long x) {
  if (t == nullptr) {
    return false;
  }
  if (x == t->key) {
    return true;
  }
  if (x > t->key) {
    return Find(t->r, x);
  }
  return Find(t->l, x);
}

long long Next(Node* t, long long x) {
  if (t == nullptr) {
    return kInf;
  }
  if (x == t->key) {
    return t->key;
  }
  if (x > t->key) {
    return Next(t->r, x);
  }
  return std::min(Next(t->l, x), t->key);
}

void Delete(Node* t) {
  if (t == nullptr) {
    return;
  }
  Delete(t->l);
  Delete(t->r);
  delete t;
}

int main() {
  size_t n;
  std::cin >> n;
  Node* t = nullptr;
  long long y = 0;
  long long mod = 1e9;
  for (size_t i = 0; i < n; ++i) {
    char operation;
    std::cin >> operation;
    if (operation == '+') {
      long long x;
      std::cin >> x;
      x = (x + y + mod) % mod;
      if (!Find(t, x)) {
        Insert(t, x);
      }
      y = 0;
    } else {
      long long x;
      std::cin >> x;
      y = Next(t, x);
      if (y == kInf) {
        y = -1;
      }
      std::cout << y << "\n";
    }
  }
  Delete(t);
}
