# DCMAKE_EXPORT_COMPILE_COMMANDS=ON generates compile_commands.json in build folder,
# which clangd will pick up
# Without this, clangd might assume the wrong C++ version
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
python3 packer.py
cmake --build .