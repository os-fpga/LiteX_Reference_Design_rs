# Vexriscv SoC with UART & SDRAM + AXI-RAM:
AXI-SDRAM application for Vexriscv based SoC.

### Instructions:
Copy your demo folder from litex installation directory ``litex/litex/soc/software/demo`` and paste it inside your project directory. Use the main.c file provided in this example in the demo application and replace it with the main.c file located inside your newly copied demo folder in project directory.

## 1. Simulation:
We can simulate this application using litex_sim tool in litex.

Run the following command to generate your SoC:
```
litex_sim --cpu-type vexriscv --bus-standard axi-lite  --with-sdram --multiaxiram --sim-debug --no-compile-gateware
```
Before running the simulation, you have to create the binary of your application code residing in demo. The python script below converts the application code to demo.bin, which is later loaded on to the SDRAM.

Run the following command to generate .bin file from .py file:
```
python3 ./demo/demo.py --build-path=build/sim
```
Run the following command to execute your application code onto the processor:
```
litex_sim --cpu-type vexriscv --bus-standard axi-lite  --with-sdram --multiaxiram --sdram-init=demo.bin --sim-debug
```


## Output:
![sdram_sim1.png](./../../Pictures/sdram_sim1.png "Optional title")

![sdram_sim_test7.png](./../../Pictures/sdram_sim_test7.png "Optional title")
![sdram_sim_test8.png](./../../Pictures/sdram_sim_test8.png "Optional title")

![sdram_sim_result.png](./../../Pictures/sdram_sim_result.png "Optional title")

## 2. Hardware:
Connect your Digilent Basys 3 board with your machine. We will be using the same design which we used in simulation to verify on the board. The following board file written in python creates the same SoC and later build and load it onto the Basys3 board.

Run the following command to generate your SoC:
```
./../../litex_installation/litex-boards/litex_boards/targets/digilent_basys3.py --cpu-type vexriscv --bus-standard axi-lite --axiram --load --build --uart-name=serial
```
Run the following command to generate .bin file from .py file:
```
python3 ./demo/demo.py --build-path=build/digilent_basys3
```
The litex_term tool load the board with the application binary through the COM port.
```
litex_term /dev/ttyUSB2 --kernel=demo.bin
```
## Output:
![sdram_hard1.png](./../../Pictures/sdram_hard1.png "Optional title")

![sdram_hard_test1.png](./../../Pictures/sdram_hard_test1.png "Optional title")
![sdram_hard_test2.png](./../../Pictures/sdram_hard_test2.png "Optional title")

![sdram_hard_result.png](./../../Pictures/sdram_hard_result.png "Optional title")

## Application
In this application code, we write and read data to the SDRAM and the results are shown on UART console (LiteX Console).

