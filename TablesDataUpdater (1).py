
# coding: utf-8

# In[6]:

import datetime
import pandas as pd
import time
import matplotlib.pyplot as plt
import time
from pymongo import MongoClient
import datetime
import sys
import json
import os
from xml.etree import ElementTree
import re
from queue import Queue
from threading import Thread
import websocket


# In[32]:

def on_message(ws, message):
    client = MongoClient('92.38.140.28',
                        authSource='bitcoin',
                        )
    db = client.bitcoin
    exchanges_col = db.exchanges_test
    pools_col = db.pools_test
    whale_wallets_col = db.whale_wallets_test
    wallets_col = db.wallets_test

    newm = message.replace('\'', '\"')
    d = json.loads(str(newm))
    pools = {}
    exchanges = {}
    whaleWallets = {}
    updatedWallets = {}


    try:
        pools = d["pools"]
    except:
        next
    try:
        exchanges = d["exchanges"]
    except:
        next
    try:
        whaleWallets = d["whaleWallets"]
    except:
        next
    try:
        updatedWallets = d["updatedWallets"]
    except:
        next

#     Remove old changes

    pools_col.update_many({},
                          {
                          '$pull': {
                            'changes_24': {
                                'timestamp': {'$lte': time.time() - 24*60*60}
                            }
                          }
                        }, upsert=False)


    exchanges_col.update_many({},
                          {
                          '$pull': {
                            'changes_24': {
                                'timestamp': {'$lte': time.time() - 24*60*60}
                            }
                          }
                        }, upsert=False)
    whale_wallets_col.update_many({},
                              {
                              '$pull': {
                                'changes_24': {
                                    'timestamp': {'$lte': time.time() - 24*60*60}
                                }
                              }
                            }, upsert=False)
    wallets_col.update_many({},
                          {
                          '$pull': {
                            'changes_24': {
                                'timestamp': {'$lte': time.time() - 24*60*60}
                            }
                          }
                        }, upsert=False)


