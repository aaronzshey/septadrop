if [[ ! -e SFML-2.5.1 ]]; then
	DOWNLOAD=TARGET=SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit
	wget https://www.sfml-dev.org/files/${DOWNLOAD}.zip
	unzip ${DOWNLOAD}.zip
	rm ${DOWNLOAD}.zip
fi
cmake SFML-2.5.1/ -DCMAKE_TOOLCHAIN_FILE=mingw-w64-x86_64.cmake ../..
cd ..
python3 packer.py
cd windows
cmake --build .
