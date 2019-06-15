# LinuxCNC and SOEM EtherCAT lib integrated
- Embedded SOEM EtherCAT master to LinuxCNC component.
- EtherCAT documents.

# 1. SOEM Test
## 1.1 INSTALL SOEM
```
git clone https://github.com/OpenEtherCATsociety/SOEM.git
mkdir build
cd build
cmake ..
sudo make install
```

## 1.2 SIMPLE TEST
- Add line to CmakeLists.txt
```
include_directories(/usr/local/include/soem/)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
```

- Run test
```
sudo simple_test enp4s0f2
sudo ./slaveinfo enp4s0f2 -sdo -map
sudo ./red_test enp4s0f2 enp4s0f2 5000
```

## 1.3 CREATE OUR CODE
- Building C SOEM code
```
cd /home/hiep/Desktop/soem/soem_linuxcnc_embedded
mkdir build
cd build
cmake ..
make
sudo ./soem_linuxcnc_embedded
```

- Create lib .so
```
ls -l /usr/local/lib/
gcc -c -fPIC -L./lib -I./include/soem -I./include -lsoem soem_linuxcnc.c -o soem_linuxcnc.o
gcc -fPIC -shared soem_linuxcnc.o -o ./lib/libsoem_linuxcnc.so
```

- Create test C code using lib .so
```
printenv
echo $LD_LIBRARY_PATH
LD_LIBRARY_PATH=/home/hiep/Desktop/soem/hiep:$LD_LIBRARY_PATH
gcc -pthread -L./lib -I./include/soem -I./include test.c -lsoem_linuxcnc -lsoem -o test
chmod 777 test
sudo ./test
```

# 2. SOEM Linux HAL Component
## 2.1 Create LinuxCNC HAL component
- Copy all file in lib/ and include/ to linuxcnc-dev/
copy pthread.so to lib/ linuxcnc-dev/
```
source /home/hiep/linuxcnc-dev/scripts/rip-environment
halcompile --install soem_linuxcnc.comp
```

## 2.2 Test SOEM LinuxCNC HAL component
```
cd /home/hiep/linuxcnc-dev/bin
sudo chmod 4750 soem_linuxcnc
sudo chown root soem_linuxcnc
source /home/hiep/linuxcnc-dev/scripts/rip-environment
linuxcnc
```

- Machine ==> Show HAL configuration
```
loadusr soem_linuxcnc
show pin
setp soem-linuxcnc.0.set-joint-pos1 -150
setp soem-linuxcnc.0.led1 TRUE
```
