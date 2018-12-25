
# PlatON mpc vm

- [PlatON mpc vm](#platon-mpc-vm)
    - [Directory Structure](#directory-structure)
    - [Building & Installing](#building--installing)
    - [Dependencies](#dependencies)


## Directory Structure

- [cmake](cmake/README.md)
- [docs](docs/README.md)
- [src](src/README.md)
- [third-code](third-code/README.md)
- [test](test/README.md)
- [tools](tools/README.md)
- [script](script/README.md)


## Building & Installing

On Ubuntu 16.04 TLS.

```bash
sudo apt install gcc g++ cmake git zlib1g-dev libssl-dev bzip2 libexpat1-dev libdb++-dev libgmp-dev libcurl4-openssl-dev libboost-all-dev
```

On CentOS 7.5.1804

```bash
sudo yum install gcc g++ cmake git zlib-devel openssl-devel bzip2 expat-devel libdb-cxx-devel gmp-devel libcurl-devel boost-devel
```

In general,

1. Install [dependencies](#dependencies).
2. Config ./config/mpc_service_config.h.in.
3. Run follow command.

**Note**: cmake version should be not less than 3.0.2

Here are the detail commands:

  - install related dependencies:
    ```
    > bash install_deps.sh
    ```

  - instlall the platon mpc core
    ```
    > mkdir build
    > cd build
    > cmake .. -DOPENSSL_ROOT_DIR=/path/to/openssl -DBOOST_INCLUDEDIR=/path/to/boost/include
    > make
    > sudo make install
    ```


## Dependencies

PlatON MPC depends on several modules, these modules can be installed simple with: ```bash install_deps.sh```. 

These modules are listed below:

**miracl**

Default supported.

If you want to test miracl,
    
    > cd test/miracl-test
    > ./gen_test.sh


**relic**

The relic is not originly ont support on window, currently it only supported on linux.

Here are the intructions to install the relic:
    ```
	> cd third-code/relic
    > mkdir build && cd build && cmake -DALIGN=16 -DARCH=X64 -DARITH=curve2251-sse -DCHECK=off -DFB_POLYN=251 -DFB_METHD="INTEG;INTEG;QUICK;QUICK;QUICK;QUICK;LOWER;SLIDE;QUICK" -DFB_PRECO=on -DFB_SQRTF=off -DEB_METHD="PROJC;LODAH;COMBD;INTER" -DEC_METHD="CHAR2" -DCOMP="-O3 -funroll-loops -fomit-frame-pointer -march=native -msse4.2 -mpclmul" -DTIMER=CYCLE -DWITH="MD;DV;BN;FB;EB;EC" -DWSIZE=64 ..
    > make && make install
    ```

Some useful issues:
[how to support for Visual Studio 2015+](https://github.com/relic-toolkit/relic/pull/45).

If you want to use relic instead of `miracl`, usage:
    
    -DOT_NP_USE_MIRACL=1

**openssl**

If `Could NOT find OpenSSL`, 
try to set the path to OpenSSL root folder in the system variable OPENSSL_ROOT_DIR,
such as:

    -DOPENSSL_ROOT_DIR=/path/to/openssl

    
**boost**

Boost libraries are required. 
On Ubuntu:
```
sudo apt install libboost-all-dev
```

On CentOS:

```
sudo yum install boost-devel
```

**Note**:
On CentOS the version of boost installed by yum is almost lower than 1.66.0, so you may have to upgrade the boost to 1.66.0.

Especially On Windows, cmake should run with macro define:
```bash
    -DBOOST_ROOT=/path/to/boost
OR
    -DBOOST_INCLUDEDIR=/path/to/boost/include
```

**zeroc-ice**

In order to be compatible, use 
[v3.6.x](https://github.com/zeroc-ice/ice/tree/3.6).

For C++, compile on windows, please
[readme](https://github.com/zeroc-ice/ice/blob/3.6/cpp/BuildInstructionsWindows.md).

To install the zeroc-ice pre-built version 3.6, you should visit
  - Ubuntu
    https://zeroc.com/downloads/ice/3.6#ubuntu
  - Redhat
    https://zeroc.com/downloads/ice/3.6#redhat
  - Windows
    https://zeroc.com/downloads/ice/3.6#windows

After install the zeroc-ice, the system variable `ICE_ROOT` should be well configured to where the zeror-ice installed .

--------

On windows, when you attend to compile `mpc_* projects` in 
both `Win32 and x64`(**Win32-Debug|Win32-Release|x64-Debug|x64-Release**),
you should produce both 32-bit(**Debug|Release**) and 64-bit(**Debug|Release**) binaries, 
and then combine them. The directory structure looks like this:
```
.
|   
+---bin
|   |   *.dll
|   |   *d.dll
|   |   *.exe
|   |   
|   \---x64
|           *.dll
|           *d.dll
|           *.exe
|       
+---include/*.h
|           
+---lib
|   |   *.lib
|   |   *d.lib
|   |   
|   \---x64
|           *.lib
|           *d.lib
```


**Here are some other deps in submodules**

- [gfags v2.2.1](https://github.com/gflags/gflags/tree/v2.2.1).
- [googletest v1.8.1](https://github.com/abseil/googletest/tree/release-1.8.1).
- [protobuffer v3.6.1](https://github.com/protocolbuffers/protobuf/tree/v3.6.1).
- [libscrypt](https://github.com/technion/libscrypt/tree/a402f4116245ce8677b3d9f4f87096b5ccbe26e9).


**Here are some other deps in special**

- secp256k1 from aleth.
- eth-simplest.


**Here are some other deps in alone**

- [libjson-rpc-cpp v1.1.1](https://github.com/cinemast/libjson-rpc-cpp/tree/v1.1.1) and it's [dependencies](https://github.com/cinemast/libjson-rpc-cpp/tree/v1.1.1#install-the-dependencies).
- [cryptopp v5.6.5](https://github.com/weidai11/cryptopp/tree/CRYPTOPP_5_6_5).

You can install them alone, or run [install_deps.sh](install_deps.sh) on linux.
