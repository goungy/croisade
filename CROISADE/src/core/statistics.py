'''
Created on 18 mai 2015

@author: Jérôme
'''
from core.Lexique import Lexique
from controller.Logger import Logger
import string


class WordStatistics(object):
    '''
    classdocs
    '''


    def __init__(self, lexique):
        '''
        Constructor
        '''
        self.lexique = lexique
        self.word_length = self.lexique.word_length
        # compute statistics: amount of words with letter "X" at position N
        self.position_letter_statistics = [ {} for i in range(self.word_length) ]
        for p in self.position_letter_statistics:
            for l in string.ascii_lowercase: p[l] = 0 
        for w in self.lexique.words:
            for idx,l in enumerate(w):
                self.position_letter_statistics[idx][l] += 1
        """
        advanced statistics: matrix of crossed possibilities: 
        abcd
        ****
        ****
        ****
        Taking line 0 column 1 as an example: number of  words on line 0 
        with a 'b' at column 1 multiplied by number of  words in column 1 
        with a 'b' at position 0 gives number of possibilities for letter 'b' at 
        position 'line 0' column 1'. We sum this for all letters at 
        position 'line 0' 'column1' to get overall posibilities of a position.
        Matrix is symmetric so we compute only half of it.
        We do not need to go through all words to try and count.
        We simply use self.position_letter_statistics which is more efficient.
        """ 
        self.absolute_statistics = [ [ 0 for j in range(self.word_length)] for i in range(self.word_length)] 
        for line in range(self.word_length):
            for column in range(line, self.word_length):
                words_on_position = 0
                for l in self.position_letter_statistics[column]:
                    words_in_line_with_letter =  self.position_letter_statistics[column][l]
                    words_on_position += words_in_line_with_letter * self.position_letter_statistics[line][l]
                self.absolute_statistics[line][column] = words_on_position
                self.absolute_statistics[column][line] = words_on_position
        Logger.log("STATISTICS", self, loglevel = Logger.FINE)
        min_line, min_column, min_number = 0, 0, self.absolute_statistics[0][0]
        for line in range(self.word_length):
            for column in range(self.word_length):
                Logger.log(str(line) + " " + str(column) + " " + str(self.absolute_statistics[line][column]), self, loglevel = Logger.FINE)
                if min_number > self.absolute_statistics[line][column]:
                    min_number, min_line, min_column = self.absolute_statistics[line][column], line, column
        Logger.log("MINIMUM: " + str(min_line) + " " + str(min_column) + " " + str(min_number), self)
        Logger.log("VERSUS: 0 0 " + str(self.absolute_statistics[0][0]), self)
        Logger.log("RATIO: " + str(self.absolute_statistics[min_line][min_column] / self.absolute_statistics[0][0]), self)
                