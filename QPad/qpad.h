#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qpad.h"
#include <qtranslator.h>

#include <qstring.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qfontdialog.h>


class QPad : public QMainWindow
{
    Q_OBJECT

public:
    QPad(QWidget *parent = nullptr);
    ~QPad();

private:
    Ui::QPadClass ui;
    QFile file;
    QString file_path;
    unsigned long def_scale;
    unsigned long scale;
    
    QFont font;
    QFont numLine_font;
    QTranslator trans;



private slots:
    void update_title();


    void open_file_action();
    void save_file_action();
    void save_file_as_action();
    void run_new();
    void create_file_action();

    void update_status();

    void scale_up();
    void scale_down();
    void scale_default();

    void insert_date();
    void setFont();

    void setNumLine(bool state);
};
