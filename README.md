# DVD_GL
A DVD screensaver thing made in C, OpenGL (freeglut) and stb_image for loading graphics.

stb_image files were download from this repo: https://github.com/nothings/stb

files are available for the public domain, aswell are being distributed under MIT license.

## Compiling
since this is for win32, compile using mingw (or any compiler you use for windows):
```
gcc -o DVD_GL.exe window.c stb_image.c -L"C:/MinGW/freeglut/lib" -lfreeglut -lopengl32 -lglu32
```
_feel free to poke and experiment with this repository._
