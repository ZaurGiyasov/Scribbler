/*!
    SymbolDataEditor - class representing an editor of the symbol data.
    It is included into the Font Editor window.

    The handwritten font is a folder with SVG images and INI file that
    contains information about the associations of characters and images,
    as well as some additional data. This data includes the limits,
    inPoint and outPoint. Limits show part of the symbol, which part of
    the symbol must be within the line. It's a rectangle with a height
    equal to the height of the line and the width equal to the width of
    the character. InPoint and outPoint are the points of entry and exit
    of connecting lines for the letter.

    Data is not stored in absolute coordinates, it's stored as a relative
    position to the image side

    This class allows to set additional data by moving markers of points
    or changing rectangle of limits. It also changes the cursor to make
    editing easier for user and converts data from absolute to relative
    coordinates.
*/
#ifndef SVGDATAEDITOR_H
#define SVGDATAEDITOR_H

#include <QtCore/QtMath>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsItem>
#include <QtSvg/QGraphicsSvgItem>
#include <QtSvg/QSvgRenderer>
#include <QtGui/QWheelEvent>

#include "svgview.h"

class SymbolDataEditor : public QGraphicsView
{
    Q_OBJECT

public:
    explicit SymbolDataEditor(QWidget *parent = 0);
    ~SymbolDataEditor();

    void load(const QString & fileName); //!< loads specified item
    void setSymbolData(const QPointF _inPoint, const QPointF _outPoint, const QRectF _limits);
    void clear();
    void disableChanges();
    QPointF getInPoint() const {return toStored(inPoint);}
    QPointF getOutPoint() const {return toStored(outPoint);}
    QRectF getLimits() const {return QRectF(toStored(limits.topLeft()),
                                            toStored(limits.bottomRight()));}

public slots:
    void enableInPointChanges();
    void enableOutPointChanges();
    void enableLimitsChanges();

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    enum Item : int {
        SymbolItem,
        InPoint,
        OutPoint,
        LimitsRect,
        NoItem
    };

    enum Side : int {
        NoSide = 0x00,
        Top = 0x01,
        Bottom = 0x02,
        Left = 0x04,
        Right = 0x08,
        AllSides = 0x10
    };
    const qreal maxScaleFactor = 40, minScaleFactor = 0.1, sceneScale = 5;
    QGraphicsScene *scene;
    qreal currentScaleFactor;
    Item itemToChange;
    Side sideToChange; //for Item::LimitsRect
    qreal pointWidth;
    bool setupPoints;
    QPointF inPoint, outPoint, dLimitsCenter;
    QRectF limits;
    QDomDocument doc;

    void limitScale(qreal factor);  //!< limited view zoom
    void loadSettings();
    QPointF toStored(const QPointF &point) const;
    QPointF fromStored(const QPointF &point) const;
    QPointF fromViewBox(const QPointF &point) const; //!< used to get points by automatic setup
    void moveItem(const QPoint pos);
    void calculateSideToChange(QPoint pos);
    void changeCursor();
    void rememberChanges();
    void correctLimits();
    void addDataItems();
    QPointF getBeginPoint();
    QPointF getEndPoint();
    QPointF getTranslatePoint();
    QPointF getMovePoint(const QString &path);
    QPointF getLastCurvePoint(const QString &path);
    QPointF getLinePoint(const QString &path);
    QStringList getPathList();
};

#endif // SVGDATAEDITOR_H
