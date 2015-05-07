

class Logger(object):

	FINE = 4
	NORMAL = 5
	
	logger_instance = None

	@staticmethod
	def initialize_logger(loglevel = NORMAL):
		Logger.logger_instance  = Logger(loglevel)

	@staticmethod
	def log(msg, caller, loglevel = NORMAL):
		Logger.logger_instance.log_instance(msg, caller, loglevel)

	def __init__(self, loglevel = NORMAL):
		self.loglevel = loglevel

	def log_instance(self, msg, caller, loglevel):
		if loglevel >= self.loglevel:
			print("[" + caller.__class__.__name__ + "] " + msg)
