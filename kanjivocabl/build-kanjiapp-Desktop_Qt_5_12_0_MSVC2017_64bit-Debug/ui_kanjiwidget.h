/********************************************************************************
** Form generated from reading UI file 'kanjiwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KANJIWIDGET_H
#define UI_KANJIWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KanjiWidget
{
public:

    void setupUi(QWidget *KanjiWidget)
    {
        if (KanjiWidget->objectName().isEmpty())
            KanjiWidget->setObjectName(QString::fromUtf8("KanjiWidget"));
        KanjiWidget->resize(800, 480);

        retranslateUi(KanjiWidget);

        QMetaObject::connectSlotsByName(KanjiWidget);
    } // setupUi

    void retranslateUi(QWidget *KanjiWidget)
    {
        KanjiWidget->setWindowTitle(QApplication::translate("KanjiWidget", "KanjiWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KanjiWidget: public Ui_KanjiWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KANJIWIDGET_H
