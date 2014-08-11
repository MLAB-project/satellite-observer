# SatelliteObserver

Software for capturing data from CubeSat like satellites. 

## Instalation
### Dependencies

* Boost (http://www.boost.org/doc/libs/1_55_0/doc/html/bbv2/installation.html)

* MLAB-I2c-modules: https://github.com/MLAB-project/MLAB-I2c-modules

### How to
* Download the source.
* Go to /sat-observer:
```
    cd /you_download_directory/sat-observer
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

#### Configuration file
You must edit the configuration file /sat-observer/install/default.ini

A configuration file example (you must fill all):
            
            [mlab]

            [TLE]
            #FileSource=server or FileSource=local
            FileSource = server
            
            #Server=spacetrack
            Server = spacetrack
            #Where the TLE file will be saved and name or the local path
            Local_path = /home/carlos/Escritorio/satellite-observer/Resources/TLE/boxscore
            
            [SPACETRACK]
            #You space-track user
            User = mail@gmail.com
            Pass = password
            #List=amateur, List=all
            List = all
            #NORAD satellite number
            Number = 39438
            
            
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
            #Where will be save the capture file (directory)
            Directory = /home/odroid/Bolidozor/samples
            Frequency = 145.98
            
* FileSource 
    * server: if you want use the SpaceTrack TLE files. (Recomended)
    * local: if you want use a local TLE file.
* Server
    * Only avaiable SpaceTrack.
* LocalPath
    * Where the TLE file will be download or where the TLE file is.
* User
    * Your SpaceTrack user.
* Password
    * Your SpaceTrack pass.
* List
    * amateur: will be use a list of amateur satellites for search the satellite.
    * all: satellite will be search in the todo list.
* Lat:
    * Latitude of station.
* Long:
    * Longitude of station.
* Hight:
    * Hight of station.
* Hour:
    * Your time zone **(of your operative system)**. For example, if you are in Spain in summer your time zone is: UTC/TMT +2. Then:  
    
        
            Hour = +2
        
#### Running sat-observer
* Go to the install directory:
```
        cd /you_download_directory/sat-observer/install
```

* Open a terminal and execute the frequency script (receiver port as an argument):

    ```
        ./frequency 1
    ```
    
* Open another terminal and execute the program:

    ```
        ./sat-observer
    ``` 











