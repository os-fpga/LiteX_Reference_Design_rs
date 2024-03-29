#!/bin/sh

# 
# Vivado(TM)
# runme.sh: a Vivado-generated Runs Script for UNIX
# Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
# 

if [ -z "$PATH" ]; then
  PATH=/home/users/moin.ijaz/vivado_instl_dir/SDK/2017.4/bin:/home/users/moin.ijaz/vivado_instl_dir/Vivado/2017.4/ids_lite/ISE/bin/lin64:/home/users/moin.ijaz/vivado_instl_dir/Vivado/2017.4/bin
else
  PATH=/home/users/moin.ijaz/vivado_instl_dir/SDK/2017.4/bin:/home/users/moin.ijaz/vivado_instl_dir/Vivado/2017.4/ids_lite/ISE/bin/lin64:/home/users/moin.ijaz/vivado_instl_dir/Vivado/2017.4/bin:$PATH
fi
export PATH

if [ -z "$LD_LIBRARY_PATH" ]; then
  LD_LIBRARY_PATH=/home/users/moin.ijaz/vivado_instl_dir/Vivado/2017.4/ids_lite/ISE/lib/lin64
else
  LD_LIBRARY_PATH=/home/users/moin.ijaz/vivado_instl_dir/Vivado/2017.4/ids_lite/ISE/lib/lin64:$LD_LIBRARY_PATH
fi
export LD_LIBRARY_PATH

HD_PWD='/home/users/moin.ijaz/SDRAM/Hardware/build/digilent_arty/gateware/digilent_arty.runs/synth_1'
cd "$HD_PWD"

HD_LOG=runme.log
/bin/touch $HD_LOG

ISEStep="./ISEWrap.sh"
EAStep()
{
     $ISEStep $HD_LOG "$@" >> $HD_LOG 2>&1
     if [ $? -ne 0 ]
     then
         exit
     fi
}

EAStep vivado -log digilent_arty.vds -m64 -product Vivado -mode batch -messageDb vivado.pb -notrace -source digilent_arty.tcl
