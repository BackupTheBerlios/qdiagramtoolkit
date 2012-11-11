#include <QtCore>
#include <QtGui>
#include "scriptcodeedit.h"

#include "scriptsyntaxhighlighter.h"

LineNumberArea::LineNumberArea(ScriptCodeEdit* edit) :
QWidget(edit)
{
	m_edit = edit;
}

void LineNumberArea::paintEvent( QPaintEvent* e )
{
	m_edit->paintLineNumberArea(e);
}

QSize LineNumberArea::sizeHint() const
{
	return QSize(m_edit->lineNumberAreaWidth(), 0);
}


ScriptCodeEdit::ScriptCodeEdit(QWidget *parent)
	: QPlainTextEdit(parent)
{
	setIndentSize(4);
	setReplaceTabs(false);
	m_completerModel = new AutoCompleteModel(this);
	m_completer = new QCompleter(m_completerModel, this);
	m_completer->setWidget(this);
	connect(m_completer, SIGNAL(activated(QString)), this, SLOT(completerActivated(QString)));
	m_markAsError = -1;
	m_lineNumberArea = new LineNumberArea(this);
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highLightCurrentLine()));

	QKeySequence ks("Ctrl+K, Ctrl+C");
	m_commentAction = new QAction(tr("Comment selection"), this);
	m_commentAction->setShortcut(ks);
	connect(m_commentAction, SIGNAL(triggered()), this, SLOT(commentSelection()));
	QShortcut* s = new QShortcut(ks, this);
	connect(s, SIGNAL(activated()), this, SLOT(commentSelection()));

	ks = QKeySequence("Ctrl+K, Ctrl+U");
	m_uncommentAction = new QAction(tr("Uncomment selection"), this);
	m_uncommentAction->setShortcut(ks);
	connect(m_uncommentAction, SIGNAL(triggered()), this, SLOT(uncommentSelection()));
	s = new QShortcut(ks, this);
	connect(s, SIGNAL(activated()), this, SLOT(uncommentSelection()));

	m_gotoAction = new QAction(tr("&Go to Line..."), this);
	m_gotoAction->setShortcut(Qt::CTRL+Qt::Key_G);
	connect(m_gotoAction, SIGNAL(triggered()), this, SLOT(gotoLineActionTriggered()));
	// Initialize syntax highlighter
	ScriptSyntaxHighlighter* h = new ScriptSyntaxHighlighter(document());
	h->addKeyword( "abstract" );
	h->addKeyword( "boolean" );
	h->addKeyword( "break" );
	h->addKeyword( "byte" );
	h->addKeyword( "case" );
	h->addKeyword( "catch" );
	h->addKeyword( "char" );
	h->addKeyword( "class" );
	h->addKeyword( "const" );
	h->addKeyword( "continue" );
	h->addKeyword( "default" );
	h->addKeyword( "delete" );
	h->addKeyword( "do" );
	h->addKeyword( "double" );
	h->addKeyword( "else" );
	h->addKeyword( "export" );
	h->addKeyword( "extends" );
	h->addKeyword( "false" );
	h->addKeyword( "final" );
	h->addKeyword( "finally" );
	h->addKeyword( "float" );
	h->addKeyword( "for" );
	h->addKeyword( "function" );
	h->addKeyword( "goto" );
	h->addKeyword( "if" );
	h->addKeyword( "implements" );
	h->addKeyword( "in" );
	h->addKeyword( "instanceof" );
	h->addKeyword( "int" );
	h->addKeyword( "long" );
	h->addKeyword( "native" );
	h->addKeyword( "new" );
	h->addKeyword( "null" );
	h->addKeyword( "package" );
	h->addKeyword( "private" );
	h->addKeyword( "protected" );
	h->addKeyword( "public" );
	h->addKeyword( "return" );
	h->addKeyword( "short" );
	h->addKeyword( "static" );
	h->addKeyword( "super" );
	h->addKeyword( "switch" );
	h->addKeyword( "synchronized" );
	h->addKeyword( "this" );
	h->addKeyword( "throw" );
	h->addKeyword( "throws" );
	h->addKeyword( "transient" );
	h->addKeyword( "true" );
	h->addKeyword( "try" );
	h->addKeyword( "typeof" );
	h->addKeyword( "undefined" );
	h->addKeyword( "var" );
	h->addKeyword( "void" );
	h->addKeyword( "while" );
	h->addKeyword( "with" );
}

