#ifndef PLACEH_QT_WORKER_H
#define PLACEH_QT_WORKER_H

#if defined(HAVE_CONFIG_H)
#include "config/placeh-config.h"
#endif

#include <QObject>


class Worker : public QObject {
    Q_OBJECT
public:
    Worker();
    ~Worker();
public Q_SLOTS:
    void process();
    void finished();
    void error(QString err);
private:
    // add your variables here
};

#endif // PLACEH_QT_WORKER_H