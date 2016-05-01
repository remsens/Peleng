#include "FileProjectFeatures.h"

#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

FileProjectFeatures::FileProjectFeatures()
{

}

FileProjectFeatures::~FileProjectFeatures()
{

}

bool FileProjectFeatures::AddStep(QString& filePath, int stepTag, const QString& tagDescription, QWidget* widget)
{
    QString stepDescription = QString("Шаг ") + QString::number(stepTag) + ". " + tagDescription;
    // начинаем читать файл
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(widget, "Ошибка добавления действия", "Невозможно открыть файл-проекта для записи");
    }
    QDomDocument doc;
       if (!doc.setContent(&file)) {
           file.close();
           QMessageBox::critical(widget, "Ошибка добавления действия", "Невозможно установить контент файла");
       }
    QDomNode n = doc.firstChild();
    bool b_write = false;
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
            QString name = e.tagName();
            if(name == "project")
            {
                n = n.firstChildElement();
                n = n.nextSiblingElement();
            } else if (name == "Steps")
            {
                QDomElement stepElement2 = doc.createElement("Step");
                QDomText stepPathText2 = doc.createTextNode(stepDescription);
                stepElement2.appendChild(stepPathText2);
                n.appendChild(stepElement2);
                b_write = true;
                break;
            } else
            {
             n = n.nextSiblingElement();
            }
         }
   }
   if (!b_write)
   {
        n = doc.firstChild();
        while(!n.isNull())
        {
            if(n.isElement())
            {
                QDomElement e = n.toElement();
                QString name = e.tagName();
                if(name == "project")
                {
                    n = n.firstChildElement();
                    n = n.nextSiblingElement();
                } else if (name == "spectralLib")
                {

                    QDomElement firstStepsElement = doc.createElement("Steps");
                    QDomElement stepElement2 = doc.createElement("Step");
                    QDomText stepPathText2 = doc.createTextNode(stepDescription);
                    stepElement2.appendChild(stepPathText2);
                    firstStepsElement.appendChild(stepElement2);
                    n.parentNode().appendChild(firstStepsElement);
                    break;
                } else
                {
                 n = n.nextSiblingElement();
                }
             }
        }
   }
   file.resize(0);
   QTextStream(&file) <<  doc.toString();
   file.close();
   return false;
}

// при нажатии на кнопку назад
bool FileProjectFeatures::DeleteStep(QString& filePath, int stepTag, QString& tagDescription, QWidget *widget)
{
    QString stepDescription = QString("Шаг ") + QString::number(stepTag) + ". " + tagDescription;
    // начинаем читать файл
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(widget, "Ошибка добавления действия", "Невозможно открыть файл-проекта для записи");
    }
    QDomDocument doc;
       if (!doc.setContent(&file)) {
           file.close();
           QMessageBox::critical(widget, "Ошибка добавления действия", "Невозможно установить контент файла");
       }
    QDomNodeList domTwm = doc.elementsByTagName( "Step" );
    for (int i = 0; i < domTwm.length(); i++ )
    {
        if ( domTwm.at(i).toElement().text() == stepDescription )
        {
            QDomNode node = domTwm.at(i);
            node.parentNode().removeChild(node);
        }
    }
   file.resize(0);
   QTextStream(&file) << doc.toString();
   file.close();
    return false;
}

// при нажатии на кнопку
