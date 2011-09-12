//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Linux.cpp
// Project: Shared
// Author(s): adamix
//
// License: See LICENSE in root directory
//
//==============================================================================

char* itoa(int value, char* str, int radix) {
static char dig[] =
"0123456789"
"abcdefghijklmnopqrstuvwxyz";
int n = 0, neg = 0;
unsigned int v;
char* p, *q;
char c;

if (radix == 10 && value < 0) {
value = -value;
neg = 1;
}
v = value;
do {
str[n++] = dig[v%radix];
v /= radix;
} while (v);
if (neg)
str[n++] = '-';
str[n] = '\0';
for (p = str, q = p + n/2; p != q; ++p, --q)
c = *p, *p = *q, *q = c;
return str;
}

