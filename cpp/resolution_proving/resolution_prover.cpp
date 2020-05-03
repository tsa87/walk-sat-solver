#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <utility>
#include <map>
#include "resolution_prover.h"

const std::string END_OF_LINE = "0";
const std::string START_OF_CLAUSE = "C";
const char DELIMITER = ' ';


ResolutionProver::ResolutionProver() {
}

ResolutionProver::~ResolutionProver() {
}

bool ResolutionProver::pl_resolution() {
  int last_size;
  clause_map new_clauses;
  clause_map resolvent;
  while (true) {
    last_size = new_clauses.size();
    for (auto ci = _clauses.begin(); ci!=_clauses.end(); ++ci){
      for (auto cj = ci; cj!=_clauses.end(); ++cj) {
        resolvent = pl_resolve(ci->second, cj->second);
        if (resolvent.count(START_OF_CLAUSE) > 0) return false;
        new_clauses.insert(resolvent.begin(), resolvent.end());
      }
    }
    if (new_clauses.size() == last_size) return true;
    _clauses.insert(new_clauses.begin(), new_clauses.end());
  }
}

clause_map ResolutionProver::pl_resolve(const literal_set& ci, const literal_set& cj) {
  clause_map resolvent;
  literal_set ci_copy;
  literal_set clause;
  for (auto di = ci.begin(); di!=ci.end(); ++di) {
    for (auto dj = cj.begin(); dj!=cj.end(); ++dj) {
      if (((*di) * -1)== *dj) {
        //Create a copy and remove literals removed through resolution
        ci_copy = ci;
        clause = cj;
        ci_copy.erase(*di);
        clause.erase(*dj);

        //Insert the newly clause into resolvent list
        clause.insert(ci_copy.begin(), ci_copy.end());
        resolvent.insert(std::make_pair(clause_to_string(clause), clause));
      }
    }
  }
  return resolvent;
}


void ResolutionProver::load_from_file(char* file_path) {
  std::string line;
  std::ifstream input_file (file_path);
  if (input_file.is_open()) {
    getline(input_file, line); //ignore comment
    getline(input_file, line); //read the config
    std::vector<std::string> tokens = split(line, DELIMITER);

    this->_num_variables = std::stoi (tokens[2]);
    this->_num_clauses = std::stoi (tokens[3]);

    int len = 0;
    for (int i = 0; i < this->_num_clauses; i++) {
      getline(input_file, line);

      std::vector<std::string> tokens = split(line, DELIMITER);

      // Remove END_OF_LINE token
      len = tokens.size();
      assert(tokens[len-1] == END_OF_LINE);
      tokens.pop_back();

      literal_set clause = tokens_to_clause(tokens);
      clause_entry clause_pair;

      for (int j = 0; j < len-1; j++) {
        clause_pair = std::make_pair(clause_to_string(clause), clause);
        this->_clauses.insert(clause_pair);
      }
    }
    input_file.close();
  }
}

std::string ResolutionProver::clause_to_string(const literal_set& clause) {
  std::string clause_string = START_OF_CLAUSE;
  for (std::set<int>::iterator it=clause.begin(); it!=clause.end(); ++it) {
    clause_string = clause_string + " " + std::to_string (*it);
  }
  return clause_string;
}

literal_set ResolutionProver::tokens_to_clause(const std::vector<std::string>& tokens) {
  literal_set clause;
  for (int i = 0; i < tokens.size(); i++) {
    clause.insert( std::stoi(tokens[i]) );
  }
  return clause;
}

std::vector<std::string> ResolutionProver::split(const std::string& sentence, char delimiter) {
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

void ResolutionProver::display_clauses() {
  for(auto clause = _clauses.begin(); clause != _clauses.end(); ++clause){
    std::cout << clause->first << std::endl;
  }
}
