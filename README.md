```sh
cd third_party
unzip linux_include.zip

cd build
cmake ..
make 

cd bin
link -s ../conf ./
```