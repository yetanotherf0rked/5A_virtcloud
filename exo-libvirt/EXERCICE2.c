#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

int main(int argc, char *argv[]) {

	virConnectPtr conn;
	char *host;
	int vcpus;
	unsigned long long node_free_memory;

	conn = virConnectOpen("qemu+ssh://user@172.30.3.39/system");
	if (conn == NULL) {
		fprintf(stderr, "Failed to open connection to qemu+ssh");
		return 1;
	}
	printf("Connection established\n");
	
	host = virConnectGetHostname(conn);
	vcpus = virConnectGetMaxVcpus(conn, NULL);
	node_free_memory = virNodeGetFreeMemory(conn);
	fprintf(stdout, "Hostname:%s\n", host);
	fprintf(stdout, "Maximum support virtual CPUs: %d\n", vcpus);
	fprintf(stdout, "Node free memory: %llu\n", node_free_memory);
	
	free(host);
	
	virConnectClose(conn);
	return 0;
}
