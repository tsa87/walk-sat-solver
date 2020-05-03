#include <iostream>
#include <chrono>
#include "SAT.h"

//'../sat_problems/4-queens.txt'
//'../sat_problems/generated_3-CSP_m=7500_n=5000.txt'
//'../sat_problems/generated_3-CSP_m=10000_n=5000.txt'
//'../sat_problems/generated_3-CSP_m=12500_n=5000.txt'
//'../sat_problems/generated_3-CSP_m=15000_n=5000.txt'

int main() {
  std::cout << "Hello, World!" << std::endl;
  char file_path[] = "../../sat_problems/generated_3-CSP_m=12500_n=5000.txt";

  SAT* sat_solver = new SAT();
  sat_solver->load_from_file(file_path);

  auto start = std::chrono::high_resolution_clock::now();
  std::map<int, bool> result = sat_solver->walk_sat(0.5, 200000);
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Walk-SAT took "<< duration.count()/1000.0 << " seconds to run." << std::endl;

  //sat_solver->display_model(result);
  delete sat_solver;

  return 0;
}
