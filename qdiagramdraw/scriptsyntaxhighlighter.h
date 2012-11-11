#ifndef SCRIPTSYNTAXHIGHLIGHTER_H
#define SCRIPTSYNTAXHIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

class ScriptSyntaxHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	ScriptSyntaxHighlighter(QTextDocument *parent = 0);
	
	void addKeyword( const QString & word );

	void addRule(const QString & name, const QRegExp & regExp, const QTextCharFormat & format);
protected:
	void highlightBlock(const QString &text);

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QMap<QString,HighlightingRule> m_rules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat classFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat functionFormat;
};

#endif // SCRIPTSYNTAXHIGHLIGHTER_H
