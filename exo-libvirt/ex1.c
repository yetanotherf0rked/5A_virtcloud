/* example ex1.c */
/* Utilisation de vitConnectOpen pour ouvrir une connexion vers un hyperviseur */
/* compile with: gcc -g -Wall ex1.c -o ex -lvirt */
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
	printf("Connexion opened\n");
	virConnectClose(conn);
	return 0;
}
