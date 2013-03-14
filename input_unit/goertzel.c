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
{0 ,  0 ,  1 ,  2 ,  2 ,  3 ,  4 ,  4 ,  5 ,  6 ,  7 ,  7 ,  8 ,  9 ,  9 ,  10 ,  11 ,  12 ,  12 ,  13 ,  14 ,  14 ,  15 ,  16 ,  16 ,  17 ,  18 ,  19 ,  19 ,  20 ,  21 ,  21 ,  22 ,  23 ,  24 ,  24 ,  25 ,  26 ,  26 ,  27 ,  28 ,  28 ,  29 ,  30 ,  31 ,  31 ,  32 ,  33 ,  33 ,  34 ,  35 ,  36 ,  36 ,  37 ,  38 ,  38 ,  39 ,  40 ,  41 ,  41 ,  42 ,  43 ,  43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  50 ,  51 ,  52 ,  53 ,  53 ,  54 ,  55 ,  55 ,  56 ,  57 ,  57 ,  58,  59 ,  60 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  65 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  70 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  77 ,  77 ,  78 ,  79 ,  79 ,  80 ,  81 ,  82 ,  82 ,  83 ,  84 ,  84 ,  85 ,  86 ,  86 ,  87 ,  88 ,  89 ,  89 ,  90 ,  91 ,  91 ,  92 ,  93 ,  94 ,  94 ,  95 ,  96 ,  96 ,  97 ,  98 ,  98 ,  99 ,  100 ,  101 ,  101 ,  102 ,  103 ,  103 ,  104 ,  105 ,  106 ,  106 ,  107 ,  108 ,  108 ,  109 ,  110 ,  111 ,  111 ,  112 ,  113 , 113 ,  114 ,  115 ,  115 ,  116 ,  117 ,  118 ,  118 ,  119 ,  120 ,  120 ,  121 ,  122 ,  123 ,  123 ,  124 ,  125 ,  125 ,  126 ,  127 ,  127 ,  128 ,  129 ,  130 ,  130 ,  131 ,  132 ,  132 ,  133 ,  134 ,  135 ,  135 ,  136 ,  137 ,  137 ,  138 ,  139 ,  140 ,  140 ,  141 ,  142 ,  142 ,  143 ,  144 ,  144 ,  145 ,  146 ,  147 ,  147 ,  148 ,  149 ,  149 ,  150 ,  151 ,  152 ,  152 ,  153 ,  154 ,  154 ,  155 ,  156 ,  156 ,  157 ,  158 ,  159 ,  159 ,  160 ,  161 ,  161 , 162 ,  163 ,  164 ,  164 ,  165 ,  166 ,  166 ,  167 ,  168 ,  168 ,  169 ,  170 ,  171 ,  171 ,  172 ,  173 ,  173 ,  174 ,  175 ,  176 ,  176 ,  177 ,  178 ,  178 ,  179}; 

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
    BYTE i, j, t, maxOverflow;
    sBYTE st;
    // determines which twiddle factors should be updated
    BYTE shouldUpdate = (BYTE)((samplesProcessed + 1) ^ samplesProcessed);
    // scale the input down if overflow was previously detected
    s = (sDWORD)(sample8bit >> scaleFactor);
    // F_s / 10
    if ((shouldUpdate & 0b00000001) && (twiddleUpdated[0] < N_SAMPLES)) {
        t = (q_1[0] >= 0) ? 0x0000 & coeff_mult[(BYTE)(q_1[0])] : \
                            0xFFFF & -coeff_mult[(BYTE)(-q_1[0])];
        q_0[0] = t*2 - q_2[0] + s;
        twiddleUpdated[0]++;
    }
    // F_s / 20
    if ((shouldUpdate & 0b00000010) && (twiddleUpdated[1] < N_SAMPLES)) {
        t = (q_1[1] >= 0) ? 0x0000 & coeff_mult[(BYTE)(q_1[1])] : \
                            0xFFFF & -coeff_mult[(BYTE)(-q_1[1])];
        q_0[1] = t*2 - q_2[1] + s;
        twiddleUpdated[1]++;
    }
    // F_s / 40
    if ((shouldUpdate & 0b00000100) && (twiddleUpdated[2] < N_SAMPLES)) {
        t = (q_1[2] >= 0) ? 0x0000 & coeff_mult[(BYTE)(q_1[2])] : \
                            0xFFFF & -coeff_mult[(BYTE)(-q_1[2])];
        q_0[2] = t*2 - q_2[2] + s;
        twiddleUpdated[2]++;
    }
    // F_s / 80
    if ((shouldUpdate & 0b00001000) && (twiddleUpdated[3] < N_SAMPLES)) {
        t = (q_1[3] >= 0) ? 0x0000 & coeff_mult[(BYTE)(q_1[3])] : \
                            0xFFFF & -coeff_mult[(BYTE)(-q_1[3])];
        q_0[3] = t*2 - q_2[3] + s;
        twiddleUpdated[3]++;
    }
    // F_s / 160
    if ((shouldUpdate & 0b00010000) && (twiddleUpdated[4] < N_SAMPLES)) {
        t = (q_1[4] >= 0) ? 0x0000 & coeff_mult[(BYTE)(q_1[4])] : \
                            0xFFFF & -coeff_mult[(BYTE)(-q_1[4])];
        q_0[4] = t*2 - q_2[4] + s;
        twiddleUpdated[4]++;
    }
    // F_s / 320
    if ((shouldUpdate & 0b00100000) && (twiddleUpdated[5] < N_SAMPLES)) {
        t = (q_1[5] >= 0) ? 0x0000 & coeff_mult[(BYTE)(q_1[5])] : \
                            0xFFFF & -coeff_mult[(BYTE)(-q_1[5])];
        q_0[5] = t*2 - q_2[5] + s;
        twiddleUpdated[5]++;
    }
    // F_s / 640
    if ((shouldUpdate & 0b01000000) && (twiddleUpdated[6] < N_SAMPLES)) {
        t = (q_1[6] >= 0) ? 0x0000 & coeff_mult[(BYTE)(q_1[6])] : \
                            0xFFFF & -coeff_mult[(BYTE)(-q_1[6])];
        q_0[6] = t*2 - q_2[6] + s;
        twiddleUpdated[6]++;
    }
    // F_s / 1280
    if ((shouldUpdate & 0b10000000) && (twiddleUpdated[7] < N_SAMPLES)) {
        t = (q_1[7] >= 0) ? 0x0000 & coeff_mult[(BYTE)(q_1[7])] : \
                            0xFFFF & -coeff_mult[(BYTE)(-q_1[7])];
        q_0[7] = t*2 - q_2[7] + s;
        twiddleUpdated[7]++;
    }
    // Check for overflow
    maxOverflow = 0;
    for (i=0; i<8; i++) {
        st = (sBYTE)((q_0[i] >> 8) & 0xFF);
        if (t < 0)
            st = -st;
        if (t > maxOverflow) {
            maxOverflow = st;
            scaleDecayCount = 0;
        }
    }
    // shift everything down by the overflow amount
    for (i=0; i<8; i++) {
        for (j=0; j<maxOverflow; j++) {
            q_0[i] >>= 1;
            scaleFactor++;
        }
    }
    // Update sample count
    samplesProcessed++;
    // Update older Q values
    for (i=0; i<8; i++) {
        q_1[i] = q_0[i];
        q_2[i] = q_1[i];
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
    sResults[0] = q_1[0]*q_1[0] - q_1[0]*q_2[0] + q_2[0]*q_2[0]; 
    sResults[1] = q_1[1]*q_1[1] + q_2[1]*q_2[1] + q_1[1]*q_2[1];
    sResults[2] = q_1[2]*q_1[2] + q_2[2]*q_2[2];
    t = (q_2[3] > 0) ? 0x0000 & coeff_mult[(BYTE)(q_2[3])] : \
                       0xFFFF & -coeff_mult(BYTE)(-q_2[3])];
    sResults[3] = q_1[3]*q_1[3] \
                 - q_1[3]*t\
                 + q_2[3]*q_2[3];
    t = (q_2[4] > 0) ? 0x0000 & coeff_mult[(BYTE)(q_2[4])] : \
                       0xFFFF & -coeff_mult[(BYTE)(-q_2[4])];
    sResults[4] = q_1[4]*q_1[4] \
                 - q_1[4]*t \
                 + q_2[4]*q_2[4];
    t = (q_2[5] > 0) ? 0x0000 & coeff_mult[(BYTE)(q_2[5])] : \
                       0xFFFF & -coeff_mult[(BYTE)(-q_2[5])];
    sResults[5] = q_1[5]*q_1[5] \
                 - q_1[5]*t \
                 + q_2[5]*q_2[5];
    t = (q_2[6] > 0) ? 0x0000 & coeff_mult[(BYTE)(q_2[6])] : \
                       0xFFFF & -coeff_mult[(BYTE)(-q_2[6])];
    sResults[6] = q_1[6]*q_1[6] \
                 - q_1[6]*t \
                 + q_2[6]*q_2[6];
    t = (q_2[7] > 0) ? 0x0000 & coeff_mult[(BYTE)(q_2[7])] : \
                       0xFFFF & -coeff_mult[(BYTE)(-q_2[7])];
    sResults[7] = q_1[7]*q_1[7] \
                 - q_1[7]*t \
                 + q_2[7]*q_2[7];
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
    
