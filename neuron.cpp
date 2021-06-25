#include "includes/header.h"


void IntF(bool init, int spike_count[Neurons], int spike_reg[Neurons], q_dt Q[Neurons], int *nsp, int *sim_counter){
	static q_dt vm[Neurons];

    if (init)
        for(int neuron = 0; neuron < Neurons; neuron++)
            vm[neuron] = vinitial;
    int spiked = 0;
    NEURON1: for (int neuron = 0; neuron < Neurons-Output; neuron++){

#pragma HLS PIPELINE
        vm[neuron] += Q[neuron];
        if (vm[neuron] > vth){
            vm[neuron] = 0;
            spike_reg[spiked] = neuron;
            spiked++;
        }
    }
    NEURON2: for (int neuron = Neurons-Output; neuron < Neurons; neuron++){
#pragma HLS PIPELINE
        vm[neuron] += Q[neuron];
        if (vm[neuron] > vth){
            spike_count[neuron-(Neurons-Output)] += 1;
            *sim_counter = *sim_counter + 1;
        }
    }
    *nsp =  spiked;
}
