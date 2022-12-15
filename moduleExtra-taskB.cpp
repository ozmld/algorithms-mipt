#include <iostream>
#include <vector>

class Image {
public:
  Image(std::vector<std::vector<int>> a, size_t h, size_t w) : data(a), height(h), width(w) {}
  size_t CountConnectivityComponent();
 private:
  std::vector<std::vector<int>> data;
  size_t height, width;
  int GetPixel(int row, int column) const {
    return ((row < 0) || (column < 0) || (row >= height) || (column >= width)) ? 0 : data[row][column];
  }
  bool ExistNeighbour(int row, int column) const ;
  void DeleteConnectivityComponent(int row, int column);
};
bool Image::ExistNeighbour(int row, int column) const {
  size_t counter = 0;
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      counter += (i != 0 || j != 0) ? GetPixel(row + i, column + j) : 0;
    }
  }
  return counter > 0;
}
void Image::DeleteConnectivityComponent(int row, int column) {
  if (GetPixel(row, column) == 0) return;
  data[row][column] = 0;
  if (ExistNeighbour(row, column)) {
    for (int i = -1; i <= 1; ++i) {
      for (int j = -1; j <= 1; ++j) {
        if ((i != 0 || j != 0) && GetPixel(row + i, column + j)) DeleteConnectivityComponent(row + i, column + j);
      }
    }
  }
}
size_t Image::CountConnectivityComponent() {
  size_t counter = 0;
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      if (GetPixel(i, j) == 0) continue;
      ++counter;
      DeleteConnectivityComponent(i, j);
    }
  }
  return counter;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  std::vector<std::vector<int>> image;
  for (size_t i = 0; i < m; ++i) {
    image.push_back({});
    for (size_t j = 0; j < n; ++j) {
      image[i].push_back(0);
      std::cin >> image[i][j];
    }
  }
  Image pict(image, m, n);
  std::cout << pict.CountConnectivityComponent();
}
