#include <QtCore>
#include <TreeFrogView>
#include "darqoinapi.h" 
#include "applicationhelper.h"

class T_VIEW_EXPORT darqoinapi_showView : public TActionView
{
  Q_OBJECT
public:
  darqoinapi_showView() : TActionView() { }
  QString toString();
};

QString darqoinapi_showView::toString()
{
  responsebody.reserve(1106);
  responsebody += QLatin1String("<!DOCTYPE html>\r\n");
    tfetch(DarqoinApi, darqoinApi);
  responsebody += QLatin1String("<html>\r\n<head>\r\n  <meta charset=\"UTF-8\">\r\n  <title>");
  responsebody += THttpUtility::htmlEscape(controller()->name() + ": " + controller()->activeAction());
  responsebody += QLatin1String("</title>\r\n</head>\r\n<body>\r\n<p style=\"color: red\">");
  tehex(error);
  responsebody += QLatin1String("</p>\r\n<p style=\"color: green\">");
  tehex(notice);
  responsebody += QLatin1String("</p>\r\n\r\n<h1>Showing Darqoin Api</h1>\r\n<dt>ID</dt><dd>");
  responsebody += THttpUtility::htmlEscape(darqoinApi.id());
  responsebody += QLatin1String("</dd><br />\r\n<dt>Api Ver</dt><dd>");
  responsebody += THttpUtility::htmlEscape(darqoinApi.apiVer());
  responsebody += QLatin1String("</dd><br />\r\n\r\n");
  responsebody += QVariant(linkTo("Edit", urla("save", darqoinApi.id()))).toString();
  responsebody += QLatin1String(" |\r\n");
  responsebody += QVariant(linkTo("Back", urla("index"))).toString();
  responsebody += QLatin1String("\r\n\r\n</body>\r\n</html>\r\n");

  return responsebody;
}

T_DEFINE_VIEW(darqoinapi_showView)

#include "darqoinapi_showView.moc"
