#include <vector>
#include <string>
#include <map>

#ifndef __SAT_H__
#define __SAT_H__

class SAT {
  public:
    SAT();
    ~SAT();
    void load_from_file(char* file_path);
    std::map<int, bool> walk_sat(float p, int max_flips);
    std::vector<std::vector<int>> get_clauses();
    int get_num_clauses();
    int get_num_variables();

    void display_clauses();
    void display_model(const std::map<int, bool>& model);

  private:
    std::vector<std::vector<int>> _clauses;
    int _num_clauses;
    int _num_variables;

    //returns clauses not satisfied by the model for the problem
    std::vector<std::vector<int>> check_assignment(const std::map<int, bool>& model);

    //check if the model satisfies the disjointed  literals clause.
    bool check_clause(const std::vector<int>& clause, const std::map<int, bool>& model);

    //split strings by the delimiter into a vector of substrings.
    std::vector<std::string> split(const std::string& sentence, char delimiter);

    //counts the number of clauses statisfied by the model if we flip var's value.
    int count_num_sat(int var, std::map<int, bool>& model);
};

#endif
