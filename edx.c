/* soon-to-be hex editor.
 * If invoked without an argument, it will convert the 
 * stdin (assumed to be a text file full of nothing but hex characters and optional white space) to 
 * its binary representation and send the output to stdout.
 * If summoned without an argument, it will do the inverse.
 */

#include <unistd.h>
#define SIZ 4096
int tohex(char *txt, char *bin);
int totext(char *txt, char *bin, int count);
int main(int argc, char **argv)
{
	int nout;
	char txt[SIZ];
	char bin[SIZ];

	if (argc > 2) {
		write(2, "<prog> <r/w>", 12);
		write(2, "\ndo not specify file names\n", 27);
		write(2, "use stdin and stdout for i/o\n", 29);
		return -1;
	} else if (argc == 2)
		goto bintotext;
	/* default is to write */
txttobin:
	while (nout = read(0, txt, SIZ) > 0) {
		nout = tohex(txt, bin);
		write(1, bin, nout);
	}
	return 0;
bintotext:
	while ((nout = read(0, bin, SIZ)) > 0) {
		nout = totext(txt, bin, nout);
		write(1, txt, sizeof(txt));
	}
	return 0;

}
/* converts byte-oriented hex text to byte-oriented little-endian binary */
int tohex(char *txt, char *bin)
{
	char c, new = 1;
	int count = 0;
	for (; (c = *txt) != '\0'; txt++) {
		if ('0' <= c && c <= '9')
			c = c - '0';
		else if ('a' <= c && c <= 'f')
			c = c - 'a' + 10;
		else if ('A' <= c && c <= 'F')
			c = c - 'A' + 10;
		else
			continue;
		if (new) {
			*bin = c * 0x10;
			new = 0;
		} else {
			*bin += c;
			bin++;
			count++;
			new = 1;
		}
	}
	return count;
}

/* converts bin to text, return number of txt  converted */
int totext(char *txt, char *bin, int count)
{
	char c = 0;
	int i;
	for (i = 0; i < count; bin++, i++) {
		c = *bin;
		c = c >> 4;
		c = c & 0xf;
		*txt++ = c + ((c < 10) ? '0' : 'a' - 10);
		c = *bin;
		c = c & 0xf;
		*txt++ = c + ((c < 10) ? '0' : 'a' - 10);
	}
	*txt = '\0';
	return i;
}
