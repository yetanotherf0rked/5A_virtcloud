#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

int main(int argc, char *argv[]) {

	virConnectPtr conn;
	conn = virConnectOpen("qemu+ssh://user@172.30.3.39/system");
	if (conn == NULL) {
		fprintf(stderr, "Failed to open connection to qemu+ssh");
		return 1;
	}

	printf("Connection established\n");
	virConnectClose(conn);
	return 0;
}
