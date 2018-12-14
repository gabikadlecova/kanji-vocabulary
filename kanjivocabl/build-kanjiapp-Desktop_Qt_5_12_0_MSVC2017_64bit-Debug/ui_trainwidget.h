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
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrainWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *detail_layout;
    QLabel *kanji_label;
    QPushButton *flip_button;

    void setupUi(QWidget *TrainWidget)
    {
        if (TrainWidget->objectName().isEmpty())
            TrainWidget->setObjectName(QString::fromUtf8("TrainWidget"));
        TrainWidget->resize(800, 480);
        verticalLayoutWidget = new QWidget(TrainWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(260, 10, 271, 461));
        detail_layout = new QVBoxLayout(verticalLayoutWidget);
        detail_layout->setObjectName(QString::fromUtf8("detail_layout"));
        detail_layout->setContentsMargins(0, 0, 0, 0);
        kanji_label = new QLabel(verticalLayoutWidget);
        kanji_label->setObjectName(QString::fromUtf8("kanji_label"));

        detail_layout->addWidget(kanji_label);

        flip_button = new QPushButton(verticalLayoutWidget);
        flip_button->setObjectName(QString::fromUtf8("flip_button"));

        detail_layout->addWidget(flip_button);


        retranslateUi(TrainWidget);

        QMetaObject::connectSlotsByName(TrainWidget);
    } // setupUi

    void retranslateUi(QWidget *TrainWidget)
    {
        TrainWidget->setWindowTitle(QApplication::translate("TrainWidget", "Form", nullptr));
        kanji_label->setText(QApplication::translate("TrainWidget", "TextLabel", nullptr));
        flip_button->setText(QApplication::translate("TrainWidget", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrainWidget: public Ui_TrainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINWIDGET_H
