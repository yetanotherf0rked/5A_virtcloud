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
/* compile with: gcc -g -Wall ex3.c -o ex3 -lvirt */
#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

int main(int argc, char *argv[])
{
	virConnectPtr conn;

	conn = virConnectOpenAuth("qemu+tcp://localhost/system", virConnectAuthPtrDefault, 0);
	if (conn == NULL) {
		fprintf(stderr, "Failed to open connection to qemu+tcp://localhost/system\n");
		return 1;
	}
	virConnectClose(conn);
	return 0;
}
