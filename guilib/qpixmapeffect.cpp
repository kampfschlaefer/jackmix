/* This file is part of the KDE libraries
    Copyright (C) 1998, 1999 Christian Tibirna <ctibirna@total.net>
              (C) 1998, 1999 Daniel M. Duley <mosfet@kde.org>
              (C) 1998, 1999 Dirk A. Mueller <mueller@kde.org>

*/

#include <qimage.h>
#include <qpainter.h>

#include "qpixmapeffect.h"
#include <qpixmap.h>
#include "qimageeffect.h"

//======================================================================
//
// Gradient effects
//
//======================================================================


QPixmap& QPixmapEffect::gradient(QPixmap &pixmap, const QColor &ca,
	const QColor &cb, GradientType eff, int ncols)
{
    if(pixmap.depth() > 8 &&
       (eff == VerticalGradient || eff == HorizontalGradient)) {

        int rDiff, gDiff, bDiff;
        int rca, gca, bca /*, rcb, gcb, bcb*/;

        register int x, y;

        rDiff = (/*rcb = */ cb.red())   - (rca = ca.red());
        gDiff = (/*gcb = */ cb.green()) - (gca = ca.green());
        bDiff = (/*bcb = */ cb.blue())  - (bca = ca.blue());

        register int rl = rca << 16;
        register int gl = gca << 16;
        register int bl = bca << 16;

        int rcdelta = ((1<<16) / (eff == VerticalGradient ? pixmap.height() : pixmap.width())) * rDiff;
        int gcdelta = ((1<<16) / (eff == VerticalGradient ? pixmap.height() : pixmap.width())) * gDiff;
        int bcdelta = ((1<<16) / (eff == VerticalGradient ? pixmap.height() : pixmap.width())) * bDiff;

        QPainter p(&pixmap);

        // these for-loops could be merged, but the if's in the inner loop
        // would make it slow
        switch(eff) {
        case VerticalGradient:
            for ( y = 0; y < pixmap.height(); y++ ) {
                rl += rcdelta;
                gl += gcdelta;
                bl += bcdelta;

                p.setPen(QColor(rl>>16, gl>>16, bl>>16));
                p.drawLine(0, y, pixmap.width()-1, y);
            }
            break;
        case HorizontalGradient:
            for( x = 0; x < pixmap.width(); x++) {
                rl += rcdelta;
                gl += gcdelta;
                bl += bcdelta;

                p.setPen(QColor(rl>>16, gl>>16, bl>>16));
                p.drawLine(x, 0, x, pixmap.height()-1);
            }
            break;
        default:
            ;
        }
    }
    else {
        QImage image = QImageEffect::gradient(pixmap.size(), ca, cb,
                                              (QImageEffect::GradientType) eff, ncols);
        pixmap.convertFromImage(image);
    }

    return pixmap;
}


// -----------------------------------------------------------------------------

QPixmap& QPixmapEffect::unbalancedGradient(QPixmap &pixmap, const QColor &ca,
         const QColor &cb, GradientType eff, int xfactor, int yfactor,
         int ncols)
{
    QImage image = QImageEffect::unbalancedGradient(pixmap.size(), ca, cb,
                                 (QImageEffect::GradientType) eff,
                                 xfactor, yfactor, ncols);
    pixmap.convertFromImage(image);

    return pixmap;
}


//======================================================================
//
// Intensity effects
//
//======================================================================



QPixmap& QPixmapEffect::intensity(QPixmap &pixmap, float percent)
{
    QImage image = pixmap.convertToImage();
    QImageEffect::intensity(image, percent);
    pixmap.convertFromImage(image);

    return pixmap;
}


// -----------------------------------------------------------------------------

QPixmap& QPixmapEffect::channelIntensity(QPixmap &pixmap, float percent,
                                     RGBComponent channel)
{
    QImage image = pixmap.convertToImage();
    QImageEffect::channelIntensity(image, percent,
                   (QImageEffect::RGBComponent) channel);
    pixmap.convertFromImage(image);

    return pixmap;
}


//======================================================================
//
// Blend effects
//
//======================================================================


