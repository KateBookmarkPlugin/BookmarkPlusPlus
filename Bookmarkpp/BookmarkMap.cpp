#include "BookmarkMap.h"
 
// Include the basics
#include <ktexteditor/document.h>
#include <ktexteditor/markinterface.h>

#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <klocale.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kdatetime.h>
#include <iostream>
#include <typeinfo>
#include <QInputDialog>
#include <QPair>
#include <map>

BookmarkMap::BookmarkMap()
{
}
BookmarkMap::~BookmarkMap()
{
}
void BookmarkMap::addDocument(KTextEditor::Document* doc)
{
  m_docmap[doc]=new DocBookmarkMap(doc);
  
}
void BookmarkMap::removeDocument(KTextEditor::Document* doc)
{
  m_docmap.remove(doc);
  std::cout<<"zatvaram dokument"<<std::endl;    
}
//TODO:ima description u MarkInterface
void BookmarkMap::addBookmark(KTextEditor::Document* doc,QString name,int line)
{
  m_docmap[doc]->addBookmark(name,line);
}
void BookmarkMap::removeBookmark(KTextEditor::Document* doc,QString name)
{
  m_docmap[doc]->removeBookmark(name);
}
//optional TODO: da ima opcija u config da li da moze da se brise bookmark sa
// ctrl+B ili samo sa nekom nasom skracenicom za brisanje bookmarka
void BookmarkMap::refresh(KTextEditor::Document *doc)
{
  m_docmap[doc]->refresh();
  qDebug()<<"jhg";
}
//return: -1 if a code doesn exist, line of named bookmark if it exists 
int BookmarkMap::getLineInDocument(KTextEditor::Document* doc,uint code)
{
  return m_docmap[doc]->getLineOfBookmark(code);
}
//return: -2 if a name doesn't exist, -1 if a name exist, but bookmark doesn't
//or line of named bookmark if it exists
int BookmarkMap::getLineInDocument(KTextEditor::Document* doc,QString name)
{
  return m_docmap[doc]->getLineOfBookmark(name);
}

QList<QString> BookmarkMap::getBookmarkNames(KTextEditor::Document* doc)
{
  return m_docmap[doc]->getBookmarkNames();
}

void BookmarkMap::serialize(KTextEditor::Document* doc)
{
  m_docmap[doc]->serialize();
}
// We need to include the moc file since we have declared slots and we are using
// the Q_OBJECT macro on the BookmarkPlusPlusView class.