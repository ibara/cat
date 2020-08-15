/*
 * Copyright (c) 2020 Brian Callahan <bcallah@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * cat -- concatenate files
 */

extern void *_syscall(void *n, void *a, void *b, void *c, void *d, void *e);

static void
_exit(int status)
{

	_syscall((void *) 1, (void *) status, (void *) 0, (void *) 0, (void *) 0, (void *) 0);
}

static long
read(int d, void *buf, unsigned long nbytes)
{

	return (long) _syscall((void *) 3, (void *) d, (void *) buf, (void *) nbytes, (void *) 0, (void *) 0);
}

static long
write(int d, const void *buf, unsigned long nbytes)
{

	return (long) _syscall((void *) 4, (void *) d, (void *) buf, (void *) nbytes, (void *) 0, (void *) 0);
}

static int
open(const char *path, int flags)
{

	return (int) _syscall((void *) 5, (void *) path, (void *) flags, (void *) 0, (void *) 0, (void *) 0);
}

static int
close(int d)
{

	return (int) _syscall((void *) 6, (void *) d, (void *) 0, (void *) 0, (void *) 0, (void *) 0);
}

static unsigned long
strlen(const char *s)
{
	char *t;

	t = (char *) s;
	while (*t != '\0')
		t++;

	return t - s;
}

static void
dputs(const char *s, int fd)
{

	write(fd, s, strlen(s));
}

static int
strcmp(const char *s1, const char *s2)
{

	while (*s1 == *s2++) {
		if (*s1++ == '\0')
			return 0;
	}

	return 1;
}

int
main(int argc, char *argv[])
{
	char c;
	int fd;

	if (argc == 1) {
		fd = 0;
		goto cat;
	}

	while (--argc) {
		if (!strcmp("-", *++argv)) {
			fd = 0;
			goto cat;
		}

		if ((fd = open(*argv, 0x0000)) == -1) {
			dputs("cat: error: could not open ", 2);
			dputs(*argv, 2);
			dputs("\n", 2);
		} else {
cat:
			while (read(fd, &c, 1) > 0)
				write(1, &c, 1);

			if (fd == 0)
				continue;

			if (close(fd) == -1) {
				dputs("cat: error: could not close ", 2);
				dputs(*argv, 2);
				dputs("\n", 2);

				_exit(1);
			}
		}
	}

	return 0;
}
