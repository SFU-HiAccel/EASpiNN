#ifndef HEADER
#define HEADER

#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include "sds_lib.h"
#include "sds_utils.h"
#include<time.h>
#include <ap_fixed.h>

using namespace std;

//Software 2410
//#define URAM_S 0
//#define URAM_L1 0
//#define BRAM_L1 0
//#define ON_CHIP_OUT 12000
//#define Neurons 2410
//#define NeuronsMod 2500
//#define Sources 784
//#define ROWS 3184
//#define COLS 1200
//#define Layer1 1200
//#define Layer2 1200
//#define Output 10
//const int Unroll = 1;
//const int Unrolll = 1;
//#define vth 1.0
//typedef float w_dt;

//Software 610
#define URAM_S 0
#define URAM_L1 0
#define BRAM_L1 0
#define ON_CHIP_OUT 12000
#define Neurons 610
#define NeuronsMod 700
#define Sources 784
#define ROWS 1384
#define COLS 500
#define Layer1 500
#define Layer2 100
#define Output 10
const int Unroll = 1;
const int Unrolll = 1;
#define vth 1.0
typedef float w_dt;

//Results for 610
// #define URAM_S 784
// #define URAM_L1 500
// #define BRAM_L1 0
// #define ON_CHIP_OUT 1000
// #define Neurons 610
// #define NeuronsMod 700
// #define Sources 784
// #define ROWS 1384
// #define COLS 500
// #define Layer1 500
// #define Layer2 100
// #define Output 10
// const int Unroll = 100;
// const int Unrolll = 1;
// #define vth 1.0
// typedef ap_fixed<16,2> w_dt;

//Results for 2410 - 16 bits
// #define URAM_S 784
// #define URAM_L1 450
// #define BRAM_L1 350
// #define ON_CHIP_OUT 12000
// #define Neurons 2410
// #define NeuronsMod 2500
// #define Sources 784
// #define ROWS 3184
// #define COLS 1200
// #define Layer1 1200
// #define Layer2 1200
// #define Output 10
// const int Unroll = 50;
// const int Unrolll = 5;
// #define vth 1.0
// typedef ap_fixed<16,2> w_dt;

//Results for 2410 - float
//#define URAM_S 392
//#define URAM_L1 150
//#define BRAM_L1 100
//#define ON_CHIP_OUT 12000
//#define Neurons 2410
//#define NeuronsMod 2500
//#define Sources 784
//#define ROWS 3184
//#define COLS 1200
//#define Layer1 1200
//#define Layer2 1200
//#define Output 10
//const int Unroll = 50;
//const int Unrolll = 5;
//#define vth 1.0
//typedef float w_dt;

//Results for 2410 - 8 bits
//#define URAM_S 784
//#define URAM_L1 1200
//#define BRAM_L1 0
//#define ON_CHIP_OUT 12000
//#define Neurons 2410
//#define NeuronsMod 2500
//#define Sources 784
//#define ROWS 3184
//#define COLS 1200
//#define Layer1 1200
//#define Layer2 1200
//#define Output 10
//const int Unroll = 100;
//const int Unrolll = 1;
//#define vth 25.0
//typedef ap_fixed<8,2> w_dt;

#define del_t 1e-3
#define vreset 0.0
#define vinitial 0.0

//typedef float w_dt;

typedef int im;
typedef int time_dt;
//typedef float q_dt;
typedef ap_fixed<32,16> q_dt;

struct weights_uram {
 w_dt weights_1_a;
 w_dt weights_1_b;
};

extern sds_utils::perf_counter hw_cycles;
void poisson(im image[Sources],
              int spike_reg[Sources],
              int intensity,
              int *spiked);
void IntF(bool init, int spike_count[Neurons], int spike_reg[Neurons], q_dt Q[Neurons], int *nsp, int *sim_counter);
void setup_snn(int *psp_1, int *nsp_1, int *psp_2, int *nsp_2,
		int poisson_spike_reg_1[Sources], int neuron_spike_reg_1[Sources],
		int poisson_spike_reg_2[Sources], int neuron_spike_reg_2[Sources],
		q_dt Q_1[Neurons], q_dt Q_2[NeuronsMod], q_dt Q_3[NeuronsMod]);

void network(int simsteps, int image_in[Sources], int intensity,
	        int spike_count_out[Output], w_dt w_1[Sources*Layer1],
			w_dt w_2[Layer1*Layer2], w_dt w_3[Layer2*Output], bool flag,
			int *counter_a, int *counter_b, int *counter_ps, int *counter_ns);
void deterministic_a(int pspiked, int poisson_spike_reg[Sources],
		q_dt Q2[NeuronsMod], w_dt w_1[Sources*Layer1], w_dt weights_1[URAM_S][Layer1],
		int **counter_a, int **counter_ps);
void deterministic_b(int nspiked,
    int neuron_spike_reg[Neurons],
    q_dt Q3[NeuronsMod], w_dt w_2[Layer1*Layer2],
	w_dt weights_u2[URAM_L1][Layer2],
    w_dt weights_b2[BRAM_L1+1][Layer2],
    w_dt weights_3[Layer2*Output], int **counter_b, int **counter_ns);

#endif
