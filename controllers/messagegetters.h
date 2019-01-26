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


QVariantMap getUserChats(QString userId)
{
  TMongoQuery chatsQ("chats");
  QVariantMap chats;
  QVariantMap chatCriteria;
  chatCriteria["users"] = userId;
  QStringList list = { "users", "messages"};

  chatsQ.find(chatCriteria, QVariantMap(), list);
  int iter = 1;
  while (chatsQ.next()) {
    QVariantMap doc = chatsQ.value();
    chats.insert(doc["_id"].toString(), doc);
    iter++;
  }
  
  return chats;
}
