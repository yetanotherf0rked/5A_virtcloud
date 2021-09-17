#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

int main(int argc, char *argv[]) {

	virConnectPtr conn;
	char *host;
	int vcpus;
	unsigned long long node_free_memory;

	// Exercice 1
	conn = virConnectOpen("qemu+ssh://user@172.30.3.39/system");
	if (conn == NULL) {
		fprintf(stderr, "Failed to open connection to qemu+ssh");
		return 1;
	}
	printf("Connection established\n");
	
	// Exercice 2
	host = virConnectGetHostname(conn);
	vcpus = virConnectGetMaxVcpus(conn, NULL);
	node_free_memory = virNodeGetFreeMemory(conn);
	fprintf(stdout, "Hostname:%s\n", host);
	fprintf(stdout, "Maximum support virtual CPUs: %d\n", vcpus);
	fprintf(stdout, "Node free memory: %llu\n", node_free_memory);

	// Exercice 3 
	int i;
	int numDomains;
	int *activeDomains;

	numDomains = virConnectNumOfDomains(conn);
	activeDomains = malloc(sizeof(int) * numDomains);
	numDomains = virConnectListDomains(conn, activeDomains, numDomains);

	printf("Active domain IDs:\n");
	for (i = 0 ; i<numDomains; i++) {
		printf("  id: %d, name: %s\n", activeDomains[i]);
	}

	free(host);
	free(activeDomains);
	virConnectClose(conn);
	return 0;
}
