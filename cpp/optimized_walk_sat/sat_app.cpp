#include <iostream>
#include <chrono>
#include <assert.h>
#include "SAT.h"

int main() {
  std::cout << "Welcome to WalkSAT Solver" << std::endl;
  char file_path[] = "../../sat_problems/4-queens.txt";

  SAT* sat_solver = new SAT();
  sat_solver->load_from_file(file_path);

  auto start = std::chrono::high_resolution_clock::now();
  std::map<int, bool> result = sat_solver->walk_sat(0.5, 200000);
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Walk-SAT took "<< duration.count()/1000.0 << " seconds to run." << std::endl;

  assert(sat_solver->verify_model(result));
  //sat_solver->display_model(result);
  delete sat_solver;

  return 0;
}
