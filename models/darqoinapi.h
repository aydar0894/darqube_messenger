#ifndef DARQOINAPI_H
#define DARQOINAPI_H

#include <QStringList>
#include <QDateTime>
#include <QVariant>
#include <QSharedDataPointer>
#include <TGlobal>
#include <TAbstractModel>

class TModelObject;
class DarqoinApiObject;
class QJsonArray;


class T_MODEL_EXPORT DarqoinApi : public TAbstractModel
{
public:
    DarqoinApi();
    DarqoinApi(const DarqoinApi &other);
    DarqoinApi(const DarqoinApiObject &object);
    ~DarqoinApi();

    int id() const;
    qlonglong apiVer() const;
    void setApiVer(const qlonglong &apiVer);
    DarqoinApi &operator=(const DarqoinApi &other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool save()   override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static DarqoinApi create(const qlonglong &apiVer);
    static DarqoinApi create(const QVariantMap &values);
    static DarqoinApi get(int id);
    static int count();
    static QList<DarqoinApi> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<DarqoinApiObject> d;

    TModelObject *modelData() override;
    const TModelObject *modelData() const override;
    friend QDataStream &operator<<(QDataStream &ds, const DarqoinApi &model);
    friend QDataStream &operator>>(QDataStream &ds, DarqoinApi &model);
};

Q_DECLARE_METATYPE(DarqoinApi)
Q_DECLARE_METATYPE(QList<DarqoinApi>)

#endif // DARQOINAPI_H
