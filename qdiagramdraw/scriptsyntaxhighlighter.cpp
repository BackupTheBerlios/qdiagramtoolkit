#include "stdafx.h"

#include "scriptsyntaxhighlighter.h"

ScriptSyntaxHighlighter::ScriptSyntaxHighlighter(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);
	//QStringList keywordPatterns;
	//keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
	//	<< "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
	//	<< "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
	//	<< "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
	//	<< "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
	//	<< "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
	//	<< "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
	//	<< "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
	//	<< "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
	//	<< "\\bvoid\\b" << "\\bvolatile\\b" << "\\bvar\\b" << "\\bif\\b";
	//foreach (QString pattern, keywordPatterns) {
	//	rule.pattern = QRegExp(pattern);
	//	rule.format = keywordFormat;
	//	m_rules.append(rule);
	//}

	// class/object rule
	QTextCharFormat f;
	f.setFontWeight(QFont::Bold);
	f.setForeground(Qt::darkMagenta);
	addRule("objects", QRegExp("QDiagram[A-Za-z]*"), f);

	//classFormat.setFontWeight(QFont::Bold);
	//classFormat.setForeground(Qt::darkMagenta);
	//rule.pattern = QRegExp("QDiagram[A-Za-z]*");
	//rule.format = classFormat;
	//m_rules.append(rule);

	f = QTextCharFormat();
	f.setForeground(Qt::red);
	addRule("Single line comment", QRegExp("//[^\n]*"), f);
	//singleLineCommentFormat.setForeground(Qt::red);
	//rule.pattern = QRegExp("//[^\n]*");
	//rule.format = singleLineCommentFormat;
	//m_rules.append(rule);

	f = QTextCharFormat();
	f.setForeground(Qt::darkGreen);
	addRule("Quotation", QRegExp("\".*\""), f);
	//quotationFormat.setForeground(Qt::darkGreen);
	//rule.pattern = QRegExp("\".*\"");
	//rule.format = quotationFormat;
	//m_rules.append(rule);

	f = QTextCharFormat();
	f.setFontItalic(true);
	f.setForeground(Qt::blue);
	addRule("Functions", QRegExp("\\b[^QDiagram][A-Za-z0-9_]+(?=\\()"), f);
	//functionFormat.setFontItalic(true);
	//functionFormat.setForeground(Qt::blue);
	//rule.pattern = QRegExp("\\b[^QDiagram][A-Za-z0-9_]+(?=\\()");
	//rule.format = functionFormat;
	//m_rules.append(rule);

	multiLineCommentFormat.setForeground(Qt::red);

	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
}

void ScriptSyntaxHighlighter::addKeyword( const QString & word )
{
	HighlightingRule r;

	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);

	r.pattern = QRegExp( "\\b" + word + "\\b" );
	r.format = keywordFormat;
	m_rules["keyword: " + word] = r;
	//m_rules.append(r);
}

void ScriptSyntaxHighlighter::addRule(const QString & name, const QRegExp & regExp, const QTextCharFormat & format)
{
	HighlightingRule r;
	r.pattern = regExp;
	r.format = format;
	m_rules[name] = r;
}

void ScriptSyntaxHighlighter::highlightBlock(const QString &text)
{

	//QTextCharFormat f;
	//f.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
	//f.setUnderlineColor(Qt::red);

	QList<HighlightingRule> rules = m_rules.values();
	Q_FOREACH(HighlightingRule rule, rules){
		QRegExp expression(rule.pattern);
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = text.indexOf(expression, index + length);
		}
	}
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = text.indexOf(commentStartExpression);

	while (startIndex >= 0) {
		int endIndex = text.indexOf(commentEndExpression, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
	}
}
