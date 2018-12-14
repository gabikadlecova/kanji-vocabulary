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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *detail_layout;
    QLabel *label;
    QLineEdit *kanji_edit;
    QLabel *label_2;
    QLineEdit *reading_edit;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *confirm_button;
    QLineEdit *meaning_edit;
    QLabel *label_3;

    void setupUi(QWidget *EditWidget)
    {
        if (EditWidget->objectName().isEmpty())
            EditWidget->setObjectName(QString::fromUtf8("EditWidget"));
        EditWidget->resize(800, 480);
        verticalLayoutWidget = new QWidget(EditWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(200, 20, 231, 421));
        detail_layout = new QVBoxLayout(verticalLayoutWidget);
        detail_layout->setSpacing(6);
        detail_layout->setObjectName(QString::fromUtf8("detail_layout"));
        detail_layout->setSizeConstraint(QLayout::SetMaximumSize);
        detail_layout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        detail_layout->addWidget(label);

        kanji_edit = new QLineEdit(verticalLayoutWidget);
        kanji_edit->setObjectName(QString::fromUtf8("kanji_edit"));

        detail_layout->addWidget(kanji_edit);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        detail_layout->addWidget(label_2);

        reading_edit = new QLineEdit(verticalLayoutWidget);
        reading_edit->setObjectName(QString::fromUtf8("reading_edit"));

        detail_layout->addWidget(reading_edit);

        gridLayoutWidget = new QWidget(EditWidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(550, 20, 221, 421));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        confirm_button = new QPushButton(gridLayoutWidget);
        confirm_button->setObjectName(QString::fromUtf8("confirm_button"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(confirm_button->sizePolicy().hasHeightForWidth());
        confirm_button->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(confirm_button, 3, 0, 1, 1);

        meaning_edit = new QLineEdit(gridLayoutWidget);
        meaning_edit->setObjectName(QString::fromUtf8("meaning_edit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(meaning_edit->sizePolicy().hasHeightForWidth());
        meaning_edit->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(meaning_edit, 2, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_3, 1, 0, 1, 1);


        retranslateUi(EditWidget);

        QMetaObject::connectSlotsByName(EditWidget);
    } // setupUi

    void retranslateUi(QWidget *EditWidget)
    {
        EditWidget->setWindowTitle(QApplication::translate("EditWidget", "Form", nullptr));
        label->setText(QApplication::translate("EditWidget", "Kanji text", nullptr));
        label_2->setText(QApplication::translate("EditWidget", "Reading", nullptr));
        confirm_button->setText(QApplication::translate("EditWidget", "Confirm", nullptr));
        label_3->setText(QApplication::translate("EditWidget", "Meaning", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditWidget: public Ui_EditWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITWIDGET_H
