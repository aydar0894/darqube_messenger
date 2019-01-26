#ifndef DARQOINAPIOBJECT_H
#define DARQOINAPIOBJECT_H

#include <TSqlObject>
#include <QSharedData>


class T_MODEL_EXPORT DarqoinApiObject : public TSqlObject, public QSharedData
{
public:
    int id {0};
    qlonglong api_ver {0};

    enum PropertyIndex {
        Id = 0,
        ApiVer,
    };

    int primaryKeyIndex() const override { return Id; }
    int autoValueIndex() const override { return Id; }
    QString tableName() const override { return QLatin1String("darqoin_api"); }

private:    /*** Don't modify below this line ***/
    Q_OBJECT
    Q_PROPERTY(int id READ getid WRITE setid)
    T_DEFINE_PROPERTY(int, id)
    Q_PROPERTY(qlonglong api_ver READ getapi_ver WRITE setapi_ver)
    T_DEFINE_PROPERTY(qlonglong, api_ver)
};

#endif // DARQOINAPIOBJECT_H
