#include "CheckLib.h"
#include <string.h>

#define UPPER(x) ((x >= 'a') && (x <= 'z') ? x - 'a' + 'A' : x)

bool upstr_equal(const char *str1, const char *str2) {
	while (*str1 && *str2) {
		if (UPPER(*str1) != UPPER(*str2)) return false;
		str1++;
		str2++;
	}
	return !(*str1 || *str2);
}

void main() {
	char *a = ansf.getWord(), *b = outf.getWord();
	if (upstr_equal(a, b))
		QUIT(_OK);
	else
		QUIT(_WA);
}