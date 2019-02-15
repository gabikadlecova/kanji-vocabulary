#include "iolib.h"

#include <fstream>
#include <codecvt>
#include <iostream>

#include <QFileDialog>

LibManip::LibManip(QWidget *parent) :
    QObject(parent)
{
    this->parent = parent;
}


void LibManip::onSaveData(const kanji_data::kanji_lib &lib)
{
    // select file for saving
    if (fileName == "") {
        auto fname = QFileDialog::getSaveFileName(this->parent, "Save kanji data");

        if (fname == "") {
            emit noSaveFileSelected();
            return;
        }

        fileName = fname.toStdString();
    }

    // save

    std::wofstream wos{ fileName };
    wos.imbue(std::locale(std::locale::empty(),
                          new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    kanji_data::write_lib(lib, wos);

    wos.close();

    // TODO errors
}

void LibManip::onLoadData()
{
    auto fname = QFileDialog::getOpenFileName(this->parent, "Open kanji data");

    if (fname == "") {
        emit noLoadFileSelected();
    }

    fileName = fname.toStdString();

    std::wifstream wis{ fileName };
    wis.imbue(std::locale(std::locale::empty(),
                          new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    kanji_data::kanji_lib lib = kanji_data::read_lib(wis);

    wis.close();

    emit dataLoaded(std::move(lib));

    // TODO errors
}
