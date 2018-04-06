#include "bundle.h"

Bundle::Bundle(const QString& name) : m_Name { name }
{

}

Bundle::Bundle() : m_Name { "" }
{

}

Bundle::~Bundle()
{

}

void Bundle::add(const QString &key, const QString &value)
{
    m_Values.insert(key, value);
}

QString Bundle::get(const QString &key)
{
    return m_Values.value(key);
}

void Bundle::addChild(Bundle bundle)
{
    m_Children.push_back(bundle);
}

Bundle& Bundle::childAt(int index)
{
    return m_Children[index];
}
