#!/usr/bin/python
# 
# Sample of measuring and frequency correction with ACOUNTER02A
#Satellite Observer. 
#The main objective of Satellite Observer is the autonomous capture 
#of signal from small satellites and the subsequent 
#decoding. It  keeps track of the trajectory, continuously updated of 
#Doppler shift and tunes the receiver.
#
#Copyright (C) 2014  Carlos Alberto Ruiz Naranjo
#carlosruiznaranjo@gmail.com

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU Affero General Public License as
#published by the Free Software Foundation, either version 3 of the
#License, or (at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Affero General Public License for more details.

#You should have received a copy of the GNU Affero General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses


import time
import datetime
import sys
import pipes
import os
from pymlab import config

#### Script Arguments ###############################################

if (len(sys.argv) > 2) or (len(sys.argv) < 1):
    sys.stderr.write("Invalid number of arguments.\n")
    sys.stderr.write("Usage: %s PORT_ADDRESS\n" % (sys.argv[0], ))
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
#frequency = fcount.get_freq()
#rfreq = fgen.get_rfreq()
#hsdiv = fgen.get_hs_div()
#n1 = fgen.get_n1_div()
rfPath = "/tmp/satObPIPE"
#os.mkfifo(rfPath)
rp = open(rfPath, 'r')
#fdco = 0
#fxtal = 0
#regs = [0, 0, 0]


#### Data Logging ###################################################

try:
    with open("frequency.log", "a") as f:
        lockk=0
        lockk15=0
        lockk35=0
        lockk55=0
        
        l15=0
        l35=0
        l55=0
        
        timee=0
        newtime=0
        
        frequency=0
        
        while True:
            now = datetime.datetime.now()
            newtime = now.second
            response = rp.read(14)
            if ( (now.second == 15) and lockk15==0 and lockk!=2 and lockk!=3):
                timee = now.second
                frequency = fcount.get_freq()
                if (len(sys.argv) == 2):
                    regs = fgen.set_freq(frequency/1e6, float(eval(response)))
                sys.stdout.write("frequency: " + str(frequency) + " Hz  Time: " + str(now.second) + "\n")
                frequency=float(eval(response))
                lockk=lockk+1
                lockk15=1
            if ( (now.second == 35) and lockk35==0 and lockk!=2 and lockk!=3):
                timee = now.second
                frequency = fcount.get_freq()
                if (len(sys.argv) == 2):
                    regs = fgen.set_freq(frequency/1e6, float(eval(response)))
                sys.stdout.write("frequency: " + str(frequency) + " Hz  Time: " + str(now.second) + "\n")
                frequency=float(eval(response))
                lockk=lockk+1
                lockk35=1
            if ( (now.second == 55) and lockk55==0 and lockk!=2 and lockk!=3):
                timee = now.second
                frequency = fcount.get_freq()
                if (len(sys.argv) == 2):
                    regs = fgen.set_freq(frequency/1e6, float(eval(response)))
                sys.stdout.write("frequency: " + str(frequency) + " Hz  Time: " + str(now.second) + "\n")
                frequency=float(eval(response))
                lockk=lockk+1
                lockk55=1
                
                
            if (lockk == 3 and now.second != 15 and now.second != 35 and now.second != 55):
                if (len(sys.argv) == 2):
                    regs = fgen.set_freq(frequency, float(eval(response)))              
                frequency = float(eval(response))
                #sys.stdout.write("*** frequency: " + str(frequency*1000000) + " Hz  Time: " + str(now.second) + "\r")
                #sys.stdout.flush()  
                
                               
            if ((now.second == 15) and lockk==3 and l15==0 and newtime!=timee):
				timee = -1
				fff=fcount.get_freq()/1000000
				if (len(sys.argv) == 2):
					regs = fgen.set_freq(fff, float(eval(response)))              
				frequency = float(eval(response))  
				sys.stdout.write("frequency: " + str(fff*1000000) + " Hz  Time: " + str(now.second) + "\n")
				#sys.stdout.write("frequency: " + str(fff*1000000) + " Hz  Time: " + str(now.second) + "\r")
				#sys.stdout.flush()
				l15=1
				l35=0
				l55=0
            if ((now.second == 35) and lockk==3 and l35==0 and newtime!=timee):
				timee = -1
				fff=fcount.get_freq()/1000000
				if (len(sys.argv) == 2):
					regs = fgen.set_freq(fff, float(eval(response)))              
				frequency = float(eval(response))  
				sys.stdout.write("frequency: " + str(fff*1000000) + " Hz  Time: " + str(now.second) + "\n")
				#sys.stdout.write("frequency: " + str(fff*1000000) + " Hz  Time: " + str(now.second) + "\r")
				#sys.stdout.flush()
				l15=0
				l35=1
				l55=0
            if ((now.second == 55) and lockk==3 and l55==0 and newtime!=timee):
				timee = -1
				fff=fcount.get_freq()/1000000
				if (len(sys.argv) == 2):
					regs = fgen.set_freq(fff, float(eval(response)))              
				frequency = float(eval(response))  
				sys.stdout.write("frequency: " + str(fff*1000000) + " Hz  Time: " + str(now.second) + "\n")
				#sys.stdout.write("frequency: " + str(fff*1000000) + " Hz  Time: " + str(now.second) + "\r")
				#sys.stdout.flush()
				l15=0
				l35=0
				l55=1
				
            if (lockk==2):
                lockk=3
            time.sleep(0.4)
except KeyboardInterrupt:
    sys.stdout.write("\r\n")
    sys.exit(0)
    f.close()
