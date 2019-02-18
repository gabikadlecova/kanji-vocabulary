#ifndef IOLIB_H
#define IOLIB_H

#include <QObject>

#include "kanjiapp/KanjiData.h"

class LibManip : public QObject
{
    Q_OBJECT

public:
    LibManip(std::string defaultName = "", QWidget *parent = nullptr);
    ~LibManip() = default;

signals:
    void dataLoaded(kanji_data::kanji_lib lib);
    void loadFailed();
    void noSaveFileSelected();
    void noLoadFileSelected();

public slots:
    void onLoadData();
    void onSaveData(const kanji_data::kanji_lib &lib);

public:
    void readLib();


private:
    QWidget *parent;

    std::string fileName;
};



#endif // IOLIB_H
