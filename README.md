# SatelliteObserver

Software for capturing signal from satellites. The satellite is identified by [Catalog Number](https://en.wikipedia.org/wiki/Satellite_Catalog_Number).

## Instalation
### Dependencies

* [Boost](http://www.boost.org/doc/libs/1_55_0/doc/html/bbv2/installation.html)

* [Pymlab library](https://github.com/MLAB-project/pymlab)

* GNURadio 3.7.5 or higher: http://gnuradio.org/redmine/projects/gnuradio/wiki/BuildGuide

### How to: NOAA decoder block 

* Download the source.
* Go to /sat-observer/gr-sat_observer:
```
    cd /your_download_directory/sat-observer/gr-sat_observer  
```
* Make the build directory:
```
    mkdir build
```
* Build the project:
```
    cd /build
    cmake ..
```
* Install:
```
    sudo make install
```


### How to: satellite-observer

```
    git clone https://github.com/MLAB-project/satellite-observer.git
    cd satellite-observer  
```
* Make the build directory:
```
    mkdir build
```
* Build the project:
```
    cd ./build
    cmake ..
```
* Install:
```
    make install
```
* You have the executable in:
```
    cd ../install
```


## Usage

###Configuration

#### SpaceTrack account
If you want to automatically download the TLE file you need an account on Space-Track:

* https://www.space-track.org/auth/createAccount

#### Server configuration file
You must edit the configuration file /sat-observer/install/default.ini

A configuration file example (you must fill all):
            
            [mlab]
            
            [CONNECTION]
            Port = 30030

            [TLE]
            #FileSource=server
            FileSource = server            
            #Server=spacetrack
            Server = spacetrack
            #DIRECTORY whre will saved the TLE files
            Local_path = /home/odroid/Bolidozor/TLE
            
            [SPACETRACK]
            #You space-track user
            User = mail@gmail.com
            Pass = password
            
            
            [POSITION]
            Lat = 49.2607
            Long = 14.6915
            Hight = 0
            
            [TIME]
            #The timezone of YOUR OPERATIVE SYSTEM
            Hour = +2
            
            [CAPTURE]
            #+- start and end of the capture (in second)
            Start = -30
            End = +30
            #Audio hardware
            Hardware = hw:1,0
            #Where the capture file (directory) will be save
            Directory = /home/odroid/Bolidozor/samples
            Frequency = 145.98
            
* FileSource 
    * server
    * local: not supported.
* Server
    * Only avaiable SpaceTrack.
* LocalPath
    * Where the TLE file will be download (/DIRECTORY)
* User
    * Your SpaceTrack user.
* Password
    * Your SpaceTrack pass.
* Lat:
    * Latitude of station.
* Long:
    * Longitude of the receiver station.
* Hight:
    * Hight of station.
* Hour:
    * Your time zone **(of your receiver system)**. For example, if you are in Spain in summer your time zone is: UTC/TMT +2. Then:

            Hour = +2
        
### Running Satellite Observer
#### Server

* Go to the install directory:
```
        cd /you_download_directory/sat-observer/install
```


* Open a terminal and execute the program:

    ```
        ./server
    ``` 
    
* Open other terminal and execute the frequency script (receiver port as an argument):

    ```
        python ./frequency.py 1
    ```
    
#### Client

* Go to the install directory:
```
        cd /you_download_directory/sat-observer/install
```

* Open other terminal and execute client:  ./client IP Port Satellite_number frequency 137.1

    ```
        ./client 127.0.0.1 30000 33591
    ```
    
#### NOAA decoder block

Satellite Observer demodulates and decodes the signal of NOAA Satellite automatically. But you can do it manually with the script: ./decoder.py

* Go to the install directory:
```
        cd /you_download_directory/sat-observer/install
```

* Open other terminal and execute the script: 

    ```
        python ./decoder.py /home/carlos/signalIn.wav /home/carlos/signalOut.wav
    ```
    
*** SignalIn format is: wav file, sample rate 48000, 2 channels, 32 bits.










