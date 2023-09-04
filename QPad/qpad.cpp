#include "qpad.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qdatetime.h>
#include <qprocess.h>

QPad::QPad(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QApplication::removeTranslator(&trans);
    trans.load(QFileInfo(qApp->arguments().at(0)).path() + "/" + QLocale::system().name(), ".");
    QApplication::installTranslator(&trans);
    
    
    ui.retranslateUi(this);

    font = QFont("Segoe UI", 11, -1);
    def_scale = font.pointSize();
    scale = def_scale;
    
    connect(ui.a_open_file, &QAction::triggered, this, &QPad::open_file_action);
    connect(ui.a_save_file, &QAction::triggered, this, &QPad::save_file_action);
    connect(ui.a_save_as_file, &QAction::triggered, this, &QPad::save_file_as_action);
    connect(ui.a_create_file, &QAction::triggered, this, &QPad::create_file_action);
    connect(ui.a_new_win, &QAction::triggered, this, &QPad::run_new);
    connect(ui.a_past_time, &QAction::triggered, this, &QPad::insert_date);

    connect(ui.textEdit, &QTextEdit::textChanged, this, &QPad::update_title);
    connect(ui.textEdit, &QTextEdit::cursorPositionChanged, this, &QPad::update_status);

    connect(ui.a_scale_down, &QAction::triggered, this, &QPad::scale_down);
    connect(ui.a_scale_up, &QAction::triggered, this, &QPad::scale_up);
    connect(ui.a_scale_default, &QAction::triggered, this, &QPad::scale_default);
    connect(ui.a_change_font, &QAction::triggered, this, &QPad::setFont);


    
    ui.textEdit->setFont(font);

    if (qApp->arguments().size() > 1) {
        file_path = qApp->arguments().at(1);
        if (QFileInfo(QFile(file_path)).exists()) {
            file.setFileName(file_path);
            try {
                file.open(QIODevice::ReadOnly);
                ui.textEdit->clear();
                ui.textEdit->document()->clear();
                ui.textEdit->document()->clearUndoRedoStacks();
                ui.textEdit->setPlainText(file.readAll());
                ui.textEdit->document()->setModified(false);
                update_title();
                file.close();
            }
            catch (std::exception) {
                QMessageBox::critical(this, tr("Error"), tr("ERROR!"));
            }
        }
    }
}

QPad::~QPad()
{}

void QPad::update_title()
{
    if (!ui.textEdit->document()->isModified())
        this->setWindowTitle("Qpad - " + QFileInfo(file).fileName());
    else
        this->setWindowTitle("Qpad - " + QFileInfo(file).fileName() + "*");
}


void QPad::open_file_action() {
    
    file_path = QFileDialog::getOpenFileName(this);
    
    if (QFileInfo(QFile(file_path)).exists()) {
        file.setFileName(file_path);
        try {
            file.open(QIODevice::ReadOnly);
            ui.textEdit->clear();
            ui.textEdit->document()->clear();
            ui.textEdit->document()->clearUndoRedoStacks();
            ui.textEdit->setPlainText(file.readAll());
            ui.textEdit->document()->setModified(false);
            update_title();
            file.close();
        }
        catch (std::exception) {
            QMessageBox::critical(this, tr("Error"), tr("ERROR!"));
        }
    }

}

void QPad::save_file_action() {
    file.setFileName(file_path);
    if (!QFileInfo(file).exists()) {
        file_path = QFileDialog::getSaveFileName(this);
        file.setFileName(file_path);
    }
    try {
        file.open(QIODevice::WriteOnly);
        file.write(ui.textEdit->toPlainText().toStdString().c_str());
        ui.textEdit->document()->setModified(false);
        update_title();
        file.close();
    }
    catch (std::exception) {
        QMessageBox::critical(this, tr("Error"), tr("ERROR!"));
    }
}

void QPad::save_file_as_action()
{
    file_path = QFileDialog::getSaveFileName(this);
    file.setFileName(file_path);
    this->save_file_action();
}

void QPad::run_new()
{
    QProcess proc(this);
    proc.startDetached("./QPad.exe");
}

void QPad::create_file_action()
{
    if (ui.textEdit->document()->isModified()) {
        int ret = QMessageBox::warning(this, tr("File not save."),
            tr("File") + tr(QFileInfo(file).fileName().toStdString().c_str()) +
            tr("not save.") + '\n' + tr("Do you want to save it ?"), 
            QMessageBox::Ok, QMessageBox::No);

        if (ret == QMessageBox::Ok) {
            this->save_file_action();
        }
    }

    try {
        file.close();
        file.setFileName("NewFile.txt");
    }
    catch (std::exception) {

    }
    ui.textEdit->clear();
    ui.textEdit->document()->clear();
    ui.textEdit->document()->clearUndoRedoStacks();
    ui.textEdit->setPlainText(file.readAll());
    ui.textEdit->document()->setModified(false);
    update_title();

}

void QPad::update_status()
{
    QTextCursor curs = ui.textEdit->textCursor();

    QString status = tr("Font Size: ") + QString::number(font.pointSize()) + "   \t" + tr("str: ") + QString::number(curs.blockNumber() + 1) +
        tr(", col: ") + QString::number(curs.columnNumber() + 1);
    ui.statusBar->showMessage(status);
}

void QPad::scale_up()
{
    ui.textEdit->selectAll();
    scale++;
    if (scale > 100) scale = 100;
    font.setPointSize(scale);
    ui.textEdit->setFont(font);

    this->update_status();
}

void QPad::scale_down()
{
    ui.textEdit->selectAll();
    scale--;
    if (scale < 1) scale = 1;
    font.setPointSize(scale);
    ui.textEdit->setFont(font);
    this->update_status();
}

void QPad::scale_default()
{
    ui.textEdit->selectAll();
    scale = def_scale;
    font.setPointSize(def_scale);
    ui.textEdit->setFont(font);
    this->update_status();
}

void QPad::insert_date()
{
    QDateTime time(QDateTime::currentDateTimeUtc());
    QDateTime cur = time.currentDateTime();
    QString str_time = "";
    str_time += QString::number(cur.currentDateTime().time().hour());
    int min = cur.currentDateTime().time().minute();
    if (min < 10) {
        str_time += ":0" + QString::number(min);
    }
    else {
        str_time += ":" + QString::number(min);
    }
    str_time += " " + QString::number(cur.date().day()) + 
        "." + QString::number(cur.date().month()) +
        "." + QString::number(cur.date().year());
    ui.textEdit->insertPlainText(str_time);
}

void QPad::setFont()
{
    QFontDialog font_dialog(font, this);    
    font_dialog.setLocale(QLocale::system());
    font_dialog.exec();
    font = font_dialog.currentFont();
    scale = font.pointSize();
    def_scale = scale;
    ui.textEdit->setFont(font);
}
