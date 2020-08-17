#ifndef CASE_H
#define CASE_H

#include <QObject>
#include <QFrame>
#include <QLabel>
#include "plateau.h"
#include "niveau.h"

class Case : public QFrame
{
    Q_OBJECT
private:
    Niveau::Index cell;
    QLabel *icon;

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

public:
    explicit Case(const Niveau::Index &c,
                        QWidget *parent = nullptr);
    virtual QSize sizeHint() const override;
    Niveau::Index getCell() const;

signals:
    void clicked();
    void rightClicked();
    void doubleClicked();

public slots:
    void update();
    void showContent();
};

#endif // CASE_H
