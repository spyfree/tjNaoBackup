/**
* @file OffscreenRenderer.h
* 
* Declaration of class OffscreenRenderer.
*
* @author Colin Graf
*/

#ifndef OffscreenRenderer_H
#define OffscreenRenderer_H

class GraphicsManager;
// class QGLPixelBuffer;
// class QGLWidget;

/**
* @class OffscreenRenderer
* A hardware accelerated offscreen rendering module that uses the Qt 4 OpenGL library.
*/
class OffscreenRenderer
{
public:
  OffscreenRenderer(GraphicsManager* graphicsManager);
  ~OffscreenRenderer();

  enum Content
  {
    IMAGE, /**< BGR 3 byte align */
    IMAGE_RGBA, /**< 4 byte align */
    DEPTH,/**< float */
  };

  void prepareRendering(int width, int height);
  void finishRendering(void* image, int width, int height, Content content);

private:
  GraphicsManager* graphicsManager;
//   QGLPixelBuffer* pbuffer;
//   QGLWidget* glWidget;
  int width;
  int height;
  char* flippingBuffer;
  bool useOffset;

  bool makeCurrent();
  bool doneCurrent();
  void determineImagePositionInBuffer(int width, int height);
};

#endif
