#include <QtCore>
#include <TreeFrogView>
#include "darqoinapi.h" 
#include "applicationhelper.h"

class T_VIEW_EXPORT darqoinapi_indexView : public TActionView
{
  Q_OBJECT
public:
  darqoinapi_indexView() : TActionView() { }
  QString toString();
};

QString darqoinapi_indexView::toString()
{
  responsebody.reserve(1565);
  responsebody += QLatin1String("<!DOCTYPE html>\r\n");
    responsebody += QLatin1String("<html>\r\n<head>\r\n  <meta charset=\"UTF-8\">\r\n  <title>");
  responsebody += THttpUtility::htmlEscape(controller()->name() + ": " + controller()->activeAction());
  responsebody += QLatin1String("</title>\r\n</head>\r\n<body>\r\n\r\n<h1>Listing Darqoin Api</h1>\r\n\r\n");
  responsebody += QVariant(linkTo("Create a new Darqoin Api", urla("create"))).toString();
  responsebody += QLatin1String("<br />\r\n<br />\r\n<table border=\"1\" cellpadding=\"5\" style=\"border: 1px #d0d0d0 solid; border-collapse: collapse;\">\r\n  <tr>\r\n    <th>ID</th>\r\n    <th>Api Ver</th>\r\n  </tr>\r\n");
  tfetch(QList<DarqoinApi>, darqoinApiList);
  for (const auto &i : darqoinApiList) {
  responsebody += QLatin1String("  <tr>\r\n    <td>");
  responsebody += THttpUtility::htmlEscape(i.id());
  responsebody += QLatin1String("</td>\r\n    <td>");
  responsebody += THttpUtility::htmlEscape(i.apiVer());
  responsebody += QLatin1String("</td>\r\n    <td>\r\n      ");
  responsebody += QVariant(linkTo("Show", urla("show", i.id()))).toString();
  responsebody += QLatin1String("\r\n      ");
  responsebody += QVariant(linkTo("Edit", urla("save", i.id()))).toString();
  responsebody += QLatin1String("\r\n      ");
  responsebody += QVariant(linkTo("Remove", urla("remove", i.id()), Tf::Post, "confirm('Are you sure?')")).toString();
  responsebody += QLatin1String("\r\n    </td>\r\n  </tr>\r\n");
  };
  responsebody += QLatin1String("</table>\r\n\r\n</body>\r\n</html>\r\n");

  return responsebody;
}

T_DEFINE_VIEW(darqoinapi_indexView)

#include "darqoinapi_indexView.moc"
