#include "parser.h"

#include <QDebug>
#include <QtXml>
#include <QMessageBox>

#if 0
Bundle Parser::parse(const QString& str)
{
    QXmlStreamReader xmlReader(str);

    //Parse the XML until we reach end of it
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
            // Read next element
            QXmlStreamReader::TokenType token = xmlReader.readNext();
            //If token is just StartDocument - go to next
            if (token == QXmlStreamReader::StartDocument)
                continue;

            //If token is StartElement - read it
            if (token == QXmlStreamReader::StartElement)
            {
                if (xmlReader.name() == "name")
                    continue;

                if (xmlReader.name() == "id")
                    qDebug() << xmlReader.readElementText();
            }
    }

    if (xmlReader.hasError())
    {
//            QMessageBox::critical(this,
//            "xmlFile.xml Parse Error", xmlReader.errorString(),
//            QMessageBox::Ok);
        qDebug() << xmlReader.errorString();
    }

    xmlReader.clear();

    return Bundle();
}

QString Parser::getValue(const QString& line, QString& key)
{
    int separatorIndex = line.indexOf('=');
    key = line.left(separatorIndex - 1);
    return line.right(separatorIndex + 1);
}
#endif
