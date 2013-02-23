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
#define N_SAMPLES 256

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
static BYTE scaleDelayCount = 0;

// variables kept over each sample iteration
static sDWORD q_0[8] = {0};
static sDWORD q_1[8] = {0};
static sDWORD q_2[8] = {0};

// Multiplication table for fast lookups.  Imitates coeff * value.
// The first dimension is a separate case for each distinct coefficient
static const BYTE coeff_mult[5][256] PROGMEM = \
{ {0 ,  0 ,  1 ,  2 ,  2 ,  3 ,  4 ,  4 ,  5 ,  6 ,  7 ,  7 ,  8 ,  9 ,  9 ,  10 ,  11 ,  12 ,  12 ,  13 ,  14 ,  14 ,  15 ,  16 ,  16 ,  17 ,  18 ,  19 ,  19 ,  20 ,  21 ,  21 ,  22 ,  23 ,  24 ,  24 ,  25 ,  26 ,  26 ,  27 ,  28 ,  28 ,  29 ,  30 ,  31 ,  31 ,  32 ,  33 ,  33 ,  34 ,  35 ,  36 ,  36 ,  37 ,  38 ,  38 ,  39 ,  40 ,  41 ,  41 ,  42 ,  43 ,  43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  50 ,  51 ,  52 ,  53 ,  53 ,  54 ,  55 ,  55 ,  56 ,  57 ,  57 ,  58 ,  59 ,  60 ,  60 ,  61 ,  62 ,  62 ,  63 ,  64 ,  65 ,  65 ,  66 ,  67 ,  67 ,  68 ,  69 ,  70 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  77 ,  77 ,  78 ,  79 ,  79 ,  80 ,  81 ,  82 ,  82 ,  83 ,  84 ,  84 ,  85 ,  86 ,  86 ,  87 ,  88 ,  89 ,  89 ,  90 ,  91 ,  91 ,  92 ,  93 ,  94 ,  94 ,  95 ,  96 ,  96 ,  97 ,  98 ,  98 ,  99 ,  100 ,  101 ,  101 ,  102 ,  103 ,  103 ,  104 ,  105 ,  106 ,  106 ,  107 ,  108 ,  108 ,  109 ,  110 ,  111 ,  111 ,  112 ,  113 ,  113 ,  114 ,  115 ,  115 ,  116 ,  117 ,  118 ,  118 ,  119 ,  120 ,  120 ,  121 ,  122 ,  123 ,  123 ,  124 ,  125 ,  125 ,  126 ,  127 ,  127 ,  128 ,  129 ,  130 ,  130 ,  131 ,  132 ,  132 ,  133 ,  134 ,  135 ,  135 ,  136 ,  137 ,  137 ,  138 ,  139 ,  140 ,  140 ,  141 ,  142 ,  142 ,  143 ,  144 ,  144 ,  145 ,  146 ,  147 ,  147 ,  148 ,  149 ,  149 ,  150 ,  151 ,  152 ,  152 ,  153 ,  154 ,  154 ,  155 ,  156 ,  156 ,  157 ,  158 ,  159 ,  159 ,  160 ,  161 ,  161 ,  162 ,  163 ,  164 ,  164 ,  165 ,  166 ,  166 ,  167 ,  168 ,  168 ,  169 ,  170 ,  171 ,  171 ,  172 ,  173 ,  173 ,  174 ,  175 ,  176 ,  176 ,  177 ,  178 ,  178 ,  179}, 
  {0 ,  0 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  10 ,  11 ,  12 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  75 ,  76 ,  77 ,  78 ,  79 ,  80 ,  81 ,  82 ,  83 ,  84 ,  84 ,  85 ,  86 ,  87 ,  88 ,  89 ,  90 ,  91 ,  92 ,  93 ,  94 ,  95 ,  96 ,  97 ,  97 ,  98 ,  99 ,  100 ,  101 ,  102 ,  103 ,  104 ,  105 ,  106 ,  107 ,  108 ,  109 ,  109 ,  110 ,  111 ,  112 ,  113 ,  114 ,  115 ,  116 ,  117 ,  118 ,  119 ,  120 ,  121 ,  121 ,  122 ,  123 ,  124 ,  125 ,  126 ,  127 ,  128 ,  129 ,  130 ,  131 ,  132 ,  133 ,  133 ,  134 ,  135 ,  136 ,  137 ,  138 ,  139 ,  140 ,  141 ,  142 ,  143 ,  144 ,  145 ,  145 ,  146 ,  147 ,  148 ,  149 ,  150 ,  151 ,  152 ,  153 ,  154 ,  155 ,  156 ,  157 ,  157 ,  158 ,  159 ,  160 ,  161 ,  162 ,  163 ,  164 ,  165 ,  166 ,  167 ,  168 ,  169 ,  169 ,  170 ,  171 ,  172 ,  173 ,  174 ,  175 ,  176 ,  177 ,  178 ,  179 ,  180 ,  181 ,  182 ,  182 ,  183 ,  184 ,  185 ,  186 ,  187 ,  188 ,  189 ,  190 ,  191 ,  192 ,  193 ,  194 ,  194 ,  195 ,  196 ,  197 ,  198 ,  199 ,  200 ,  201 ,  202 ,  203 ,  204 ,  205 ,  206 ,  206 ,  207 ,  208 ,  209 ,  210 ,  211 ,  212 ,  213 ,  214 ,  215 ,  216 ,  217 ,  218 ,  218 ,  219 ,  220 ,  221 ,  222 ,  223 ,  224 ,  225 ,  226 ,  227 ,  228 ,  229 ,  230 ,  230 ,  231 ,  232 ,  233 ,  234}, 
  {0 ,  0 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 ,  70 ,  71 ,  72 ,  73 ,  74 ,  75 ,  76 ,  77 ,  78 ,  79 ,  80 ,  81 ,  82 ,  83 ,  84 ,  85 ,  86 ,  87 ,  88 ,  89 ,  90 ,  91 ,  92 ,  93 ,  94 ,  95 ,  96 ,  97 ,  98 ,  99 ,  100 ,  101 ,  102 ,  102 ,  103 ,  104 ,  105 ,  106 ,  107 ,  108 ,  109 ,  110 ,  111 ,  112 ,  113 ,  114 ,  115 ,  116 ,  117 ,  118 ,  119 ,  120 ,  121 ,  122 ,  123 ,  124 ,  125 ,  126 ,  127 ,  128 ,  129 ,  130 ,  131 ,  132 ,  133 ,  134 ,  135 ,  136 ,  137 ,  138 ,  139 ,  140 ,  141 ,  142 ,  143 ,  144 ,  145 ,  146 ,  147 ,  148 ,  149 ,  150 ,  151 ,  152 ,  153 ,  153 ,  154 ,  155 ,  156 ,  157 ,  158 ,  159 ,  160 ,  161 ,  162 ,  163 ,  164 ,  165 ,  166 ,  167 ,  168 ,  169 ,  170 ,  171 ,  172 ,  173 ,  174 ,  175 ,  176 ,  177 ,  178 ,  179 ,  180 ,  181 ,  182 ,  183 ,  184 ,  185 ,  186 ,  187 ,  188 ,  189 ,  190 ,  191 ,  192 ,  193 ,  194 ,  195 ,  196 ,  197 ,  198 ,  199 ,  200 ,  201 ,  202 ,  203 ,  204 ,  204 ,  205 ,  206 ,  207 ,  208 ,  209 ,  210 ,  211 ,  212 ,  213 ,  214 ,  215 ,  216 ,  217 ,  218 ,  219 ,  220 ,  221 ,  222 ,  223 ,  224 ,  225 ,  226 ,  227 ,  228 ,  229 ,  230 ,  231 ,  232 ,  233 ,  234 ,  235 ,  236 ,  237 ,  238 ,  239 ,  240 ,  241 ,  242 ,  243 ,  244 ,  245 ,  246 ,  247 ,  248 ,  249}, 
  {0 ,  0 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 ,  70 ,  71 ,  72 ,  73 ,  74 ,  75 ,  76 ,  77 ,  78 ,  79 ,  80 ,  81 ,  82 ,  83 ,  84 ,  85 ,  86 ,  87 ,  88 ,  89 ,  90 ,  91 ,  92 ,  93 ,  94 ,  95 ,  96 ,  97 ,  98 ,  99 ,  100 ,  101 ,  102 ,  103 ,  104 ,  105 ,  106 ,  107 ,  108 ,  109 ,  110 ,  111 ,  112 ,  113 ,  114 ,  115 ,  116 ,  117 ,  118 ,  119 ,  120 ,  121 ,  122 ,  123 ,  124 ,  125 ,  126 ,  127 ,  128 ,  129 ,  130 ,  131 ,  132 ,  133 ,  134 ,  135 ,  136 ,  137 ,  138 ,  139 ,  140 ,  141 ,  142 ,  143 ,  144 ,  145 ,  146 ,  147 ,  148 ,  149 ,  150 ,  151 ,  152 ,  153 ,  154 ,  155 ,  156 ,  157 ,  158 ,  159 ,  160 ,  161 ,  162 ,  163 ,  164 ,  165 ,  166 ,  167 ,  168 ,  169 ,  170 ,  171 ,  172 ,  173 ,  174 ,  175 ,  176 ,  177 ,  178 ,  179 ,  180 ,  181 ,  182 ,  183 ,  184 ,  185 ,  186 ,  187 ,  188 ,  189 ,  190 ,  191 ,  192 ,  193 ,  194 ,  195 ,  196 ,  197 ,  198 ,  199 ,  200 ,  201 ,  202 ,  203 ,  204 ,  205 ,  206 ,  206 ,  207 ,  208 ,  209 ,  210 ,  211 ,  212 ,  213 ,  214 ,  215 ,  216 ,  217 ,  218 ,  219 ,  220 ,  221 ,  222 ,  223 ,  224 ,  225 ,  226 ,  227 ,  228 ,  229 ,  230 ,  231 ,  232 ,  233 ,  234 ,  235 ,  236 ,  237 ,  238 ,  239 ,  240 ,  241 ,  242 ,  243 ,  244 ,  245 ,  246 ,  247 ,  248 ,  249 ,  250 ,  251 ,  252}, 
  {0 ,  0 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 ,  38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,  64 ,  65 ,  66 ,  67 ,  68 ,  69 ,  70 ,  71 ,  72 ,  73 ,  74 ,  75 ,  76 ,  77 ,  78 ,  79 ,  80 ,  81 ,  82 ,  83 ,  84 ,  85 ,  86 ,  87 ,  88 ,  89 ,  90 ,  91 ,  92 ,  93 ,  94 ,  95 ,  96 ,  97 ,  98 ,  99 ,  100 ,  101 ,  102 ,  103 ,  104 ,  105 ,  106 ,  107 ,  108 ,  109 ,  110 ,  111 ,  112 ,  113 ,  114 ,  115 ,  116 ,  117 ,  118 ,  119 ,  120 ,  121 ,  122 ,  123 ,  124 ,  125 ,  126 ,  127 ,  128 ,  129 ,  130 ,  131 ,  132 ,  133 ,  134 ,  135 ,  136 ,  137 ,  138 ,  139 ,  140 ,  141 ,  142 ,  143 ,  144 ,  145 ,  146 ,  147 ,  148 ,  149 ,  150 ,  151 ,  152 ,  153 ,  154 ,  155 ,  156 ,  157 ,  158 ,  159 ,  160 ,  161 ,  162 ,  163 ,  164 ,  165 ,  166 ,  167 ,  168 ,  169 ,  170 ,  171 ,  172 ,  173 ,  174 ,  175 ,  176 ,  177 ,  178 ,  179 ,  180 ,  181 ,  182 ,  183 ,  184 ,  185 ,  186 ,  187 ,  188 ,  189 ,  190 ,  191 ,  192 ,  193 ,  194 ,  195 ,  196 ,  197 ,  198 ,  199 ,  200 ,  201 ,  202 ,  203 ,  204 ,  205 ,  206 ,  207 ,  208 ,  209 ,  210 ,  211 ,  212 ,  213 ,  214 ,  215 ,  216 ,  217 ,  218 ,  219 ,  220 ,  221 ,  222 ,  223 ,  224 ,  225 ,  226 ,  227 ,  228 ,  229 ,  230 ,  231 ,  232 ,  233 ,  234 ,  235 ,  236 ,  237 ,  238 ,  239 ,  240 ,  241 ,  242 ,  243 ,  244 ,  245 ,  246 ,  247 ,  248 ,  249 ,  250 ,  251 ,  252 ,  253} };

