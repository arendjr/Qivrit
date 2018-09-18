#include "mainwindow.h"
#include "lessonselectiondialog.h"
#include "shinkeyeventfilter.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QKeySequence>
#include <QMessageBox>
#include <QShortcut>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow()),
    m_mode(Home) {

    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    QFont font = ui->modernHebrewLineEdit->font();
    font.setPointSize(font.pointSize() + 4);
    ui->modernHebrewLineEdit->setFont(font);
    ui->vocalizedHebrewLineEdit->setFont(font);

    ShinKeyEventFilter *shinKeyEventFilter = new ShinKeyEventFilter(this);
    ui->vocalizedHebrewLineEdit->installEventFilter(shinKeyEventFilter);
    ui->answerEdit->installEventFilter(shinKeyEventFilter);

    setUpShortcuts();

    ui->startAlphabetTrainingButton->setFocus();

    connectSignals();
}

MainWindow::~MainWindow() {

    delete ui;
}

void MainWindow::returnToHome() {

    m_mode = Home;
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onStartAlphabetTrainingButtonClicked() {

    m_currentLesson.setQuestionLanguage("hebrew");
    m_currentLesson.setAnswerLanguage("english");

    m_currentLesson.resetQuestions();
    m_currentLesson.load(":/lessons/alphabet.json");
    m_currentLesson.resetAnswers();

    proceedToNextQuestion();

    ui->explanationLabel->setText("Please give the name of the Hebrew character below...");
    ui->questionLabel->setStyleSheet("font-size: 72pt");

    m_mode = AlphabetTraining;
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onStartWordsNSentencesButtonClicked() {

    LessonSelectionDialog dialog(this);
    int result = dialog.exec();
    if (result == QDialog::Rejected) {
        return;
    }

    QString questionLanguage = dialog.selectedQuestionLanguage();
    QString answerLanguage = dialog.selectedAnswerLanguage();

    QString explanation;
    if (questionLanguage == "hebrew" || questionLanguage == "modern hebrew") {
        if (answerLanguage == "english") {
            explanation = tr("Please give the translation of the Hebrew word or sentence below...");
        } else if (answerLanguage == "english phonetic") {
            explanation = tr("Please give the pronunciation of the Hebrew word or sentence below...");
        }
    } else {
        if (answerLanguage == "modern hebrew") {
            explanation = tr("Please translate the word or sentence below to Hebrew...");
        }
    }

    m_currentLesson.setQuestionLanguage(questionLanguage);
    m_currentLesson.setAnswerLanguage(answerLanguage);

    m_currentLesson.resetQuestions();
    foreach (QString lesson, dialog.selectedLessons()) {
        m_currentLesson.load(lesson);
    }
    m_currentLesson.resetAnswers();

    proceedToNextQuestion();

    ui->explanationLabel->setText(explanation);
    ui->questionLabel->setStyleSheet("font-size: 32pt");

    m_mode = WordsNSentences;
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onStartTranslationHelperButtonClicked() {

    m_mode = TranslationHelper;
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::onNewLessonLinkClicked() {

    m_editLesson.reset();
    m_editFileName = "";

    setTranslationIndex(0);

    ui->saveButton->setEnabled(false);

    m_mode = EditLesson;
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onContinueButtonClicked() {

    if (!ui->answerFeedbackLabel->text().isEmpty()) {
        proceedToNextQuestion();
        return;
    }

    QString givenAnswer = ui->answerEdit->text().simplified();
    QString correctAnswer = m_currentLesson.currentAnswer().simplified();
    if (givenAnswer.toLower() == correctAnswer.toLower()) {
        ui->answerFeedbackLabel->setText(tr("Good!"));
        m_currentLesson.markCurrentQuestionAnswered();
    } else {
        ui->answerFeedbackLabel->setText(tr("I'm sorry, the correct answer is: %1").arg(correctAnswer));
    }
}

void MainWindow::onNextButtonClicked() {

    if (m_mode == EditLesson && m_translationIndex < m_editLesson.numTranslations()) {
        updateAllTranslations();
        setTranslationIndex(m_translationIndex + 1);
    }
}

void MainWindow::onPreviousButtonClicked() {

    if (m_mode == EditLesson && m_translationIndex > 0) {
        updateAllTranslations();
        setTranslationIndex(m_translationIndex - 1);
    }
}

void MainWindow::onEnglishLineEditingFinished() {

    updateTranslation("english", ui->englishLineEdit->text());
}

void MainWindow::onEnglishPhoneticLineEditingFinished() {

    updateTranslation("english phonetic", ui->englishPhoneticLineEdit->text());
}

void MainWindow::onModernHebrewLineEditingFinished() {

    updateTranslation("modern hebrew", ui->modernHebrewLineEdit->text());

    if (ui->vocalizedHebrewLineEdit->text() == "" && ui->modernHebrewLineEdit->text() != "") {
        ui->vocalizedHebrewLineEdit->setText(ui->modernHebrewLineEdit->text());
        onVocalizedHebrewLineEditingFinished();
    }
}

void MainWindow::onVocalizedHebrewLineEditingFinished() {

    updateTranslation("hebrew", ui->vocalizedHebrewLineEdit->text());
}

void MainWindow::onSaveButtonClicked() {

    if (m_mode != EditLesson || !ui->saveButton->isEnabled()) {
        return;
    }

    updateAllTranslations();

    if (m_editFileName.isEmpty()) {
        m_editFileName = QFileDialog::getSaveFileName(this, tr("Save Lesson"),
            QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first(),
            tr("Lesson files (*.json)")
        );
        if (m_editFileName.isEmpty()) {
            return;
        }
    }

    bool result = m_editLesson.save(m_editFileName);
    if (!result) {
        QMessageBox::warning(this, qApp->applicationName(),
            tr("Sorry, the lesson could not be saved. Please try saving to another location.")
        );
        return;
    }

    ui->saveButton->setEnabled(false);
}

void MainWindow::onOpenLinkClicked() {

    if (ui->saveButton->isEnabled()) {
        if (!confirm(
            tr("There are unsaved changes in the lesson. "
               "Are you sure you want to open another lesson?"),
            tr("Open"), tr("Keep Editing")
        )) {
            return;
        }
    }

    m_editFileName = QFileDialog::getOpenFileName(this, tr("Open Lesson"),
        QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first(),
        tr("Lesson files (*.json)")
    );
    if (m_editFileName.isEmpty()) {
        return;
    }

    m_editLesson.reset();
    bool result = m_editLesson.load(m_editFileName);
    if (!result) {
        QMessageBox::warning(this, qApp->applicationName(),
            tr("Sorry, the lesson could not be opened.")
        );
        return;
    }

    setTranslationIndex(0);

    ui->saveButton->setEnabled(false);
}

void MainWindow::onExitLinkClicked() {

    if (ui->saveButton->isEnabled()) {
        if (!confirm(
            tr("There are unsaved changes in the lesson. Are you sure you want to exit?"),
            tr("Exit"), tr("Keep Editing")
        )) {
            return;
        }
    }

    returnToHome();
}

bool MainWindow::confirm(const QString &text, const QString &okLabel, const QString &cancelLabel) {
    QMessageBox messageBox;
    messageBox.setText(text);
    QPushButton *okButton = messageBox.addButton(okLabel, QMessageBox::DestructiveRole);
    messageBox.addButton(cancelLabel, QMessageBox::RejectRole);

    messageBox.exec();

    return messageBox.clickedButton() == okButton;
}

void MainWindow::setUpShortcuts() {

    QShortcut *closeShortcut = new QShortcut(QKeySequence::Close, this);
    connect(closeShortcut, SIGNAL(activated()), SLOT(returnToHome()));
    closeShortcut = new QShortcut(QKeySequence("Ctrl+'"), this);
    connect(closeShortcut, SIGNAL(activated()), SLOT(returnToHome()));

    QShortcut *nextShortcut = new QShortcut(QKeySequence("Ctrl+N"), this);
    connect(nextShortcut, SIGNAL(activated()), SLOT(onNextButtonClicked()));
    nextShortcut = new QShortcut(QKeySequence("Ctrl+מ"), this);
    connect(nextShortcut, SIGNAL(activated()), SLOT(onNextButtonClicked()));

    QShortcut *previousShortcut = new QShortcut(QKeySequence("Ctrl+P"), this);
    connect(previousShortcut, SIGNAL(activated()), SLOT(onPreviousButtonClicked()));
    previousShortcut = new QShortcut(QKeySequence("Ctrl+פ"), this);
    connect(previousShortcut, SIGNAL(activated()), SLOT(onPreviousButtonClicked()));

    QShortcut *saveShortcut = new QShortcut(QKeySequence::Save, this);
    connect(saveShortcut, SIGNAL(activated()), SLOT(onSaveButtonClicked()));
    saveShortcut = new QShortcut(QKeySequence("Ctrl+ד"), this);
    connect(saveShortcut, SIGNAL(activated()), SLOT(onSaveButtonClicked()));
}

void MainWindow::connectSignals() {

    connect(ui->startAlphabetTrainingButton, SIGNAL(clicked()),
            SLOT(onStartAlphabetTrainingButtonClicked()));
    connect(ui->startWordsNSentencesButton, SIGNAL(clicked()),
            SLOT(onStartWordsNSentencesButtonClicked()));
    connect(ui->startTranslationHelpButton, SIGNAL(clicked()),
            SLOT(onStartTranslationHelperButtonClicked()));
    connect(ui->newLessonLabel, SIGNAL(linkActivated(QString)),
            SLOT(onNewLessonLinkClicked()));

    connect(ui->answerEdit, SIGNAL(returnPressed()),
            SLOT(onContinueButtonClicked()));
    connect(ui->continueButton, SIGNAL(clicked()),
            SLOT(onContinueButtonClicked()));
    connect(ui->exitLabel, SIGNAL(linkActivated(QString)),
            SLOT(returnToHome()));

    connect(ui->nextButton, SIGNAL(clicked()),
            SLOT(onNextButtonClicked()));
    connect(ui->previousButton, SIGNAL(clicked()),
            SLOT(onPreviousButtonClicked()));
    connect(ui->englishLineEdit, SIGNAL(editingFinished()),
            SLOT(onEnglishLineEditingFinished()));
    connect(ui->englishPhoneticLineEdit, SIGNAL(editingFinished()),
            SLOT(onEnglishPhoneticLineEditingFinished()));
    connect(ui->modernHebrewLineEdit, SIGNAL(editingFinished()),
            SLOT(onModernHebrewLineEditingFinished()));
    connect(ui->vocalizedHebrewLineEdit, SIGNAL(editingFinished()),
            SLOT(onVocalizedHebrewLineEditingFinished()));
    connect(ui->saveButton, SIGNAL(clicked()),
            SLOT(onSaveButtonClicked()));
    connect(ui->openLabel, SIGNAL(linkActivated(QString)),
            SLOT(onOpenLinkClicked()));
    connect(ui->exitEditLabel, SIGNAL(linkActivated(QString)),
            SLOT(onExitLinkClicked()));
}

void MainWindow::proceedToNextQuestion() {

    if (m_currentLesson.numUnansweredQuestions() == 0) {
        QMessageBox::information(this, qApp->applicationName(),
                                 tr("Very good! You answered all questions correctly at least once!"));
        returnToHome();
        return;
    }

    ui->questionLabel->setText(m_currentLesson.nextQuestion());
    ui->answerFeedbackLabel->setText("");
    ui->answerEdit->setText("");
    ui->answerEdit->setFocus();

    int n = m_currentLesson.numUnansweredQuestions();
    if (m_mode == AlphabetTraining) {
        ui->remainingQuestionsLabel->setText(n == 1 ? tr("1 character remaining...") :
                                                      tr("%1 characters remaining...").arg(n));
    } else {
        ui->remainingQuestionsLabel->setText(n == 1 ? tr("1 phrase remaining...") :
                                                      tr("%1 phrases remaining...").arg(n));
    }
}

void MainWindow::setTranslationIndex(int translationIndex) {

    m_translationIndex = translationIndex;

    ui->englishLineEdit->setText(m_editLesson.getTranslation(m_translationIndex, "english"));
    ui->englishPhoneticLineEdit->setText(m_editLesson.getTranslation(m_translationIndex, "english phonetic"));
    ui->modernHebrewLineEdit->setText(m_editLesson.getTranslation(m_translationIndex, "modern hebrew"));
    ui->vocalizedHebrewLineEdit->setText(m_editLesson.getTranslation(m_translationIndex, "hebrew"));

    ui->nextButton->setEnabled(m_editLesson.numTranslations() > m_translationIndex);
    ui->previousButton->setEnabled(m_translationIndex > 0);
}

void MainWindow::updateTranslation(const QString &language, const QString &text) {

    if (m_editLesson.getTranslation(m_translationIndex, language) != text) {
        m_editLesson.setTranslation(m_translationIndex, language, text);

        ui->saveButton->setEnabled(true);
        ui->nextButton->setEnabled(true);
    }
}

void MainWindow::updateAllTranslations() {

    QString englishText = ui->englishLineEdit->text();
    QString englishPhoneticText = ui->englishPhoneticLineEdit->text();
    QString modernHebrewText = ui->modernHebrewLineEdit->text();
    QString vocalizedHebrewText = ui->vocalizedHebrewLineEdit->text();

    if (englishText != "" || englishPhoneticText != "" ||
        modernHebrewText != "" || vocalizedHebrewText != "") {
        m_editLesson.setTranslation(m_translationIndex, "english", englishText);
        m_editLesson.setTranslation(m_translationIndex, "english phonetic", englishPhoneticText);
        m_editLesson.setTranslation(m_translationIndex, "modern hebrew", modernHebrewText);
        m_editLesson.setTranslation(m_translationIndex, "hebrew", vocalizedHebrewText);

        ui->saveButton->setEnabled(true);
    }
}
