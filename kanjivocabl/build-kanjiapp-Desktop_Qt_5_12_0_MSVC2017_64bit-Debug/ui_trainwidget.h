/********************************************************************************
** Form generated from reading UI file 'trainwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAINWIDGET_H
#define UI_TRAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrainWidget
{
public:

    void setupUi(QWidget *TrainWidget)
    {
        if (TrainWidget->objectName().isEmpty())
            TrainWidget->setObjectName(QString::fromUtf8("TrainWidget"));
        TrainWidget->resize(800, 480);

        retranslateUi(TrainWidget);

        QMetaObject::connectSlotsByName(TrainWidget);
    } // setupUi

    void retranslateUi(QWidget *TrainWidget)
    {
        TrainWidget->setWindowTitle(QApplication::translate("TrainWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrainWidget: public Ui_TrainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINWIDGET_H
