#include "messengerendpoint.h"
#include "messagegetters.h"
#include <QMap>


QMap<QString, QString> logged_in_users;

MessengerEndpoint::MessengerEndpoint()
    : ApplicationEndpoint()
{ }

MessengerEndpoint::MessengerEndpoint(const MessengerEndpoint &)
    : ApplicationEndpoint()
{ }

bool MessengerEndpoint::onOpen(const TSession &)
{
    return true;
}

void MessengerEndpoint::onClose(int)
{ }

void MessengerEndpoint::onTextReceived(const QString & text)
{
    QJsonDocument message = QJsonDocument::fromJson(text.toUtf8());
    QString action = message["action"].toString();
    QString sender = message["sender"].toString();
    QString strJson(QJsonDocument(message["body"].toObject()).toJson(QJsonDocument::Compact));
    QJsonDocument body = QJsonDocument(message["body"].toObject());



    if(action == "login")
    {
      QVariantMap result;
      QVariantMap chats = getUserChats(sender);
      result["type"] = "get_user_chats";
      result["data"] = chats;
      QJsonDocument res = QJsonDocument::fromVariant(result);
      QString strJson(res.toJson(QJsonDocument::Compact));
      foreach (QVariant key, chats.keys())
      {
        subscribe(key.toString());
      }
      logged_in_users[sender] = QString(socketId());
      sendText(socketId(), strJson);
    }

    else if(action == "get_user_chats")
    {
      QVariantMap result;
      QVariantMap chats = getUserChats(sender);
      result["type"] = "get_user_chats";
      result["data"] = chats;
      QJsonDocument res = QJsonDocument::fromVariant(result);
      QString strJson(res.toJson(QJsonDocument::Compact));
      foreach (QVariant key, chats.keys())
      {
        subscribe(key.toString());
      }
      sendText(socketId(), strJson);
    }

    else if(action == "send_message")
    {
      QJsonDocument messageBody = body;

      QString id = messageBody["id"].toString();
      TMongoQuery chatsQ("chats");
      QVariantMap criteria;
      criteria["_id"] = id;
      QVariantMap result;
      QVariantMap updObject;
      QVariantMap messagesCriteria;
      QVariantMap pushCriteria;
      QVariantMap contentCriteria;
      contentCriteria["type"] = messageBody["type"].toString();
      contentCriteria["data"] = messageBody["data"].toString();

      updObject["sender"] = sender;
      updObject["content"] = contentCriteria;
      updObject["time"] = "3546645364";
      messagesCriteria["messages"] = updObject;
      pushCriteria["$push"] = messagesCriteria;
      chatsQ.update(criteria, pushCriteria, true);
      pushCriteria["id"] = id;
      pushCriteria["type"] = "new_message";
      QJsonDocument res = QJsonDocument::fromVariant(pushCriteria);
      QString strJson(res.toJson(QJsonDocument::Compact));
      publish(id, strJson);
    }

    else if(action == "create_group_chat")
    {
      QJsonDocument messageBody = body;

      TMongoQuery chatsQ("chats");

      QVariantMap messagesCriteria;
      QVariantMap result;
      messagesCriteria["name"] = messageBody["chatName"].toString();
      messagesCriteria["messages"] = QStringList();
      QStringList users = messageBody["users"].toString().split(",");
      messagesCriteria["users"] = users;
      QString id;


      QStringList list = {"_id"};

      chatsQ.insert(messagesCriteria);
      QVariantMap res_find = chatsQ.findOne(messagesCriteria , list);
      id = res_find["_id"].toString();
      // tInfo("ID:  ");
      // tInfo(id.toUtf8());
      result["data"] = messagesCriteria;
      result["type"] = "create_group_chat";
      QJsonDocument res = QJsonDocument::fromVariant(result);
      QString strJson(res.toJson(QJsonDocument::Compact));
      for(int i = 0; i < users.length(); i++){
        if(logged_in_users.contains(users[i]) && users[i] != sender){
          sendText(logged_in_users[users[i]].toInt(), strJson);
        }
      }

      subscribe(id);
      publish(id, strJson);
    }
    
    else if(action == "add_users_to_chat")
    {
      QJsonDocument messageBody = body;
      QString id = messageBody["id"].toString();
      TMongoQuery chatsQ("chats");
      QVariantMap result;
      QVariantMap idCriteria;
      QVariantMap pushCriteria;
      QVariantMap eachCriteria;
      QVariantMap usersCriteria;

      idCriteria["_id"] = id;
      eachCriteria["$each"] = messageBody["users"].toString().split(",");
      usersCriteria["users"] = eachCriteria;
      pushCriteria["$push"] = usersCriteria;

      chatsQ.update(idCriteria, pushCriteria, true);
      pushCriteria["type"] = "add_users_to_chat";
      pushCriteria["id"] = id;
      QJsonDocument res = QJsonDocument::fromVariant(pushCriteria);
      QString strJson(res.toJson(QJsonDocument::Compact));
    }

    else if(action == "remove_user_from_chat")
    {
      QJsonDocument messageBody = body;
      QString id = messageBody["id"].toString();
      TMongoQuery chatsQ("chats");
      QVariantMap result;
      QVariantMap idCriteria;
      QVariantMap pushCriteria;
      QVariantMap eachCriteria;

      idCriteria["_id"] = id;
      eachCriteria["users"] = messageBody["users"].toString().split(",");
      pushCriteria["$pull"] = eachCriteria;

      chatsQ.update(idCriteria, pushCriteria, true);
      pushCriteria["type"] = "remove_user_from_chat";
      pushCriteria["id"] = id;
      QJsonDocument res = QJsonDocument::fromVariant(pushCriteria);
      QString strJson(res.toJson(QJsonDocument::Compact));
    }

    else if(action == "get_chat_users")
    {
      QJsonDocument messageBody = body;
      QString id = messageBody["id"].toString();
      QVariantMap chatCriteria;
      chatCriteria["_id"] = id;
      TMongoQuery chatsQ("chats");
      QVariant result;
      QVariantMap chat = chatsQ.findOne(chatCriteria);

      result = chat["users"];
      QJsonDocument res = QJsonDocument::fromVariant(result);
      QString strJson(res.toJson(QJsonDocument::Compact));
      sendText(socketId(), strJson);
    }



    // write code
}

void MessengerEndpoint::onBinaryReceived(const QByteArray &)
{ }


// Don't remove below this line
T_DEFINE_CONTROLLER(MessengerEndpoint)
