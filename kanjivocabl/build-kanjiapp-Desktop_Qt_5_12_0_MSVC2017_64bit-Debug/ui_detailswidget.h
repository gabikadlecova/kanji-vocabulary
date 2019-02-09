/********************************************************************************
** Form generated from reading UI file 'detailswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DETAILSWIDGET_H
#define UI_DETAILSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DetailsWidget
{
public:

    void setupUi(QWidget *DetailsWidget)
    {
        if (DetailsWidget->objectName().isEmpty())
            DetailsWidget->setObjectName(QString::fromUtf8("DetailsWidget"));
        DetailsWidget->resize(800, 480);

        retranslateUi(DetailsWidget);

        QMetaObject::connectSlotsByName(DetailsWidget);
    } // setupUi

    void retranslateUi(QWidget *DetailsWidget)
    {
        DetailsWidget->setWindowTitle(QApplication::translate("DetailsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DetailsWidget: public Ui_DetailsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DETAILSWIDGET_H
