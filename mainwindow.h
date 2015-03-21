#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lesson.h"
#include "rawlesson.h"

#include <QMainWindow>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:
        enum Mode {
            Home = 0,
            AlphabetTraining,
            WordsNSentences,
            TranslationHelper,
            EditLesson
        };

        explicit MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();

    public slots:
        void returnToHome();

    private slots:
        void onStartAlphabetTrainingButtonClicked();
        void onStartWordsNSentencesButtonClicked();
        void onStartTranslationHelperButtonClicked();
        void onNewLessonLinkClicked();

        void onContinueButtonClicked();

        void onNextButtonClicked();
        void onPreviousButtonClicked();
        void onEnglishLineEditingFinished();
        void onEnglishPhoneticLineEditingFinished();
        void onModernHebrewLineEditingFinished();
        void onVocalizedHebrewLineEditingFinished();
        void onSaveButtonClicked();
        void onOpenLinkClicked();
        void onExitLinkClicked();

    private:
        Ui::MainWindow *ui;

        Mode m_mode;

        Lesson m_currentLesson;

        RawLesson m_editLesson;
        QString m_editFileName;
        int m_translationIndex;

        void setUpShortcuts();
        void connectSignals();
        void proceedToNextQuestion();
        void setTranslationIndex(int translationIndex);
        void updateTranslation(const QString &language, const QString &text);
        void updateAllTranslations();
};

#endif // MAINWINDOW_H
