/********************************************************************************
** Form generated from reading UI file 'kanjilistwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KANJILISTWIDGET_H
#define UI_KANJILISTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KanjiListWidget
{
public:
    QListView *kanji_list;

    void setupUi(QWidget *KanjiListWidget)
    {
        if (KanjiListWidget->objectName().isEmpty())
            KanjiListWidget->setObjectName(QString::fromUtf8("KanjiListWidget"));
        KanjiListWidget->resize(800, 480);
        kanji_list = new QListView(KanjiListWidget);
        kanji_list->setObjectName(QString::fromUtf8("kanji_list"));
        kanji_list->setGeometry(QRect(200, 10, 411, 451));

        retranslateUi(KanjiListWidget);

        QMetaObject::connectSlotsByName(KanjiListWidget);
    } // setupUi

    void retranslateUi(QWidget *KanjiListWidget)
    {
        KanjiListWidget->setWindowTitle(QApplication::translate("KanjiListWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KanjiListWidget: public Ui_KanjiListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KANJILISTWIDGET_H