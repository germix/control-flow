#include "Highlighter.h"
#include <QRegExp>
#include <QRegExpValidator>

Highlighter::Highlighter(QTextDocument* doc) : QSyntaxHighlighter(doc)
{
	QTextCharFormat keywordFormat;
	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);
	QStringList typePatterns;
	QStringList keywordPatterns;
	/*
	keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
						<< "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
						<< "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
						<< "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
						<< "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
						<< "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
						<< "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
						<< "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
						<< "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
						<< "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b";
	*/
	keywordPatterns	<< "\\bchar\\b"
					<< "\\bvoid\\b"
					<< "\\bint\\b"
					<< "\\bshort\\b"
					<< "\\blong\\b"
					<< "\\bfloat\\b"
					<< "\\bdouble\\b"
					<< "\\bsigned\\b"
					<< "\\bunsigned\\b"
					<< "\\bconst\\b"
					;
	keywordPatterns << "\\benum\\b"
					<< "\\bunion\\b"
					<< "\\bstruct\\b"
					<< "\\btypedef\\b"
					;

	keywordPatterns
					<< "\\bif\\b"
					<< "\\belse\\b"
					<< "\\bfor\\b"
					<< "\\bdo\\b"
					<< "\\bwhile\\b"
					;
	addRules(keywordFormat, keywordPatterns);
}
Highlighter::~Highlighter()
{
}
void Highlighter::addRules(const QTextCharFormat fmt, const QStringList& patterns)
{
	HighlightingRule rule;
	foreach(const QString& pattern, patterns)
	{
		rule.format = fmt;
		rule.pattern = QRegExp(pattern);
		highlightingRules.append(rule);
	}
}

// https://doc.qt.io/archives/qq/qq21-syntaxhighlighter.html
void Highlighter::highlightBlock(const QString& text)
{
	enum { NormalState = -1, InsideCStyleComment , InsideCStyleString1, InsideCStyleString2};

	int state = previousBlockState();
	int start = 0;

	QColor stringColor(128, 0, 128);
	QColor commentColor1(0, 128, 0);
	QColor commentColor2(0, 128, 0);
	QColor operatorColor(0, 0, 255);

	foreach (const HighlightingRule &rule, highlightingRules)
	{
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}

	for (int i = 0; i < text.length(); ++i){

		if (state == InsideCStyleComment)
		{
			if (text.mid(i, 2) == "*/") {
				state = NormalState;
				setFormat(start, i - start + 2, commentColor1);
			}
		}
		else if(state == InsideCStyleString1)
		{
			if(text.mid(i, 1) == "\"")
			{
				state = NormalState;
				setFormat(start, i - start + 1, stringColor);
			}
		}
		else if(state == InsideCStyleString2)
		{
			if(text.mid(i, 1) == "\'")
			{
				state = NormalState;
				setFormat(start, i - start + 1, stringColor);
			}
		}
		else
		{
			if (text.mid(i, 2) == "//") {
				setFormat(i, text.length() - i, commentColor2);
				break;
			} else if (text.mid(i, 2) == "/*") {
				start = i;
				state = InsideCStyleComment;
			}
			else if(text.mid(i, 1) == "\"")
			{
				start = i;
				state = InsideCStyleString1;
			}
			else if(text.mid(i, 1) == "\'")
			{
				start = i;
				state = InsideCStyleString2;
			}
			else if(
					   text.mid(i, 1) == "["
					|| text.mid(i, 1) == "]"
					|| text.mid(i, 1) == "("
					|| text.mid(i, 1) == ")"
					|| text.mid(i, 1) == "{"
					|| text.mid(i, 1) == "}"
					|| text.mid(i, 1) == "="
					|| text.mid(i, 1) == "|"
					|| text.mid(i, 1) == ";"
					)
			{

				setFormat(i, 1, operatorColor);
			}
		}
	}
	if (state == InsideCStyleComment)
		setFormat(start, text.length() - start, commentColor1);

	setCurrentBlockState(state);
}
