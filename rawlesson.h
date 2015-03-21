#ifndef RAWLESSON_H
#define RAWLESSON_H

#include <QObject>
#include <QMap>
#include <QStringList>


class RawLesson : public QObject {

    Q_OBJECT

    public:
        explicit RawLesson(QObject *parent = 0);

        void reset();
        bool load(const QString &fileName);
        bool save(const QString &fileName);

        QString getTranslation(int index, const QString &language) const;
        void setTranslation(int index, const QString &language, const QString &text);

        int numTranslations() const;

    private:
        QList<QMap<QString, QString> > m_translations;
};

#endif // RAWLESSON_H
