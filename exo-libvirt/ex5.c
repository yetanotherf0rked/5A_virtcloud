/* example ex5.c */
/* compile with: gcc -g -Wall ex5.c -o ex5 -lvirt */
#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

int main(int argc, char *argv[])
{
	virConnectPtr conn;

	conn = virConnectOpen("qemu:///system");
	if (conn == NULL) {
		fprintf(stderr, "Failed to open connection to qemu:///system\n");
		return 1;
	}
	/* now the connection has a single reference to it */

	virConnectRef(conn);

	/* now the connection has two references to it */

	virConnectClose(conn);

	/* now the connection has one reference */

	virConnectClose(conn);

	/* now the connection has no references, and has been garbage
	 *      * collected - it should no longer be used */

	return 0;
}
