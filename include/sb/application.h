#ifndef OPENGL_APPLICATION_H
#define OPENGL_APPLICATION_H

class Application
{
public:
    virtual void startup() { };
    virtual void render(double currentTime) = 0;
    virtual void shutdown() { };
};

#endif