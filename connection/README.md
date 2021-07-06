

run req:  
   SSE3 hardware  
linux - `libcrypto` (openssl package)  
windows - `libcrypto-1_1-x64.dll`  ( ... )

build req:  
 - windows  
ms visual studio(C++) 2012

https://github.com/pocoproject/openssl/tree/develop/build/win64/bin/release  
https://github.com/openssl/openssl/tree/master/include  
, copy `.lib` to say  `C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib\x64`  
also copy its .h files to `C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include\openssl`
and dlls to `C:\Windows\System32`

didnt test it with msys but nothing stops it from build there i guess   

 - linux  
`sudo apt install libssl-*`  

## How to:  

	Set initial value of  startNonce = 2083236893 and  unixtime = 1231006505 in BlockZero.cpp
    to generate original Bitcoin Block 0 hash  
windows  
vs win64 command prompt (standard command prompt works too if env tuned right ) 
```
nmake
```
libux build (for non-pc - support of `#pragma pack(1)` ) : 
```
c++ BlockZero.cpp -o BlockZero -lcrypto
```
2.
BlockZero [PubKey] [TimeStamp] [nBits]

![Screen1](/Untitled%201.jpg)
2a.  
windows
```
BlockZero 04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks" 486604799
```
linux
```
./BlockZero 04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks" 486604799
```

idk why but linux has 10% more hashrate (parrot 3.21)  
