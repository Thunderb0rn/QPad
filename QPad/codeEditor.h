#pragma once

#include <qwidget.h>
#include <qplaintextedit.h>


class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget* parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();

    void _setFont(QFont font) { this->setFont(font); lineNumberArea->setFont(font); }
    void numberLneHide(bool state) {
        if (!state) {
            lineNumberArea->setVisible(false);
            lineNumberArea->setHidden(true);
        }
        else {
            lineNumberArea->setHidden(false);
            lineNumberArea->setVisible(true);
        }
    }

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect& rect, int dy);

private:
    QWidget* lineNumberArea;
};