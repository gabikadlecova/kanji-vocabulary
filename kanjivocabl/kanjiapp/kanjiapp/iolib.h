#ifndef IOLIB_H
#define IOLIB_H

#include <QObject>

#include "kanjiapp/KanjiData.h"


/*!
 * \brief The Settings class Represents settings of the app.
 */
class Settings
{
public:
    Settings():
        defaultPath(""),
        kanjiPerRep(30),
        maxLevel(30),
        multiplier(2)
    {}

    Settings(std::string path,
             int perRep,
             kanji_data::kanji_compound::kanji_level lev,
             std::int_least8_t mult):
        defaultPath(path),
        kanjiPerRep(perRep),
        maxLevel(lev),
        multiplier(mult)
    {}

    std::string defaultPath;
    int kanjiPerRep;

    kanji_data::kanji_compound::kanji_level maxLevel;
    int_least8_t multiplier;
};

/*!
 * \brief The LibManip class provides methods for data loading and saving.
 * The LibManip class loads data from and saves data to a file. The file path
 * is either provided on construction, or is determined dynamically via a
 * QFileDialog instance.
 */
class LibManip : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief LibManip can be constructed with a default file path or without.
     * \param defaultName Default load and save path
     * \param parent QObject parent
     */
    LibManip(std::string settingsPath, QWidget *parent = nullptr);
    virtual ~LibManip() override = default;

    Settings s;

signals:
    /*!
     * \brief dataLoaded Signals that data has been loaded to \p lib.
     * \param lib The loaded data in a kanji_lib variable.
     */
    void dataLoaded(kanji_data::kanji_lib lib);
    /*!
     * \brief loadFailed Is emitted on a format error in the file.
     */
    void loadFailed();
    /*!
     * \brief noSaveFileSelected Is emitted if no file was selected in the save dialog.
     */
    void noSaveFileSelected();
    /*!
     * \brief noLoadFileSelected Is emitted if no file was selected in the load dialog.
     */
    void noLoadFileSelected();

    /*!
     * \brief settingsLoaded Signals that app settings have been loaded.
     * \param s The loaded app settings.
     */
    void settingsLoaded(Settings s);

public slots:
    /*!
     * \brief onLoadData Handler for data load requests.
     * Opens a QFileDialog for file selection, loads data from the specified file.
     * Emits loadFailed() and noLoadFileSelected() on failure, settingsLoaded() on
     * success.
     */
    void onLoadData();
    /*!
     * \brief onSaveData Saves data to the default location or enables selecting a path.
     * \param lib Data which is to be saved.
     * Saves data from lib to the path specified in #LibManip or opens a QFileDialog
     * to enable save path selection.
     * Emits noSaveFileSelected() on failure.
     */
    void onSaveData(const kanji_data::kanji_lib &lib, bool def);

    /*!
     * \brief onLoadData Handler for settings load requests.
     * Opens a QFileDialog for file selection, loads data from the specified file.
     * Emits loadFailed() and noLoadFileSelected() on failure, dataLoaded() on success.
     */
    void onLoadSettings();
    /*!
     * \brief onSaveSettings Saves settings to default path or enables selection.
     * \param s Settings to be saved.
     * \param def Indicates whether it is saved to the default path.
     */
    void onSaveSettings(Settings s, bool def);

public:
    /*!
     * \brief readLib Reads data from the #fileName path.
     */
    void readLib(std::string fname);

    /*!
     * \brief readSettings Reads settings from the #settingsName path.
     */
    void readSettings(std::string fname);

private:
    QWidget *parent;

    std::string filePath;
    std::string settingsPath;
};

#endif // IOLIB_H
