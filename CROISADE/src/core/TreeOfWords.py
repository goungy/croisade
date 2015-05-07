
from controller.Logger import Logger

class TreeOfWords(object):

	def __init__(self, lexique):
		Logger.log('Creating TreeOfWords',self)
		self.lexique = lexique
		self.root = LettersNode()
		for w in lexique.get_words(): self.add_word(w)
		Logger.log('Added '+str(self.get_count_of_words())+' in TreeOFWords',self)

	def add_word(self, word):
		Logger.log('Adding word ::'+word+':: to TreeOfWords',self, Logger.FINE)
		self.root.add_word(word)		

	def get_count_of_words(self):
		return self.root.get_count_of_words()
	
class LettersNode(object):
	
	def __init__(self, is_final_letter_of_word = False):
		Logger.log('Creating LettersNode',self, Logger.FINE)
		self.count_of_words = 0
		self.hash_of_letters_node = {}
		self.is_final_letter_of_word = is_final_letter_of_word

	def add_word(self, word):
		Logger.log('Adding suffix ::'+word+':: to TreeOfWords',self, Logger.FINE)
		self.count_of_words += 1
		first_letter_word = word[0]
		if not first_letter_word in self.hash_of_letters_node.keys():
			self.hash_of_letters_node[first_letter_word] = LettersNode(len(word) == 1)
		if len(word) > 1 :  self.hash_of_letters_node[first_letter_word].add_word(word[1:])

	def get_count_of_words(self):
		return self.count_of_words
