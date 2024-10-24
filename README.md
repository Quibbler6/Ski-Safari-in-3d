# Project Architecture

```
├─include
│  ├─GL
│  ├─GLFW
│  └─glm
│      ├─detail
│      ├─ext
│      ├─gtc
│      ├─gtx
│      └─simd
├─lib
├─output
├─shaders
├─src
├─texture
│      ├─skybox # images that construct skybox
│      └─star # the star's texture
├─glfw3.dll # the despendency binary file
├─main.exe # the executable file
└─Makefile
```

# Compile and Run

I suppose you have already installed the mingw or the cmake, since this OpenGL project needed to be compiled by the `Makefile` I wrote.

You can use the following command to compile.

```bash
$ make 
```

You can use the following command to compile and run.

```bash
$ make run
```

You can use the following command to clean all compiled products.

```bash
$ make clean
```

By the way, I've already generated the executable file, you can directly run this `main.exe` file. But please make sure that `main.exe` and `glfw3.dll` is in the same directory. You can use the following command to directly run the executable file.

```bash
$ .\main.exe
```

