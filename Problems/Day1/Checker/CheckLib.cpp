#include "CheckLib.h"
#include <windows.h>

//////////////////////////////////
///    Automat Gen Functions
//////////////////////////////////
#define AUTOMAT_TERM_UNSUCCESS_STATE 0
#define AUTOMAT_BEGIN_STATE 1
#define AUTOMAT_TERM_SUCCESS_STATE 2
#define forDigits for (char c='0'; c<='9'; c++)

#define AINT_STATE_COUNT 5
#define AINT_WAIT_SIGN AUTOMAT_BEGIN_STATE
#define AINT_WAIT_DIGIT 3
#define AINT_READ_DIGIT 4

static char** GenIntegerAutomat() {
	char** res=new char* [AINT_STATE_COUNT];
	for (int i=0; i<AINT_STATE_COUNT; i++) {
		res[i]=new char [256];
		memset(res[i],AUTOMAT_TERM_UNSUCCESS_STATE,256);
	}
	//AINT_WAIT_SIGN
	res[AINT_WAIT_SIGN]['-']=res[AINT_WAIT_SIGN]['+']=AINT_WAIT_DIGIT;
	forDigits res[AINT_WAIT_SIGN][c]=AINT_READ_DIGIT;
	//AINT_WAIT_DIGIT
	forDigits res[AINT_WAIT_DIGIT][c]=AINT_READ_DIGIT;
	//AINT_READ_DIGIT
	forDigits res[AINT_READ_DIGIT][c]=AINT_READ_DIGIT;
	res[AINT_READ_DIGIT][0]=AUTOMAT_TERM_SUCCESS_STATE;
	return res;
}

#define AREAL_STATE_COUNT 10
#define AREAL_WAIT_SIGN AUTOMAT_BEGIN_STATE
#define AREAL_WAIT_INT_PART 3
#define AREAL_READ_INT_PART 4
#define AREAL_WAIT_FRAC_PART 5
#define AREAL_READ_FRAC_PART 6
#define AREAL_WAIT_EXP_SIGN 7
#define AREAL_WAIT_EXP 8
#define AREAL_READ_EXP 9

static char** GenDoubleAutomat() {
	char** res=new char* [AREAL_STATE_COUNT];
	for (int i=0; i<AREAL_STATE_COUNT; i++) {
		res[i]=new char [256];
		memset(res[i],AUTOMAT_TERM_UNSUCCESS_STATE,256);
	}
	//AREAL_WAIT_SIGN
	res[AREAL_WAIT_SIGN]['-']=res[AREAL_WAIT_SIGN]['+']=AREAL_WAIT_INT_PART;
	forDigits res[AREAL_WAIT_SIGN][c]=AREAL_READ_INT_PART;
	//AREAL_WAIT_INT_PART
	forDigits res[AREAL_WAIT_INT_PART][c]=AREAL_READ_INT_PART;
	//AREAL_READ_INT_PART
	forDigits res[AREAL_READ_INT_PART][c]=AREAL_READ_INT_PART;
	res[AREAL_READ_INT_PART][0]=AUTOMAT_TERM_SUCCESS_STATE;
	res[AREAL_READ_INT_PART]['.']=AREAL_WAIT_FRAC_PART;
	res[AREAL_READ_INT_PART]['e']=res[AREAL_READ_INT_PART]['E']=AREAL_WAIT_EXP_SIGN;
	//AREAL_WAIT_FRAC_PART
	forDigits res[AREAL_WAIT_FRAC_PART][c]=AREAL_READ_FRAC_PART;
	//AREAL_READ_FRAC_PART
	forDigits res[AREAL_READ_FRAC_PART][c]=AREAL_READ_FRAC_PART;
	res[AREAL_READ_FRAC_PART][0]=AUTOMAT_TERM_SUCCESS_STATE;
	res[AREAL_READ_FRAC_PART]['e']=res[AREAL_READ_FRAC_PART]['E']=AREAL_WAIT_EXP_SIGN;
	//AREAL_WAIT_EXP_SIGN
	res[AREAL_WAIT_EXP_SIGN]['+']=res[AREAL_WAIT_EXP_SIGN]['-']=AREAL_WAIT_EXP;
	forDigits res[AREAL_WAIT_EXP_SIGN][c]=AREAL_READ_EXP;
	//AREAL_WAIT_EXP
	forDigits res[AREAL_WAIT_EXP][c]=AREAL_READ_EXP;
	//AREAL_READ_EXP
	forDigits res[AREAL_READ_EXP][c]=AREAL_READ_EXP;
	res[AREAL_READ_EXP][0]=AUTOMAT_TERM_SUCCESS_STATE;
	return res;
}

static char **intAutomat=GenIntegerAutomat();
static char **realAutomat=GenDoubleAutomat();

//////////////////////////////////
///    Some Needful Functions
//////////////////////////////////
static char* ExtractFileName(const char* FileName) {
	char *r1((char*)FileName), *r2((char*)FileName);
	while (*r2) {
		if ((*r2=='\\') || (*r2=='/')) r1=r2;
		r2++;
	}
	return r1+1;
}

