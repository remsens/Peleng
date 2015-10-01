#ifndef THREADSPLASH_H
#define THREADSPLASH_H

#include <QObject>

#include <QtWidgets>
class ThreadSplash : public QThread
{
    Q_OBJECT
public:
    ThreadSplash();
    ~ThreadSplash();
    void run();
public slots:
    void slotClose();

private:
    QLabel *labelSplash;

};

#endif // THREADSPLASH_H
