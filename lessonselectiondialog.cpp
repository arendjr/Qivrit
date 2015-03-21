#include "lessonselectiondialog.h"
#include "ui_lessonselectiondialog.h"

#include <QMessageBox>


LessonSelectionDialog::LessonSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LessonSelectionDialog()) {

    ui->setupUi(this);

    m_lessons[":/lessons/colors.json"] = tr("Colors");
    m_lessons[":/lessons/common1.json"] = tr("Frequently Used Words & Phrases Pt. 1");
    m_lessons[":/lessons/common2.json"] = tr("Frequently Used Words & Phrases Pt. 2");
    m_lessons[":/lessons/common3.json"] = tr("Frequently Used Words & Phrases Pt. 3");
    m_lessons[":/lessons/pronounspossessives.json"] = tr("Pronouns, Possessives & Objectives");

    m_questionLanguages["hebrew"] = tr("Vocalized Hebrew");
    m_questionLanguages["modern hebrew"] = tr("Modern Hebrew");
    m_questionLanguages["english"] = tr("English");

    m_answerLanguages["english"] = tr("English");
    m_answerLanguages["english phonetic"] = tr("English Phonetic");
    m_answerLanguages["modern hebrew"] = tr("Modern Hebrew");

    QStringList lessons = m_lessons.values();
    qSort(lessons);
    ui->lessonsListWidget->addItems(lessons);

    QStringList questionLanguages = m_questionLanguages.values();
    qSort(questionLanguages);
    ui->questionsLanguageCombo->addItems(questionLanguages);
    ui->questionsLanguageCombo->setCurrentIndex(2);

    QStringList answerLanguages = m_answerLanguages.values();
    qSort(answerLanguages);
    ui->answersLanguageCombo->addItems(answerLanguages);
    ui->answersLanguageCombo->setCurrentIndex(0);
}

LessonSelectionDialog::~LessonSelectionDialog() {

    delete ui;
}

QStringList LessonSelectionDialog::selectedLessons() const {

    QStringList lessons;
    QList<QListWidgetItem *> items = ui->lessonsListWidget->selectedItems();
    foreach (QListWidgetItem *item, items) {
        lessons.append(m_lessons.key(item->text()));
    }
    return lessons;
}

QString LessonSelectionDialog::selectedQuestionLanguage() const {

    return m_selectedQuestionLanguage;
}

QString LessonSelectionDialog::selectedAnswerLanguage() const {

    return m_selectedAnswerLanguage;
}

void LessonSelectionDialog::accept() {

    if (ui->lessonsListWidget->selectedItems().size() == 0) {
        QMessageBox::information(this, qApp->applicationName(),
                                 tr("Please select one or more lessons first."));
        return;
    }

    m_selectedQuestionLanguage = m_questionLanguages.key(ui->questionsLanguageCombo->currentText());
    m_selectedAnswerLanguage = m_answerLanguages.key(ui->answersLanguageCombo->currentText());

    if ((m_selectedQuestionLanguage == "hebrew" || m_selectedQuestionLanguage == "modern hebrew") &&
        (m_selectedAnswerLanguage == "hebrew" || m_selectedAnswerLanguage == "modern hebrew")) {
        QMessageBox::information(this, qApp->applicationName(),
                                 tr("Training from Hebrew to Hebrew isn't very useful, don't you think?"));
        return;
    } else if ((m_selectedQuestionLanguage == "english" || m_selectedQuestionLanguage == "english phonetic") &&
               (m_selectedAnswerLanguage == "english" || m_selectedAnswerLanguage == "english phonetic")) {
        QMessageBox::information(this, qApp->applicationName(),
                                 tr("Training from English to English isn't very useful, don't you think?"));
        return;
    }

    QDialog::accept();
}
