/* 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include <unistd.h>

#include <sys/disk.h>

//TODO: this should be rewritten as OR of individual defines from sys/fsctl.h
#define RESIZE_PARTITION 0x80086802

int main(int argc, char *argv[]) {
	
	if ( argc != 3 ) {
		printf("Usage: %s <path to filesystem> <size in bytes>\n", argv[0]);
		exit(-1);
	};
	
	char *device = "/dev/rdisk0s1";	
	int fd = open(device, O_RDWR | O_SHLOCK);

	if ( fd < 0 ) {
		printf("[-] Couldn't open disk %s.\n", device);
		exit(-1);
	};

	uint32_t blocksize = 0;
	if ( ioctl(fd, DKIOCGETBLOCKSIZE, &blocksize) != 0 ) {
		printf("[-] ioctl DKIOCGETBLOCKSIZE failed.\n");
		close(fd);
		exit(-1);
	};
	
	close(fd);

	uint64_t required_size = atoll(argv[2]);

	uint64_t mod;
	if ( (mod = required_size % blocksize) != 0 ) {
		printf("[-] Required size has to be multiple of blocksize (%u).\n", blocksize);
		required_size = required_size - mod + (uint64_t) blocksize;
		printf("[i] Adjusting size to %llu to match next block.\n", required_size);
	}

	
	printf("Resizing %s to %llu bytes.\n", argv[1], required_size);
	
	int err;
	if ( ( err = fsctl(argv[1], RESIZE_PARTITION, &required_size, 0)) != 0) {
		printf("[-] HFS resize failed. errno=%i\n", err);
	};

	return 0;
}

// vim:ft=c
