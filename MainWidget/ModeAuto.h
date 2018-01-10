#ifndef AUTOM_H
#define AUTOM_H

#include <QWidget>
#include <QPointer>

class QPushButton;

class ModeAuto : public QWidget
{
    Q_OBJECT
public:
    explicit ModeAuto(QWidget *parent = nullptr);
private:
    QPointer<QPushButton> go;
public slots:
    void sl_go();
};

#endif // AUTOM_H

