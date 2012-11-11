#ifndef SCRIPTCODEEDIT_H
#define SCRIPTCODEEDIT_H

#include <QAbstractTableModel>
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>

#include <autocompletemodel.h>

class ScriptCodeEdit;
class QCompleter;
class QTextDocument;


class LineNumberArea : public QWidget 
{
public:
	LineNumberArea(ScriptCodeEdit* editor );

	QSize sizeHint() const;
protected:
	void paintEvent( QPaintEvent* e );
private:
	ScriptCodeEdit* m_edit;
};

class ScriptCodeEdit : public QPlainTextEdit
{
	Q_OBJECT
	Q_PROPERTY(bool replaceTabs READ replaceTabs WRITE setReplaceTabs)
	Q_PROPERTY(int indentSize READ indentSize WRITE setIndentSize);
public:
	ScriptCodeEdit(QWidget* parent = 0);
	~ScriptCodeEdit();

	AutoCompleteModel* autoCompleteModel() const;

	void clearMarkedLines();

	int indentSize() const;

	int lineNumberAreaWidth() const;

	void markLineAsError( int line );

	void paintLineNumberArea( QPaintEvent* e );
	bool replaceTabs() const;
	/**
	 * Sets the indent size in characters.
	 */
	void setIndentSize(int chars);

	void setReplaceTabs(bool on);
public slots:
	void commentSelection();
	void uncommentSelection();
private slots:
	void completerActivated(const QString & text);
	void gotoLineActionTriggered();
	void highLightCurrentLine();
	void updateLineNumberArea( const QRect & rect, int dy );
	void updateLineNumberAreaWidth( int newBlockCount );
protected:
	void contextMenuEvent( QContextMenuEvent* ee);
	bool event(QEvent* e);
 	void keyPressEvent(QKeyEvent* e);
	void resizeEvent(QResizeEvent* e);
private:
	QTextEdit::ExtraSelection currentLineMarker() const; 

	QAction* m_commentAction;
	QCompleter* m_completer;
	AutoCompleteModel* m_completerModel;
	QAction* m_gotoAction;
	int m_indentSize;
	int m_markAsError;
	bool m_replaceTabs;
	QList<QTextEdit::ExtraSelection> cMarkedLines;
	LineNumberArea* m_lineNumberArea;
	QAction* m_uncommentAction;
};

#endif // SCRIPTCODEEDIT_H
