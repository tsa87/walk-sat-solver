import random

LITERALS_PER_CLAUSE = 3
CLAUSES_TO_GENERATE = 12500
SYMBOL_COUNT = 5000
OUTPUT_PATH = f"../sat_problems/generated_{LITERALS_PER_CLAUSE}-CSP_m={CLAUSES_TO_GENERATE}_n={SYMBOL_COUNT}.txt"

"""
    Generates a Conjunctive Normal Form (CNF) clause.
    Args:
      k: # of literals per clause.
      m: # of clauses to generate.
      n: # of symbols.
    Returns:
      A CNF clause.
"""
def rand_cnf_generator(k, m, n):
    clauses = []
    generated_clauses = set()
    symbols = [i for i in range(1, n+1)]

    while m > 0:
        random.shuffle(symbols)
        clause = [symbol if random.random() < 0.5 else -1*symbol for symbol in symbols[:k]]
        clause_str = clause_to_string(clause)

        if clause_str not in generated_clauses:
            clauses.append(clause)
            generated_clauses.add(clause_str)
            m -= 1

    return clauses, symbols

def clause_to_string(clause):
    clause.sort()
    return " ".join([str(literal) for literal in clause])

def string_to_clause(string):
    return [int(i) for i in string.split()]

def write_to(clauses, symbols, file_path):
    file = open(file_path, 'w+')
    file.write("c " + "auto-generated SAT problem\n")
    num_vars = len(symbols)
    num_clauses = len(clauses)
    file.write("p " + "cnf " + str(num_vars) + " " + str(num_clauses) + "\n")
    for clause in clauses:
        clause = [str(literal) for literal in clause]
        file.write(" ".join(clause) + " 0\n")
    file.close()

if __name__ == "__main__":
    clauses, symbols = rand_cnf_generator(LITERALS_PER_CLAUSE, CLAUSES_TO_GENERATE , SYMBOL_COUNT)
    write_to(clauses, symbols, OUTPUT_PATH)
