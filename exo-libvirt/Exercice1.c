/*
 *
 *
 * To test the above program, the following configuration must be present:
 * /etc/libvirt/libvirtd.conf
 * listen_tls = 0
 * listen_tcp = 1
 * auth_tcp = "sasl"
 * /etc/sasl2/libvirt.conf
 * mech_list: digest-md5
 * A virt user has been added to the SASL database:
 * # saslpasswd2 -a libvirt virt # this will prompt for a password
 * libvirtd has been started with --listen
 */

/* example ex3.c */
/* compile with: gcc -g -Wall Exercice1.c -o Exercice1 -lvirt */
#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

void domainsDisplay(virConnectPtr conn, virDomainInfo info, virDomainPtr *domains){
	size_t i;
	int numDomains;
	//On affiche les VMs actives
	numDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_ACTIVE);
	if (numDomains < 0)
    	printf("Erreur\n");
    printf("------------\nActive Domain Ids\n------------\n");

    for (i = 0; i < numDomains; i++) {
    	virDomainGetInfo(domains[i],&info);
    	printf("Id : %i - ",virDomainGetID(domains[i]));
     	printf("Name : %s - \n",virDomainGetName(domains[i]));
     	printf("State : %c\n",info.state);
     	printf("MaxMem : %li\n",virDomainGetMaxMemory(domains[i]));
     	printf("Memory : %li\n",info.memory);
     	printf("Number of virtual CPUs : %d\n",info.nrVirtCpu);
     	printf("CPU time : %lli\n",info.cpuTime);
     	printf("\n");
     }
    if (numDomains == 0){
    	printf("Aucun domaine actif \n");
    }
    free(domains);
//On affiche les VM inactives
	numDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_INACTIVE);
	if (numDomains < 0)
    	printf("Erreur \n");
    printf("------------\nInactive Domain Ids\n------------\n");
    for (i = 0; i < numDomains; i++) {
    	virDomainGetInfo(domains[i],&info);
    	printf("Id : %i - ",virDomainGetID(domains[i]));
     	printf("Name : %s - \n",virDomainGetName(domains[i]));
     	printf("State : %c\n",info.state);
     	printf("MaxMem : %li\n",virDomainGetMaxMemory(domains[i]));
     	printf("Memory : %li\n",info.memory);
     	printf("Number of virtual CPUs : %d\n",info.nrVirtCpu);
     	printf("CPU time : %lli\n",info.cpuTime);
     	printf("\n");
     }
    if (numDomains == 0){
   		printf("Aucun domaine inactif \n");
    }
	free(domains);
}

