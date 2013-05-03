/*
 * goertzel.c
 * Fixed-point implementation of the Goertzel frequency-finding algorithm.
 * Strange stuff abound, see each function for explanation.
 * by William Moy
 */

#include "common.h"
#include "goertzel.h"
#include "types.h"

#define SCALE_DECAY_TICK 100
#define N_SAMPLES 20

// how many samples have been processed in the current run
static DWORD samplesProcessed = 0;
// boolean describing if all the samples have been processed and is ready
// for magnitude calculation
static BYTE goertzelReady = 0;
// scale factor for shifting an overflowed 8-bit number
static BYTE scaleFactor = 0;
// delay counter that waits for SCALE_DECAY_TICK magnitudes until it
// decrements the scale.  hopefully this will allows us to see better
// decibel ranges.
static BYTE scaleDecayCount = 0;

// variables kept over each sample iteration
static sDWORD q_0[8] = {0};
static sDWORD q_1[8] = {0};
static sDWORD q_2[8] = {0};
// counts the number of times each variable (twiddle factor) is updated
static BYTE twiddleUpdated[8] = {0};

// Multiplication table for fast lookups.  Imitates coeff * value.
static const BYTE coeff_mult[256] PROGMEM = \
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243}; 

/** 
 * goertzel_process_sample
 * Performs one iteration of the Goertzel algorithm.  This is done for each
 * sample from 1 to N.  Updates the Q_x values with each iteration.  Some
 * speedups and tricks are used here.  A multiplication lookup table is used to
 * remove any actual multiplication operations.
 * All coefficients use the same m/N ratio, so only one multiplication lookup
 * table is needed.
 * Although the actual sampling frequency remains constant, only samples that
 * are needed are used.  By taking every 16th sample, for example, we can
 * effectively divide the sampling frequency by 16.
 * args:
 *  sample8bit: one 8-bit sample of the signal
 * returns:
 *  none
 */
void goertzel_process_sample(BYTE sample8bit) {
    sDWORD s;
    BYTE i,  t, maxOverflow;
    sBYTE st;
    // mask that determines which twiddle factors should be updated.
    // 0th bit should change every time, 4th bit should change every
    // 16th time, etc.
    BYTE shouldUpdate = (BYTE)((samplesProcessed + 1) ^ samplesProcessed);
    BYTE freqMask = 1;
    // scale the input down if overflow was previously detected
    s = (sDWORD)(sample8bit >> scaleFactor);
    // F_s / (10*2^i)
    for (i=0; i<8; i++) {
        // only update if shouldUpdate bit got switched on && not done sampling
        if ((shouldUpdate & freqMask) && (twiddleUpdated[i] < N_SAMPLES)) {
            t = (q_1[i] >= 0) ? 0x0000 & coeff_mult[(BYTE)(q_1[i])] : \
                                0xFF00 | -coeff_mult[(BYTE)(-q_1[i])];
            q_0[i] = t*2 - q_2[i] + s;
            twiddleUpdated[i]++;
        }
        freqMask <<= 1;
    }
    // Check for overflow
    maxOverflow = 0;
    for (i=0; i<8; i++) {
        st = (sBYTE)(q_0[i] >> 8);
        if (st < 0)
            st = ~st; // invert, NOT -
        if (st > maxOverflow) {
            maxOverflow = st;
            scaleDecayCount = 0;
        }
    }
    // shift everything down by the overflow amount
    for (i=0; i<8; i++)
        q_0[i] >>= maxOverflow;
    // add the overflow amount to the static scale factor
    scaleFactor += maxOverflow;
    // Update sample count
    samplesProcessed++;
    // Update older Q values
    for (i=0; i<8; i++) {
        q_2[i] = q_1[i];
        q_1[i] = q_0[i];
    }
    // if the last needed sample is done
    if (twiddleUpdated[7] >= N_SAMPLES) { 
        goertzelReady = 1;
        samplesProcessed = 0;
        for (i=0; i<8; i++)
            twiddleUpdated[i] = 0; // clear update count
        if (++scaleDecayCount < SCALE_DECAY_TICK)
            ;// keep the scaleFactor
        else
            --scaleFactor;
    }
}

/** 
 * goertzel_is_ready
 * Indicates if all the individual samples have been processed
 * args:
 *  none
 * returns:
 *  BYTE: 1 if ready, 0 if not
 */
BYTE goertzel_is_ready(void) {
    return goertzelReady;
}

/** 
 * goertzel_process_magnitudes
 * Calculates the squared magnitude of each target frequency.
 * args:
 *  results: a 16-bit array with 8 values to store the result of the
 *  computation
 * returns:
 *  none
 */
void goertzel_process_magnitudes(DWORD* results) {
    BYTE i;
    sDWORD t;
    sDWORD* sResults = (sDWORD*)results;
    // calculate squared magnitudes
    // TODO: overflow checking?
    for (i=0; i<8; i++) {
        t = (q_2[i] > 0) ? 0x0000 & coeff_mult[(BYTE)(q_2[i])] : \
                           0xFFFF & -coeff_mult[(BYTE)(-q_2[i])];
        sResults[i] = q_1[i]*q_1[i] - q_1[i]*t + q_2[i]*q_2[i];
    }
    // clean up for next run
    for (i=0; i<8; i++) {
        q_0[i] = 0;
        q_1[i] = 0;
        q_2[i] = 0;
    }
    goertzelReady = 0;
}

void goertzel_reset(void) {
    BYTE i;
    for (i=0; i<8; i++) {
        q_0[i] = 0;
        q_1[i] = 0;
        q_2[i] = 0;
    }
    goertzelReady = 0;
    samplesProcessed = 0;
    scaleFactor = 0;
}
    
