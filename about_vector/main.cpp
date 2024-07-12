// #if defined(_MSC_VER) && (_MSC_VER >= 1600)
// #pragma execution_character_set("utf-8")
// #endif

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

// using namespace std;
/*
void demo_of_swap() {
  std::vector<int> vec_a({1, 2, 3});
  foreach (auto var, vec_a) {
    std::cout << var << std ::endl;
  }

  std::vector<int> vec_b = {4, 5, 6};
  vec_a.swap(vec_b);

  std::cout << "vec_a:" << std ::endl;
  foreach (auto var, vec_a) {
    std::cout << var << std ::endl;
  }

  std::cout << "vec_b:" << std ::endl;
  foreach (auto var, vec_b) {
    std::cout << var << std ::endl;
  }
}
*/

struct Obj {
  int id;
  double val;
};

void updateOrInsertObj(std::vector<Obj>& obj_vec, const Obj& new_obj) {

  auto it =
      std::find_if(obj_vec.begin(), obj_vec.end(),
                   [&new_obj](const Obj& obj) { return obj.id == new_obj.id; });

  if (it != obj_vec.end()) {

    *it = new_obj;
  } else {

    obj_vec.push_back(new_obj);
  }
}

void printObjVec(const std::vector<Obj>& obj_vec) {
  for (const auto& obj : obj_vec) {
    std::cout << "ID: " << obj.id << ", Value: " << obj.val << std::endl;
  }
}

int main() {
  std::cout << "Hello World!" << std::endl;
  std::vector<Obj> obj_vec = {{1, 10.5}, {2, 20.7}, {3, 30.9}};

  Obj new_obj = {2, 50.0};

  updateOrInsertObj(obj_vec, new_obj);
  printObjVec(obj_vec);

  std::cout << "Hello World!" << std::endl;

  new_obj = {4, 40.0};
  updateOrInsertObj(obj_vec, new_obj);
  printObjVec(obj_vec);

  std::cout << "Hello World!" << std::endl;
  // demo_of_swap();

  return 0;
}
