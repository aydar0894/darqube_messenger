
# coding: utf-8

# In[8]:

import cryptocompare
import requests
import datetime
import pandas as pd
import matplotlib.pyplot as plt
import time
from pymongo import MongoClient
import datetime
import sys
import os
from xml.etree import ElementTree
import re
import pandas as pd
import matplotlib.pyplot as plt
from pymongo import MongoClient
from queue import Queue
from threading import Thread


# In[2]:

class DownloadWorker(Thread):
    def __init__(self, queue):
        Thread.__init__(self)
        self.queue = queue

    def run(self):
        while True:           
            exchange = self.queue.get()
            get_data(exchange)
            self.queue.task_done()


# In[15]:

def get_data(exchange=''):
    client = MongoClient('92.38.140.28',
                         authSource='bitcoin',
                         )
    db = client.bitcoin
    chunks = db.exchunks    
    exchanges = db.exchanges
    accum = []
    if exchange.endswith('\n'):
        exchange = exchange[:-1]
    print(exchange)
    url = 'https://www.walletexplorer.com/wallet/' + exchange + '/addresses?page=1'
    response = requests.get(url)
    content = str(response.content)
    pages = int(re.findall(r'Page 1 / ([^>]+)<a href=', content)[0]) 
    for i in range(pages):
        url = 'https://www.walletexplorer.com/wallet/' + exchange + '/addresses?page=' + str(i + 1)
#         print(url)
        response = requests.get(url)
        content = str(response.content)
  #     result = re.findall('<table>.*</table>', content) 
        result = re.findall(r'<a href=[^>]*>([^<]+)</a></td>', content) 
        if i % 100 = 0:
            chunk = chunks.insert_one({'name': exchange, 'wallets': accum})
            accum = []
        else:
            accum.extend(result)
#         exchanges.update_one({'name': exchange}, {'$push':  {'chunks': id}}, upsert=True)        
#         for val in result:
#             exchanges.update_one({'name': exchange}, {'$push':  {'wallets': val}}, upsert=True)
#         print(result)
#     print(pages)


# In[16]:

def main():
    text_file = open("exchanges.txt", "r")
    exchanges = text_file.readlines()
#     print(exhanges)
#     print(len(exhanges))
    text_file.close()
    queue = Queue()
    
    
    for x in range(20):
        worker = DownloadWorker(queue)       
        worker.daemon = False
        worker.start()
    
    for exc in exchanges:
        queue.put(exc)
#     get_data("Bittrex.com")


# In[17]:

main()


# In[11]:

client = MongoClient('92.38.140.28',
                         authSource='bitcoin',
                         )
db = client.bitcoin
chunks = db.exchunks
chunks.find().count()


# In[ ]:



