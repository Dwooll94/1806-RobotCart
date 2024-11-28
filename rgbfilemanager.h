#ifndef RGBFILEMANAGER_H
#define RGBFILEMANAGER_H
#include "RGBControlStruct.h"
#include <QWindow>
#include <QByteArray>
#include <QFile>

class RGBFileManager
{
public:
    RGBFileManager();
    RGBControlStruct loadDefaultControlStruct();
    bool saveDefaultControlStruct(const RGBControlStruct in);
    bool loadSavedRGBControlStruct(RGBControlStruct& out);
    bool saveRGBControlStructToFile(const RGBControlStruct in);

private:

    bool parseControlStructJSON(QByteArray json,RGBControlStruct& out);
    QByteArray serializeControlStructToJSONString(const RGBControlStruct in);
    bool saveFile(QFile& file, const RGBControlStruct in);
    bool loadFile(QFile& file, RGBControlStruct& out);
};

#endif // RGBFILEMANAGER_H
