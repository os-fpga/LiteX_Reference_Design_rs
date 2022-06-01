import os

from migen import *

from litex.soc.interconnect import axi
from litex.soc.interconnect import wishbone

from litex.soc.integration.soc import *
from litex.soc.integration.doc import AutoDoc, ModuleDoc
from litex.soc.integration.soc import SoCRegion
from litex.soc.interconnect.axi import *



def _to_signal(obj):
    return obj.raw_bits() if isinstance(obj, Record) else obj


class Open(Signal): pass
class gpio_axi(Module, AutoDoc, AutoCSR):
    """Verilog RTL-based UART 16550"""
    def __init__(self, platform, pads, default_enable=0):
        self.intro = ModuleDoc("""GPIO_AXI: A verilog RTL-based GPIOs derived from Xilinx Vivado IP library.""")

        self.bus  = bus = axi.AXILiteInterface(address_width=32,data_width=32)
        self.CLK = Signal()
        self.RSTN = Signal()
        self.GPIN = Signal(32)
        self.GPOUT = Signal(32)
        self.INT = Signal()
        self.AWADDR = Signal(32)
        self.AWPROT = Signal(3)
        self.AWVALID = Signal()
        self.AWREADY = Signal()
        self.WDATA = Signal(32)
        self.WSTRB = Signal(4)
        self.WVALID = Signal()
        self.WREADY = Signal()
        self.BRESP = Signal(2)
        self.BVALID = Signal()
        self.BREADY = Signal()
        self.ARADDR = Signal(32)
        self.ARPROT = Signal(3)
        self.ARVALID = Signal()
        self.ARREADY = Signal()
        self.RDATA = Signal(32)
        self.RRESP = Signal(2)
        self.RVALID = Signal()
        self.RREADY = Signal()

        # # #

        internal = not (hasattr(pads, "o")  and hasattr(pads, "i"))
        nbits    = len(pads) if internal else len(pads.o)

        if internal:
            if isinstance(pads, Record):
                pads = pads.flatten()


        self.specials += Instance("AXI4LITE_GPIO",
            i_CLK=ClockSignal(),
            i_RSTN=~ResetSignal(),
            i_GPIN=self.GPIN,
            o_GPOUT=self.GPOUT,
            o_INT=self.INT,
            i_AWADDR=bus.aw.addr,
            i_AWPROT=Open(),
            i_AWVALID=bus.aw.valid,
            o_AWREADY=bus.aw.ready,
            i_WDATA=bus.w.data,
            i_WSTRB=bus.w.strb,
            i_WVALID=bus.w.valid,
            o_WREADY=bus.w.ready,
            o_BRESP=bus.b.resp,
            o_BVALID=bus.b.valid,
            i_BREADY=bus.b.ready,
            i_ARADDR=bus.ar.addr,
            i_ARPROT=Open(),
            i_ARVALID=bus.ar.valid,
            o_ARREADY=bus.ar.ready,
            o_RDATA=bus.r.data,
            o_RRESP=bus.r.resp,
            o_RVALID=bus.r.valid,
            i_RREADY=bus.r.ready,
        )

        for i in range(nbits):
            self.comb += pads.i[i].eq(self.GPIN[i])
            self.comb += pads.o[i].eq(self.GPOUT[i])


        platform.add_source_dir(path="/home/users/bilal.ahmed/litex_dev/Litex_dev/litex/soc/cores/ip/AXI_GPIO")