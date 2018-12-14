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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrainFlipWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *detail_layout;
    QLabel *kanji_label;
    QLabel *meaning_label;
    QLabel *reading_label;
    QHBoxLayout *choices_layout;
    QPushButton *ok_button;
    QPushButton *maybe_button;
    QPushButton *no_button;

    void setupUi(QWidget *TrainFlipWidget)
    {
        if (TrainFlipWidget->objectName().isEmpty())
            TrainFlipWidget->setObjectName(QString::fromUtf8("TrainFlipWidget"));
        TrainFlipWidget->resize(800, 480);
        verticalLayoutWidget = new QWidget(TrainFlipWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(260, 10, 281, 451));
        detail_layout = new QVBoxLayout(verticalLayoutWidget);
        detail_layout->setObjectName(QString::fromUtf8("detail_layout"));
        detail_layout->setContentsMargins(0, 0, 0, 0);
        kanji_label = new QLabel(verticalLayoutWidget);
        kanji_label->setObjectName(QString::fromUtf8("kanji_label"));

        detail_layout->addWidget(kanji_label);

        meaning_label = new QLabel(verticalLayoutWidget);
        meaning_label->setObjectName(QString::fromUtf8("meaning_label"));

        detail_layout->addWidget(meaning_label);

        reading_label = new QLabel(verticalLayoutWidget);
        reading_label->setObjectName(QString::fromUtf8("reading_label"));

        detail_layout->addWidget(reading_label);

        choices_layout = new QHBoxLayout();
        choices_layout->setObjectName(QString::fromUtf8("choices_layout"));
        ok_button = new QPushButton(verticalLayoutWidget);
        ok_button->setObjectName(QString::fromUtf8("ok_button"));

        choices_layout->addWidget(ok_button);

        maybe_button = new QPushButton(verticalLayoutWidget);
        maybe_button->setObjectName(QString::fromUtf8("maybe_button"));

        choices_layout->addWidget(maybe_button);

        no_button = new QPushButton(verticalLayoutWidget);
        no_button->setObjectName(QString::fromUtf8("no_button"));

        choices_layout->addWidget(no_button);


        detail_layout->addLayout(choices_layout);


        retranslateUi(TrainFlipWidget);

        QMetaObject::connectSlotsByName(TrainFlipWidget);
    } // setupUi

    void retranslateUi(QWidget *TrainFlipWidget)
    {
        TrainFlipWidget->setWindowTitle(QApplication::translate("TrainFlipWidget", "Form", nullptr));
        kanji_label->setText(QApplication::translate("TrainFlipWidget", "TextLabel", nullptr));
        meaning_label->setText(QApplication::translate("TrainFlipWidget", "TextLabel", nullptr));
        reading_label->setText(QApplication::translate("TrainFlipWidget", "TextLabel", nullptr));
        ok_button->setText(QApplication::translate("TrainFlipWidget", "OK", nullptr));
        maybe_button->setText(QApplication::translate("TrainFlipWidget", "Maybe", nullptr));
        no_button->setText(QApplication::translate("TrainFlipWidget", "Wrong", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrainFlipWidget: public Ui_TrainFlipWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINFLIPWIDGET_H
