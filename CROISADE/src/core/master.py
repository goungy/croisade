'''
Created on 18 mai 2015

@author: Jérôme
'''
import socket
import socketserver 
import threading
from controller.Logger import Logger
from core.communication import Communication

class Master(object):
    '''
    Master object to send data to workers
    '''


    def __init__(self, host='localhost', port = 6969):
        '''
        Constructor
        '''
        self.port = port
        self.host = host
        #self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #self.socket.bind((self.host, self.port))
        #self.socket.listen(1)
        self.word_iterator = None
        Logger.log("Master initialized", self)
    
    """
    @staticmethod   
    def handle_client(addr, conn, word_iterator):
        # Handling connection with one worker
        word_idx = word_iterator.next()
        Logger.log('Connected by' + str(addr), Master)
        data = conn.recv(1024).decode("utf-8")
        Logger.log("Received data: " + str(data), Master)
        if data == Communication.REQUEST_WORD:
            if word_idx >=0 : 
                Master.send_word(addr, conn, word_idx)
            else: Master.send_finish_signal(addr, conn)
        else: 
            Logger.log("Unknown request, ignoring", Master)
        conn.close()
    """
    
    def run(self, lexique, word_statistics):
        self.word_iterator = MasterWordIterator(lexique, word_statistics)
        
        server = SimpleServer((self.host, self.port), SingleTCPHandler, self.word_iterator)
        # terminate with Ctrl-C
        try:
            server.serve_forever()
        except KeyboardInterrupt:
            return
            #sys.exit(0)
        Logger.log("Master has finished, shutting down", Master)
        """
        while True:
            Logger.log('Listening for incoming connections', self)
            conn, addr = self.socket.accept()
            threading.Thread(target = Master.handle_client, args = [addr, conn, self.word_iterator]).start()
            pass
        """

class SimpleServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    # Ctrl-C will cleanly kill all spawned threads
    daemon_threads = True
    # much faster rebinding
    allow_reuse_address = True

    def __init__(self, server_address, RequestHandlerClass, word_iterator):
        socketserver.TCPServer.__init__(self, server_address, RequestHandlerClass)
        self.word_iterator = word_iterator
        self.has_been_shutdown = False
    
    def shutdown_simpleserver(self):
        self.has_been_shutdown = False
        #self.shutdown()

class SingleTCPHandler(socketserver.BaseRequestHandler):
    "One instance per connection.  Override handle(self) to customize action."
    def handle(self):
        word_iterator = self.server.word_iterator
        word_idx = word_iterator.next()
        Logger.log('Connected by' + str(self.client_address[0]), self)
        data = self.request.recv(1024).decode("utf-8")
        Logger.log("Received data: " + str(data), self)
        if data == Communication.REQUEST_WORD:
            if word_idx >=0 : 
                self.send_word(word_idx)
            else: 
                self.send_finish_signal()
                self.server.shutdown_simpleserver()
        else: 
            Logger.log("Unknown request, ignoring", self)

    def send_word(self, word_idx):
        Logger.log("Word request from " + str(self.client_address[0]), self)
        response = bytes(Communication.SENDING_WORD + ':' + str(word_idx), 'UTF-8')
        Logger.log("Sending response: " + str(response), Master)                    
        self.request.sendall(response)                  

    def send_finish_signal(self):
        Logger.log("Word request from " + str(self.client_address[0]), self)
        response = bytes(Communication.NO_MORE_WORDS + ':', 'UTF-8')
        Logger.log("Sending response: " + str(response), Master)                    
        self.request.sendall(response)    
        """
        # self.request is the client connection
        data = self.request.recv(1024)  # clip input at 1Kb
        reply = pipe_command(my_unix_command, data)
        if reply is not None:
            self.request.send(reply)
        self.request.close()
        """

class MasterWordIterator(object):
    def __init__(self, lexique, word_statistics):
        self.lexique = lexique
        self.word_statistics = word_statistics
        self.sorted_word_statistics = self.word_statistics.return_ordered_workload_per_word()
        self.idx = 0
        
    def next(self):
        lock = threading.RLock()
        lock.acquire()
        actual_word_idx = -1
        if self.idx >= len(self.sorted_word_statistics): 
            Logger.log("Need to do something as we have been through all words", self)
        else:
            actual_word = self.sorted_word_statistics[self.idx][0]
            possibilities = self.sorted_word_statistics[self.idx][1]
            Logger.log("Picking word " + actual_word + " with " + str(possibilities) + " possibilities", self )
            actual_word_idx = self.lexique.get_index_of_word(actual_word)
            self.idx += 1
        lock.release()
        return actual_word_idx
    
    
