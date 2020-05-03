#include <iostream>
#include "resolution_prover.h"

int main() {
  char file_path[] = "../input/4-queens.txt";

  ResolutionProver* resolution_prover = new ResolutionProver();
  resolution_prover->load_from_file(file_path);

  bool result = resolution_prover->pl_resolution();
  std::cout << result << std::endl;

  delete resolution_prover;
}
