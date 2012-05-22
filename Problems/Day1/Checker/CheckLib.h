#ifndef _CheckLib
#define _CheckLib

#include <stdio.h>

#define _OK 0
#define _WA 1     // WRONG ANSWER
#define _PE 2     // PRESENTATION ERROR
#define _ERI 3    // ERROR INPUT FILE
#define _ERA 4    // ERROR ANSWER FILE

typedef unsigned int UINT;

void QUIT(const UINT code);
void QUIT(const UINT code, const char* Text);

#define STD_READ_STREAM_BUFFER_SIZE 2048

#define MAX_WORD_LENGHT 30

class StdReadStream{
private:
	char fbuff[STD_READ_STREAM_BUFFER_SIZE], wbuff[MAX_WORD_LENGHT+1];;
	UINT fpos, ffill;
protected:
	FILE* f;
	bool FetchBuffer();
	virtual void PEQuit(const char* Text)=0;
	StdReadStream(const char* FileName); 
public:
	bool eof();
	bool eoln();
	bool seekeof();
	char getChar();
	char peekChar();
	char* getWord();
	int getInteger();
	__int64 getInt64();
	double getDouble();
	bool getLine(const char* buff, const UINT size);
};

class InReadStream: public StdReadStream {
private:
	void PEQuit(const char* Text);
public:
	InReadStream(const char* FileName);
};

class OutReadStream: public StdReadStream {
private:
	void PEQuit(const char* Text);
public:
	OutReadStream(const char* FileName);
};

class AnsReadStream: public StdReadStream {
private:
	void PEQuit(const char* Text);
public:
	AnsReadStream(const char* FileName);
};

extern InReadStream inf;
extern OutReadStream outf;
extern AnsReadStream ansf;

#endif