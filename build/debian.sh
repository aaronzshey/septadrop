TARGET=septadrop_1.0_amd64
./build.sh
mkdir -p ${TARGET}/usr/bin/
cp septadrop ${TARGET}/usr/bin/
dpkg-deb --build ${TARGET}