QPixmap& QPixmapEffect::blend(QPixmap &pixmap, float initial_intensity,
			  const QColor &bgnd, GradientType eff,
			  bool anti_dir, int ncols)
{

    QImage image = pixmap.convertToImage();
    if (image.depth() <=8)
        image = image.convertDepth(32); //Sloww..

    QImageEffect::blend(image, initial_intensity, bgnd,
                  (QImageEffect::GradientType) eff, anti_dir);

    unsigned int tmp;

    if(pixmap.depth() <= 8 ) {
        if ( ncols < 2 || ncols > 256 )
            ncols = 3;
        QColor *dPal = new QColor[ncols];
        for (int i=0; i<ncols; i++) {
            tmp = 0 + 255 * i / ( ncols - 1 );
            dPal[i].setRgb ( tmp, tmp, tmp );
        }
        QImageEffect::dither(image, dPal, ncols);
        pixmap.convertFromImage(image);
        delete [] dPal;
    }
    else
        pixmap.convertFromImage(image);

    return pixmap;
}


//======================================================================
//
// Hash effects
//
//======================================================================

QPixmap& QPixmapEffect::hash(QPixmap &pixmap, Lighting lite,
			 unsigned int spacing, int ncols)
{
    QImage image = pixmap.convertToImage();
    QImageEffect::hash(image, (QImageEffect::Lighting) lite, spacing);

    unsigned int tmp;

    if(pixmap.depth() <= 8 ) {
        if ( ncols < 2 || ncols > 256 )
            ncols = 3;
        QColor *dPal = new QColor[ncols];
        for (int i=0; i<ncols; i++) {
            tmp = 0 + 255 * i / ( ncols - 1 );
            dPal[i].setRgb ( tmp, tmp, tmp );
        }
        QImageEffect::dither(image, dPal, ncols);
        pixmap.convertFromImage(image);
        delete [] dPal;
    }
    else
        pixmap.convertFromImage(image);

    return pixmap;
}


//======================================================================
//
// Pattern effects
//
//======================================================================

#if 0
void KPixmapEffect::pattern(KPixmap &pixmap, const QColor &ca,
	const QColor &cb, unsigned pat[8])
{
    QImage img = pattern(pixmap.size(), ca, cb, pat);
    pixmap.convertFromImage(img);
}
#endif

// -----------------------------------------------------------------------------

QPixmap QPixmapEffect::pattern(const QPixmap& pmtile, QSize size,
                       const QColor &ca, const QColor &cb, int ncols)
{
    if (pmtile.depth() > 8)
	ncols = 0;

    QImage img = pmtile.convertToImage();
    QImageEffect::flatten(img, ca, cb, ncols);
    QPixmap pixmap;
    pixmap.convertFromImage(img);

    return QPixmapEffect::createTiled(pixmap, size);
}


// -----------------------------------------------------------------------------

QPixmap QPixmapEffect::createTiled(const QPixmap& pixmap, QSize size)
{
    QPixmap pix(size);

    QPainter p(&pix);
    p.drawTiledPixmap(0, 0, size.width(), size.height(), pixmap);

    return pix;
}


//======================================================================
//
// Fade effects
//
//======================================================================

QPixmap& QPixmapEffect::fade(QPixmap &pixmap, double val, const QColor &color)
{
    QImage img = pixmap.convertToImage();
    QImageEffect::fade(img, val, color);
    pixmap.convertFromImage(img);

    return pixmap;
}


// -----------------------------------------------------------------------------
QPixmap& QPixmapEffect::toGray(QPixmap &pixmap, bool fast)
{
    QImage img = pixmap.convertToImage();
    QImageEffect::toGray(img, fast);
    pixmap.convertFromImage(img);

    return pixmap;
}

// -----------------------------------------------------------------------------
QPixmap& QPixmapEffect::desaturate(QPixmap &pixmap, float desat)
{
    QImage img = pixmap.convertToImage();
    QImageEffect::desaturate(img, desat);
    pixmap.convertFromImage(img);

    return pixmap;
}
// -----------------------------------------------------------------------------
QPixmap& QPixmapEffect::contrast(QPixmap &pixmap, int c)
{
    QImage img = pixmap.convertToImage();
    QImageEffect::contrast(img, c);
    pixmap.convertFromImage(img);

    return pixmap;
}

//======================================================================
//
// Dither effects
//
//======================================================================

// -----------------------------------------------------------------------------
QPixmap& QPixmapEffect::dither(QPixmap &pixmap, const QColor *palette, int size)
{
    QImage img = pixmap.convertToImage();
    QImageEffect::dither(img, palette, size);
    pixmap.convertFromImage(img);

    return pixmap;
}

//======================================================================
//
// Other effects
//
//======================================================================

QPixmap QPixmapEffect::selectedPixmap( const QPixmap &pix, const QColor &col )
{
    QImage img = pix.convertToImage();
    QImageEffect::selectedImage(img, col);
    QPixmap outPix;
    outPix.convertFromImage(img);
    return outPix;
}
