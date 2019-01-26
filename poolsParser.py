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
from queue import Queue
from threading import Thread

class DownloadWorker(Thread):
    def __init__(self, queue):
        Thread.__init__(self)
        self.queue = queue

    def run(self):
        while True:
            pool = self.queue.get()
            get_pools_data(pool)
            self.queue.task_done()


def get_pools_data(pool=''):
    client = MongoClient('92.38.140.28',
                    username='bitcoin',
                    password='bitqubedar',
                    authSource='bitcoin',
                    authMechanism='SCRAM-SHA-1')
    db = client.bitcoin
    chunks = db.poolchunks
    pools = db.pools
    wallets = db.wallets
    accum = []
    if pool.endswith('\n'):
        pool = pool[:-1]
    print(pool)
    pl = 'https://www.walletexplorer.com/wallet/' + pool
    resp = requests.get(pl)
    cont = str(resp.content)
    pool_bal = re.findall(r'<td class="amount"[^>]*([^<]+)</td>', cont)
    pools.insert_one({'name': pool,
                        "balance" : pool_bal[0].replace('&nbsp;', '').replace('>', ''),
                        "turnover_24" : 0,
                        "change_24" : 0,
                        "av_turnover" : 0,
                        "av_change" : 0
                         })
    url = 'https://www.walletexplorer.com/wallet/' + pool + '/addresses?page=1'
    response = requests.get(url)
    content = str(response.content)
    pages = int(re.findall(r'Page 1 / ([^>]+)<a href=', content)[0])
    for i in range(pages):
        url = 'https://www.walletexplorer.com/wallet/' + pool + '/addresses?page=' + str(i + 1)
#         print(url)
        response = requests.get(url)
        content = str(response.content)
  #     result = re.findall('<table>.*</table>', content)
        wallet = re.findall(r'<a href=[^>]*>([^<]+)</a></td>', content)
        balance = re.findall(r'<td class="amount"[^>]*([^<]+)</td>', content)
        if i % 100 == 0 and i != 0:
            chunk = chunks.insert_one({'name': pool, 'wallets': accum})
            print(len(accum))
            accum = []
        else:
            accum.extend(wallet)
        if i == pages - 1:
            chunk = chunks.insert_one({'name': pool, 'wallets': accum})
            accum = []
        ky = 0
        for w in wallet:
            wallets.insert_one({
                                'address': w,
                                "balance": balance[ky].replace('&nbsp;', '').replace('>', ''),
                                "turnover_24" : 0,
                                "change_24" : 0,
                                "av_turnover" : 0,
                                "av_change" : 0,
                                "pool" : pool,
                                "exchange" : "",
                                "whale_wallet" : "false"
                            })
            ky = ky + 1

def main():
    text_file = open("pools.txt", "r")
    exchanges = text_file.readlines()
    text_file.close()
    queue = Queue()


    for x in range(20):
        worker = DownloadWorker(queue)
        worker.daemon = False
        worker.start()

    for exc in exchanges:
        queue.put(exc)

main()
