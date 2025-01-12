#ifndef QVTKITEM_H
#define QVTKITEM_H

#include <QObject>
#include <QString>
#include <QQuickFramebufferObject>
#include "QVtkRenderer.h"
#include "DisplayProperties.h"

namespace mb_system {
  /**
     QVtkItem and QVtkRenderer coordinate with one another to
     render VTK scenes within a QQuickItem specified in QML. A QVtkItem object
     is created when specified in QML, and the accompanying QVtkRenderer is 
     created when QVtkItem's createRenderer() function is invoked by the Qt 
     framework. The QVtkItem object code runs in the main thread, is 
     responsible for accepting user input such as mouse zoom, rotate, etc, 
     and making that input available to its accompanying QVtkRenderer object,
     which runs in the renderer thread.
     See https://www.qt.io/blog/2015/05/11/integrating-custom-opengl-rendering-with-qt-quick-via-qquickframebufferobject
  */
  class QVtkItem : public QQuickFramebufferObject
  {
    Q_OBJECT

    Q_PROPERTY(QString pickedPoint READ getPickedPoint
               WRITE setPickedPoint NOTIFY pickedPointChanged)

    /// Indicate whether app task is busy
    Q_PROPERTY(bool busy READ getAppBusy WRITE setAppBusy NOTIFY busyChanged)

    
  public:

    QVtkItem();

    /// Create and return renderer.
    /// Called by Qt framework, overrides
    /// QQuickFramebufferObject::createRenderer()
    QQuickFramebufferObject::Renderer *createRenderer() const override;

    /// Set grid file name
    void setGridFilename(char *gridFilename);

    /// Get name of grid file
    char *getGridFilename() {
      return gridFilename_;
    }

    /// Return latest wheel event.
    /// Called by the QVtkRenderer during QVtkRenderer::synchronize()
    QWheelEvent *latestWheelEvent() {
      return wheelEvent_.get();
    }

    /// Return latest mouse button press event.
    /// Called by the QVtkRenderer during QVtkRenderer::synchronize()  
    QMouseEvent *latestMouseButtonEvent() {
      return mouseButtonEvent_.get();
    }

    /// Return latest mouse move event.
    /// Called by the QVtkRenderer during QVtkRenderer::synchronize()  
    QMouseEvent *latestMouseMoveEvent() {
      return mouseMoveEvent_.get();
    }

    /// Get display properties
    const DisplayProperties *displayProperties() {
      return &displayProperties_;
    }

    /// Toggle axes display
    void showAxes(bool show) {
      displayProperties_.showAxes = show;
    }

    /// Vertical exaggeration
    void setVerticalExagg(float verticalExagg) {
      qDebug() << "setVerticalExagg() " << verticalExagg;
      displayProperties_.verticalExagg = verticalExagg;
    }
    

  signals:

    /// User picked a point on the vtk surface
    void pickedPointChanged(QString msg);

    void busyChanged(bool busy);

  public slots:

  public:
    
    /// Set user-picked point coordinates
    void setPickedPoint(QString msg);

    /// Get string representation of picked point
    QString getPickedPoint();

    /// Set app busy status
    void setAppBusy(bool busy);

    /// Get app busy status
    bool getAppBusy();
    
    
  protected:

    /// Display properties, e.g. visible axes, etc.
    DisplayProperties displayProperties_;

    /// Handle mouse wheel event
    virtual void wheelEvent(QWheelEvent *event) override;

    /// Handle mouse button press event
    virtual void mousePressEvent(QMouseEvent *event) override;

    /// Handle mouse button release event
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    /// Handle mouse move event
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    
    
    /// Name of associated grid file
    char *gridFilename_;
 
    /// Latest wheel event
    std::shared_ptr<QWheelEvent> wheelEvent_;

    /// Latest mouse button event
    std::shared_ptr<QMouseEvent> mouseButtonEvent_;

    /// Latest mouse move event
    std::shared_ptr<QMouseEvent> mouseMoveEvent_;

    /// Latest user-picked point coordinates
    QString pickedCoords_;

    /// Indicate to gui whether app task is busy
    bool appTaskBusy_;
  };


}

#endif // QVTKITEM_H
