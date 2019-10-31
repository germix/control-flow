#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
#include <QSyntaxHighlighter>

#include <QSyntaxHighlighter>

 #include <QHash>
 #include <QTextCharFormat>

class Highlighter : public QSyntaxHighlighter
{
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;
public:
	Highlighter(QTextDocument* doc);
	~Highlighter();
public:
	virtual void highlightBlock(const QString& text);
private:
	void addRules(const QTextCharFormat fmt, const QStringList& patterns);
};

#endif // HIGHLIGHTER_H
