# 
# Synthesis run script generated by Vivado
# 

proc create_report { reportName command } {
  set status "."
  append status $reportName ".fail"
  if { [file exists $status] } {
    eval file delete [glob $status]
  }
  send_msg_id runtcl-4 info "Executing : $command"
  set retval [eval catch { $command } msg]
  if { $retval != 0 } {
    set fp [open $status w]
    close $fp
    send_msg_id runtcl-5 warning "$msg"
  }
}
set_msg_config  -ruleid {1}  -id {Common 17-55}  -new_severity {WARNING} 
create_project -in_memory -part xc7a100tcsg324-1

set_param project.singleFileAddWarning.threshold 0
set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_property webtalk.parent_dir /home/users/moin.ijaz/SDRAM/Hardware/build/digilent_arty/gateware/digilent_arty.cache/wt [current_project]
set_property parent.project_path /home/users/moin.ijaz/SDRAM/Hardware/build/digilent_arty/gateware/digilent_arty.xpr [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property ip_output_repo /home/users/moin.ijaz/SDRAM/Hardware/build/digilent_arty/gateware/digilent_arty.cache/ip [current_project]
set_property ip_cache_permissions {read write} [current_project]
read_verilog -library xil_defaultlib {
  /home/users/moin.ijaz/litex_installation/pythondata-cpu-vexriscv/pythondata_cpu_vexriscv/verilog/VexRiscv.v
  /home/users/moin.ijaz/litex_installation/litex/litex/soc/cores/axi_ram/axi_ram.v
  /home/users/moin.ijaz/SDRAM/Hardware/build/digilent_arty/gateware/digilent_arty.v
}
# Mark all dcp files as not used in implementation to prevent them from being
# stitched into the results of this synthesis run. Any black boxes in the
# design are intentionally left as such for best results. Dcp files will be
# stitched into the design at a later time, either when this synthesis run is
# opened, or when it is stitched into a dependent implementation run.
foreach dcp [get_files -quiet -all -filter file_type=="Design\ Checkpoint"] {
  set_property used_in_implementation false $dcp
}
read_xdc /home/users/moin.ijaz/SDRAM/Hardware/build/digilent_arty/gateware/digilent_arty.xdc
set_property used_in_implementation false [get_files /home/users/moin.ijaz/SDRAM/Hardware/build/digilent_arty/gateware/digilent_arty.xdc]
set_property processing_order EARLY [get_files /home/users/moin.ijaz/SDRAM/Hardware/build/digilent_arty/gateware/digilent_arty.xdc]


synth_design -top digilent_arty -part xc7a100tcsg324-1


# disable binary constraint mode for synth run checkpoints
set_param constraints.enableBinaryConstraints false
write_checkpoint -force -noxdef digilent_arty.dcp
create_report "synth_1_synth_report_utilization_0" "report_utilization -file digilent_arty_utilization_synth.rpt -pb digilent_arty_utilization_synth.pb"