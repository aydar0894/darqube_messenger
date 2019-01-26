import requests
import datetime
import pandas as pd
import matplotlib.pyplot as plt
from pymongo import MongoClient

def daily_price_historical(symbol, comparison_symbol, all_data=True, limit=1, aggregate=1, exchange=''):
    url = 'https://min-api.cryptocompare.com/data/histoday?fsym={}&tsym={}&limit={}&aggregate={}'\
            .format(symbol.upper(), comparison_symbol.upper(), limit, aggregate)
    if exchange:
        url += '&e={}'.format(exchange)
    if all_data:
        url += '&allData=true'
    page = requests.get(url)
    data = page.json()['Data']
    df = pd.DataFrame(data)
    df['timestamp'] = [datetime.datetime.fromtimestamp(d) for d in df.time]
    return data
def main():
    data=daily_price_historical('BTC','USD')
    client = MongoClient()
    db = client.bitcoin
    usd_courses_history = db.usd_courses_history
    for val in data:
        usd_courses_history.insert_one(val)

main()
