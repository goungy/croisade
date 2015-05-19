from controller.Logger import Logger

class Lexique(object):

	def __init__(self, filepath, word_length):
		self.filepath = filepath
		self.word_length = int(word_length)
		Logger.log("opening " + filepath, self)
		with open(filepath) as f:
			self.words = [ w.strip('\n') for w in f if len(w.strip('\n')) == self.word_length ]
		Logger.log(str(len(self.words)) + " words of length " + str(self.word_length) + " loaded from " + self.filepath, self) 
		
	def get_index_of_word(self, word):
		return self.words.index(word)
