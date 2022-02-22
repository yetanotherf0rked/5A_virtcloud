#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

void domainDisplay(
		int numDomains,
		virDomainPtr* domains,
		virDomainInfo domInfo
	){
		for (int i=0; i<numDomains; i++) {
			virDomainGetInfo(domains[i], &domInfo);
			printf(" \tid : %i, name : %s\n \t\tstate : %d\n \t\tmaxMem : %li\n \t\tmemory : %li\n \t\tnrvirtcpu : %d\n \t\tcpuTime : %lli\n\n ",
					virDomainGetID(domains[i]),
					virDomainGetName(domains[i]),
					domInfo.state,
					virDomainGetMaxMemory(domains[i]),
					domInfo.memory,
					domInfo.nrVirtCpu,
					domInfo.cpuTime
			);
	}
}

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
	virDomainPtr *domains;
	virDomainInfo domInfo;
	
	// Active Domains
	int activeDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_ACTIVE);
	
	printf("====== ACTIVE DOMAIN IDS\n");
	if (activeDomains < 0)
		printf("No active domain\n");
	else
		domainDisplay(activeDomains, domains, domInfo);
	
	free(domains);

	// Inactive Domains
	int inactiveDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_INACTIVE);

	printf("====== INACTIVE DOMAIN IDS\n");
	if (inactiveDomains < 0)
		printf("No inactive domain \n");
	else
		domainDisplay(inactiveDomains, domains, domInfo);

	free(domains);


	// Exercice 5
	virDomainPtr vm = NULL;
	activeDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_ACTIVE);
	if (activeDomains <= 0)
		printf("No active domain to shutdown/restart\n");
	else {
		vm = domains[0];
		printf("Shutdown domain\n");
		virDomainDestroy(vm);
		domainDisplay(activeDomains, domains, domInfo);

		printf("Restart domain\n");
		virDomainCreate(vm);
		domainDisplay(activeDomains, domains, domInfo);

		free(domains);
		vm = NULL;
	}

	// Exercice 6
	const char* target = "qemu+ssh://172.30.3.39/system";
	virConnectPtr targetConn = virConnectOpenAuth(target, virConnectAuthPtrDefault, 0);

	// On migre la première vm active
	activeDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_ACTIVE);

	if (activeDomains <= 0)
		printf("No active domain to shutdown/restart\n");
	else {
		vm = domains[0];
		virDomainMigrate3(vm,targetConn, NULL, 0, VIR_MIGRATE_UNSAFE);
	}
	
	free(host);
	virConnectClose(conn);
	return 0;
}
