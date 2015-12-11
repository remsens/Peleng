#include "histplugin.h"



HistPlugin::HistPlugin( QObject *parent)
    : QObject(parent)
{

}

HistPlugin::~HistPlugin()
{
    for (int i = 0; i < m_listWindows.size(); i++)
    {     
        delete m_listWindows.at(i);
        m_listWindows.removeAt(i);
    }
}

QObject* HistPlugin::GetObjectPointer()
{
    return this;
}

void HistPlugin::OnClose(HistPlotterWidget* w)
{
    for (int i = 0; i < m_listWindows.size(); i++)
    {
        if (m_listWindows.at(i) == w)
        {
            delete m_listWindows.at(i);
            m_listWindows.removeAt(i);
        }
    }
}

void HistPlugin::sendAttr(qint32 channel, Attributes *attr)
{
    emit this->replotChannel(channel,attr);
}

void HistPlugin::Execute(HyperCube *cube, Attributes *attr)
{
    // TODO
    // проверять, построена уже такая гистограмма или нет


    HistPlotterWidget *ContrastPlotter = new HistPlotterWidget(cube,attr);
    m_listWindows.append(ContrastPlotter);
    if (ContrastPlotter->exec()==QDialog::Accepted) {
        emit this->replotChannel(0,attr);
        qDebug() << "update plot 2d";
    };
    //ContrastPlotter->Plot();



  /*  HistPlotter *plotter= new HistPlotter(cube, attr);//TODO
    connect(plotter, SIGNAL(Close(HistPlotter*)), this, SLOT(OnClose(HistPlotter*)));
    connect(plotter, SIGNAL(changeAttr(qint32, Attributes *)), this, SLOT(sendAttr(qint32, Attributes *)));
    plotter->Plot();*/
    //attr->SetPointsList();


}
