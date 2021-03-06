/* Webcamod, webcam capture plasmoid.
 * Copyright (C) 2011-2013  Gonzalo Exequiel Pedone
 *
 * Webcamod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamod is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamod. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email     : hipersayan DOT x AT gmail DOT com
 * Web-Site 1: http://github.com/hipersayanX/Webcamoid
 * Web-Site 2: http://kde-apps.org/content/show.php/Webcamoid?content=144796
 */

#include "plasmoid.h"

Plasmoid::Plasmoid(QObject *parent, const QVariantList &args): Plasma::PopupApplet(parent, args)
{
}

Plasmoid::~Plasmoid()
{
    if (this->hasFailedToLaunch())
    {
    }
    else
    {
    }
}

void Plasmoid::init()
{
    this->m_minimumPlasmoidSize = QSizeF(32, 32);
    this->setPassivePopup(true);
    this->setPopupIcon(QIcon::fromTheme("camera-web"));
    this->setHasConfigurationInterface(true);
    this->setAspectRatioMode(Plasma::IgnoreAspectRatio);
    this->setMinimumSize(this->m_minimumPlasmoidSize);

    this->m_mainWidget = QSharedPointer<MainWidget>(new MainWidget(NULL, this));

    this->m_defaultPlasmoidSize = QSizeF(this->m_mainWidget->size());
    this->resize(this->m_defaultPlasmoidSize);

    this->m_graphicsWidget = new QGraphicsWidget(this);
    this->setGraphicsWidget(this->m_graphicsWidget);

    this->m_glyGraphicsWidget = new QGraphicsGridLayout(this->m_graphicsWidget);
    this->m_graphicsWidget->setLayout(this->m_glyGraphicsWidget);

    this->m_proxyWidget = new QGraphicsProxyWidget(this->m_graphicsWidget);
    this->m_proxyWidget->setWidget(this->m_mainWidget.data());
    this->m_proxyWidget->resize(this->m_defaultPlasmoidSize);
    this->m_proxyWidget->setMinimumSize(this->m_minimumPlasmoidSize);
    this->m_glyGraphicsWidget->addItem(this->m_proxyWidget, 0, 0, 1, 1);
}

void Plasmoid::createConfigurationInterface(KConfigDialog *configDialog)
{
    this->m_mainWidget->addWebcamConfigDialog(configDialog);
    this->m_mainWidget->addEffectsConfigDialog(configDialog);
    this->m_mainWidget->addVideoFormatsConfigDialog(configDialog);
    this->m_mainWidget->addStreamsConfigDialog(configDialog);
    this->m_mainWidget->addGeneralConfigsDialog(configDialog);

    QObject::connect(configDialog,
                     SIGNAL(okClicked()),
                     this->m_mainWidget.data(),
                     SLOT(saveConfigs()));

    QObject::connect(configDialog,
                     SIGNAL(cancelClicked()),
                     this->m_mainWidget.data(),
                     SLOT(saveConfigs()));
}

K_EXPORT_PLASMA_APPLET(webcamoid, Plasmoid)