int main(int argc, char *argv[])
{	
	//Exo 1 : Ouvrir un hyperviseur distant
	virConnectPtr conn;
	int vcpus;
	unsigned long long node_free_memory;
	char *host;

	conn = virConnectOpenAuth("qemu+ssh://localhost/system", virConnectAuthPtrDefault, 0);
	if (conn == NULL) {
		fprintf(stderr, "Failed to open connection to qemu+ssh://localhost/system\n");
		return 1;
	}

	//Exo 2 : Hostname, CPU dispo, mémoire dispo
	host = virConnectGetHostname(conn);
	fprintf(stdout, "Hostname:%s\n", host);
	free(host);
	vcpus = virConnectGetMaxVcpus(conn, NULL);
	fprintf(stdout, "Maximum support virtual CPUs: %d\n", vcpus);
	node_free_memory = virNodeGetFreeMemory(conn);
	fprintf(stdout, "Node free memory: %llu\n", node_free_memory);


	//Exo 3 : Afficher les domaines actifs et inactifs
	// On utilisera ensuite la fonction domainsDisplay
	virDomainInfo info;
	virDomainPtr *domains;
	size_t i;
	int numDomains;
	int noRunning = 0;
	// Domaines actifs.
	numDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_ACTIVE);
	if (numDomains < 0)
    	printf("Erreur\n");
    printf("\n------------\nActive Domain Ids\n------------\n");

    for (i = 0; i < numDomains; i++) {
    	virDomainGetInfo(domains[i],&info);
    	printf("Id : %i - ",virDomainGetID(domains[i]));
     	printf("Name : %s - \n",virDomainGetName(domains[i]));
     	printf("State : %c\n",info.state);
     	printf("MaxMem : %li\n",virDomainGetMaxMemory(domains[i]));
     	printf("Memory : %li\n",info.memory);
     	printf("Number of virtual CPUs : %d\n",info.nrVirtCpu);
     	printf("CPU time : %lli\n",info.cpuTime);
     	printf("\n\n");
     }
    if (numDomains == 0){
    	printf("Aucun domaine actif \n");
    	noRunning = 1;
    }

	free(domains);
	//Domaines inactifs
	numDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_INACTIVE);
	if (numDomains < 0)
    	printf("Erreur \n");
    printf("\n------------\nInactive Domain Ids\n------------\n");
    for (i = 0; i < numDomains; i++) {
    	virDomainGetInfo(domains[i],&info);
    	printf("Id : %i - ",virDomainGetID(domains[i]));
     	printf("Name : %s - \n",virDomainGetName(domains[i]));
     	printf("State : %c\n",info.state);
     	printf("MaxMem : %li\n",virDomainGetMaxMemory(domains[i]));
     	printf("Memory : %li\n",info.memory);
     	printf("Number of virtual CPUs : %d\n",info.nrVirtCpu);
     	printf("CPU time : %lli\n",info.cpuTime);
     	printf("\n\n");
     }
    if (numDomains == 0){
   		printf("Aucun domaine inactif \n");
    }
    //Si besoin, on lance une VM pour être sûr qu'il y en ait une d'active pour la suite du code
    if(noRunning){
    	printf(">>> Starting %s so that the rest of the program runs well...\n",virDomainGetName(domains[0]));
    	virDomainCreate(domains[0]);
    }
	free(domains);


	//Exercice 5 : Arrêter puis redémarrer une VM
	//On récupère en paramètre une VM active
	numDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_ACTIVE);
	virDomainPtr vm = domains[0];

	//On éteint cette VM
	printf(">>> Shutting down the first active domain... (%s)\n",virDomainGetName(vm));
	virDomainDestroy(vm);
	free(domains);

	domainsDisplay(conn,info,domains);

	//On redémarre la VM
	printf(">>> Restarting %s...\n",virDomainGetName(vm));
	virDomainCreate(vm);

	domainsDisplay(conn,info,domains);

	//Exercice 6 : Suspendre une VM en l'état.
	printf(">>> Suspending %s\n",virDomainGetName(vm));
	virDomainSave(vm,"/var/lib/libvirt/qemu/save/saveVM");

	domainsDisplay(conn,info,domains);

	printf(">>> Restoring %s\n", virDomainGetName(vm));
	virDomainRestore(conn,"/var/lib/libvirt/qemu/save/saveVM");

	domainsDisplay(conn,info,domains);

	//Exercice 6 : Migrer une VM chez le voisin
	const char* adresseVoisin = "qemu+ssh://172.30.3.31/system";
	virConnectPtr connVoisin;
	connVoisin = virConnectOpenAuth(adresseVoisin, virConnectAuthPtrDefault, 0);

	//On récupère un pointeur sur une VM active, sinon on en allume une :
	numDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_ACTIVE);
	if (numDomains == 0){
    	numDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_INACTIVE);
		if (numDomains < 0)
    		printf("Erreur \n");
    	virDomainCreate(domains[0]);
    	numDomains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_ACTIVE);
		vm = domains[0];
	}
	else
		vm = domains[0];

	virDomainMigrate3(vm,connVoisin, NULL, 0, VIR_MIGRATE_UNSAFE);

	virConnectClose(conn);
	return 0;
}
