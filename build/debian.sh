TARGET=septadrop_1.1.0_amd64
mkdir -p ${TARGET}/usr/bin/
cd ..
cargo build --release
cp target/release/septadrop build/${TARGET}/usr/bin/
cd build
dpkg-deb --build ${TARGET}