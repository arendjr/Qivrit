#ifndef LESSON_H
#define LESSON_H

#include <QObject>
#include <QMap>
#include <QStringList>


class Lesson : public QObject {

    Q_OBJECT

    public:
        explicit Lesson(QObject *parent = 0);

        void setQuestionLanguage(const QString &languageName);
        void setAnswerLanguage(const QString &languageName);

        void resetQuestions();
        void load(const QString &fileName);

        QString nextQuestion();
        QString currentQuestion() const;
        QString currentAnswer() const;

        void resetAnswers();
        void markCurrentQuestionAnswered();
        int numUnansweredQuestions() const;

    private:
        QString m_questionLanguage;
        QString m_answerLanguage;

        QMap<QString, QString> m_questions;
        QStringList m_unansweredQuestions;
        QString m_currentQuestion;
        QString m_previousQuestion;
};

#endif // LESSON_H
