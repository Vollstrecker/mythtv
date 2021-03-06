// -*- Mode: c++ -*-
/* ============================================================
 * File  : gltexture.cpp
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2006-07-24
 * Description : 
 * 
 * Copyright 2004-2006 Renchi Raju, Daniel Kristjansson
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * ============================================================ */

// Qt headers
#include <QImage>

// MythGallery headers
#include "thumbview.h"
#include "gltexture.h"
#include "galleryutil.h"

/** \fn GLTexture::Init(const QImage&)
 *  \brief Create the texture initialized with QImage
 */
void GLTexture::Init(const QImage &image)
{
    Deinit();
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    /* actually generate the texture */
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    /* enable linear filtering  */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

/**
 *  \brief Delete the texture
 */
void GLTexture::Deinit(void)
{
    if (tex)
        glDeleteTextures(1, &tex);
}

void GLTexture::Bind(void)
{
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glRotatef(GetAngle(), 0.0F, 0.0F, 1.0F);
    glBindTexture(GL_TEXTURE_2D, tex);
}

void GLTexture::MakeQuad(float alpha, float scale)
{
    Bind();

    glBegin(GL_QUADS);
    glColor4f(1.0F, 1.0F, 1.0F, alpha);

    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(-GetTextureX() * scale, -GetTextureY() * scale, 0.0F);

    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(+GetTextureX() * scale, -GetTextureY() * scale, 0.0F);

    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(+GetTextureX() * scale, +GetTextureY() * scale, 0.0F);

    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(-GetTextureX() * scale, +GetTextureY() * scale, 0.0F);
    glEnd();
}

void GLTexture::ScaleTo(const QSize &dest, ScaleMax scaleMax)
{
    QSize sz = GalleryUtil::ScaleToDest(GetSize(), dest, scaleMax);
    if ((sz.width()   > 0) && (sz.height()   > 0) &&
        (dest.width() > 0) && (dest.height() > 0))
    {
        SetScale((float)sz.width()  / (float)dest.width(),
                 (float)sz.height() / (float)dest.height());
    }
}

void GLTexture::SetItem(ThumbItem *thumbItem, const QSize &sz)
{
    item = thumbItem;
    if (item)
    {
        angle = item->GetRotationAngle();
        SetSize(sz);

        if (angle % 180 != 0)
            SwapWidthHeight();
    }
}

QString GLTexture::GetDescription(const QString &status) const
{
    if (item)
        return item->GetDescription(status, GetSize(), angle);

    return QString();
}