ScriptCodeEdit::~ScriptCodeEdit()
{

}

AutoCompleteModel* ScriptCodeEdit::autoCompleteModel() const
{
	return m_completerModel;
}

void ScriptCodeEdit::clearMarkedLines()
{
	m_markAsError = -1;
	cMarkedLines.clear();
	QList<QTextEdit::ExtraSelection> e;
	e.append(currentLineMarker());
	setExtraSelections(e);
	update();
}

void ScriptCodeEdit::commentSelection()
{
	QTextCursor c(textCursor());
	if (c.hasSelection()){
		int end = textCursor().selectionEnd();
		c.setPosition(textCursor().selectionStart());
		QTextBlock b = c.block();
		QTextCursor ec(document());
		ec.beginEditBlock();
		while(b.isValid() && b.position() < textCursor().selectionEnd()){
			ec.setPosition(b.position());
			ec.insertText("//");
			b = b.next();
		}
		ec.endEditBlock();
	} else {
		c.movePosition(QTextCursor::StartOfLine);
		c.insertText("//");
	}
}

void ScriptCodeEdit::completerActivated(const QString & text)
{
	QTextCursor c(textCursor());
	c.movePosition(QTextCursor::StartOfWord);
	c.setPosition(textCursor().position(), QTextCursor::KeepAnchor);
	c.removeSelectedText();
	c.insertText(text);
}

QTextEdit::ExtraSelection ScriptCodeEdit::currentLineMarker() const
{
	QTextEdit::ExtraSelection s;

	// Mark current line
	QColor mLineColor = QColor(Qt::yellow).lighter(160);

	s.format.setBackground(mLineColor);
	s.format.setProperty(QTextFormat::FullWidthSelection, true);
	s.cursor = textCursor();
	s.cursor.clearSelection();

	return s;
}

void ScriptCodeEdit::contextMenuEvent( QContextMenuEvent* e )
{
	if (contextMenuPolicy() == Qt::DefaultContextMenu){
		QMenu* m = createStandardContextMenu();
		m->addSeparator();
		m->addAction(m_commentAction);
		m->addAction(m_uncommentAction);
		m->addSeparator();
		m->addAction(m_gotoAction);
		//...
		m->exec(e->globalPos());
		delete m;
		e->accept();
	} else {
		QPlainTextEdit::contextMenuEvent(e);
	}
}

bool ScriptCodeEdit::event(QEvent *event)
{
	if (event->type() == QEvent::ToolTip) {
		QHelpEvent *helpEvent = static_cast<QHelpEvent*>(event);
		QTextCursor cm = cursorForPosition(helpEvent->pos());
		if (!cm.isNull()){
			cm.select(QTextCursor::WordUnderCursor);
			if (!cm.selectedText().isEmpty()){
				QTextCursor co(cm);/* = cursorForPosition(helpEvent->pos());*/
				co.movePosition(QTextCursor::StartOfWord);
				QString t;
				if (!co.movePosition(QTextCursor::PreviousWord, QTextCursor::MoveAnchor, 1)){
					t = m_completerModel->toolTip(cm.selectedText(), "");
				} else {
					if (co.selectedText() == "."){
						co.movePosition(QTextCursor::PreviousWord, QTextCursor::MoveAnchor, 1);
						co.select(QTextCursor::WordUnderCursor);
						t = m_completerModel->toolTip(co.selectedText(), cm.selectedText());
					} else {
						t = m_completerModel->toolTip(cm.selectedText(), "");
					}
				}
				if (!t.isEmpty()) {
					QToolTip::showText(helpEvent->globalPos(), t);
				} else {
					QToolTip::hideText();
					event->ignore();
				}
			}
		}
		return true;
	}
	return QPlainTextEdit::event(event);
}

