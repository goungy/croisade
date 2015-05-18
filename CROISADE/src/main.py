import sys

from core.Lexique import Lexique
from core.statistics import WordStatistics
from controller.Logger import Logger

Logger.initialize_logger(loglevel = Logger.NORMAL)

lex_file = '../data/lexiques/ultimate_556105.txt'
word_length = 10

if len(sys.argv) == 2:
    word_length = sys.argv[1]

l = Lexique(lex_file, word_length)
word_statistics = WordStatistics(l)



