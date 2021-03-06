//
// Copyright (c) 2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include <stdio.h>
#ifdef NANOVG_GLEW
#  include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg_gl.h"
#include "demo.h"
#include "perf.h"


void errorcb(int error, const char* desc)
{
	printf("GLFW error %d: %s\n", error, desc);
}

int blowup = 0;
int screenshot = 0;
int premult = 0;

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	NVG_NOTUSED(scancode);
	NVG_NOTUSED(mods);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		blowup = !blowup;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		screenshot = 1;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		premult = !premult;
}

#define CACHE 1

int main()
{
	GLFWwindow* window;
	DemoData data;
	NVGcontext* vg = NULL;
	PerfGraph fps;
	double prevt = 0;
	
#if CACHE
    int runs = 0;
	float r = 0;
	NVGdisplayList* cache = NULL;
#endif


	if (!glfwInit()) {
		printf("Failed to init GLFW.");
		return -1;
	}

	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

	glfwSetErrorCallback(errorcb);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#ifdef DEMO_MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);
#endif

	window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);
//	window = glfwCreateWindow(1000, 600, "NanoVG", glfwGetPrimaryMonitor(), NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key);

	glfwMakeContextCurrent(window);
#ifdef NANOVG_GLEW
    if(glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
		return -1;
	}
#endif

#ifdef DEMO_MSAA
	vg = nvgCreateGL2(NVG_STENCIL_STROKES | NVG_DEBUG);
#else
	vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif
	if (vg == NULL) {
		printf("Could not init nanovg.\n");
		return -1;
	}

	if (loadDemoData(vg, &data) == -1)
		return -1;

	glfwSwapInterval(0);

	glfwSetTime(0);
	prevt = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		double mx, my, t, dt;
		int winWidth, winHeight;
		int fbWidth, fbHeight;
		float pxRatio;

		t = glfwGetTime();
		dt = t - prevt;
		prevt = t;
		updateGraph(&fps, dt);

		glfwGetCursorPos(window, &mx, &my);
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// Calculate pixel ration for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;

		// Update and render
		glViewport(0, 0, fbWidth, fbHeight);
		if (premult)
			glClearColor(0,0,0,0);
		else
			glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

#if CACHE
		if (runs++ % 60 == 0) //draw to display list
		{
			if (!cache)
				cache = nvgCreateDisplayList(-1);
			
			nvgGlobalAlpha(vg, 1.0f); //draw to cache at full alpha, we can modify this later
            
            nvgResetDisplayList(cache);
			nvgBindDisplayList(vg, cache);
            
			renderDemo(vg, mx, my, winWidth, winHeight, t, blowup, &data);
			
            nvgBindDisplayList(vg, NULL);
        }
        
		if (cache) //draw display list to screen with custom transform
		{
			float scale = 0.8f;
			nvgTranslate(vg, winWidth/2, winHeight/2);
			scale = scale + ((1-scale) * (sinf(t)/(3.14f)));
			nvgScale(vg, scale, scale);
			
			nvgRotate(vg, r += 0.2f * dt);
			nvgTranslate(vg, -winWidth/2 , -winHeight/2 );
            //nvgGlobalAlpha(vg, scale);
			
			nvgDrawDisplayList(vg, cache);
            
			nvgResetTransform(vg);
		}
#else
		renderDemo(vg, mx,my, winWidth,winHeight, t, blowup, &data);
#endif
		
		renderGraph(vg, 5,5, &fps);

		nvgEndFrame(vg);

		if (screenshot) {
			screenshot = 0;
			saveScreenShot(fbWidth, fbHeight, premult, "dump.png");
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	freeDemoData(vg, &data);

#if CACHE
	if (cache)
		nvgDeleteDisplayList(cache);
#endif

	nvgDeleteGL2(vg);

	glfwTerminate();
	return 0;
}
