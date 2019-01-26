#ifndef MESSENGERENDPOINT_H
#define MESSENGERENDPOINT_H

#include "applicationendpoint.h"

class T_CONTROLLER_EXPORT MessengerEndpoint : public ApplicationEndpoint
{
    Q_OBJECT
public:
    MessengerEndpoint();
    MessengerEndpoint(const MessengerEndpoint &other);

protected:
    bool onOpen(const TSession &httpSession) override;
    void onClose(int closeCode) override;
    void onTextReceived(const QString &text) override;
    void onBinaryReceived(const QByteArray &binary) override;
};

#endif // MESSENGERENDPOINT_H
