#ifndef K351P_DVP_H_
#define K351P_DVP_H_

//#define K351P_FRAM_W_ALGIN 1

#if K351P_FRAM_W_ALGIN
#define K351P_FRAM_W_500 480
#define K351P_FRAM_W_2000 1984
#define K351P_FRAM_W_250X250 224
#else
#define K351P_FRAM_W_500 500
#define K351P_FRAM_W_2000 2000
#define K351P_FRAM_W_250X250 224
#endif

//------------------2000P15-----------------------
// mclk=24M pclk=75.6M
#define FRAM_W_2000P15   2400
#define FRAM_H_2000P15   2100
#define ACTIVE_W_2000P15 K351P_FRAM_W_2000
#define ACTIVE_H_2000P15 2000
#define PIC_IN_W_2000P15 K351P_FRAM_W_2000
#define PIC_IN_H_2000P15 2000
#define OFFSET_X_2000P15 0
#define OFFSET_Y_2000P15 0

//------------------500P2-----------------------
// mclk=6M pclk=3M
#define FRAM_W_500P2   800
#define FRAM_H_500P2   1875
#define ACTIVE_W_500P2 K351P_FRAM_W_500
#define ACTIVE_H_500P2 500
#define PIC_IN_W_500P2 K351P_FRAM_W_500
#define PIC_IN_H_500P2 500
#define OFFSET_X_500P2 0
#define OFFSET_Y_500P2 0

//------------------500P5-----------------------
// mclk=6M pclk=3M
#define FRAM_W_500P5   800
#define FRAM_H_500P5   750
#define ACTIVE_W_500P5 K351P_FRAM_W_500
#define ACTIVE_H_500P5 500
#define PIC_IN_W_500P5 K351P_FRAM_W_500
#define PIC_IN_H_500P5 500
#define OFFSET_X_500P5 0
#define OFFSET_Y_500P5 0

//------------------250X250P60-----------------------
// mclk=24M pclk=75.6M
#define FRAM_W_250X250P60   4200
#define FRAM_H_250X250P60   300
#define ACTIVE_W_250X250P60 K351P_FRAM_W_250X250
#define ACTIVE_H_250X250P60 250
#define PIC_IN_W_250X250P60 K351P_FRAM_W_250X250
#define PIC_IN_H_250X250P60 250
#define OFFSET_X_250X250P60 0
#define OFFSET_Y_250X250P60 0


#include "cfg/k351p_dvp_500p2.h"
#include "cfg/k351p_dvp_500p5.h"
#include "cfg/k351p_dvp_2000p15.h"
#include "cfg/k351p_dvp_2000p15_slave.h"
#include "cfg/k351p_dvp_250x250p60.h"


#endif /* K351P_DVP_H_ */