void ScriptCodeEdit::gotoLineActionTriggered()
{
	QTextCursor c(textCursor());
	bool ok;
	int ln = QInputDialog::getInt(this, tr("Go to Line"), tr("Line number (1 - %1):").arg(blockCount()), c.blockNumber() + 1, 1, 2147483647, 1, &ok);
	if (ok){
		ln--;
		if (ln < c.blockNumber()){
			c.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor, c.blockNumber() - ln);
		} else {
			c.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, ln - c.blockNumber());
		}
		setTextCursor(c);
	}
}

void ScriptCodeEdit::highLightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;
	if (!isReadOnly()){
		QTextEdit::ExtraSelection selection;
		selection.format.setBackground(QColor(Qt::yellow).lighter(160));
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}
	extraSelections += cMarkedLines;
	setExtraSelections(extraSelections);
}

int ScriptCodeEdit::indentSize() const
{
	return m_indentSize;
}

int ScriptCodeEdit::lineNumberAreaWidth() const
{
	int d = 1;
	int m = qMax(1, blockCount());
	while(m >= 10){
		m /= 10;
		d++;
	}
	int s = 22 + fontMetrics().width(QLatin1Char('9')) * d;
	return s;
}

void ScriptCodeEdit::keyPressEvent(QKeyEvent* e)
{
	if (m_completer && m_completer->popup()->isVisible()) {
		// The following keys are forwarded by the completer to the widget
		switch (e->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Escape:
		case Qt::Key_Tab:
		case Qt::Key_Backtab:
			e->ignore();
			return; // let the completer do default behavior
		default:
			break;
		}
	}
	if (m_gotoAction && m_gotoAction->shortcut().matches(QKeySequence(e->modifiers() + e->key())) == QKeySequence::ExactMatch){
		gotoLineActionTriggered();
		e->accept();
	} else if (e->key() == Qt::Key_Backtab){
		if (textCursor().hasSelection()){
			QTextCursor c(textCursor());
			c.setPosition(textCursor().selectionStart());
			QTextBlock b = c.block();
			QTextCursor ec(document());
			ec.beginEditBlock();
			while(b.isValid() && b.position() < textCursor().selectionEnd()){
				ec.setPosition(b.position());
				if (b.text().startsWith("\t")){
					ec.deleteChar();
				}
				b = b.next();
			}
			ec.endEditBlock();
		} else {
			e->ignore();
		}
	} else if (e->key() == Qt::Key_Period){
		if (!textCursor().atBlockStart()){
			QTextCursor c(textCursor());
			c.movePosition(QTextCursor::PreviousWord);
			c.select(QTextCursor::WordUnderCursor);
			if (m_completerModel->contains(c.selectedText())){
				m_completerModel->setObjectType(c.selectedText(), AutoCompleteModel::Member);
				m_completer->setCompletionPrefix("");
				textCursor().insertText(".");
				QRect r = cursorRect();
				r.setWidth(200);
				r.translate(lineNumberAreaWidth(), 5);
				m_completer->complete(r);
			} else {
				textCursor().insertText(".");
			}
			e->accept();
		}
	} else if (e->key() == Qt::Key_Space){
		if (e->modifiers() == Qt::CTRL){
			QTextCursor c(textCursor());
			c.movePosition(QTextCursor::StartOfWord);
			c.setPosition(textCursor().position(), QTextCursor::KeepAnchor);
			m_completer->setCompletionPrefix(c.selectedText());
			QRect r = cursorRect();
			r.setWidth(200);
			r.translate(lineNumberAreaWidth(), 5);
			m_completer->complete(r);
			e->ignore();
		} else {
			QPlainTextEdit::keyPressEvent(e);
		}
	} else if (e->key() == Qt::Key_Tab){
		if (textCursor().hasSelection()){
			int end = textCursor().selectionEnd();
			QTextCursor c(textCursor());
			c.setPosition(textCursor().selectionStart());
			QTextBlock b = c.block();
			QTextCursor ec(document());
			ec.beginEditBlock();
			while(b.isValid() && b.position() < textCursor().selectionEnd()){
				ec.setPosition(b.position());
				ec.insertText("\t");
				b = b.next();
			}
			ec.endEditBlock();
		} else {
			QPlainTextEdit::keyPressEvent(e);
		}
	} else {
		QPlainTextEdit::keyPressEvent(e);
	}
}

