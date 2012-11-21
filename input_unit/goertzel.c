/*
 *
 *
 *
 */

#include "goertzel.h"

void goertzel_process_sample(BYTE sample8bit) {
    DWORD s = (DWORD)sample8bit;
    BYTE i;
    // 16kHz
    q_0[0] = -q_1[0] - q_1[0] - q_2[0] + s;
    // 8kHz
    q_0[1] = -q_2[1] + s;
    // 4kHz
    q_0[2] = coeff_mult[0][q_1[2]]*2 - q_2[2] + s;
    // 2kHz
    q_0[3] = coeff_mult[1][q_1[3]]*2 - q_2[3] + s;
    // 1kHz
    q_0[4] = coeff_mult[2][q_1[4]]*2 - q_2[4] + s;
    // 500Hz
    q_0[5] = coeff_mult[3][q_1[5]]*2 - q_2[5] + s;
    // 250Hz
    q_0[6] = coeff_mult[4][q_1[6]]*2 - q_2[6] + s;
    // 125Hz
    q_0[7] = q_1[7]*2 - q_2[7] + s;
    // Update older Q values
    for (i=0; i<8; i++) {
        q_1[i] = q_0[i];
        q_2[i] = q_1[i];
    }
    if (++samplesProcessed >= N_SAMPLES) {
        goertzelReady = 1;
        samplesProcessed = 0;
    }
}

BYTE goertzel_is_ready(void) {
    return goertzelReady;
}

void goertzel_process_magnitudes(DWORD* results) {
    BYTE i;
    // level out the values over 255 and under -255
    for (i=0; i<8; i++) {
        if (0x8000 & q_0[i])
            q_0[i] |= 0xFF00;
        else
            q_0[i] &= 0x00FF;
        if (0x8000 & q_1[i])
            q_1[i] |= 0xFF00;
        else
            q_1[i] &= 0x00FF;
        if (0x8000 & q_2[i])
            q_2[i] |= 0xFF00;
        else
            q_2[i] &= 0x00FF;
    }        
    // calculate squared magnitudes
    for (i=0; i<8; i++0) {
        results[i] = q_1[i]*q_1[i] + q_2[i]*q_2[i] - q_1*coeff_mult[q_2];
    }
    // clean up for next run
    for (i=0; i<8; i++) {
        q_0[i] = 0;
        q_1[i] = 0;
        q_2[i] = 0;
    }
    goertzelReady = 0;
}
