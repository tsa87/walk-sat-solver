#include <vector>
#include <string>
#include <map>
#include <set>

#ifndef __RESOLUTION_PROVER_H___
#define __RESOLUTION_PROVER_H___

typedef std::set<int> literal_set;
typedef std::pair<std::string, literal_set> clause_entry;
typedef std::map<std::string, literal_set> clause_map;

class ResolutionProver {
  public:
    ResolutionProver();
    ~ResolutionProver();

    //Resolution Prover
    bool pl_resolution();

    //Utilities
    void display_clauses();
    void load_from_file(char* file_path);

  private:
    clause_map _clauses;
    int _num_clauses;
    int _num_variables;

    clause_map pl_resolve(const literal_set& ci, const literal_set& cj);
    literal_set tokens_to_clause(const std::vector<std::string>& tokens);
    std::string clause_to_string(const literal_set& clause);

    std::vector<std::string> split(const std::string& sentence, char delimiter);
};

#endif
