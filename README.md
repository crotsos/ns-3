ns-3
====

INSTALL
====
sudo apt-get build-dep ns-3
sudo apt-get install valgrind libgtk2.0-dev

sudo apt-get install bzr 
bzr branch lp:pybindgen

sudo apt-get install gccxml
cd pybindgen/
./waf configure
./war 
./war install

cd ..
./waf configure --enable-tests --enable-examples


