Torrijas
========

Torrijas is a multi-platform C++11 library for building 2d games and other graphical applications with vector graphics. It is based on NanoVG for the graphics rendering. It works on Linux, OS X and Windows.

## Screenshot

![screenshot of some text rendered with the sample program](/torrijas-test/images/screenshot.png?raw=true)

Usage
=====

The Torrijas API is modeled loosely on Cocos2d API. If you know Cocos2d-x, you're up to speed with Torrijas in no time.

## Build Requirements

* Ubuntu 14.04+
* or Mac OS X 10.9+ with Xcode 6+
* or Windows 7+ with Visual Studio 2015+
* CMake 2.8+
* GLFW3 (in Windows it should be located in C:/glfw)
* GLEW (in Windows it should be located in C:/glew)

## Building and running tests on Linux and Mac

```
$ cd torrijas
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
$ cd torrijas-test
$ ./torrijas-test
```

## Building and running tests on Windows

Well... IT WORKS, but I have it working with Qt Creator, because my Visual Studio free license has expired u_u

## Create your own project

Make a copy of the torrijas-template folder, rename it, add it to the last line of the main CMakeLists.txt and you should be good to go.

## Main features

* Resolution independent real time vector graphics
* Scene management (parent / child nodes)
* Sprites and sprite sheets
* Text with support for .ttf fonts
* Actions (more in the future):
	* Trasformation actions: Move, Rotate, Scale, etc.
	* Composable actions: Sequence, Repeat, Reverse
	* Misc actions: CallBack
* FrameBuffer effects
* Mouse / Keyboard support
* Open Source Commercial Friendly (zlib): compatible with open and closed source projects
* OpenGL 2.1 based

## API Reference

Not yet :p

## License

The library is licensed under [zlib license](LICENSE.txt)

## Links

* Uses [NanoVG](http://github.com/memononen/nanovg) for vector graphics rendering.
* Uses [GLFW](http://www.glfw.org) for creating windows with OpenGL contexts and receiving input and events.
* Uses [stb_truetype](http://nothings.org) (or, optionally, [freetype](http://freetype.org)) for font rendering.
* Uses [stb_image](http://nothings.org) for image loading and writing.
* Uses [GLEW](http://glew.sourceforge.net/) for wrangling OpenGL extensions or something.
