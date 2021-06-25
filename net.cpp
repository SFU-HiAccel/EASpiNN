#include "includes/header.h"

void setup_snn(int *psp_1, int *nsp_1, int *psp_2, int *nsp_2,
		int poisson_spike_reg_1[Sources], int neuron_spike_reg_1[Sources],
		int poisson_spike_reg_2[Sources], int neuron_spike_reg_2[Sources],
		q_dt Q_1[Neurons], q_dt Q_2[NeuronsMod], q_dt Q_3[NeuronsMod]) {
#pragma HLS INLINE off
    NL3: for(int spiker = 0; spiker < *psp_1; spiker++){
#pragma HLS PIPELINE
        poisson_spike_reg_2[spiker] = poisson_spike_reg_1[spiker];
    }

    *psp_2 = *psp_1;
    NL4: for(int spiker = 0; spiker < *nsp_1; spiker++){
#pragma HLS PIPELINE
        neuron_spike_reg_2[spiker] = neuron_spike_reg_1[spiker];
    }
    *nsp_2 = *nsp_1;
    NL5: for (int neuron = 0; neuron < Neurons; neuron++){
#pragma HLS PIPELINE
        Q_1[neuron] = Q_2[neuron] +Q_3[neuron];
        Q_2[neuron] = 0;
        Q_3[neuron] = 0;
    }
    *psp_1 = 0;
    *nsp_1 = 0;
}
