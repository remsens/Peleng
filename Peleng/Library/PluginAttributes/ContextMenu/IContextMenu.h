#ifndef ICONTEXTMENU
#define ICONTEXTMENU

#include <QString>
#include <QList>
#include <QMap>
#include <QPixmap>

struct ContextMenu
{
    QString namePlugin;
    QString discriptionPlugin;
    QPixmap iconPlugin;
};

class IContextPluginMenu
{
public:
    virtual ~IContextPluginMenu()
    {
    }
   virtual QList<ContextMenu> GetContextMenuPlugin(const QMap<QString, QString>& availablePlugins) = 0;

};

#endif // ICONTEXTMENU

