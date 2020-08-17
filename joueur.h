#ifndef JOUEUR_H
#define JOUEUR_H

#include <QWidget>

class Joueur : public QWidget
{
    Q_OBJECT
public:
    explicit Joueur(QWidget *parent = nullptr);
private:
    QString nom;
signals:

};

#endif // JOUEUR_H
