#ifndef INPUT_H
#define INPUT_H
#include <QString>

#ifndef EOF
#define EOF (-1)
#endif

class Input
{
	QString		buf;
	int			pos;
public:
	int			line;
	int			column;
	QString		lexeme;
public:
	Input();
	~Input();
public:
	int peek();
	int read();
	bool init(const QString& txt);
};

#endif // INPUT_H
