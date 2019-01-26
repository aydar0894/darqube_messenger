#include <QtCore>
#include <TreeFrogView>
#include "darqoinapi.h" 
#include "applicationhelper.h"

class T_VIEW_EXPORT darqoinapi_saveView : public TActionView
{
  Q_OBJECT
public:
  darqoinapi_saveView() : TActionView() { }
  QString toString();
};

QString darqoinapi_saveView::toString()
{
  responsebody.reserve(1501);
  responsebody += QLatin1String("<!DOCTYPE html>\r\n");
    tfetch(QVariantMap, darqoinApi);
  responsebody += QLatin1String("<html>\r\n<head>\r\n  <meta charset=\"UTF-8\">\r\n  <title>");
  responsebody += THttpUtility::htmlEscape(controller()->name() + ": " + controller()->activeAction());
  responsebody += QLatin1String("</title>\r\n</head>\r\n<body>\r\n<p style=\"color: red\">");
  tehex(error);
  responsebody += QLatin1String("</p>\r\n<p style=\"color: green\">");
  tehex(notice);
  responsebody += QLatin1String("</p>\r\n\r\n<h1>Editing Darqoin Api</h1>\r\n\r\n");
  responsebody += QVariant(formTag(urla("save", darqoinApi["id"]), Tf::Post)).toString();
  responsebody += QLatin1String("\r\n  <p>\r\n    <label>ID<br /><input type=\"text\" name=\"darqoinApi[id]\" value=\"");
  responsebody += THttpUtility::htmlEscape(darqoinApi["id"]);
  responsebody += QLatin1String("\" readonly=\"readonly\" /></label>\r\n  </p>\r\n  <p>\r\n    <label>Api Ver<br /><input type=\"text\" name=\"darqoinApi[apiVer]\" value=\"");
  responsebody += THttpUtility::htmlEscape(darqoinApi["apiVer"]);
  responsebody += QLatin1String("\" /></label>\r\n  </p>\r\n  <p>\r\n    <input type=\"submit\" value=\"Save\" />\r\n  </p>\r\n</form>\r\n\r\n");
  responsebody += QVariant(linkTo("Show", urla("show", darqoinApi["id"]))).toString();
  responsebody += QLatin1String(" |\r\n");
  responsebody += QVariant(linkTo("Back", urla("index"))).toString();
  responsebody += QLatin1String("\r\n</body>\r\n</html>\r\n");

  return responsebody;
}

T_DEFINE_VIEW(darqoinapi_saveView)

#include "darqoinapi_saveView.moc"
