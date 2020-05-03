#include <vector>
#include <string>
#include <map>
#include <set>

#ifndef __SAT_H__
#define __SAT_H__

class SAT {
  public:
    SAT();
    ~SAT();
    std::map<int, bool> walk_sat(float p, int max_flips);
    void load_from_file(char* file_path);
    void display_model(const std::map<int, bool>& model);
    bool verify_model(const std::map<int, bool>& model);
  private:
    std::map<std::string, std::vector<int>> _clauses;
    std::set<std::string> _unsat_clauses;
    int _num_clauses;
    int _num_variables;

    void check_assignment(const std::map<int, bool>& model, int changed_symbol);
    bool check_clause(const std::vector<int>& clause, const std::map<int, bool>& model);
    int count_unsat_change(std::map<int, bool>& model, int var);
    bool symbol_exists(const std::vector<int>& clause, int symbol);
    //Utils
    std::vector<std::string> split(const std::string& sentence, char delimiter);
    std::string clause_to_string(const std::vector<int>& clause);
};

#endif
