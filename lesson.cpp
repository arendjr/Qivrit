#include "lesson.h"
#include "qjson/json_driver.hh"

#include <QFile>
#include <QVariantMap>


template<typename Key, typename T>
static Key takeNthKeyFromMap(const QMap<Key, T> &map, int n) {

    typename QMap<Key, T>::ConstIterator it = map.constBegin();
    while (n > 0) {
        ++it;
        n--;
    }
    return it.key();
}


Lesson::Lesson(QObject *parent) :
    QObject(parent),
    m_questionLanguage("hebrew"),
    m_answerLanguage("english") {
}

void Lesson::setQuestionLanguage(const QString &languageName) {

    m_questionLanguage = languageName;
}

void Lesson::setAnswerLanguage(const QString &languageName) {

    m_answerLanguage = languageName;
}

void Lesson::resetQuestions() {

    m_questions.clear();
}

void Lesson::load(const QString &fileName) {

    QFile file(fileName);
    bool result = file.open(QIODevice::ReadOnly);
    if (!result) {
        qWarning() << "Could not open lesson data file" << fileName;
        return;
    }

    bool error;
    JSonDriver driver;
    QVariantList content = driver.parse(&file, &error).toList();
    if (error) {
        qWarning() << "Could not read JSON lesson data from file" << fileName;
        return;
    }

    for (QVariantList::ConstIterator it = content.constBegin();
         it != content.constEnd(); ++it) {
        QString question = QString::fromUtf8((*it).toMap()[m_questionLanguage].toByteArray());
        QString answer = QString::fromUtf8((*it).toMap()[m_answerLanguage].toByteArray());
        if (!question.isEmpty() && !answer.isEmpty()) {
            m_questions[question] = answer;
        }
    }

    if (m_questions.size() == 0) {
        qWarning() << "Found no entries in JSON lesson data from file" << fileName;
        return;
    }
}

QString Lesson::nextQuestion() {

    QString previousQuestion = m_currentQuestion;

    while (previousQuestion == m_currentQuestion) {
        if (m_unansweredQuestions.size() > 7) {
            int index = qrand() % m_unansweredQuestions.size();
            m_currentQuestion = m_unansweredQuestions[index];
        } else {
            int index = qrand() % 7;
            if (index < m_unansweredQuestions.size()) {
                m_currentQuestion = m_unansweredQuestions[index];
            } else {
                if (m_questions.size() < 2) {
                    break;
                }

                index = qrand() % m_questions.size();
                m_currentQuestion = takeNthKeyFromMap(m_questions, index);
            }
        }
    }

    return m_currentQuestion;
}

QString Lesson::currentQuestion() const {

    return m_currentQuestion;
}

QString Lesson::currentAnswer() const {

    return m_questions[m_currentQuestion];
}

void Lesson::resetAnswers() {

    m_unansweredQuestions = m_questions.keys();
}

void Lesson::markCurrentQuestionAnswered() {

    m_unansweredQuestions.removeAll(m_currentQuestion);
}

int Lesson::numUnansweredQuestions() const {

    return m_unansweredQuestions.size();
}
