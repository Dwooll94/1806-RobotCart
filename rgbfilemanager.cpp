#include "rgbfilemanager.h"
#include <QByteArray>
#include <QWindow>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QIODeviceBase>
#include <QByteArray>
#include <QFileDialog>
#include "RGBControlStruct.h"
#include <iostream>

const QString FILE_EXTENSION = ".SRGBP";
const QString DEFAULT_FILE = "RGB_Defaults" + FILE_EXTENSION;
const QString DEFAULT_PATH = "/1806-RobotCart-Settings/RGB/";

RGBFileManager::RGBFileManager()
{
    QDir defaultDir(QDir::homePath().append(DEFAULT_PATH));
    if(!defaultDir.exists())
    {
        defaultDir.mkpath(".");
    }
}

RGBControlStruct RGBFileManager::loadDefaultControlStruct()
{
    QString fname = QDir::homePath() + DEFAULT_PATH + DEFAULT_FILE;
    QFile defaultFile(fname);
    RGBControlStruct out;
    out.override = false;
    loadFile(defaultFile, out);
    return out;
}

bool RGBFileManager::saveDefaultControlStruct(const RGBControlStruct in)
{
    QString fname = QDir::homePath() + DEFAULT_PATH + DEFAULT_FILE;
    QFile defaultFile(fname);
    return saveFile(defaultFile, in);
}

bool RGBFileManager::loadSavedRGBControlStruct(RGBControlStruct& out)
{
    QString fname = QFileDialog::getOpenFileName(nullptr, "Load RGB Settings", QDir::homePath() + DEFAULT_PATH, "SWAT RGB Profiles (*.SRGBP)");
    QFile file(fname);
    return loadFile(file, out);
}

bool RGBFileManager::parseControlStructJSON(QByteArray json, RGBControlStruct& out)
{
    QJsonParseError error;
    QJsonDocument parsedJson = QJsonDocument::fromJson(json,&error);
    if(error.error == QJsonParseError::NoError)
    {
        QJsonObject jObject = parsedJson.object();
        RGBControlStruct readStruct;
        readStruct.override = out.override; //preserve override
        readStruct.animationType = static_cast<RGBAnimationType>(jObject.value("animationType").toInt(0));
        readStruct.red = jObject.value("red").toInt(0);
        readStruct.green = jObject.value("green").toInt(255);
        readStruct.blue = jObject.value("blue").toInt(0);
        readStruct.white = jObject.value("white").toInt(255);
        readStruct.twinklePercent = jObject.value("twinklePercent").toInt(100);
        readStruct.brightness = jObject.value("brightness").toDouble(1.0);
        readStruct.speed = jObject.value("speed").toDouble(0.5);
        readStruct.sparking = jObject.value("sparking").toDouble(0.5);
        readStruct.cooling = jObject.value("cooling").toDouble(0.3);
        out = readStruct;
        return true;
    }
    else
    {
        std::cout << "Error parsing saved RGB JSON:" << error.errorString().toStdString() << std::endl;
        return false;
    }
}

bool RGBFileManager::saveRGBControlStructToFile(const RGBControlStruct in)
{
    QString fname = QFileDialog::getSaveFileName(nullptr, "Save RGB Settings", QDir::homePath() + DEFAULT_PATH, "SWAT RGB Profiles (*.SRGBP)");
    if(!fname.endsWith(FILE_EXTENSION))
    {
        fname = fname.append(FILE_EXTENSION);
    }
    QFile file(fname);
    return saveFile(file, in);
}

QByteArray RGBFileManager::serializeControlStructToJSONString(const RGBControlStruct in)
{
    QJsonObject serializedControlStruct
    {
        {"animationType", in.animationType},
        {"red", in.red},
        {"green", in.green},
        {"blue", in.blue},
        {"white", in.white},
        {"twinklePercent", in.twinklePercent},
        {"brightness", in.brightness},
        {"speed", in.speed},
        {"sparking", in.sparking},
        {"cooling", in.cooling}
    };
    QJsonDocument serializedDoc(serializedControlStruct);
    return serializedDoc.toJson(QJsonDocument::JsonFormat::Indented);
}

bool RGBFileManager::saveFile(QFile& file, const RGBControlStruct in)
{
    if(file.open(QIODeviceBase::ReadWrite))
    {
        QDataStream fileStream(&file);
        fileStream << serializeControlStructToJSONString(in);
        file.close();
        return true;
    }
    return false;
}

bool RGBFileManager::loadFile(QFile& file, RGBControlStruct& out)
{
    if(file.exists())
    {
        if(file.open(QIODeviceBase::ReadOnly))
        {
            QDataStream fileStream(&file);
            QByteArray json;
            fileStream >> json;
            bool retval = parseControlStructJSON(json, out);
            file.close();
            return retval;
        }
    }
    return false;
}