#     Insert changes

    for pool in pools:

        print(str(pools[pool]['wallets']))
        pool_info = pools_col.find_one({"name": pools[pool]['name']})
        prev_change = sum([pair['change'] for pair in pool_info['changes_24']])
        prev_turnover = sum([abs(pair['change']) for pair in pool_info['changes_24']])
        change_24 = (prev_change + int(pools[pool]['amount']))/100000000
        turnover_24 = (prev_turnover + abs(float(pools[pool]['amount'])))/100000000
        pools[pool]["change_24"] = change_24
        pools[pool]["turnover_24"] = turnover_24
        pools_col.update_one({
                              'name': pools[pool]['name']
                            },{
                              '$push': {
                                'changes_24': {
                                    'timestamp': time.time(),
                                    'change': pools[pool]['amount']
                                }
                              }
                            }, upsert=True)
        pools_col.update_one({
                              'name': pools[pool]['name']
                            },{
                              '$set': {
                                'change_24' : change_24,
                                'turnover_24' : turnover_24
                              }
                            }, upsert=True)

    for exchange in exchanges:

        exchange_info = exchanges_col.find_one({"name": exchanges[exchange]['name']})
        prev_change = sum([pair['change'] for pair in exchange_info['changes_24']])
        prev_turnover = sum([abs(pair['change']) for pair in exchange_info['changes_24']])
        change_24 = (prev_change + int(exchanges[exchange]['amount']))/100000000
        turnover_24 = (prev_turnover + abs(float(exchanges[exchange]['amount'])))/100000000
        exchanges[exchange]["change_24"] = change_24
        exchanges[exchange]["turnover_24"] = turnover_24

        exchanges_col.update_one({
                              'name': exchanges[exchange]['name']
                            },{
                              '$push': {
                                'changes_24': {
                                    'timestamp': time.time(),
                                    'change': exchanges[exchange]['amount']
                                }
                              }
                            }, upsert=True)
        exchanges_col.update_one({
                              'name': exchanges[exchange]['name']
                            },{
                              '$set': {
                                'change_24' : change_24,
                                'turnover_24' : turnover_24
                              }
                            }, upsert=True)

    for whaleWallet in whaleWallets:

        whale_wallet_info = whale_wallets_col.find_one({"address": whaleWallets[whaleWallet]['address']})
        prev_change = sum([pair['change'] for pair in whale_wallet_info['changes_24']])
        prev_turnover = sum([abs(pair['change']) for pair in whale_wallet_info['changes_24']])
        change_24 = (prev_change + int(whaleWallets[whaleWallet]['amount']))/100000000
        turnover_24 = (prev_turnover + abs(whaleWallets[whaleWallet]['amount']))/100000000

        whaleWallets[whaleWallet]["change_24"] = change_24
        whaleWallets[whaleWallet]["turnover_24"] = turnover_24

        whale_wallets_col.update_one({
                              'address': whaleWallets[whaleWallet]['address']
                            },{
                              '$push': {
                                'changes_24': {
                                    'timestamp': time.time(),
                                    'change': whaleWallets[whaleWallet]['amount']
                                }
                              }
                            }, upsert=True)
        whale_wallets_col.update_one({
                              'address': whaleWallets[whaleWallet]['address']
                            },{
                              '$set': {
                                'change_24' : change_24,
                                'turnover_24' : turnover_24
                              }
                            }, upsert=True)

    for updatedWallet in updatedWallets:

        updated_wallet_info = wallets_col.find_one({"address": updatedWallets[updatedWallet]['address']})
        prev_change = sum([pair['change'] for pair in updated_wallet_info['changes_24']])
        prev_turnover = sum([abs(pair['change']) for pair in updated_wallet_info['changes_24']])
        change_24 = (prev_change + int(updatedWallets[updatedWallet]['amount']))/100000000
        turnover_24 = (prev_turnover + abs(updatedWallets[updatedWallet]['amount']))/100000000

        updatedWallets[updatedWallet]["change_24"] = change_24
        updatedWallets[updatedWallet]["turnover_24"] = turnover_24

        wallets_col.update_one({
                              'address': updatedWallets[updatedWallet]['address']
                            },{
                              '$push': {
                                'changes_24': {
                                    'timestamp': time.time(),
                                    'change': updatedWallets[updatedWallet]['amount']
                                }
                              }
                            }, upsert=True)
        wallets_col.update_one({
                              'address': updatedWallets[updatedWallet]['address']
                            },{
                              '$set': {
                                'change_24' : change_24,
                                'turnover_24' : turnover_24
                              }
                            }, upsert=True)


#     Insert empty changes

    pools_col.update_many({
                    'name': { '$nin': list(pools.keys())}
                     },{
                      '$push': {
                        'changes_24': {
                            'timestamp': time.time(),
                            'change': 0
                        }
                      }
                    }, upsert=False)
    exchanges_col.update_many({
                    'name': { '$nin': list(exchanges.keys())}
                     },{
                      '$push': {
                        'changes_24': {
                            'timestamp': time.time(),
                            'change': 0
                        }
                      }
                    }, upsert=False)
    whale_wallets_col.update_many({
                    'address': { '$nin': list(whaleWallets.keys())}
                     },{
                      '$push': {
                        'changes_24': {
                            'timestamp': time.time(),
                            'change': 0
                        }
                      }
                    }, upsert=False)
    wallets_col.update_many({
                    'address': { '$nin': list(updatedWallets.keys())}
                     },{
                      '$push': {
                        'changes_24': {
                            'timestamp': time.time(),
                            'change': 0
                        }
                      }
                    }, upsert=False)




    result = {"pools": pools, "exchanges": exchanges, "whaleWallets": whaleWallets, "updatedWallets": updatedWallets}
    print(result)
    ws.send(str(result))

def on_open(ws):
    ws.send("python ws")
    print("Opened")
def on_error(ws, error):
    print(error)
def on_close(ws):
    print("Closed")


# In[36]:

if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://localhost:8800/tables",
                              on_message = on_message,
                              on_error = on_error,
                              on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()
