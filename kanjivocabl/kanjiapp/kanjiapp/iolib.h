#ifndef IOLIB_H
#define IOLIB_H

#include <QObject>

#include "kanjiapp/KanjiData.h"

class LibManip : public QObject
{
    Q_OBJECT

public:
    LibManip(QWidget *parent = nullptr);
    ~LibManip() = default;

signals:
    void dataLoaded(kanji_data::kanji_lib lib);
    void noSaveFileSelected();
    void noLoadFileSelected();

public slots:
    void onLoadData();
    void onSaveData(const kanji_data::kanji_lib &lib);

public:
    kanji_data::kanji_lib read_lib();

private:
    QWidget *parent;

    std::string fileName;
};



#endif // IOLIB_H
