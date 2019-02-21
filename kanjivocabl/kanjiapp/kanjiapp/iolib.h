#ifndef IOLIB_H
#define IOLIB_H

#include <QObject>

#include "kanjiapp/KanjiData.h"

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
    LibManip(std::string defaultName = "", QWidget *parent = nullptr);
    virtual ~LibManip() override = default;

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

public slots:
    /*!
     * \brief onLoadData Handler for data load requests.
     * Opens a QFileDialog for file selection, loads data from the specified file.
     * Emits loadFailed() and noLoadFileSelected() on failure, dataLoaded() on success.
     */
    void onLoadData();
    /*!
     * \brief onSaveData Saves data to the default location or enables selecting a path.
     * \param lib Data which is to be saved.
     * Saves data from lib to the path specified in #LibManip or opens a QFileDialog
     * to enable save path selection.
     * Emits noSaveFileSelected() on failure.
     */
    void onSaveData(const kanji_data::kanji_lib &lib);

public:
    /*!
     * \brief readLib Reads data from the #fileName path.
     */
    void readLib();

private:
    QWidget *parent;

    std::string fileName; /*!< Default load and save path. */
};

#endif // IOLIB_H
