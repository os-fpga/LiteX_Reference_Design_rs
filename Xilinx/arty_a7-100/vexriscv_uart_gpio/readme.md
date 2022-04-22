# Create a vexriscv SoC with uart and Axi_gpio intgerated onto the SoC.

```
./../litex_dev/litex-boards/litex_boards/targets/digilent_arty.py --integrated-main-ram-size=0x10000 --cpu-type=vexriscv --axigpio --build--load --uart-name=serial


```
## Generate the bin file

```
python3 ./demo/demo.py --build-path=build/digilent_basys3 

or

litex_bare_metal_demo --build-path=build/digilent_basys3
```

## Loading demo.bin through UART and reading UART comm port.
```
litex_term /dev/ttyUSB1 --kernel=demo.bin

```