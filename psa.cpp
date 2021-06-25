#include "includes/header.h"

//Poisson Spikes
void deterministic_a(int pspiked, int poisson_spike_reg[Sources],
		q_dt Q2[NeuronsMod], w_dt w_1[Sources*Layer1], w_dt weights_1[URAM_S][Layer1],
		int **counter_a, int **counter_ps) {
#pragma HLS INLINE off

	**counter_a = **counter_a + 1;
	static w_dt weight_buf[Layer1];
#pragma HLS ARRAY_RESHAPE variable=weight_buf cyclic factor=Unrolll
	PSPIKED: for (int spiker = 0; spiker < pspiked; spiker++) {
		**counter_ps = **counter_ps + 1;
        int pre = poisson_spike_reg[spiker];
        if (pre < URAM_S) {
			PCOLS1: for (int post1 = 0; post1 < Layer1/Unroll; post1++) {
		#pragma HLS pipeline
					PCOLS2: for (int post2 = 0; post2 < Unroll; post2++) {
		#pragma HLS unroll factor=Unroll
					Q2[(post1*Unroll)+post2] += weights_1[pre][(post1*Unroll)+post2];
					}
				}
        }
        else {
        	for (int i=0; i<Layer1;i++) {
#pragma HLS pipeline
        		weight_buf[i] = w_1[(pre*Layer1)+i];
        	}
            PCOLS3: for (int post1 = 0; post1 < Layer1/Unrolll; post1++) {
        #pragma HLS pipeline
                    PCOLS4: for (int post2 = 0; post2 < Unrolll; post2++) {
        #pragma HLS unroll factor=Unrolll
            		Q2[(post1*Unrolll)+post2] += weight_buf[(post1*Unrolll)+post2];
                    }
            	}
        }
    }
}

void deterministic_b(int nspiked,
    int neuron_spike_reg[Neurons],
    q_dt Q3[NeuronsMod], w_dt w_2[Layer1*Layer2],
	w_dt weights_u2[URAM_L1][Layer2],
    w_dt weights_b2[BRAM_L1+1][Layer2],
    w_dt weights_3[Layer2*Output], int **counter_b, int **counter_ns) {
	//Neuron Spikes

#pragma HLS INLINE off
	**counter_b = **counter_b + 1;
	static w_dt weight_buf[Layer2];
#pragma HLS ARRAY_RESHAPE variable=weight_buf cyclic factor=Unrolll
	NSPIKED: for (int spiker = 0; spiker < nspiked; spiker++) {
		**counter_ns = **counter_ns + 1;
        int pre = neuron_spike_reg[spiker];
        if (pre+Sources >= ROWS)
            continue;
        if (pre < (URAM_L1)) {
            NCOLS1: for (int post = 0; post < Layer2/Unroll; post++) {
#pragma HLS pipeline
            	NCOLS2: for (int post2 = 0; post2 < Unroll; post2++) {
#pragma HLS unroll factor=Unroll
            		Q3[(post*Unroll)+post2+Layer1] += weights_u2[pre][(post*Unroll)+post2];
            	}
            }
        }
        else if (pre < (BRAM_L1+URAM_L1) && pre < (Layer1)) {
            NCOLS3: for (int post = 0; post < Layer2/Unrolll; post++) {
#pragma HLS pipeline
            	NCOLS4: for (int post2 = 0; post2 < Unrolll; post2++) {
#pragma HLS unroll factor=Unrolll
            		Q3[(post*Unrolll)+post2+Layer1] += weights_b2[(pre-URAM_L1)][(post*Unrolll)+post2];
            	}
            }
        }
        else if (pre < Layer1) {
        	for (int i=0; i<Layer2;i++) {
#pragma HLS pipeline
        		weight_buf[i] = w_2[(pre*Layer2)+i];
        	}
            NCOLS5: for (int post1 = 0; post1 < Layer2/Unrolll; post1++) {
        #pragma HLS pipeline
                NCOLS6: for (int post2 = 0; post2 < Unrolll; post2++) {
        #pragma HLS unroll factor=Unrolll
            		Q3[(post1*Unrolll)+post2+Layer1] += weight_buf[(post1*Unrolll)+post2];
                   }
            }
        }
        else {
            NCOLS7: for (int post = 0; post < Output; post++) {
#pragma HLS pipeline
                Q3[post+Layer1+Layer2] += weights_3[(pre-Layer1)*Output+post];
            }
        }
	}
}


