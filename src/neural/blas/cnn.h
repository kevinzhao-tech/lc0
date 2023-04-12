#include <unistd.h>

#ifndef CNN_H
#define CNN_H

#ifdef __cplusplus
extern "C" {
#endif

#define H2F_AXI_MASTER_BASE   0xC0000000

// CNN Accelerator
#define CNN_BASE    (H2F_AXI_MASTER_BASE + 0)
#define CNN_SPAN    0x4440   

// offset is 0x4400 in Qsys
// 0x4400 = 17408 bytes = 544 words

// shifting right 2 times is the same as divide by 4 bytes (word size)
#define BYTE_TO_WORD(X) (X >> 2U)

#define ACCEL_START_OFFSET          BYTE_TO_WORD(0x4400)+0
#define ACCEL_MEM_OFFSET           BYTE_TO_WORD(0x4400)+1
#define ACCEL_NEW_WTS_OFFSET        BYTE_TO_WORD(0x4400)+2
#define ACCEL_OUTPUT_ADDR_OFFSET    BYTE_TO_WORD(0x4400)+3 // currently not used

// offset is 0x2000 in Qsys
#define ACCEL_IACT_OFFSET           BYTE_TO_WORD(0x2000)
#define ACCEL_OACT_OFFSET           BYTE_TO_WORD(0x0000)
#define ACCEL_WT_OFFSET             BYTE_TO_WORD(0x4000)

/**
 * @brief Holds the SA parameters such as its base address etc.
 * 
 */
typedef struct {
    volatile unsigned int* CNN_base_ptr;
} cnn_state;

cnn_state *cnn_setup();

/**
 * @brief Load wts into SA, must be size 4, at given offset
 * 
 * @param cnn the cnn_state returned by cnn_setup
 * @param wts source
 * @param offset offset within the accelerator
 */
void cnn_load_wts(cnn_state *cnn, const float *wts, size_t offset);

/**
 * @brief Load iacts into SA, must be size 4, at given offset
 * 
 * @param cnn the cnn_state returned by cnn_setup
 * @param iacts source
 * @param offset offset within the accelerator
 */
void cnn_load_iacts(cnn_state *cnn, const float *iacts, size_t offset);

/**
 * @brief Retrieve oacts from SA
 * 
 * @param cnn the cnn_state returned by cnn_setup
 * @param oacts destination
 * @param offset offset within the accelerator
 */
void cnn_get_oacts(cnn_state *cnn, float *oacts, size_t offset);

/**
 * @brief Start compute
 * 
 * @param cnn the cnn_state returned by cnn_setup
 */
void cnn_compute(cnn_state *cnn);

/**
 * @brief blocking call, stalls if the compute is not complete yet
 * 
 * @param cnn the cnn_state returned by cnn_setup
 */
void cnn_compute_iscomplete(cnn_state *cnn);

/**
 * @brief set the offset within the SRAM block in which the SA should use
 * 
 * @param cnn the cnn_state returned by cnn_setup
 * @param offset offset within the accelerator
 */
void cnn_set_compute_offset(cnn_state *cnn, size_t offset);

/**
 * @brief Initialize the wts, must call cnn_set_compute_offset first, if the new offset is different
 * 
 * @param cnn the cnn_state returned by cnn_setup
 */
void cnn_init_wts(cnn_state *cnn);


#ifdef __cplusplus
}
#endif

#endif