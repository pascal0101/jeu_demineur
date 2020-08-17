#ifndef PLATEAU_H
#define PLATEAU_H

#include <QMainWindow>
#include <QFrame>
#include <QGridLayout>
#include "niveau.h"


class Plateau : public QMainWindow
{
    Q_OBJECT
private:
    Niveau *game;
    QGridLayout *grid_layout;

    void paintGrid();
public:
    Plateau(QWidget *parent = nullptr);
    virtual QSize sizeHint() const override;
    ~Plateau();
    int countTimer;


signals:

public slots:


};
#endif // PLATEAU_H
