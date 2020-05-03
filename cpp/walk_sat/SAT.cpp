#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <random>
#include <map>
#include "SAT.h"

const std::string END_OF_LINE = "0";
const char DELIMITER = ' ';

SAT::SAT() {
}

SAT::~SAT() {
}

std::vector<std::vector<int>> SAT::get_clauses() { return this->_clauses; }
int SAT::get_num_clauses() { return this->_num_clauses; }
int SAT::get_num_variables() { return this->_num_variables; }


void SAT::load_from_file(char* file_path) {
  std::string line;
  std::ifstream input_file (file_path);
  if (input_file.is_open()) {
    getline(input_file, line); //ignore comment
    getline(input_file, line); //read the config
    std::vector<std::string> tokens = split(line, DELIMITER);

    this->_num_variables = std::stoi (tokens[2]);
    this->_num_clauses = std::stoi (tokens[3]);

    std::vector<std::vector<int>> clauses(this->_num_clauses, std::vector<int>());
    this->_clauses = clauses;

    int len = 0;
    for (int i = 0; i < this->_num_clauses; i++) {
      getline(input_file, line);
      tokens = split(line, DELIMITER);
      len = tokens.size();

      assert(tokens[len-1] == END_OF_LINE);

      for (int j = 0; j < len-1; j++) {
        this->_clauses[i].push_back( std::stoi(tokens[j]) );
      }
    }
    input_file.close();
  }
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
    }
    else {
      word_len ++;
      inword = true;
    }
  }
  if (inword) tokens.push_back(sentence.substr(start_index, word_len));

  return tokens;
}

std::map<int, bool> SAT::walk_sat(float p, int max_flips) {
  std::map<int, bool> model;

  srand(time(0));
  for (int i = 1; i < this->_num_variables+1; i++) {
    model[i] = (rand() % 2 == 0 ? true : false);
  }
  std::vector<std::vector<int>> unsat_clauses = check_assignment(model);

  int num_flips = 0;
  while (num_flips < max_flips) {
    if (unsat_clauses.empty()) return model;
    std::vector<int> choosen_clause = unsat_clauses[rand()%unsat_clauses.size()];

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    int choosen_var;
    if (distribution(generator) < p) {
      choosen_var = abs(choosen_clause[rand()%choosen_clause.size()]);
    }
    else {
      int max_sat = -1;
      int var;
      for (int i = 0; i < choosen_clause.size(); i++) {
        int var = abs(choosen_clause[i]);
        int sat_count = count_num_sat(var, model);
        if (sat_count > max_sat){
          choosen_var = var;
          max_sat = sat_count;
        }
      }
    }
    model[choosen_var] = !model[choosen_var];
    unsat_clauses = check_assignment(model);

    num_flips++;
  }

  model.clear();  //If no solution found, return {0: false}
  model[0] = false;
  return model;
}

std::vector<std::vector<int>> SAT::check_assignment(const std::map<int, bool>& model) {
  std::vector<std::vector<int>> unsat_clauses;
  for (int i = 0; i < get_num_clauses(); i++){
    if (!check_clause(this->_clauses[i], model)){
      unsat_clauses.push_back(this->_clauses[i]);
    }
  }
  return unsat_clauses;
}

bool SAT::check_clause(const std::vector<int>& clause, const std::map<int, bool>& model) {
  for(std::vector<int>::const_iterator it = clause.begin(); it != clause.end(); ++it){
    if (((*it > 0) && (model.at(abs(*it)))) || ((*it < 0) && (!model.at(abs(*it))))) {
      //std::cout << "TRUE\n";
      return true;
    }
  }
  //std::cout << "FALSE\n";
  return false;
}

int SAT::count_num_sat(int var, std::map<int, bool>& model) {
  model[var] = !model[var];
  int sat_count = check_assignment(model).size();
  model[var] = !model[var];
  return sat_count;
}

void SAT::display_clauses() {
  for(std::vector<std::vector<int>>::iterator clause = _clauses.begin(); clause != _clauses.end(); ++clause){
    for (std::vector<int>::iterator literal = clause->begin(); literal != clause->end(); ++literal){
      std::cout << *literal << " ";
    }
    std::cout << std::endl;
  }
}

void SAT::display_model(const std::map<int, bool>& model) {
  for(std::map<int, bool>::const_iterator var = model.begin(); var != model.end(); ++var){
    std::cout << var->first << " " << var->second << std::endl;
  }
}
