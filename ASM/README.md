# OoT Redux Assembly

- Download [armips](https://github.com/Kingcom/armips/releases/latest) and [MakePPF3](https://www.romhacking.net/utilities/353/) binaries and put them into the `tools` directory.
- Put the ROM you want to patch at `roms/base.z64`. This needs to be an uncompressed ROM; OoTRandomizer will produce one at ZOOTDEC.z64 when you run it with a compressed ROM.
- Run `python build.py`, which will:
  - create `roms/patched.z64`
  - update some `txt` files in `build/` and in `../data/generated/`. Check `git status` to see which ones have changed. Make sure you submit them all together!

To recompile the C modules, use the `--compile-c` option. This requires the N64 development tools to be installed: <https://github.com/glankk/n64>

To generate symbols for the Project 64 debugger, use the `--pj64sym` option:

```
python build.py --pj64sym 'path_to_pj64/Saves/THE LEGEND OF ZELDA.sym'
```

## Building (MSYS2)

Download and install [MSYS2](https://www.msys2.org/). 

Run `MSYS2 MINGW32`:

```
pacman -S git python
git clone https://github.com/glankk/n64 n64
cd n64
./install_deps
./configure --prefix=/opt/n64 --enable-vc
make install-toolchain -j 10
make && make install
make install-sys
make install-local-exec
export PATH=$PATH:/opt/n64/bin
```

Open `.bashrc` in `MSYS2/home/USER`, add to file and save:

```
export PATH=$PATH:"/opt/n64/bin"
```

Re-open `MSYS2 MINGW32` and run:

```
./build.sh
```
