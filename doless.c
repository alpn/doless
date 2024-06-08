/*
 * Copyright (c) 2024 github.com/alpn
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

#include <err.h>
#include <stdio.h>
#include <unistd.h>

#define ALLPROM "stdio rpath wpath cpath dpath tmppath inet mcast " \
                "fattr chown flock unix dns getpw sendfd recvfd tape " \
                "tty proc exec prot_exec settime ps vminfo id pf wroute " \
                "audio video bpf unveil error"

int
main(int argc, char* argv[]) {

    int ch, unveil_libs = 0;
    char* path_to_unveil = NULL;
    char* promises = NULL;

    if (pledge("stdio unveil exec", NULL) == -1)
        err(1, "pledge");

    closefrom(STDERR_FILENO + 1);

	while ((ch = getopt(argc, argv, "p:r:w:W:x:X:c:C:A:l")) != -1) {
		switch (ch) {
		case 'r':
            path_to_unveil = optarg; 
            if (unveil(path_to_unveil, "r") == -1)
                err(1, "unveil");
			break;
		case 'w':
            path_to_unveil = optarg; 
            if (unveil(path_to_unveil, "w") == -1)
                err(1, "unveil");
			break;
		case 'W':
            path_to_unveil = optarg; 
            if (unveil(path_to_unveil, "rw") == -1)
                err(1, "unveil");
			break;
		case 'x':
            path_to_unveil = optarg; 
            if (unveil(path_to_unveil, "x") == -1)
                err(1, "unveil");
			break;
		case 'X':
            path_to_unveil = optarg; 
            if (unveil(path_to_unveil, "rx") == -1)
                err(1, "unveil");
			break;
		case 'c':
            path_to_unveil = optarg; 
            if(unveil(path_to_unveil, "c") == -1)
                err(1, "unveil");
			break;
		case 'C':
            path_to_unveil = optarg;
            if(unveil(path_to_unveil, "rc") == -1)
                err(1, "unveil");
			break;
		case 'A':
            path_to_unveil = optarg;
            if (unveil(path_to_unveil, "rwxc") == -1)
                err(1, "unveil");
			break;
		case 'l':
            unveil_libs = 1;
			break;
		case 'p':
            if (promises)
                goto bad;
            promises = optarg;
			break;
		default:
            goto bad;
			break;
		}
	}

	argv += optind;
	argc -= optind;

    const char* command = argv[0];

    if (!path_to_unveil && !promises && !unveil_libs)
        goto bad;

    if (unveil_libs) {
        if (unveil("/usr/libexec/ld.so", "r") == -1)
            err(1, "unveil");
        if (unveil("/var/run/ld.so.hints", "r") == -1)
            err(1, "unveil");
        if (unveil("/usr/lib/", "r") == -1)
            err(1, "unveil");
        if (unveil("/usr/local/lib/", "r") == -1)
            err(1, "unveil");
    }

    if (path_to_unveil || unveil_libs) {
        if (unveil(command, "x") == -1)
            err(1, "unveil");

        if (unveil(NULL, NULL) == -1)
            err(1, "unveil");
    }

    if (pledge("stdio exec", NULL) == -1)
        err(1, "pledge");

    // XXX: unveiled paths persist execv only if the process has pledged
    // see - /src/sys/kern/kern_exec.c:577
    if (!promises)
        promises = ALLPROM;

    if (pledge(NULL, promises) == -1)
        err(1, "pledge");

    if (execv(command, argv) == -1) {
        perror("execv()");
        return 1;
    }

bad:
    printf("usage: doless [-p \"promise ...\"] [-rwxcWXCA \"path\" ...] [-l] command [args ...]\n");
    return 1;
}
