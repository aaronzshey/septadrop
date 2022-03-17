TARGET=septadrop_1.0.1_amd64
./build.sh
mkdir -p ${TARGET}/usr/bin/
cp septadrop ${TARGET}/usr/bin/
dpkg-deb --build ${TARGET}