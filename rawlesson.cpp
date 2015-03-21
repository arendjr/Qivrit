#include "rawlesson.h"
#include "qjson/json_driver.hh"

#include <QFile>
#include <QVariantMap>


RawLesson::RawLesson(QObject *parent) :
    QObject(parent) {
}

void RawLesson::reset() {

    m_translations.clear();
}

bool RawLesson::load(const QString &fileName) {

    QFile file(fileName);
    bool result = file.open(QIODevice::ReadOnly);
    if (!result) {
        qWarning() << "Could not open lesson data file" << fileName << "for reading";
        return false;
    }

    bool error;
    JSonDriver driver;
    QVariantList content = driver.parse(&file, &error).toList();
    if (error) {
        qWarning() << "Could not read JSON lesson data from file" << fileName;
        return false;
    }

    for (QVariantList::ConstIterator it = content.constBegin();
         it != content.constEnd(); ++it) {
        QMap<QString, QString> map;
        map["hebrew"] = QString::fromUtf8((*it).toMap()["hebrew"].toByteArray());
        map["modern hebrew"] = QString::fromUtf8((*it).toMap()["modern hebrew"].toByteArray());
        map["english"] = QString::fromUtf8((*it).toMap()["english"].toByteArray());
        map["english phonetic"] = QString::fromUtf8((*it).toMap()["english phonetic"].toByteArray());
        m_translations.append(map);
    }

    return true;
}

bool RawLesson::save(const QString &fileName) {

    QFile file(fileName);
    bool result = file.open(QIODevice::WriteOnly);
    if (!result) {
        qWarning() << "Could not open lesson data file" << fileName << "for writing";
        return false;
    }

    file.write("[\n");
    for (int i = 0; i < m_translations.size(); i++) {
        file.write(QString("{ \"hebrew\": \"%1\", "
                             "\"modern hebrew\": \"%2\", "
                             "\"english\": \"%3\", "
                             "\"english phonetic\": \"%4\" }")
                   .arg(m_translations[i]["hebrew"].replace("\"", "\\\""))
                   .arg(m_translations[i]["modern hebrew"].replace("\"", "\\\""))
                   .arg(m_translations[i]["english"].replace("\"", "\\\""))
                   .arg(m_translations[i]["english phonetic"].replace("\"", "\\\"")).toUtf8());
        if (i < m_translations.size() - 1) {
            file.write(",");
        }
        file.write("\n");
    }
    file.write("]\n");

    return true;
}

QString RawLesson::getTranslation(int index, const QString &language) const {

    if (index >= m_translations.size()) {
        return "";
    }

    return m_translations[index][language];
}

void RawLesson::setTranslation(int index, const QString &language, const QString &text) {

    for (int i = m_translations.size(); i < index + 1; i++) {
        m_translations.append(QMap<QString, QString>());
    }

    m_translations[index][language] = text;
}

int RawLesson::numTranslations() const {

    return m_translations.size();
}
