# Vexriscv SoC with UART & Multiple RAMs
This example integrates 2 AXI based RAMs onto a Vexriscv SoC.

### Instructions:
Copy your demo folder from litex installation directory ``litex/litex/soc/software/demo`` and paste it inside your project directory. Use the main.c file provided in this example in the demo application and replace it with the main.c file located inside your newly copied demo folder in project directory.


## 1. Simulation
We can simulate this IP using litex_sim tool in litex.

Run the following command to generate your SoC:
```
litex_sim --integrated-main-ram-size=0x10000 --cpu-type vexriscv --multiaxiram --sim-debug --no-compile-gateware
```
Run the following command to generate .bin file from .py file:
```
python3 ./demo/demo.py --build-path=build/sim
```
Before running the simulation, you have to create the binary of your application code residing in demo. The python script below converts the application code to demo.bin, which is later loaded on to the RAM.
```
litex_sim --integrated-main-ram-size=0x10000 --cpu-type vexriscv --ram-init=demo.bin --multiaxiram --sim-debug 
```
### Output:
![mul_test_7.png](./../../Pictures/mul_test_7.png "Optional title")
![mul_test_8.png](./../../Pictures/mul_test_8.png "Optional title")

![mul_sim_result.png](./../../Pictures/mul_sim_result.png "Optional title")

## Application
This test invokes both memories and writes and read data subsequently and randomly, inorder to make sure that the integration is properly done. Secondly, it stresses the Interconnect as both slave IPs are being communicated with simultaneously.