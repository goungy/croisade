'''
Created on 18 mai 2015

@author: Jérôme
'''
import socket
from controller.Logger import Logger
from core.communication import Communication

class Worker(object):
    '''
    Worker object to compute
    '''


    def __init__(self, host, port = 6969):
        '''
        Constructor
        '''
        self.port = port
        self.host = host
        Logger.log("Worker initialized", self)
        
    def run(self, lexique, word_statistics):
        while True:
            Logger.log("Worker initializing socket", self)
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            Logger.log("Worker connecting to master server " + self.host + " on port " + str(self.port), self)
            self.socket.connect((self.host, self.port))
    
            self.socket.sendall(bytes(Communication.REQUEST_WORD, 'UTF-8'))
            Logger.log("Sent REQUEST_WORD to master, waiting for response", self)
            data = self.socket.recv(1024).decode("utf-8")
            data_split = data.split(':')
            Logger.log("Receiving response:" + str(data), self)
            response_type = data_split[0]
            if response_type == Communication.SENDING_WORD:
                word_idx = data_split[1]
                Logger.log("Received word idx " + str(word_idx), self)           
            elif response_type == Communication.NO_MORE_WORDS:
                Logger.log("Received NO_MORE_WORDS '"+ str(data)+"': exiting now", self)
                break
            else:
                Logger.log("Received unknown response '"+ str(data)+"': exiting now", self)
                break
            self.socket.close()
        Logger.log("Worker exiting now", self)
        pass