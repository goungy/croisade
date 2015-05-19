#slave: 
# D:\workspace\croisade\CROISADE\src
# c:\python34\python main.py --lex_file=..\data\lexiques\ultimate_556105.txt --word_size=10 --master_host=localhost --master_port=7070

#master
# D:\workspace\croisade\CROISADE\src
# c:\python34\python main.py --lex_file=..\data\lexiques\ultimate_556105.txt --word_size=10 --master_host=localhost --master --master_port=7070
import sys
import argparse

from core.Lexique import Lexique
from core.statistics import WordStatistics
from controller.Logger import Logger
from core.master import Master
from core.worker import Worker

Logger.initialize_logger(loglevel = Logger.NORMAL)
#Logger.initialize_logger(loglevel = Logger.FINE)

parser = argparse.ArgumentParser(description='Process arguments for croisade')
parser.add_argument('--lex_file', required=True)
parser.add_argument('--word_size', dest='word_size', required=True, type=int)
parser.add_argument('--master_host', dest='master_host', required=True)
parser.add_argument('--master_port', dest='master_port', required=True, type=int)
parser.add_argument('--master', dest='be_master', action='store_true')
args = parser.parse_args()

lex_file = '../data/lexiques/ultimate_556105.txt'
lex_file = args.lex_file

word_length = 10
word_length = args.word_size

be_master = args.be_master

master_host = '127.0.0.1'
master_host = args.master_host

master_port = 6969
master_port = args.master_port

l = Lexique(lex_file, word_length)
word_statistics = WordStatistics(l)

if be_master:
    runner = Master(host = master_host, port = master_port)
else:
    runner = Worker(master_host, port = master_port)

runner.run(l, word_statistics)
    
            




