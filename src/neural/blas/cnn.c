#include "cnn.h"
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/mman.h>
#include <sys/shm.h> 

cnn_state *cnn_setup() {
    cnn_state *cnn = malloc(sizeof(cnn_state));

    volatile void* CNN_virtual_base;

    // ACCEL parameter addr
    int fd;	

	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) 	{
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( NULL );
	}

    CNN_virtual_base = mmap( NULL, CNN_SPAN, (PROT_READ | PROT_WRITE ), MAP_SHARED, fd, CNN_BASE);
    if (CNN_virtual_base == MAP_FAILED) {
        printf("ERROR: mmap() for CNN failed...\n");
        close( fd );
        return NULL;
    }
    // CNN registers & memory
    cnn->CNN_base_ptr = (unsigned int*)(CNN_virtual_base);

    return cnn;
}

void cnn_load_wts(cnn_state *cnn, const float *wts, size_t offset) {
	*(cnn->CNN_base_ptr + ACCEL_NEW_WTS_OFFSET) = 1U;

    size_t size = 4;
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; ++j) {
			// printf("Writing to :%x\n", CNN_base_ptr + ACCEL_WT_OFFSET + i*size + j);
			*((volatile float*) (cnn->CNN_base_ptr + ACCEL_WT_OFFSET + j*size + i)) = *(wts + i*size + j);
		}
	}
	*(cnn->CNN_base_ptr + ACCEL_START_OFFSET) = 1U;
	*(cnn->CNN_base_ptr + ACCEL_NEW_WTS_OFFSET) = 0U;
}

void cnn_load_iacts(cnn_state *cnn, const float *iacts, size_t offset) {
    size_t size = 4;
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; ++j) {
			// printf("Writing to :%x\n", CNN_base_ptr + ACCEL_WT_OFFSET + i*size + j);
            *((volatile float*) (cnn->CNN_base_ptr + ACCEL_IACT_OFFSET + i*size + j)) = *(iacts + i*size + j);
        }
    }
}

void cnn_get_oacts(cnn_state *cnn, float *oacts, size_t offset) {
	// read oact
    size_t size = 4;
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; ++j) {
			float elem = *((volatile float*) (cnn->CNN_base_ptr + ACCEL_OACT_OFFSET + i*size + j));
            *(oacts + i*size + j) = elem;
		}
	}
}

void cnn_compute(cnn_state *cnn) {
	// start
	*(cnn->CNN_base_ptr + ACCEL_START_OFFSET) = 1;
}

void cnn_compute_iscomplete(cnn_state *cnn) {
    // make modification to rtl so that reads will deassert slave_waitrequest if in READY state
	int result = *(cnn->CNN_base_ptr + ACCEL_START_OFFSET);
}

void cnn_set_compute_offset(cnn_state *cnn, size_t offset) {
	*(cnn->CNN_base_ptr + ACCEL_MEM_OFFSET) = offset;
}

void cnn_init_wts(cnn_state *cnn) {
	// transfer weights
	*(cnn->CNN_base_ptr + ACCEL_START_OFFSET) = 0U;

	// clear new_wts
	*(cnn->CNN_base_ptr + ACCEL_NEW_WTS_OFFSET) = 0U;
}