
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deus.h"
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char** argv) {
	
	int f = open("/dev/deus-ctrl", O_RDWR);
	return 0;
}
