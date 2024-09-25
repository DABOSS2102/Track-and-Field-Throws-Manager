#ifndef THROWFORM_H
#define THROWFORM_H

#include <QMainWindow>

#include "ui_throwform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class throwform; }
QT_END_NAMESPACE

class throwform : public QMainWindow
{
    Q_OBJECT

public:
    throwform(QWidget *parent = nullptr);
    ~throwform();

private:
    Ui::throwform *ui;
};
#endif // THROWFORM_H
