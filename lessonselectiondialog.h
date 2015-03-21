#ifndef LESSONSELECTIONDIALOG_H
#define LESSONSELECTIONDIALOG_H

#include <QDialog>
#include <QMap>


namespace Ui {
    class LessonSelectionDialog;
}

class LessonSelectionDialog : public QDialog {

    Q_OBJECT

    public:
        explicit LessonSelectionDialog(QWidget *parent = 0);
        ~LessonSelectionDialog();

        QStringList selectedLessons() const;
        QString selectedQuestionLanguage() const;
        QString selectedAnswerLanguage() const;

    public slots:
        virtual void accept();

    private:
        Ui::LessonSelectionDialog *ui;

        QMap<QString, QString> m_lessons;
        QMap<QString, QString> m_questionLanguages;
        QMap<QString, QString> m_answerLanguages;

        QString m_selectedQuestionLanguage;
        QString m_selectedAnswerLanguage;
};

#endif // LESSONSELECTIONDIALOG_H
