Torrijas
========

Torrijas is a multi-platform C++11 library for building 2d games and other graphical applications with vector graphics. 

It is based on [NanoVG](http://github.com/memononen/nanovg) for the graphics rendering. 
It works on Linux, OS X and Windows.

![screenshot of some text rendered with the sample program](/torrijas-test/images/screenshot.png?raw=true)

Usage
=====

The Torrijas API is modeled loosely on Cocos2d API. If you know Cocos2d-x, you're up to speed with Torrijas in no time.

## Build Requirements

* Ubuntu 14.04+ with gcc 4.8+
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

## Expected build folder

By default, Torrijas expects to be built in a child folder of the main torrijas folder.

If you build it elsewhere, you'll get a nice crash because it isn't going to find the default font file.

You can change the default font file path in the ApplicationConfig class.

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
* OpenGL 2+ based

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

## Change log

v0.1.2

* OPTIMIZATION: Images with opacity are now rendered with display lists if they are enabled.
* OPTIMIZATION: Display lists are released when they are not needed anymore.
* FIX: Text with display lists enabled can be rendered incorrectly if the glyph atlas grows.
* OTHER: Expected build folder info added to readme.
* OTHER: Some CLion warnings removed.

v0.1.1

* FIX: Linux build.

v0.1

* Initial release.

