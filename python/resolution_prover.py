import random
from rand_cnf_generator import clause_to_string
from rand_cnf_generator import string_to_clause
import time
import copy

INPUT_PATH = "3-queens.txt"

class ResolutionProver:
    def __init__(self, clauses=[]):
        self.clauses = clauses

    def load_from_file(self, file_path):
        file = open(file_path, "r")
        _ = file.readline()
        configs = file.readline().split()
        num_var, num_clauses = int(configs[-2]), int(configs[-1])

        for i in range(num_clauses):
            tokens = [int(token) for token in file.readline().split()]

            if tokens[-1] != 0:
                raise Exception("clause lines should end with 0")
            self.clauses.append(clause_to_string(tokens[:-1]))


    #https://github.com/aimacode/aima-python/blob/master/logic.py
    #simplified
    def pl_resolution(self):
        new = set()
        while True:
            n = len(self.clauses)
            pairs = [(self.clauses[i], self.clauses[j])
                for i in range(n) for j in range(i + 1, n)]
            for (ci, cj) in pairs:
                resolvents = self.pl_resolve(ci, cj)
                if "" in resolvents:
                    return True
                new = new.union(set(resolvents))
            if new.issubset(set(self.clauses)):
                return False
            for c in new:
                if c not in self.clauses:
                    self.clauses.append(c)


    def pl_resolve(self, ci, cj):
        """Return all clauses that can be obtained by resolving clauses ci and cj."""
        clauses = []
        ci = set(ci.split())
        cj = set(cj.split())
        for di in ci:
            for dj in cj:
                if -1*int(di) == int(dj):
                    di_removed = ci.copy()
                    di_removed.remove(di)
                    dj_removed = cj.copy()
                    dj_removed.remove(dj)
                    clauses.append(clause_to_string(list(di_removed.union(dj_removed))))
        return clauses

test = ResolutionProver()
test.load_from_file(INPUT_PATH)

start = time.time()
result = test.pl_resolution()
end = time.time()

print(f"Python Resolution Prover took {end-start} seconds to run")
print(f"This problem is {not result}")