/** 
 * goertzel_process_sample
 * Performs one iteration of the Goertzel algorithm.  This is done for each
 * sample from 1 to N.  Updates the Q_x values with each iteration.  Some
 * speedups and tricks are used here.  A multiplication lookup table is used to
 * remove any actual multiplication operations.
 * cos(1/1*2*pi) = 1
 * cos(1/2*2*pi) = -1
 * cos(1/4*2*pi) = 0
 * are used to completely bypass the need for any simulated
 * multiplication.  cos(1/256*2*pi) = ~1 is close enough to ignore the
 * coefficient entirely, BUT is untested.
 * args:
 *  sample8bit: one 8-bit sample of the signal
 * returns:
 *  none
 */
void goertzel_process_sample(BYTE sample8bit) {
    sDWORD s;
    BYTE i, j, t, maxOverflow;
    sBYTE st;
    // scale the input down if overflow was previously detected
    s = (sDWORD)(sample8bit >> scaleFactor);
    // sample frequency
    q_0[0] = q_1[0] + q_1[0] - q_2[0] + s;
    // sample frequency / 2
    q_0[1] = -q_1[1] - q_1[1] - q_2[1] + s;
    // F_s / 4
    q_0[2] = -q_2[2] + s;
    // F_s / 8
    t = (q_1[3] >= 0) ? coeff_mult[0][(BYTE)(q_1[3])] : \
                        -coeff_mult[0][(BYTE)(-q_1[3])];
    q_0[3] = t*2 - q_2[3] + s;
    // F_s / 16
    t = (q_1[4] >= 0) ? coeff_mult[1][(BYTE)(q_1[4])] : \
                        -coeff_mult[1][(BYTE)(-q_1[4])];
    q_0[4] = t*2 - q_2[4] + s;
    // F_s / 32 
    t = (q_1[5] >= 0) ? coeff_mult[2][(BYTE)(q_1[5])] : \
                        -coeff_mult[2][(BYTE)(-q_1[5])];
    q_0[5] = t*2 - q_2[5] + s;
    // F_s / 64
    t = (q_1[6] >= 0) ? coeff_mult[3][(BYTE)(q_1[6])] : \
                        -coeff_mult[3][(BYTE)(-q_1[6])];
    q_0[6] = t*2 - q_2[6] + s;
    // F_s / 128
    t = (q_1[7] >= 0) ? coeff_mult[4][(BYTE)(q_1[7])] : \
                        -coeff_mult[4][(BYTE)(-q_1[7])];
    q_0[7] = t*2 - q_2[7] + s;
    // Check for overflow, if overflowed, then shift everything down
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
    for (i=0; i<8; i++) {
        for (j=0; j<maxOverflow; j++) {
            q_0[i] >>= 1;
            scaleFactor++;
        }
    }
    // Update older Q values
    for (i=0; i<8; i++) {
        q_1[i] = q_0[i];
        q_2[i] = q_1[i];
    }
    if (++samplesProcessed >= N_SAMPLES) {
        goertzelReady = 1;
        samplesProcessed = 0;
        if (++scaleDecayCount < SCALE_DECAY_TICK)
            // keep the scaleFactor
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
    t = (q_2[3] > 0) ? 0x0000 & coeff_mult[0][(BYTE)(q_2[3])] : \
                       0xFFFF & -coeff_mult[0][(BYTE)(-q_2[3])];
    sResults[3] = q_1[3]*q_1[3] \
                 - q_1[3]*t\
                 + q_2[3]*q_2[3];
    t = (q_2[4] > 0) ? 0x0000 & coeff_mult[1][(BYTE)(q_2[4])] : \
                       0xFFFF & -coeff_mult[1][(BYTE)(-q_2[4])];
    sResults[4] = q_1[4]*q_1[4] \
                 - q_1[4]*t \
                 + q_2[4]*q_2[4];
    t = (q_2[5] > 0) ? 0x0000 & coeff_mult[2][(BYTE)(q_2[5])] : \
                       0xFFFF & -coeff_mult[2][(BYTE)(-q_2[5])];
    sResults[5] = q_1[5]*q_1[5] \
                 - q_1[5]*t \
                 + q_2[5]*q_2[5];
    t = (q_2[6] > 0) ? 0x0000 & coeff_mult[3][(BYTE)(q_2[6])] : \
                       0xFFFF & -coeff_mult[3][(BYTE)(-q_2[6])];
    sResults[6] = q_1[6]*q_1[6] \
                 - q_1[6]*t \
                 + q_2[6]*q_2[6];
    t = (q_2[7] > 0) ? 0x0000 & coeff_mult[4][(BYTE)(q_2[7])] : \
                       0xFFFF & -coeff_mult[4][(BYTE)(-q_2[7])];
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
    
