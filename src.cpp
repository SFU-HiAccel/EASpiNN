#include "includes/header.h"

#pragma SDS data zero_copy(image_in, spike_count_out, w_1, w_2, w_3)
void network(int simsteps, int image_in[Sources], int intensity,
	        int spike_count_out[Output], w_dt w_1[Sources*Layer1],
			w_dt w_2[Layer1*Layer2], w_dt w_3[Layer2*Output], bool flag,
			int *counter_a, int *counter_b, int *counter_ps, int *counter_ns) {
    static int poisson_spike_reg_1[Sources], poisson_spike_reg_2[Sources];
    static int neuron_spike_reg_1[Neurons], neuron_spike_reg_2[Neurons];
    static int image[Sources];
    static int spike_count[Output];
    static int nsp_1, nsp_2;
    static int psp_1, psp_2;
    int sim_counter = 0;

    static w_dt weights_1[URAM_S][Layer1];
#pragma HLS RESOURCE variable=weights_1 core=XPM_MEMORY uram
#pragma HLS ARRAY_RESHAPE variable=weights_1 cyclic factor=Unroll dim=2
#pragma HLS data_pack variable=weights_1
    static w_dt weights_u2[URAM_L1][Layer2];
#pragma HLS RESOURCE variable=weights_uram2 core=XPM_MEMORY uram
#pragma HLS ARRAY_RESHAPE variable=weights_uram2 cyclic factor=Unroll dim=2
#pragma HLS data_pack variable=weights_uram2
    static w_dt weights_b2[BRAM_L1+1][Layer2];
#pragma HLS ARRAY_RESHAPE variable=weights_b2 cyclic factor=Unrolll dim=2
#pragma HLS data_pack variable=weights_uram2

    static w_dt weights_3[ON_CHIP_OUT];

    static q_dt Q_1[Neurons], Q_2[NeuronsMod], Q_3[NeuronsMod];
#pragma HLS array_partition variable=Q_2 cyclic factor=Unroll
#pragma HLS array_partition variable=Q_3 cyclic factor=Unroll

    if (!(flag)){
    	for (int i = 0; i < Layer2*Output; i++) {
    		weights_3[i] = w_3[i];
    	}
    	for(int i = 0; i < URAM_S; i++) {
    		for (int j = 0; j < Layer1; j++) {
    			weights_1[i][j] = w_1[(i*Layer1)+j];
    		}
    	}
    	for(int i = 0; i < URAM_L1; i++) {
    		for (int j = 0; j < Layer2; j++) {
    			weights_u2[i][j] = w_2[(i*Layer2)+j];
    		}
    	}
    	for(int i = 0; i < BRAM_L1; i++) {
    		for (int j = 0; j < Layer2; j++) {
    			weights_b2[i][j] = w_2[(URAM_L1*Layer2)+(i*Layer2)+j];
    		}
    	}
    }
    else
    {
        nsp_1 = nsp_2 = 0;
        psp_1 = psp_2 = 0;
        memcpy(image, image_in, sizeof(int)*Sources);
        SETUP: for (int neuron = 0; neuron < Neurons; neuron++) {
#pragma HLS PIPELINE
            Q_1[neuron] = 0;
            Q_2[neuron] = 0;
            Q_3[neuron] = 0;
        }
        SETUP2: for (int neuron = 0; neuron < Output; neuron++) {
#pragma HLS PIPELINE
            spike_count[neuron] = 0;
        }
    	SIMSTEP: for (int simstep = 0; simstep < 40; simstep++) {
    		setup_snn(&psp_1, &nsp_1, &psp_2, &nsp_2, poisson_spike_reg_1, neuron_spike_reg_1, poisson_spike_reg_2, neuron_spike_reg_2, Q_1, Q_2, Q_3);
            poisson(image, poisson_spike_reg_1, intensity, &psp_1);
            IntF(simstep == 0, spike_count, neuron_spike_reg_1, Q_1, &nsp_1, &sim_counter);

            deterministic_a(psp_2, poisson_spike_reg_2, Q_2, w_1, weights_1, &counter_a,
            		&counter_ps);
            deterministic_b(nsp_2, neuron_spike_reg_2, Q_3, w_2, weights_u2, weights_b2, weights_3,
            		&counter_b, &counter_ns);
           if(sim_counter>1) {
           	break;
           }
    	}
        OUT: for (int neuron = 0; neuron < Output; neuron++){
#pragma HLS PIPELINE
            spike_count_out[neuron] = spike_count[neuron];
        }
    }
}
