
# coding: utf-8

# In[34]:

# db.usd_courses_history.updateOne({name: "Gdax"}, {$addToSet:  { history:{$each: [{e: 5, c: 4}]}}});
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

list_coin_exchange_pairs = [["BTC",    "Kraken",     "USD"]  ,
                            ["BTC",    "Poloniex",   "USD"]  ,
                            ["BTC",    "Bitfinex",   "USD"]  ,
                            ["BTC",    "BitTrex",    "USD"]
                           ]


# In[51]:

def hourly_price_historical(symbol, comparison_symbol, toTs, limit=100, aggregate=1, exchange=''):
    url = 'https://min-api.cryptocompare.com/data/histohour?fsym={}&tsym={}&limit={}&aggregate={}&toTs={}'    .format(symbol.upper(), comparison_symbol.upper(), limit, aggregate, toTs)
    if exchange:
        url += '&e={}'.format(exchange)
    page = requests.get(url)
    data = page.json()['Data']
    df = pd.DataFrame(data)
    df['timestamp'] = [datetime.datetime.fromtimestamp(d) for d in df.time]
    return [df, data]


# In[54]:

def parse():
    client = MongoClient('mongodb://bitcoin:bitqubedar@localhost:27017/')
    db = client.bitcoin
    usd_courses_history = db.usd_courses_history

    for coin_exchange_pair in list_coin_exchange_pairs:
        date_tmp = int(time.time())
        cntr = 0
        coin            = coin_exchange_pair[0]
        exchange        = coin_exchange_pair[1]
        countercurrency = coin_exchange_pair[2]
        date_from = int(time.mktime(datetime.datetime.strptime("2009-01-09-00:00:00","%Y-%m-%d-%H:%M:%S").timetuple()))
        flag_coin_skip = False
        while date_tmp > date_from:
            cntr += 1
            try:
                res=hourly_price_historical(coin,countercurrency,toTs=date_tmp,limit=(30*24),aggregate=1, exchange=exchange)
            except:
                # if no data found at all
                if counter == 1:
                    print("      No data for currency available, Skipping")
                flag_coin_skip = True

            if flag_coin_skip == True:
                break

            cols = ['timestamp', 'time', 'open', 'high', 'low', 'close', 'volumefrom', 'volumeto']
            df = res[0]
            df = df[cols]
            date_tmp=df['time'].iloc[0]
            data = res[1]
            for val in data:
                usd_courses_history.update_one({'name': exchange}, {'$push':  {'history': val}}, upsert=True)
            print("Iteration" + str(cntr) + ":\n")
            print(data[0])
    return 0


# In[55]:

parse()


# In[ ]:
