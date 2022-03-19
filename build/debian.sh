TARGET=septadrop_1.1.0_amd64
mkdir -p ${TARGET}/usr/bin/
rm -r ${TARGET}/usr/games/septadrop/
mkdir -p ${TARGET}/usr/games/septadrop/
cd ..
cargo rustc --release -- --cfg debian
cp target/release/septadrop build/${TARGET}/usr/bin/
upx --best --lzma build/${TARGET}/usr/bin/septadrop
cp -r res/* build/${TARGET}/usr/games/septadrop/
cd build
dpkg-deb --build ${TARGET}