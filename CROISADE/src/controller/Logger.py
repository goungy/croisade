

class Logger(object):
	@staticmethod
	def log(msg, caller):
		print("[" + caller.__class__.__name__ + "] " + msg)
