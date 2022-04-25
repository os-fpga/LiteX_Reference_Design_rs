# Method 1 to load demo.bin on the fly.

```
./../litex_dev/litex-boards/litex_boards/targets/digilent_arty.py --integrated-main-ram-size=0x10000 --variant a7-100 --cpu-type=vexriscv --cpu-type=vexriscv --build--load --uart-name=serial --axiram

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