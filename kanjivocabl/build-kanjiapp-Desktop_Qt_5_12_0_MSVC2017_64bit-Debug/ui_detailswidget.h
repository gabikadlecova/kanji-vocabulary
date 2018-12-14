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
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DetailsWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *detail_layout;
    QLabel *kanji_label;
    QLabel *meaning_label;
    QLabel *reading_label;
    QPushButton *edit_button;
    QPushButton *reset_button;

    void setupUi(QWidget *DetailsWidget)
    {
        if (DetailsWidget->objectName().isEmpty())
            DetailsWidget->setObjectName(QString::fromUtf8("DetailsWidget"));
        DetailsWidget->resize(800, 480);
        verticalLayoutWidget = new QWidget(DetailsWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(270, 10, 251, 441));
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

        edit_button = new QPushButton(verticalLayoutWidget);
        edit_button->setObjectName(QString::fromUtf8("edit_button"));

        detail_layout->addWidget(edit_button);

        reset_button = new QPushButton(verticalLayoutWidget);
        reset_button->setObjectName(QString::fromUtf8("reset_button"));

        detail_layout->addWidget(reset_button);


        retranslateUi(DetailsWidget);

        QMetaObject::connectSlotsByName(DetailsWidget);
    } // setupUi

    void retranslateUi(QWidget *DetailsWidget)
    {
        DetailsWidget->setWindowTitle(QApplication::translate("DetailsWidget", "Form", nullptr));
        kanji_label->setText(QApplication::translate("DetailsWidget", "TextLabel", nullptr));
        meaning_label->setText(QApplication::translate("DetailsWidget", "TextLabel", nullptr));
        reading_label->setText(QApplication::translate("DetailsWidget", "TextLabel", nullptr));
        edit_button->setText(QApplication::translate("DetailsWidget", "Edit", nullptr));
        reset_button->setText(QApplication::translate("DetailsWidget", "Reset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DetailsWidget: public Ui_DetailsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DETAILSWIDGET_H
