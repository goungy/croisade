

class Logger(object):

	FINE = 4
	NORMAL = 5
	ERROR = 10
	
	logger_instance = None

	@staticmethod
	def initialize_logger(loglevel = NORMAL):
		Logger.logger_instance  = Logger(loglevel)

	@staticmethod
	def log(msg, caller, loglevel = NORMAL):
		if not Logger.logger_instance: 
			print("[ERROR] one needs to call Logger.initialize_logger before calling Logger.log")
			raise RuntimeError("Error: one needs to call Logger.initialize_logger before calling Logger.log")
		Logger.logger_instance.log_instance(msg, caller, loglevel)

	def __init__(self, loglevel = NORMAL):
		self.loglevel = loglevel

	def log_instance(self, msg, caller, loglevel):
		if loglevel >= self.loglevel:
			print("[" + caller.__class__.__name__ + "] " + msg)
