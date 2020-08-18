#ifndef CHRONO_H
#define CHRONO_H

#include <QObject>

class Chrono : public QObject
{
    Q_OBJECT
public:
    explicit Chrono(QObject *parent = nullptr);

signals:

};

#endif // CHRONO_H
