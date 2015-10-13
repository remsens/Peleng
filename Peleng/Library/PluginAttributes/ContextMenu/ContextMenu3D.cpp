#include "ContextMenu3D.h"

ContextMenu3D::ContextMenu3D()
{

}
ContextMenu3D::~ContextMenu3D()
{

}
QList<ContextMenu> ContextMenu3D::GetContextMenuPlugin(const QMap<QString, QString>& availablePlugins)
{
    // Формируем контекстное меню для 3D плагина
    // 2D, Spectr, LinePlotter
    QList<ContextMenu> context3DPlugin;
    ContextMenu ctxMenu;
    ctxMenu.namePlugin = "Spectr UI";
    ctxMenu.discriptionPlugin = "Спектр";
    QPixmap icon("qrc:/pluginsIcons/icons/Spectr.png");
    ctxMenu.iconPlugin = icon;
    context3DPlugin.append(ctxMenu);

    ctxMenu.namePlugin = "Line Plotter UI";
    ctxMenu.discriptionPlugin = "Спектральный срез";
    icon.load("qrc:/pluginsIcons/icons/LinePlotter.png");
    context3DPlugin.append(ctxMenu);

    ctxMenu.namePlugin = "2D UI";
    ctxMenu.discriptionPlugin = "2D изображение";
    icon.load("qrc:/pluginsIcons/icons/2D.png");
    context3DPlugin.append(ctxMenu);
    return CreateContextMenu(context3DPlugin, availablePlugins);
}