static char* ExtractParamFromCommandLine(const UINT N) {
	static char buff[1024]; *buff=0;
	char* p1(GetCommandLineA()); UINT len;
	UINT i(0);
	while (1) {
		while (*p1==' ') p1++;
		if (!*p1) return buff;
		len=0;
		if (*p1=='"') {
			p1++;
			while (*(p1+len) && (*(p1+len)!='"')) len++;
			if (!*(p1+len)) return buff;
		} else {
			while (*(p1+len) && (*(p1+len)!=' ')) len++;
			if (!*(p1+len)) return buff;
		}
		if (i==N) {
			memcpy(buff, p1, min(len,(sizeof buff)-1));
			buff[len]=0;
			return buff;
		}
		p1+=len+1;
		i++;
	}
	
}
static bool checkAutomat(const char** automat, char* s) {
	char st(AUTOMAT_BEGIN_STATE);
	while ((st!=AUTOMAT_TERM_UNSUCCESS_STATE) && (st!=AUTOMAT_TERM_UNSUCCESS_STATE)) {
		st=automat[st][(unsigned char)*s];
		if (!*s) break;
		s++;
	}
	return (st==AUTOMAT_TERM_SUCCESS_STATE);
}
//////////////////////////////////
///    StdReadStream Class
//////////////////////////////////
StdReadStream::StdReadStream(const char* FileName) { 
	fpos=0; ffill=0;
} 

bool StdReadStream::FetchBuffer() {
	if (!f) return false;
	fpos=0;
	ffill=(UINT)fread(fbuff, 1, STD_READ_STREAM_BUFFER_SIZE, f);
	return (ffill!=0);
}

bool StdReadStream::eof() {
	return (!f) || ((ffill==fpos) && feof(f));
}

bool StdReadStream::eoln() {
	return (eof() || (peekChar()==13));
}

bool StdReadStream::seekeof() {
	char c;
	while (!eof()) {
		c=peekChar();
		if ((c!=' ') && (c>19)) return false;
		fpos++;
	}
	return true;
}

char StdReadStream::peekChar() {
	if (ffill==fpos) {
		if (!FetchBuffer()) PEQuit("Unexpected end of file");
	}
	return fbuff[fpos];
}

char StdReadStream::getChar() {
	if (ffill==fpos) {
		if (!FetchBuffer()) PEQuit("Unexpected end of file");
	}
	return fbuff[fpos++];
}
char* StdReadStream::getWord() {
	if (seekeof()) PEQuit("Unexpected end of file");
	UINT p(0); char c;
	while (!eof()) {
		c=peekChar();
		if ((c==' ') || (c<20)) break;
		wbuff[p++]=c;
		fpos++;
		if (p>MAX_WORD_LENGHT) PEQuit("Max word length exceeded");
	}
	wbuff[p]=0;
	return wbuff;
}
int StdReadStream::getInteger() {
	char *a(getWord());
	if (!checkAutomat((const char**)intAutomat,a)) PEQuit("Fail to parse integer");
	return atoi(a);
}
__int64 StdReadStream::getInt64() {
	char *a(getWord());
	if (!checkAutomat((const char**)intAutomat,a)) PEQuit("Fail to parse integer");
	return _atoi64(a);
}
double StdReadStream::getDouble() {
	char *a(getWord());
	if (!checkAutomat((const char**)realAutomat,a)) PEQuit("Fail to parse double");
	return atof(a);
}

bool StdReadStream::getLine(const char* buff, const UINT size) {
	char* p((char*)buff);
	UINT c((UINT)size-1);
	while (!eoln() && c) {
		*(p++)=getChar();
		c--;
	}
	*p=0;
	if (eoln()) {
		if (!eof()) {
			getChar(); getChar();
		}
		return true;
	} else
		return false;
}

//////////////////////////////////
///    InReadStream Class
//////////////////////////////////
InReadStream::InReadStream(const char* FileName):StdReadStream(FileName){
	if (fopen_s(&f,FileName,"rb")) {
		char Temp[256];
		strcpy_s(Temp,"Cannot open INPUT file '"); strcat_s(Temp,ExtractFileName(FileName)); strcat_s(Temp,"'");
		PEQuit(Temp);
	}
	else
		FetchBuffer();
}
void InReadStream::PEQuit(const char* Text) {
	QUIT(_ERI, Text);
}
//////////////////////////////////
///    OutReadStream Class
//////////////////////////////////
OutReadStream::OutReadStream(const char* FileName):StdReadStream(FileName) {
	if (fopen_s(&f,FileName,"rb")) {
		char Temp[256];
		strcpy_s(Temp,"Cannot open OUTPUT file '"); strcat_s(Temp,ExtractFileName(FileName)); strcat_s(Temp,"'");
		PEQuit(Temp);
	} 
	else
		FetchBuffer();
}
void OutReadStream::PEQuit(const char* Text) {
	QUIT(_PE, Text);
}
//////////////////////////////////
///    AnsReadStream Class
//////////////////////////////////
AnsReadStream::AnsReadStream(const char* FileName):StdReadStream(FileName){
	if (fopen_s(&f,FileName,"rb")) 
		f=0;
	else
		FetchBuffer();
}
void AnsReadStream::PEQuit(const char* Text) {
	QUIT(_ERA, Text);
}
//////////////////////////////////
///    Quit Functions
//////////////////////////////////
static void PRE_QUIT(const UINT code) {
	switch (code) {
		case _OK: printf("%s","OK"); break;
		case _WA: printf("%s","WA"); break;
		case _PE: printf("%s","PE"); break;
		case _ERI: printf("%s","!I"); break;
		case _ERA: printf("%s","!A"); break;
		default: ExitProcess(1);
	}
}

void QUIT(const UINT code) {
	PRE_QUIT(code);
	fflush(stdout);
	ExitProcess(((code==_ERI) || (code==_ERA))?1:0);
}

void QUIT(const UINT code, const char* Text) {
	PRE_QUIT(code);
	if (Text) printf(" %s", Text);
	fflush(stdout);
	ExitProcess(((code==_ERI) || (code==_ERA))?1:0);
}

InReadStream inf(ExtractParamFromCommandLine(1));
OutReadStream outf(ExtractParamFromCommandLine(2));
AnsReadStream ansf(ExtractParamFromCommandLine(3));