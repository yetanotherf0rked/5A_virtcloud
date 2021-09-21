#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

int main(int argc, char *argv[]) {

	virConnectPtr conn;
	char *host;
	int vcpus;
	unsigned long long node_free_memory;

	// Exercice 1
	conn = virConnectOpen("qemu+ssh://localhost/system");
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
	virDomainPtr *allDomains;
	int numDomains = 0;
	char **inactiveDomains; // inactive domain names (as they don't have IDs)
	int *activeDomains; // active domains ids
	int i;
	int numActiveDomains = virConnectNumOfDomains(conn);
	int numInactiveDomains = virConnectNumOfDefinedDomains(conn);
	virDomainPtr dom;
	virDomainInfo * dominfo;

	allDomains = malloc(
		sizeof(virDomainPtr) * (numActiveDomains + numInactiveDomains)
	);
	inactiveDomains = malloc(
		sizeof(char*) * numInactiveDomains
	);
	activeDomains = malloc(
		sizeof(int) * numActiveDomains
	);
	numActiveDomains = virConnectListDomains(
		conn, 
		activeDomains, 
		numActiveDomains
	);
	numInactiveDomains = virConnectListDefinedDomains(
		conn,
		inactiveDomains,
		numInactiveDomains
	);

	for (i=0; i<numActiveDomains; i++) {
		dom = virDomainLookupByID(conn, activeDomains[i]);
		printf(
			"  ID: %d, Name: %s (active)\n", 
			activeDomains[i], 
			virDomainGetName(dom)
		);
		virDomainGetInfo(dom, dominfo);
		printf("    state: %c\n", dominfo->state);
		printf("    maxMem: %ld\n", dominfo->maxMem);
		printf("    memory: %ld\n", dominfo->memory);
		printf("    nrVirtCpu: %d\n", dominfo->nrVirtCpu);
		printf("    cpuTime: %lld\n", dominfo->cpuTime);
	}

	// for (i=0; i<numActiveDomains; i++) {
		
	// }
	
	
	free(activeDomains);
	free(inactiveDomains);
	free(host);
	virConnectClose(conn);
	return 0;
}
