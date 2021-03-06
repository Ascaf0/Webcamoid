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

#ifndef MEDIATOOLS_H
#define MEDIATOOLS_H

#include <QtGui>
#include <qb.h>

#include "commons.h"
#include "appenvironment.h"

class COMMONSSHARED_EXPORT MediaTools: public QObject
{
    Q_OBJECT
    Q_ENUMS(RecordFrom)

    Q_PROPERTY(QString device READ device
                              WRITE setDevice
                              RESET resetDevice
                              NOTIFY deviceChanged)

    Q_PROPERTY(bool effectsPreview READ effectsPreview
                                   WRITE setEffectsPreview
                                   RESET resetEffectsPreview)

    Q_PROPERTY(bool playAudioFromSource READ playAudioFromSource
                                        WRITE setPlayAudioFromSource
                                        RESET resetPlayAudioFromSource)

    Q_PROPERTY(RecordFrom recordAudioFrom READ recordAudioFrom
                                          WRITE setRecordAudioFrom
                                          RESET resetRecordAudioFrom)

    Q_PROPERTY(bool recording READ recording
                              WRITE setRecording
                              RESET resetRecording)

    Q_PROPERTY(QList<QStringList> videoRecordFormats READ videoRecordFormats
                                                     WRITE setVideoRecordFormats
                                                     RESET resetVideoRecordFormats)

    Q_PROPERTY(QList<QStringList> streams READ streams
                                          WRITE setStreams
                                          RESET resetStreams)

    Q_PROPERTY(QSize windowSize READ windowSize
                                WRITE setWindowSize
                                RESET resetWindowSize)

    public:
        enum RecordFrom
        {
            RecordFromNone,
            RecordFromSource,
            RecordFromMic
        };

        explicit MediaTools(QObject *parent=NULL);
        ~MediaTools();

        Q_INVOKABLE QString device();
        Q_INVOKABLE QSize videoSize(const QString &device);
        Q_INVOKABLE bool effectsPreview();
        Q_INVOKABLE bool playAudioFromSource();
        Q_INVOKABLE RecordFrom recordAudioFrom();
        Q_INVOKABLE bool recording();
        Q_INVOKABLE QList<QStringList> videoRecordFormats();
        Q_INVOKABLE QList<QStringList> streams();
        Q_INVOKABLE QSize windowSize();

        Q_INVOKABLE QVariantList videoSizes(const QString &device);
        Q_INVOKABLE QList<QStringList> captureDevices();
        Q_INVOKABLE QVariantList listControls(const QString &device);
        Q_INVOKABLE void setControls(const QString &device, const QVariantMap &controls);
        Q_INVOKABLE QMap<QString, QString> availableEffects();
        Q_INVOKABLE QStringList currentEffects();
        Q_INVOKABLE QString bestRecordFormatOptions(QString fileName="");

    private:
        QString m_device;
        bool m_showEffectsPreview;
        bool m_playAudioFromSource;
        RecordFrom m_recordAudioFrom;
        bool m_recording;
        QList<QStringList> m_videoRecordFormats;
        QList<QStringList> m_streams;
        QSize m_windowSize;

        AppEnvironment *m_appEnvironment;
        QbElementPtr m_pipeline;
        QbElementPtr m_source;
        QbElementPtr m_effects;
        QbElementPtr m_effectsPreview;
        QbElementPtr m_audioSwitch;
        QbElementPtr m_audioOutput;
        QbElementPtr m_mic;
        QbElementPtr m_record;
        QbElementPtr m_webcamConfig;
        QStringList m_effectsList;
        QSize m_curFrameSize;
        QMutex m_mutex;

        QString hashFromName(QString name="");
        QString nameFromHash(QString hash="");

    signals:
        void devicesModified();
        void frameSizeChanged(QSize size);
        void deviceChanged(QString device);
        void recordingChanged(bool recording);
        void frameReady(const QImage &frame);
        void previewFrameReady(const QImage &frame, QString effectName);
        void error(QString message);

    public slots:
        void mutexLock();
        void mutexUnlock();
        void setDevice(QString device);
        void setVideoSize(const QString &device, const QSize &size);
        void setEffectsPreview(bool effectsPreview);
        void setPlayAudioFromSource(bool playAudioFromSource);
        void setRecordAudioFrom(RecordFrom recordAudioFrom);
        void setRecording(bool recording, QString fileName="");
        void setVideoRecordFormats(QList<QStringList> videoRecordFormats);
        void setStreams(QList<QStringList> streams);
        void setWindowSize(QSize windowSize);
        void resetDevice();
        void resetVideoSize(const QString &device);
        void resetEffectsPreview();
        void resetPlayAudioFromSource();
        void resetRecordAudioFrom();
        void resetRecording();
        void resetVideoRecordFormats();
        void resetWindowSize();

        void reset(QString device);
        void loadConfigs();
        void saveConfigs();
        void setEffects(QStringList effects=QStringList());
        void clearVideoRecordFormats();
        void resetStreams();
        void setStream(QString dev_name="", QString description="");
        void setVideoRecordFormat(QString suffix="", QString options="");

    private slots:
        void iStream(const QbPacket &packet);

        void aboutToQuit();
        void webcamsChanged(const QStringList &webcams);
};

#endif // MEDIATOOLS_H
