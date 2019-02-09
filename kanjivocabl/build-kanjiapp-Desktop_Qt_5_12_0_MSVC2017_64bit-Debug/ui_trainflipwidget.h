/********************************************************************************
** Form generated from reading UI file 'trainflipwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAINFLIPWIDGET_H
#define UI_TRAINFLIPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrainFlipWidget
{
public:

    void setupUi(QWidget *TrainFlipWidget)
    {
        if (TrainFlipWidget->objectName().isEmpty())
            TrainFlipWidget->setObjectName(QString::fromUtf8("TrainFlipWidget"));
        TrainFlipWidget->resize(800, 480);

        retranslateUi(TrainFlipWidget);

        QMetaObject::connectSlotsByName(TrainFlipWidget);
    } // setupUi

    void retranslateUi(QWidget *TrainFlipWidget)
    {
        TrainFlipWidget->setWindowTitle(QApplication::translate("TrainFlipWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrainFlipWidget: public Ui_TrainFlipWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINFLIPWIDGET_H
