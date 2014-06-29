# SatelliteObserver

Software for capturing data from CubeSat like satellites. 

## Instalation
### Dependencies

SatelliteObserver needs: Boost (http://www.boost.org/doc/libs/1_55_0/doc/html/bbv2/installation.html)

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
******************* **Follow ALL steps.**


## Usage

###Configuration
#### SpaceTrack account
If you want to have updated the TLE files you must create an account SpaceCrack.

* https://www.space-track.org/auth/login

#### Configuration file
You must edit the configuration file /sat-observer/Resources/conf/default.ini
and put it in your **executable directory** with the name **default.ini**

A configuration file example:
            
            
            [TLE]
            #FileSource=server or FileSource=local
            FileSource = server
            
            #Server=spacetrack
            Server = spacetrack
            #Where the TLE file will be saved
            Local_path = /home/carlos/Escritorio/mlab/Resources/TLE/boxscore
            
            [SPACETRACK]
            User = carlosruiznaranjo@gmail.com
            Pass = esasummerofcodeinspace
            #List=amateur, List=all
            List = amateur
            #NORAD satellite number
            Number = 27844
            
            
            [POSITION]
            Lat = 55.6167
            Long = 12.65
            Hight = 0
            
            [TIME]
            #GMT +2
            Hour = +2
            
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
* Execute the program:

    ```
        ./sat-observer
    ```











