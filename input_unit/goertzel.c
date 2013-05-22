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
static const DWORD coeff_mult[1024] PROGMEM = \
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 651, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 670, 671, 672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 685, 686, 687, 688, 689, 690, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 709, 710, 711, 712, 713, 714, 715, 716, 717, 718, 719, 720, 721, 722, 723, 724, 725, 726, 727, 728, 729, 729, 730, 731, 732, 733, 734, 735, 736, 737, 738, 739, 740, 741, 742, 743, 744, 745, 746, 747, 748, 748, 749, 750, 751, 752, 753, 754, 755, 756, 757, 758, 759, 760, 761, 762, 763, 764, 765, 766, 767, 768, 768, 769, 770, 771, 772, 773, 774, 775, 776, 777, 778, 779, 780, 781, 782, 783, 784, 785, 786, 787, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, 797, 798, 799, 800, 801, 802, 803, 804, 805, 806, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 826, 826, 827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844, 845, 845, 846, 847, 848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863, 864, 865, 865, 866, 867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 878, 879, 880, 881, 882, 883, 884, 884, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894, 895, 896, 897, 898, 899, 900, 901, 902, 903, 904, 904, 905, 906, 907, 908, 909, 910, 911, 912, 913, 914, 915, 916, 917, 918, 919, 920, 921, 922, 923, 923, 924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939, 940, 941, 942, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959, 960, 961, 962, 962, 963, 964, 965, 966, 967, 968, 969, 970, 971, 972}; 

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
void goertzel_process_sample(DWORD sample10bit) {
    sDWORD s, t;
    BYTE i, maxOverflow;
    sBYTE st;
    // mask that determines which twiddle factors should be updated.
    // 0th bit should change every time, 4th bit should change every
    // 16th time, etc.
    BYTE shouldUpdate = (BYTE)((samplesProcessed + 1) ^ samplesProcessed);
    BYTE freqMask = 1;
    // scale the input down if overflow was previously detected
    s = (sDWORD)(sample10bit >> scaleFactor);
    // F_s / (10*2^i)
    for (i=0; i<8; i++) {
        // only update if shouldUpdate bit got switched on && not done sampling
        if ((shouldUpdate & freqMask) && (twiddleUpdated[i] < N_SAMPLES)) {
            t = (q_1[i] >= 0) ? coeff_mult[q_1[i]] : \
                                -coeff_mult[-q_1[i]];
            q_0[i] = t*2 - q_2[i] + s;
            twiddleUpdated[i]++;
        }
        freqMask <<= 1;
    }
    // Check for overflow
    maxOverflow = 0;
    for (i=0; i<8; i++) {
        st = (sBYTE)(q_0[i] >> 10);
        if (st < 0)
            st = ~st; // invert, NOT -
        if (st > maxOverflow) {
            maxOverflow = st;
            scaleDecayCount = 0;
        }
    }
    // shift everything down by the overflow amount
    for (i=0; i<8; i++) {
        q_0[i] >>= maxOverflow;
        q_1[i] >>= maxOverflow;
    }
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
                           0xFF00 | -coeff_mult[(BYTE)(-q_2[i])];
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
    
