#include "saveable.h"
#include "parser.h"

#include <QDebug>
#include <QFile>

#if 0
Bundle::Bundle()
{

}

Bundle::Bundle(const QString& filePath)
{
    readFromFile(filePath);
}

Bundle::~Bundle()
{

}

void Bundle::addChild(const Bundle& bundle)
{
    m_Children.push_back(bundle);
}

Bundle& Bundle::childAt(int index)
{
    return m_Children[index];
}

int Bundle::childrenCount() const
{
    return m_Children.size();
}

bool Bundle::readFromFile(const QString& filePath)
{
    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly))
        return false;

    QTextStream in(&inputFile);
    QString text;

    while (!in.atEnd())
       text.append(in.readLine());

    inputFile.close();

    Bundle b = Parser::parse(text);
    b.writeToFile(filePath);

    return true;
}

bool Bundle::writeToFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    stream << toText();

    file.close();
    return true;
}

void Bundle::process(QXmlStreamWriter& writer) const
{
    writer.writeStartElement("set");

    for (QMap<QString, QString>::const_iterator i = m_Members.constBegin(); i != m_Members.constEnd(); ++i)
    {
        writer.writeStartElement("item");
        writer.writeAttribute(i.key(), i.value());
        writer.writeEndElement();
    }

    for (const Bundle& bundle : m_Children)
        bundle.process(writer);

    writer.writeEndElement();
}

QString Bundle::toText() const
{
#if 0
    QString ret;
    ret.append("{").append("\n");

    for (QMap<QString, QString>::const_iterator i = m_Members.constBegin(); i != m_Members.constEnd(); ++i)
    {
        ret.append(i.key());
        ret.append("=");
        ret.append(i.value());

        ret.append("\n");
    }

    for (const Bundle& bundle : m_Children)
        ret.append(bundle.toText(toAppend));

    ret.append("}").append("\n");
    return ret;
#endif

    QString ret;

    QXmlStreamWriter xmlWriter(&ret);
    xmlWriter.setAutoFormatting(true);

    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("actions");
    process(xmlWriter);
    xmlWriter.writeEndDocument();

    return ret;
}
#endif

bool Saveable::writeToFile(const QString& filePath, const QString& text)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    stream << text;

    file.close();
    return true;
}

bool Saveable::readFromFile(const QString& filePath, QString& outText)
{
    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly))
        return false;

    QTextStream in(&inputFile);
    QString text;

    while (!in.atEnd())
       text.append(in.readLine());

    inputFile.close();
    outText = text;

    return true;
}
