#include <windows.h>
#include <wingdi.h>

#include <memory>

#include "gl.h"
#include "gl/wglext.h"

#include "sb/application.h"
#include "sb/chapter2.h"
#include "sb/chapter3.h"


PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;


#define GL_FUNC(type, name) type name;
#include "glfunctions.def"

bool g_running = false;


LRESULT CALLBACK Win32WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
bool LoadGLFunctions();

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE _prev, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = Win32WindowProc;
    windowClass.hIcon = NULL;
    windowClass.hCursor = NULL;
    windowClass.lpszClassName = "OpenGL Window Class";
    windowClass.hIconSm = NULL;

    auto registered = RegisterClassEx(&windowClass);
    if (!registered)
    {
        return 1;
    }

    HWND windowHandle = CreateWindow(
        "OpenGL Window Class",
        "OpenGL",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800,
        600,
        0, 0,
        instance,
        0
    );

    if (!windowHandle)
    {
        return 2;
    }

    HDC deviceContext = GetDC(windowHandle);
    if (!deviceContext)
    {
        return 3;
    }

    PIXELFORMATDESCRIPTOR desiredPixelFormat = {};
    desiredPixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    desiredPixelFormat.nVersion = 1;
    desiredPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    desiredPixelFormat.cColorBits = 24;
    desiredPixelFormat.cAlphaBits = 8;
    desiredPixelFormat.cAccumBits = 0;
    desiredPixelFormat.cDepthBits = 24;
    desiredPixelFormat.cStencilBits = 8;

    int pixelFormatResult = ChoosePixelFormat(deviceContext, &desiredPixelFormat);
    if (!pixelFormatResult)
    {
        return 4;
    }

    BOOL isPixelFormatSet = SetPixelFormat(deviceContext, pixelFormatResult, &desiredPixelFormat);
    if (!isPixelFormatSet)
    {
        return 5;
    }

    HGLRC temporaryRenderContext = wglCreateContext(deviceContext);
    if (!temporaryRenderContext)
    {
        return 6;
    }

    BOOL isTempCurrent = wglMakeCurrent(deviceContext, temporaryRenderContext);
    if (!isTempCurrent)
    {
        return 7;
    }

    // TODO (Emil): Change this to use glGetIntegerv(GL_NUM_EXTENSIONS) and glGetStringi(GL_EXTENSIONS, i).
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
    if (!wglGetExtensionsStringARB)
    {
        return 8;
    }

    const char * extensions = wglGetExtensionsStringARB(deviceContext);

    const char * hasWglArbCreateContext = strstr(extensions, "WGL_ARB_create_context");
    const char * hasWglArbCreateContextProfile = strstr(extensions, "WGL_ARB_create_context_profile");

    if (!hasWglArbCreateContext || !hasWglArbCreateContextProfile)
    {
        return 9;
    }

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(temporaryRenderContext);

    pixelFormatResult = ChoosePixelFormat(deviceContext, &desiredPixelFormat);
    if (!pixelFormatResult)
    {
        return 10;
    }

    isPixelFormatSet = SetPixelFormat(deviceContext, pixelFormatResult, &desiredPixelFormat);
    if (!isPixelFormatSet)
    {
        return 11;
    }

    int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
        NULL, NULL
    };

    auto renderContext = wglCreateContextAttribsARB(deviceContext, NULL, attributes);
    if (!renderContext)
    {
        return 12;
    }

    BOOL isCurrent = wglMakeCurrent(deviceContext, renderContext);
    if (!isCurrent)
    {
        return 13;
    }

    bool loaded = LoadGLFunctions();
    if (!loaded)
    {
        return 14;
    }

    g_running = true;

    auto app = std::make_unique<Chapter3_7>();

    ShowWindow(windowHandle, nCmdShow);

    GLint number;
    glGetIntegerv(GL_NUM_EXTENSIONS, &number);

    for (GLuint i = 0; i < (GLuint)number; ++i)
    {
        const GLubyte * extension = glGetStringi(GL_EXTENSIONS, i);

        int j = i + 2;
    }

    app->startup();

    double time = 0.0;

    while (g_running)
    {
        MSG message;
        while (PeekMessage(&message, windowHandle, 0, 0, PM_REMOVE))
        {
            switch (message.message)
            {
                case WM_KEYDOWN:
                {
                    if (message.wParam == VK_ESCAPE)
                    {
                        PostQuitMessage(0);
                        g_running = false;
                    }
                } break;
                default:
                {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                } break;
            }
        }

        time += 0.0005;

        app->render(time);

        SwapBuffers(deviceContext);
    }

    app->shutdown();

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(renderContext);

    return 0;
}

LRESULT CALLBACK Win32WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
        case WM_QUIT:
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            g_running = false;
        } break;
        default:
        {
            return DefWindowProc(windowHandle, message, wParam, lParam);
        } break;
    }

    return 0;
}

bool LoadGLFunctions()
{
    HMODULE dll = LoadLibrary("opengl32.dll");
    if (!dll)
    {
        return false;
    }

    auto LoadFunction = [dll](const char * functionName) -> void *
    {
        void * proc = wglGetProcAddress(functionName);

        if (
            proc == 0
            || proc == (void *)0x1
            || proc == (void *)0x2
            || proc == (void *)0x3
            || proc == (void *)-1
        )
        {
            proc = (void *)GetProcAddress(dll, functionName);
        }

        if (!proc)
        {
            return nullptr;
        }

        return proc;
    };

    #define GL_FUNC(type, name)           \
        name = (type)LoadFunction(#name); \
        if (!name)                        \
        {                                 \
            return false;                 \
        }
    #include "glfunctions.def"

    FreeLibrary(dll);

    return true;
}
