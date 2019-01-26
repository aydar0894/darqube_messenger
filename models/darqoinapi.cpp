#include <TreeFrogModel>
#include "darqoinapi.h"
#include "darqoinapiobject.h"

DarqoinApi::DarqoinApi()
    : TAbstractModel(), d(new DarqoinApiObject())
{
    d->id = 0;
    d->api_ver = 0;
}

DarqoinApi::DarqoinApi(const DarqoinApi &other)
    : TAbstractModel(), d(new DarqoinApiObject(*other.d))
{ }

DarqoinApi::DarqoinApi(const DarqoinApiObject &object)
    : TAbstractModel(), d(new DarqoinApiObject(object))
{ }

DarqoinApi::~DarqoinApi()
{
    // If the reference count becomes 0,
    // the shared data object 'DarqoinApiObject' is deleted.
}

int DarqoinApi::id() const
{
    return d->id;
}

qlonglong DarqoinApi::apiVer() const
{
    return d->api_ver;
}

void DarqoinApi::setApiVer(const qlonglong &apiVer)
{
    d->api_ver = apiVer;
}

DarqoinApi &DarqoinApi::operator=(const DarqoinApi &other)
{
    d = other.d;  // increments the reference count of the data
    return *this;
}

DarqoinApi DarqoinApi::create(const qlonglong &apiVer)
{
    DarqoinApiObject obj;
    obj.api_ver = apiVer;
    if (!obj.create()) {
        return DarqoinApi();
    }
    return DarqoinApi(obj);
}

DarqoinApi DarqoinApi::create(const QVariantMap &values)
{
    DarqoinApi model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

DarqoinApi DarqoinApi::get(int id)
{
    TSqlORMapper<DarqoinApiObject> mapper;
    return DarqoinApi(mapper.findByPrimaryKey(id));
}

int DarqoinApi::count()
{
    TSqlORMapper<DarqoinApiObject> mapper;
    return mapper.findCount();
}

QList<DarqoinApi> DarqoinApi::getAll()
{
    return tfGetModelListByCriteria<DarqoinApi, DarqoinApiObject>(TCriteria());
}

QJsonArray DarqoinApi::getAllJson()
{
    QJsonArray array;
    TSqlORMapper<DarqoinApiObject> mapper;

    if (mapper.find() > 0) {
        for (TSqlORMapperIterator<DarqoinApiObject> i(mapper); i.hasNext(); ) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(DarqoinApi(i.next()).toVariantMap())));
        }
    }
    return array;
}

TModelObject *DarqoinApi::modelData()
{
    return d.data();
}

const TModelObject *DarqoinApi::modelData() const
{
    return d.data();
}

QDataStream &operator<<(QDataStream &ds, const DarqoinApi &model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream &operator>>(QDataStream &ds, DarqoinApi &model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(DarqoinApi)
