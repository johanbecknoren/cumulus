#ifndef _FBO_H_
#define _FBO_H_

#include <GL/glew.h>
#include <GL/glfw.h>


class Fbo {
public:
    Fbo(){};
    Fbo(int width, int height, int int_method);
    ~Fbo(){};

    //static void useDepthFbo(Fbo *out, Fbo *in);

	const int getWidth() { return width; }
	const int getHeight() { return height; }

    static void useFbo(Fbo *out, const Fbo *in, const  Fbo *in2);
    static void useArrayFbo(Fbo *out, const Fbo *in);
    static void useFbo(Fbo *out, const  Fbo *in, const  Fbo *in2, const  Fbo *in3);
    static void useDepthFbo(Fbo *out, const Fbo *in);

    const GLuint getDepthId() const { return depth; };
    const GLuint getTexId() const { return texid; };

private:
    GLuint texid;
    GLuint fb;
    GLuint rb;
    GLuint depth;
    int width, height;

    static int lastw;
    static int lasth;

};
#endif