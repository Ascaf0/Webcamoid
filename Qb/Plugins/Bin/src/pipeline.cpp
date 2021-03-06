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

#include "pipeline.h"

Pipeline::Pipeline(QObject *parent): QObject(parent)
{
    this->resetElements();
    this->resetLinks();
    this->resetConnections();
    this->resetProperties();
    this->resetError();
}

QMap<QString, QbElementPtr> Pipeline::elements() const
{
    return this->m_elements;
}

QList<QStringList> Pipeline::links() const
{
    return this->m_links;
}

QList<QStringList> Pipeline::connections() const
{
    return this->m_connections;
}

QVariantMap Pipeline::properties() const
{
    return this->m_properties;
}

QString Pipeline::error() const
{
    return this->m_error;
}

QString Pipeline::addElement(QbElementPtr element)
{
    QString name;

    if (element->objectName().isEmpty())
        name = QString("&%1").arg((quint64) element.data());
    else
        name = element->objectName();

    this->m_elements[name] = element;

    return name;
}

void Pipeline::removeElement(QString elementName)
{
    QList<QStringList> connections = this->m_connections;

    foreach (QStringList connection, connections)
        if (connection[0] == elementName ||
            connection[2] == elementName)
        {
            QbElement *sender = this->m_elements[connection[0]].data();
            QbElement *receiver = this->m_elements[connection[2]].data();

            QMetaMethod signal = this->methodByName(sender, connection[1], QMetaMethod::Signal);
            QMetaMethod slot = this->methodByName(receiver, connection[3], QMetaMethod::Slot);

            QObject::disconnect(sender, signal, receiver, slot);
            this->m_connections.removeOne(connection);
        }

    QList<QStringList> links = this->m_links;

    foreach (QStringList link, links)
        if (link[0] == elementName ||
            link[1] == elementName)
        {
            this->m_elements[link[0]]->unlink(this->m_elements[link[1]]);
            this->m_links.removeOne(link);
        }

    this->m_elements.remove(elementName);
}

QList<QbElementPtr> Pipeline::inputs() const
{
    QList<QbElementPtr> inputs;

    foreach (QStringList link, this->m_links)
        if (link[0] == "IN.")
            inputs << this->m_elements[link[1]];

    return inputs;
}

QList<QbElementPtr> Pipeline::outputs() const
{
    QList<QbElementPtr> outputs;

    foreach (QStringList link, this->m_links)
        if (link[1] == "OUT.")
            outputs << this->m_elements[link[0]];

    return outputs;
}

QMetaMethod Pipeline::methodByName(QObject *object, QString methodName, QMetaMethod::MethodType methodType)
{
    QMetaMethod rMethod;

    for (int i = 0; i < object->metaObject()->methodCount(); i++)
    {
        QMetaMethod method = object->metaObject()->method(i);

        if (method.methodType() == methodType &&
            QRegExp(QString("\\s*%1\\s*\\(.*").arg(methodName)).exactMatch(method.signature()))
        {
            rMethod = method;

            break;
        }
    }

    return rMethod;
}

void Pipeline::solveConnections(QString self)
{
    for (int i = 0; i < this->m_connections.length(); i++)
        if (this->m_connections[i][0] == "this?")
            this->m_connections[i][0] = self;
        else if (this->m_connections[i][2] == "this?")
            this->m_connections[i][2] = self;
}

void Pipeline::addLinks(QStringList links)
{
    QStringList link;

    foreach (QString element,  links)
    {
        link << element;

        if (link.length() == 2)
        {
            this->m_links << link;
            link.removeFirst();
        }
    }
}

bool Pipeline::linkAll()
{
    foreach (QStringList link, this->m_links)
        if (link[0] != "IN." &&
            link[1] != "OUT.")
        {
            if (!this->m_elements.contains(link[0]))
            {
                this->m_error = QString("No element named '%1'").arg(link[0]);

                return false;
            }
            else if (!this->m_elements.contains(link[1]))
            {
                this->m_error = QString("No element named '%1'").arg(link[1]);

                return false;
            }
            else
                this->m_elements[link[0]]->link(this->m_elements[link[1]]);
        }

    return true;
}

bool Pipeline::unlinkAll()
{
    foreach (QStringList link, this->m_links)
        if (link[0] != "IN." &&
            link[1] != "OUT.")
        {
            if (!this->m_elements.contains(link[0]))
            {
                this->m_error = QString("No element named '%1'").arg(link[0]);

                return false;
            }
            else if (!this->m_elements.contains(link[1]))
            {
                this->m_error = QString("No element named '%1'").arg(link[1]);

                return false;
            }
            else
                this->m_elements[link[0]]->unlink(this->m_elements[link[1]]);
        }

    return true;
}

bool Pipeline::connectAll()
{
    foreach (QStringList connection, this->m_connections)
    {
        QbElement *sender = this->m_elements[connection[0]].data();
        QbElement *receiver = this->m_elements[connection[2]].data();

        if (!sender)
        {
            this->m_error = QString("No element named '%1'").arg(connection[0]);

            return false;
        }

        if (!receiver)
        {
            this->m_error = QString("No element named '%1'").arg(connection[2]);

            return false;
        }

        QMetaMethod signal = this->methodByName(sender, connection[1], QMetaMethod::Signal);
        QMetaMethod slot = this->methodByName(receiver, connection[3], QMetaMethod::Slot);

        QObject::connect(sender, signal, receiver, slot);
    }

    return true;
}

bool Pipeline::disconnectAll()
{
    foreach (QStringList connection, this->m_connections)
    {
        QbElement *sender = this->m_elements[connection[0]].data();
        QbElement *receiver = this->m_elements[connection[2]].data();

        if (!sender)
        {
            this->m_error = QString("No element named '%1'.").arg(connection[0]);

            return false;
        }

        if (!receiver)
        {
            this->m_error = QString("No element named '%1'.").arg(connection[2]);

            return false;
        }

        QMetaMethod signal = this->methodByName(sender, connection[1], QMetaMethod::Signal);
        QMetaMethod slot = this->methodByName(receiver, connection[3], QMetaMethod::Slot);

        QObject::disconnect(sender, signal, receiver, slot);
    }

    return true;
}

void Pipeline::cleanAll()
{
    this->unlinkAll();
    this->disconnectAll();

    this->resetElements();
    this->resetLinks();
    this->resetConnections();
    this->resetProperties();
    this->resetError();
}

void Pipeline::setElements(const QMap<QString, QbElementPtr> &elements)
{
    this->m_elements = elements;
}

void Pipeline::setLinks(const QList<QStringList> &links)
{
    this->m_links = links;
}

void Pipeline::setConnections(const QList<QStringList> &connections)
{
    this->m_connections = connections;
}

void Pipeline::setProperties(const QVariantMap &properties)
{
    this->m_properties = properties;
}

void Pipeline::setError(QString error)
{
    this->m_error = error;
}

void Pipeline::resetElements()
{
    this->setElements(QMap<QString, QbElementPtr>());
}

void Pipeline::resetLinks()
{
    this->setLinks(QList<QStringList>());
}

void Pipeline::resetConnections()
{
    this->setConnections(QList<QStringList>());
}

void Pipeline::resetProperties()
{
    this->setProperties(QVariantMap());
}

void Pipeline::resetError()
{
    this->setError("");
}
