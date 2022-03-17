TARGET=septadrop_*_amd64
./build.sh
mkdir -p ${TARGET}/usr/bin/
cp septadrop ${TARGET}/usr/bin/
dpkg-deb --build ${TARGET}