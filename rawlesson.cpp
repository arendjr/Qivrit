#include "rawlesson.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
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

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if (!document.isArray()) {
        qWarning() << "Could not read JSON lesson data from file" << fileName;
        return false;
    }

    for (auto value : document.array()) {
        auto object = value.toObject();
        QMap<QString, QString> map;
        map["hebrew"] = object["hebrew"].toString();
        map["modern hebrew"] = object["modern hebrew"].toString();
        map["english"] = object["english"].toString();
        map["english phonetic"] = object["english phonetic"].toString();
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
