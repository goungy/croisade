import sys

from core.Lexique import Lexique


lex_file = '/home/jerome/projets/croisade/CROISADE/data/lexiques/ultimate_556105.txt'
word_length = 3

if len(sys.argv) == 2:
    word_length = sys.argv[1]

l = Lexique(lex_file, word_length)


