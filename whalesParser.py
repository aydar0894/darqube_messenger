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

def get_whales_data():
    client = MongoClient('92.38.140.28',
                    username='bitcoin',
                    password='bitqubedar',
                    authSource='bitcoin',
                    authMechanism='SCRAM-SHA-1')
    db = client.bitcoin
    whales = db.whale_wallets
    wallets = db.wallets


    for i in range(10):
        if i == 0:
            url = 'https://bitinfocharts.com/ru/top-100-richest-bitcoin-addresses.html'
        else:
            url = 'https://bitinfocharts.com/ru/top-100-richest-bitcoin-addresses.html-' + str(i+1)
        print(i)
        response = requests.get(url)
        content = str(response.content)

        addresses = re.findall(r'<a href="https://bitinfocharts.com/ru/bitcoin/address/[^>]*>([^<]+)</a>', content)
        balances = re.findall(r'<td class="hidden-phone"[^>]*>([^<]+) BTC', content)
        k = 0
        for addr in addresses:
            whale = whales.insert_one({"address" : addr,
            "balance" : balances[k],
            "turnover_24" : 0,
            "change_24" : 0,
            "av_turnover" : 0,
            "av_change" : 0,
            "pool" : "",
            "exchange" : "",
            "whale_wallet" : "true"
            })
            wallets.insert_one({
                                'address': addr,
                                "balance": balances[k],
                                "turnover_24" : 0,
                                "change_24" : 0,
                                "av_turnover" : 0,
                                "av_change" : 0,
                                "pool" : "",
                                "exchange" : "",
                                "whale_wallet" : "true"
                            })
            k = k + 1

            
get_whales_data()
