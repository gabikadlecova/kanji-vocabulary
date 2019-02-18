#include "iolib.h"

#include <fstream>
#include <codecvt>
#include <iostream>

#include <QFileDialog>

// Construct LibManip. If the parameter isn't an empty string,
// LibManip::readLib() or LibManip::onSaveData() can be called immediately to load
// or save data respectively.
LibManip::LibManip(std::string fileName, QWidget *parent) :
    QObject(parent),
    fileName(fileName)
{
    this->parent = parent;
}

// Saves data to path LibManip::fileName or opens a QFileDialog for
// path selection. If the dialog is dismissed, LibManip::noSaveFileName()
// is emitted. The data till be encoded in utf8.
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

    // save to file

    std::wofstream wos{ fileName };
    wos.imbue(std::locale(std::locale::empty(),
                          new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    kanji_data::write_lib(lib, wos);

    wos.close();
}

// Loads data from the path selected through a QFileDialog. Calls LibManip::readLib()
void LibManip::onLoadData()
{
    auto fname = QFileDialog::getOpenFileName(this->parent, "Open kanji data");

    if (fname == "") {
        emit noLoadFileSelected();
        return;
    }

    fileName = fname.toStdString();

    readLib();

}

// Loads data from the path LibManip::fileName.
// The data is expected to be in utf8. Emits LibManip::dataLoaded() on
// success and LibManip::loadFailed() on failure.
void LibManip::readLib()
{
    std::wifstream wis{ fileName };
    wis.imbue(std::locale(std::locale::empty(),
                          new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    try {
        kanji_data::kanji_lib lib = kanji_data::read_lib(wis);
        wis.close();

        emit dataLoaded(std::move(lib));
    }
    catch (const std::logic_error &e) {
        // format error
        wis.close();

        emit loadFailed();
        return;
    }
}
