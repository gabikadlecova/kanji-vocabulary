#include "iolib.h"

#include <QFileDialog>
#include <QMessageBox>

#include <fstream>
#include <codecvt>
#include <iostream>
#include <string>


bool saveToDefault()
{
    QMessageBox defBox;
    defBox.setText("Set to default");
    defBox.setInformativeText("Do you want to set the path as the default path?");
    defBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    auto res = defBox.exec();
    return res == QMessageBox::Yes;
}


// Construct LibManip. If the parameter isn't an empty string,
// LibManip::readLib() or LibManip::onSaveData() can be called immediately to load
// or save data respectively.
LibManip::LibManip(std::string settingsPath, QWidget *parent) :
    QObject(parent),
    settingsPath(std::move(settingsPath))
{
    this->parent = parent;

    readSettings(this->settingsPath);

    filePath = s.defaultPath;
}


// Saves data to path LibManip::fileName or opens a QFileDialog for
// path selection. If the dialog is dismissed, LibManip::noSaveFileName()
// is emitted. The data till be encoded in utf8.
void LibManip::onSaveData(const kanji_data::kanji_lib &lib, bool def)
{
    std::string path = filePath;
    // select file for saving
    if (!def || filePath == "") {
        auto fname = QFileDialog::getSaveFileName(this->parent, "Save kanji data");

        if (fname == "") {
            emit noSaveFileSelected();
            return;
        }

        path = fname.toStdString();

        if (saveToDefault()) {
            s.defaultPath = fname.toStdString();
        }
    }

    // save to file
    std::wofstream wos{ path };
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

    readLib(fname.toStdString());

    if (saveToDefault()){
        s.defaultPath = fname.toStdString();
    }

}


// Loads data from the path LibManip::fileName.
// The data is expected to be in utf8. Emits LibManip::dataLoaded() on
// success and LibManip::loadFailed() on failure.
void LibManip::readLib(std::string fname)
{
    std::wifstream wis{ fname };
    wis.imbue(std::locale(std::locale::empty(),
                          new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    try {
        kanji_data::kanji_lib lib = kanji_data::read_lib(wis);
        wis.close();

        emit dataLoaded(std::move(lib));
    }
    catch (const kanji_data::format_error &e) {
        wis.close();

        emit loadFailed();
        return;
    }
}


// loads settings
void LibManip::onLoadSettings()
{
    auto fname = QFileDialog::getOpenFileName(this->parent, "Load settings");

    if (fname == "") {
        emit noLoadFileSelected();
        return;
    }

    readSettings(fname.toStdString());
    emit settingsLoaded(s);
}


// saves settings
void LibManip::onSaveSettings(Settings s, bool def)
{
    this->s = s;

    std::string path = settingsPath;
    if (!def) {
        auto fname = QFileDialog::getSaveFileName(this->parent, "Save settings");

        if (fname == "") {
            emit noSaveFileSelected();
            return;
        }

        path = fname.toStdString();
    }

    std::ofstream os{ path };

    os << s.defaultPath << std::endl;
    os << s.kanjiPerRep << std::endl;
    os << s.maxLevel << std::endl;
    os << static_cast<int>(s.multiplier) << std::endl;  // avoid writing char

}


// reads settings from the file
void LibManip::readSettings(std::string fname)
{
    std::ifstream is{ fname };

    if (!is.good()) {
        emit loadFailed();
    }

    // settings load

    std::string defPath;
    std::getline(is, defPath);

    std::string kanjiPerRepStr;
    std::getline(is, kanjiPerRepStr);
    int kanjiPerRep = std::stoi(kanjiPerRepStr);

    std::string maxLevelStr;
    std::getline(is, maxLevelStr);
    kanji_data::kanji_compound::kanji_level maxLevel = std::stoi(maxLevelStr);

    std::string multiplierStr;
    std::getline(is, multiplierStr);

    int_least8_t multiplier = static_cast<int_least8_t>(std::stoi(multiplierStr));

    s = Settings(defPath, kanjiPerRep, maxLevel, multiplier);
}
