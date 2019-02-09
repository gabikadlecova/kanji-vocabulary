/********************************************************************************
** Form generated from reading UI file 'editwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITWIDGET_H
#define UI_EDITWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditWidget
{
public:

    void setupUi(QWidget *EditWidget)
    {
        if (EditWidget->objectName().isEmpty())
            EditWidget->setObjectName(QString::fromUtf8("EditWidget"));
        EditWidget->resize(800, 480);

        retranslateUi(EditWidget);

        QMetaObject::connectSlotsByName(EditWidget);
    } // setupUi

    void retranslateUi(QWidget *EditWidget)
    {
        EditWidget->setWindowTitle(QApplication::translate("EditWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditWidget: public Ui_EditWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITWIDGET_H
