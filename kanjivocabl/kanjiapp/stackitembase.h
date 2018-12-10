#ifndef STACKITEMBASE_H
#define STACKITEMBASE_H

#include <QWidget>

namespace Ui {
class StackItemBase;
}

class StackItemBase : public QWidget
{
    Q_OBJECT

public:
    explicit StackItemBase(QWidget *parent = nullptr);
    ~StackItemBase();

private:
    Ui::StackItemBase *ui;
};

#endif // STACKITEMBASE_H
