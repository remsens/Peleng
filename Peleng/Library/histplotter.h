#ifndef HISTPLOTTER_H
#define HISTPLOTTER_H

#include <QObject>

class HistPlotter : public QObject
{
    Q_OBJECT
public:
    explicit HistPlotter(QObject *parent = 0);

signals:

public slots:
};

#endif // HISTPLOTTER_H
