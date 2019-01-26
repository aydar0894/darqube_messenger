#include <QJsonArray>
#include <TSqlQuery>
#include <QJsonValue>
#include <QString>
#include <QVariant>
#include <QVariantMap>
#include "ssystem.h"
#include <QVector>
#include <TMongoQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTextCodec>
#include <QTextDecoder>


QVariantMap getWhaleWallets()
{
  TMongoQuery wallets("whale_wallets");
  QVariantMap walCriteria;
  QVariantMap whale_wallets;
  wallets.find();
  while (wallets.next()) {
    QVariantMap doc = wallets.value();
    whale_wallets.insert(doc["address"].toString(), doc);
  }
  QVariantMap result;
  result.insert("wallets", whale_wallets);
  return result;
}

QVariantMap getExchanges()
{
  TMongoQuery exchangesQ("exchanges");
  QVariantMap exchanges;
  exchangesQ.find();
  while (exchangesQ.next()) {
    QVariantMap doc = exchangesQ.value();
    exchanges.insert(doc["name"].toString(), doc);
  }

  QVariantMap result;
  result.insert("exchanges", exchanges);
  return result;
}

QVariantMap getEmission()
{
  QJsonObject res;
  std::string bash = "curl \"https://blockchain.info/q/totalbc\"";
  QString result = "";
  std::string s = ssystem(bash.c_str());
  std::istringstream iss(s);
  std::string line;
  while ( std::getline(iss, line) )
  {
    result.append(QString::fromUtf8(line.c_str()));
  }

  result = result.remove(result.length() - 1, 1);
  QVariantMap doc;
  doc.insert("emission", result);
  return doc;
}


QVariantMap getPools()
{
  TMongoQuery poolsQ("pools");
  QVariantMap pools;
  poolsQ.find();
  while (poolsQ.next()) {
    QVariantMap doc = poolsQ.value();
    pools.insert(doc["name"].toString(), doc);
  }
  QVariantMap result;
  result.insert("pools", pools);
  return result;
}
