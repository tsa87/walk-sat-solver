#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <random>
#include "SAT.h"

const std::string END_OF_LINE = "0";
const char DELIMITER = ' ';

SAT::SAT() {
}

SAT::~SAT() {
}


std::map<int, bool> SAT::walk_sat(float p, int max_flips) {
  std::map<int, bool> model;

  srand(time(0));
  for (int i = 1; i < this->_num_variables+1; i++) {
    model[i] = (rand() % 2 == 0 ? true : false);
  }
  check_assignment(model, 0);

  int num_flips = 0;
  while (num_flips < max_flips) {
    if (_unsat_clauses.empty()) return model;

    auto it = _unsat_clauses.begin();
    std::advance(it, rand()%_unsat_clauses.size());
    std::vector<int> choosen_clause = _clauses[*it];

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    int choosen_var;
    if (distribution(generator) < p) {
      choosen_var = abs(choosen_clause[rand()%choosen_clause.size()]);
    }
    else {
      int var;
      int least_unsat_count = _num_clauses;
      for (int i = 0; i < choosen_clause.size(); i++) {
        int var = abs(choosen_clause[i]);
        int unsat_change = count_unsat_change(model, var);
        if (unsat_change < least_unsat_count){
          choosen_var = var;
          least_unsat_count = unsat_change;
        }
      }
    }
    model[choosen_var] = !model[choosen_var];
    check_assignment(model, choosen_var);

    num_flips++;
  }

  model.clear();  //If no solution found, return {0: false}
  model[0] = false;
  return model;
}


void SAT::check_assignment(const std::map<int, bool>& model, int changed_symbol) {
  // On initalization only
  if (changed_symbol == 0){
    for (auto it = _clauses.begin(); it != _clauses.end(); ++it) {
      if (!check_clause(it->second, model)) _unsat_clauses.insert(it->first);
    }
  }
  else {
    for (auto it = _clauses.begin(); it != _clauses.end(); ++it) {
      if (symbol_exists(it->second, changed_symbol)) {
        if(check_clause(it->second, model)){
          _unsat_clauses.erase(it->first);
        }
        else {
          _unsat_clauses.insert(it->first);
        }
      }
    }
  }
}

bool SAT::verify_model(const std::map<int, bool>& model) {
  for (auto it = _clauses.begin(); it != _clauses.end(); ++it) {
    if (!check_clause(it->second, model)) return false;
  }
  return true;
}

bool SAT::symbol_exists(const std::vector<int>& clause, int symbol){
  for (int i = 0; i < clause.size(); i ++) {
    if (abs(clause[i]) == symbol) return true;
  }
  return false;
}

bool SAT::check_clause(const std::vector<int>& clause, const std::map<int, bool>& model) {
  for(std::vector<int>::const_iterator it = clause.begin(); it != clause.end(); ++it){
    if (((*it > 0) && (model.at(abs(*it)))) || ((*it < 0) && (!model.at(abs(*it))))) {
      return true;
    }
  }
  return false;
}


int SAT::count_unsat_change(std::map<int, bool>& model, int var) {
  model[var] = !model[var];
  int change = 0;
  for (auto it = _clauses.begin(); it != _clauses.end(); ++it) {
    if (symbol_exists(it->second, var)) {
      bool result = check_clause(it->second, model);
      if(result && (_unsat_clauses.find(it->first) != _unsat_clauses.end())) change--;
      else if (!result && (_unsat_clauses.find(it->first) == _unsat_clauses.end())) change++;
    }
  }
  model[var] = !model[var];
  return change;
}

//Utility Functions
void SAT::load_from_file(char* file_path) {
  std::string line;
  std::ifstream input_file (file_path);
  if (input_file.is_open()) {
    getline(input_file, line); //ignore comment
    getline(input_file, line); //read the config
    std::vector<std::string> tokens = split(line, DELIMITER);

    this->_num_variables = std::stoi (tokens[2]);
    this->_num_clauses = std::stoi (tokens[3]);

    for (int i = 0; i < this->_num_clauses; i++) {
      getline(input_file, line);
      tokens = split(line, DELIMITER);
      std::vector<int> clause;
      for (int j = 0; j < tokens.size()-1; j++) {
        clause.push_back( std::stoi(tokens[j]) );
      }
      std::pair<std::string, std::vector<int>> entry;
      entry = std::make_pair (clause_to_string(clause), clause);
      _clauses.insert(entry);
    }
    input_file.close();
  }
}


std::string SAT::clause_to_string(const std::vector<int>& clause) {
  std::string string_encode = "C";
  for (int i = 0; i < clause.size(); i++) {
    string_encode = string_encode + " " + std::to_string(clause[i]);
  }
  return string_encode;
}

std::vector<std::string> SAT::split(const std::string& sentence, char delimiter) {
  std::vector<std::string> tokens;

  int start_index = 0;
  int word_len = 0;
  bool inword = false;

  for (int i = 0; i < sentence.length(); i++) {
    if (sentence[i] == delimiter) {
      if (inword) {
        std::string token = sentence.substr(start_index, word_len);
        tokens.push_back(token);
      }
      start_index = i+1;
      word_len = 0;
      inword = false;
    }
    else {
      word_len ++;
      inword = true;
    }
  }
  if (inword) tokens.push_back(sentence.substr(start_index, word_len));

  return tokens;
}

void SAT::display_model(const std::map<int, bool>& model) {
  for(std::map<int, bool>::const_iterator var = model.begin(); var != model.end(); ++var){
    std::cout << var->first << " " << var->second << std::endl;
  }
}
