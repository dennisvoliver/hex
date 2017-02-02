/* This is a nibble-oriented little-endian hexadecimal bin-to-text and text-to-bin converter.
 * This is a down-to-earth simpler version of edx.
 * It makes no attempt to be human-readable.
 * For example, the output from
 *	$ nib r <file
 * where file contains a binary representation of the number 128 will be
 * 08 instead of the expected 0x80, since each hexadecimal digit is
 * a faithful conversion of a nibble, and the whole binary is expected
 * to be in a low-endian representation.
 * The purpose of this program is to make it so that its output/input
 * can be piped to/from a program that reverses the order of characters
 * in a string.
 * It also strictly recognizes a-f for 10-15 and not
 * the uppercase version.
 * My rationale is that liberal acceptance of input,
 * especially to accommodate trivial differences,
 * should be made high up above the abstraction stack.
 * If you want it to accept A-F, use a converter.
 * I refuse complicate simple code for a little convenience.
 * For human-parsing, one can easily make a program that reverses the order
 * of every two consecutive characters.
 * I've opted to make the arrangements of the nibbles a job of other programs
 * instead of this one.
 * If invoked without any arguments, it will
 * assumed to be converting from binary to text.
 * If argument supplied, then text to binary.
 */

#include <unistd.h>
#include <string.h>
#define SIZ 4096
int tohex(char *txt, char *bin);
int totext(char *txt, char *bin, int count);
int main(int argc, char **argv)
{
	int nout;
	char txt[SIZ];
	char bin[SIZ];
	char errm[] = "<prog> [w]\nlow-endian one char per nibbles\nuse stdin and stdout for i/o\nsmall letters only\ndefault is to bin-text\n";

	if (argc > 2) {
		write(2, errm, strlen(errm) + 1);
		return -1;
	} else if (argc == 2)
		goto txttobin;
	/* default is to read binary */
bintotext:
	while ((nout = read(0, bin, SIZ)) > 0) {
		nout = totext(txt, bin, nout);
		write(1, txt, sizeof(txt));
	}
	return 0;

txttobin:
	while (nout = read(0, txt, SIZ) > 0) {
		nout = tohex(txt, bin);
		write(1, bin, nout);
	}
	return 0;
}
/* converts nibble-oriented hex text to nibble-oriented little-endian binary */
int tohex(char *txt, char *bin)
{
	char c, new = 1;
	int count = 0;
	for (; (c = *txt) != '\0'; txt++) {
		if ('0' <= c && c <= '9')
			c = c - '0';
		else if ('a' <= c && c <= 'f')
			c = c - 'a' + 10;
		else
			continue;
		if (new) {
			*bin = c;
			new = 0;
		} else {
			*bin += c * 0x10;
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
		c = c & 0xf;
		*txt++ = c + ((c < 10) ? '0' : 'a' - 10);
		c = *bin;
		c = c >> 4;
		c = c & 0xf;
		*txt++ = c + ((c < 10) ? '0' : 'a' - 10);
	}
	*txt = '\0';
	return i;
}
