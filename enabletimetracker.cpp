#include "enabletimetracker.h"
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
#include <iostream>

const QString FILE_EXTENSION = ".ODO";
const QString DEFAULT_FILE = "Timer" + FILE_EXTENSION;
const QString DEFAULT_PATH = "/1806-RobotCart-Settings/Odometry/";


EnableTimeTracker::EnableTimeTracker()
{
    std::cout<< "Loading enable time" << std::endl;
    QString fname = QDir::homePath() + DEFAULT_PATH + DEFAULT_FILE;
    QFile defaultFile(fname);
    if(defaultFile.exists())
    {
        if(defaultFile.open(QIODeviceBase::ReadOnly))
        {
            QDataStream fileStream(&defaultFile);
            QByteArray json;
            fileStream >> json;
            QJsonParseError error;
            QJsonDocument parsedJson = QJsonDocument::fromJson(json,&error);
            if(error.error == QJsonParseError::NoError)
            {
                QJsonObject jObject = parsedJson.object();
                m_minutesEnabled = jObject.value("minutes").toDouble();
                std::cout << "Load Successful!" << std::endl;
            }
            defaultFile.close();
        }
        else
        {
            std::cout << "Could not load enable time!" << std::endl;
        }
    }
}

double EnableTimeTracker::getHoursEnabled()
{
    return m_minutesEnabled / 60.0;
}

void EnableTimeTracker::log30SecsEnabled()
{
    std::cout<< "Saving enable time..." << std::endl;
    m_minutesEnabled += 0.5;
    QJsonObject serializedTime
    {
        {"minutes", m_minutesEnabled}
    };
    QJsonDocument serializedDoc(serializedTime);
    QString fname = QDir::homePath() + DEFAULT_PATH + DEFAULT_FILE;
    QFile defaultFile(fname);
    if(defaultFile.open(QIODeviceBase::ReadWrite))
    {
        QDataStream fileStream(&defaultFile);
        fileStream << serializedDoc.toJson(QJsonDocument::JsonFormat::Indented);
        defaultFile.close();
        std::cout << "Save Successful!" <<std::endl;
    }
    else
    {
        std::cout << "Save Failed!" << std::endl;
    }
}