void ScriptCodeEdit::markLineAsError(int line)
{
	m_markAsError = line - 1;
	update();
}

void ScriptCodeEdit::paintLineNumberArea( QPaintEvent* e )
{
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();

	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int)blockBoundingGeometry(block).height();

	QPainter p(m_lineNumberArea);
	QLinearGradient lg(QPointF(0,0), QPointF(lineNumberAreaWidth(), 0));
	lg.setColorAt(0, Qt::lightGray);
	lg.setColorAt(1, Qt::white);
	p.fillRect(e->rect(), QBrush(lg));

	while(block.isValid() && top <= e->rect().bottom()){
		if (block.isVisible() && bottom >= e->rect().top()){
			QString mNumber = QString::number(blockNumber + 1);
			if (m_markAsError == blockNumber){
				p.save();
				p.setBrush(QBrush(QColor(Qt::red)));
				p.drawEllipse(QRectF(4, top, fontMetrics().height() - 2, fontMetrics().height() - 2));
				p.restore();
			}
			p.setPen(Qt::black);
			p.drawText(0, top, lineNumberAreaWidth(), fontMetrics().height(), Qt::AlignRight, mNumber);
		}
		block = block.next();
		top = bottom;
		bottom = top + (int)blockBoundingGeometry(block).height();
		++blockNumber;
	}
}

bool ScriptCodeEdit::replaceTabs() const
{
	return m_replaceTabs;
}

void ScriptCodeEdit::resizeEvent( QResizeEvent* e )
{
	QPlainTextEdit::resizeEvent(e);

	QRect r = contentsRect();
	m_lineNumberArea->setGeometry(QRect(r.left(), r.top(), lineNumberAreaWidth(), r.height()));
}

void ScriptCodeEdit::setIndentSize(int chars)
{
	m_indentSize = chars;
	setTabStopWidth(fontMetrics().width(" ") * m_indentSize);
}

void ScriptCodeEdit::setReplaceTabs(bool on)
{
	if (m_replaceTabs != on){
		m_replaceTabs = on;
		if (m_replaceTabs){
			QTextCursor c(document());

		}
	}
}

void ScriptCodeEdit::uncommentSelection()
{
	QTextCursor c(textCursor());
	if (c.hasSelection()){
		int end = textCursor().selectionEnd();
		c.setPosition(textCursor().selectionStart());
		QTextBlock b = c.block();
		QTextCursor ec(document());
		ec.beginEditBlock();
		while(b.isValid() && b.position() < textCursor().selectionEnd()){
			ec.setPosition(b.position());
			ec.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
			if (ec.selectedText() == "//"){
				ec.removeSelectedText();
			}
			b = b.next();
		}
		ec.endEditBlock();
	} else {
		c.movePosition(QTextCursor::StartOfLine);
		c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
		if (c.selectedText() == "//"){
			c.removeSelectedText();
		}
	}
}

void ScriptCodeEdit::updateLineNumberArea( const QRect & rect, int dy )
{
	if (dy){
		m_lineNumberArea->scroll(0, dy);
	} else {
		m_lineNumberArea->update(0, rect.y(), lineNumberAreaWidth(), rect.height());
	}
	if (rect.contains(viewport()->rect())){
		updateLineNumberAreaWidth(0);
	}
}

void ScriptCodeEdit::updateLineNumberAreaWidth( int newBlockCount )
{
	Q_UNUSED(newBlockCount);
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
