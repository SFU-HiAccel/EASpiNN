## Introduction

EASpiNN is the framework for asynchronous fashion Spiking Neural Networks acceleration on Xilinx SoC FPGA boards. The framework is scalable to run any dense layer networks on a given Xilinx SoC FPGA board. The networks have been evaluated on MNIST dataset.

## Download EASpiNN

	git clone https://github.com/SFU-HiAccel/EASpiNN.git

## Setup Requirements

1. **Evaluated hardware platforms:**
    * **Xilinx SoC FPGA:**
      * Xilinx ZCU 104 Board

2. **Software tools:**
    * **HLS tool:**
      * Xilinx SDSoC 2019.1

## Accelerate SNN Algorithm using EASpiNN

Assuming you are in the project home directory of the checked out EASpiNN github repo.

  * The following network configurations are evaluated.
    
	Network	      | Configuration
	------------- | -------------
	Dense-1       | Input-500-100-Output
	Dense-2       | Input-1200-1200-Output
	
  * Choose the network that you wish to run. Include the network in `tb.cpp`
  * Modify the configuartion in `includes/header.h`
  * The Unroll parameters for each board varies based on the available resources in the board. 
  * The parameters are choosen such that the resource utilization is around 80% utilized for all the resources (BRAM, URAM, DSP, LUT, FF).


## Build EASpiNN Design

  * Create a project in SDSoC application for every network.
  * Add all the files in the project.
    * **Software Mode**
      * Build the project.
    * **Hardware Mode**
      * Toggle `network` function to Hardware.
      * Set Hardware at 100MHz.
      * Build the project.
  
##  Run EASpiNN
    * Once the build is over, an SD Card folder is created.
    * Copy the folder contents to an SD card.
    * Insert the SD card to the FPGA board.
    * Set the FPGA board in SD card boot mode.
    * Boot the FPGA and visualize in serial monitor.
    * Change the directory to mount (`cd /mnt`)
    * Run the application `./snn <number of images>`.
    

## Contacts

If you have further questions about EASpiNN, please contact:

* **Sathish Panchapakesan**, MASc Student

* HiAccel Lab, Simon Fraser University (SFU)

* Email: sathishp@sfu.ca 

* Website: http://www.sfu.ca/~sathishp/


