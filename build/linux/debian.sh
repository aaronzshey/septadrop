TARGET=septadrop_1.0_amd64
./build.sh
cp septadrop ${TARGET}/usr/bin/
dpkg-deb --build ${TARGET}