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

#ifndef QBAPPLICATION_H
#define QBAPPLICATION_H

#include "qbplugin.h"

class QbApplication: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList pluginsPaths READ pluginsPaths
                                        WRITE setPluginsPaths
                                        RESET resetPluginsPaths)

    public:
        explicit QbApplication(QObject *parent=NULL);
        ~QbApplication();

        Q_INVOKABLE QStringList pluginsPaths();
        Q_INVOKABLE QbElementPtr newInstance(QString pluginId);
        Q_INVOKABLE void deleteInstance(QString pluginId);

    private:
        QStringList m_pluginsPaths;
        QPluginLoader m_pluginLoader;
        QMap<QString, QSharedPointer<QbPlugin> > m_plugins;
        QMap<QString, QString> m_pluginPath;
        QMap<QString, int> m_instances;

        bool isLoaded(QString pluginId);
        bool load(QString pluginId);
        bool unload(QString pluginId);

    public slots:
        void setPluginsPaths(QStringList pluginsPaths);
        void resetPluginsPaths();
};

#endif // QBAPPLICATION_H
