import sys

from core.Lexique import Lexique
from core.statistics import WordStatistics
from controller.Logger import Logger

Logger.initialize_logger(loglevel = Logger.NORMAL)
#Logger.initialize_logger(loglevel = Logger.FINE)

lex_file = '../data/lexiques/ultimate_556105.txt'
word_length = 12

if len(sys.argv) == 2:
    word_length = sys.argv[1]

l = Lexique(lex_file, word_length)
word_statistics = WordStatistics(l)

nb_workers = 4
words_for_each_worker = [ [0, []] for i in range(nb_workers)]
for w in l.words:
    min_worker, min_workload = words_for_each_worker[0] , words_for_each_worker[0][0]
    for worker in words_for_each_worker[1:]:
        if min_workload > worker[0]: min_worker , min_workload = worker, worker[0]
    min_worker[1].append(w)
    min_worker[0] += word_statistics.workload_per_word[w]


for idx,w in enumerate(words_for_each_worker):
    print("worker " + str(idx) + ": " + str(w[0]) + " possibilities with " + str(len(w[1])) + " words")
    sum += len(w[1])

    
            




