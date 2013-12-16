#include "fbo.h"
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int Fbo::lastw = 0;
int Fbo::lasth = 0;



Fbo::Fbo(int width, int height, int int_method): width(width), height(height)
{
    glGenFramebuffers(1, &fb);
    glGenRenderbuffers(1, &rb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    if(false)//int_method == 0) 
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else 
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);

    // Renderbuffer
    // initialize depth renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, rb);
    //util::CHECK_FRAMEBUFFER_STATUS();

    //printf("Framebuffer object %d\n", fb);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Fbo::useFbo(Fbo *out, const Fbo *in1, const Fbo *in2)
{
    GLint curfbo;

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curfbo);
    if (curfbo == 0)
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        lastw = viewport[2] - viewport[0];
        lasth = viewport[3] - viewport[1];
    }

    if (out != 0L)
        glViewport(0, 0, out->width, out->height);
    else
        glViewport(0, 0, lastw, lasth);

    if (out != 0L)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, out->fb);
        glViewport(0, 0, out->width, out->height);
    }
    else
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE1);
    if (in2 != 0L)
        glBindTexture(GL_TEXTURE_2D, in2->texid);
    else
        glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    if (in1 != 0L)
        glBindTexture(GL_TEXTURE_2D, in1->texid);
    else
        glBindTexture(GL_TEXTURE_2D, 0);
}