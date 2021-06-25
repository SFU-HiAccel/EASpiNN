#include "includes/header.h"

using namespace std;

#include "includes/610/neuron_labels.h"
#include "includes/610/weights.h"

// #include "includes/2410/neuron_labels.h"
// #include "includes/2410/weights.h"

sds_utils::perf_counter hw_cycles;

int main(int argc, char *argv[]) {
    if (argc < 3){
        cout << "Usage : <executable> <TEST_CASES> <SIMSTEPS>" << endl;
        exit(1);
    }

    srand(time(NULL));
    float correct = 0;
    int intensity = 2;
    int label;
    string line;
    ifstream images("includes/testing_10k_images.txt");
    ifstream labels("includes/testing_10k_labels.txt");

    static int* image = (int*)sds_alloc_non_cacheable(sizeof(int) * Sources);
    static int* spike_count = (int*)sds_alloc_non_cacheable(sizeof(int) * Output);

    static w_dt* weights_1 = (w_dt*)sds_alloc_non_cacheable(sizeof(w_dt) * Sources * Layer1);
    static w_dt* weights_2 = (w_dt*)sds_alloc_non_cacheable(sizeof(w_dt) * Layer1 * Layer2);
    static w_dt* weights_3 = (w_dt*)sds_alloc_non_cacheable(sizeof(w_dt) * Layer2 * Output);


    for(int i = 0; i < Sources * Layer1; i++) {
        weights_1[i] = weights[i];
    }

    for(int i = 0; i < Layer1; i++) {
    	for(int j = 0; j < Layer2; j++) {
    	weights_2[(i*Layer2)+j] = weights[(Sources*COLS)+(i*COLS)+j];
    	}
    }

    for(int i = 0; i < Layer2; i++) {
    	for(int j = 0; j < Output; j++) {
    	weights_3[(i*Output)+j] = weights[(Sources*COLS)+(Layer1*COLS)+(i*COLS)+j];
    	}
    }

    static bool flag = false;

    int TEST_CASES;
    int simsteps;

    int acounter_a = 0;
    int acounter_b = 0;
    int acounter_ps = 0;
    int acounter_ns = 0;
    int counter_a = 0;
    int counter_b = 0;
    int counter_ps = 0;
    int counter_ns = 0;
    TEST_CASES = atoi(argv[1]);
    simsteps = atoi(argv[2]);
 
// Calling the HW Function to copy the weights to the ON-CHIP Memory
    network(simsteps, image, intensity, spike_count, weights_1, weights_2, weights_3, flag, &counter_a,
    		&counter_b, &counter_ps, &counter_ns);

	flag = true;

    for (int tcase = 0; tcase < TEST_CASES; tcase++) {
        if (images.is_open()) {
            for (int j = 0; j < Sources; j++) {
                getline(images, line);
                image[j] = stoi(line);
            }
        }
        if (labels.is_open()) {
            getline(labels, line);
            label = stoi(line);
        }

        intensity = 1000;

        for (int neuron = 0; neuron < Output; neuron++) {
            spike_count[neuron] = 0;
        }
// Calling the Network Function for every Image
		hw_cycles.start();
        network(simsteps, image, intensity, spike_count, weights_1, weights_2, weights_3, flag, &counter_a,
        		&counter_b, &counter_ps, &counter_ns);
    	hw_cycles.stop();
        intensity += 1;
        acounter_a = (acounter_a+counter_a)/2;
        acounter_b = (acounter_b+counter_b)/2;
        acounter_ps = (acounter_ps+counter_ps)/2;
        acounter_ns = (acounter_ns+counter_ns)/2;
        counter_a = 0;
        counter_b = 0;
        counter_ps = 0;
        counter_ns = 0;
        // Accuracy Calculation
        int dom = 0;
        for (int neuron = 0; neuron < Output; neuron++) {
            if (spike_count[neuron] > spike_count[dom]) {
                dom = neuron;
            }
        }
        if (label == neuron_labels[dom])
            correct += 1.0;
    }
//
//    float avg_simstep;
//    cout << "Total simsteps - " << total_simstep << endl;
//    avg_simstep = total_simstep/TEST_CASES;
//    cout << "Average simsteps - " << avg_simstep << endl;

    cout << "acounter_a " <<  acounter_a << endl;
    cout << "acounter_b " <<  acounter_b << endl;
    cout << "acounter_ps " <<  acounter_ps << endl;
    cout << "acounter_ns " <<  acounter_ns << endl;
//    int acounter_b = 0;
//    int acounter_ps = 0;
//    int acounter_ns = 0;
    cerr << correct * 100 / TEST_CASES << endl;
    uint64_t hw_avg_cycles = hw_cycles.avg_cpu_cycles();
    std::cout << "Number of CPU cycles for Hardware operation: "
              << hw_avg_cycles << std::endl;
    images.close();
    labels.close();
 
    return 0;
}
