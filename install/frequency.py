#!/usr/bin/python
# 
# Sample of measuring and frequency correction with ACOUNTER02A

import time
import datetime
import sys
import pipes
import os
from pymlab import config

#### Script Arguments ###############################################

if (len(sys.argv) > 3) or (len(sys.argv) < 2):
    sys.stderr.write("Invalid number of arguments.\n")
    sys.stderr.write("Usage: %s PORT_ADDRESS [REQUIERED_MHz]\n" % (sys.argv[0], ))
    sys.exit(1)

port    = eval(sys.argv[1])
#### Sensor Configuration ###########################################

cfg = config.Config(
    i2c = {
        "port": port,
    },
    bus = [
        {
            "type": "i2chub",
            "address": 0x70,
	       	"children": [
                        { "name":"counter", "type":"acount02", "channel": 2, },
                        { "name":"clkgen", "type":"clkgen01", "channel": 5, },
		    ],
        },
    ],
)
cfg.initialize()

print "RMDS Station frequency management test software \r\n"
fcount = cfg.get_device("counter")
fgen = cfg.get_device("clkgen")
time.sleep(0.5)
frequency = fcount.get_freq()
rfreq = fgen.get_rfreq()
hsdiv = fgen.get_hs_div()
n1 = fgen.get_n1_div()
rfPath = "/tmp/satObPIPE"
#os.mkfifo(rfPath)
rp = open(rfPath, 'r')
#fdco = 0
#fxtal = 0
#regs = [0, 0, 0]


#### Data Logging ###################################################

try:
    with open("frequency.log", "a") as f:
        while True:
            now = datetime.datetime.now()
            if (now.second == 5) or (now.second == 10) or (now.second == 15) or (now.second == 20) or (now.second == 25) or (now.second == 30) or (now.second == 35) or (now.second == 40) or (now.second == 45) or (now.second == 50) or (now.second == 55):
                frequency = fcount.get_freq()
                if (len(sys.argv) == 3):
                    response = rp.read(10)
                    regs = fgen.set_freq(frequency/1e6, float(eval(response)))              
                now = datetime.datetime.now()

            rfreq = fgen.get_rfreq()
            hsdiv = fgen.get_hs_div()
            n1 = fgen.get_n1_div()
            fdco = (frequency/1e6) * hsdiv * n1
            fxtal = fdco / rfreq 
            sys.stdout.write("frequency: " + str(frequency) + " Hz  Time: " + str(now.second))
            sys.stdout.write(" RFREQ: " + str(rfreq) + " HSDIV: " + str(hsdiv) + " N1: " + str(n1))
            sys.stdout.write(" fdco: " + str(fdco) + " fxtal: " + str(fxtal) + "\r")
            f.write("%d\t%s\t%.3f\n" % (time.time(), datetime.datetime.now().isoformat(), frequency))

            sys.stdout.flush()
            time.sleep(0.9)
except KeyboardInterrupt:
    sys.stdout.write("\r\n")
    sys.exit(0)
    f.close()
