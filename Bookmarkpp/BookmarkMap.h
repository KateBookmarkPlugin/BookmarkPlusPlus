// Avoid multiple header inclusion
#ifndef BOOKMARKMAP_H
#define BOOKMARKMAP_H
 
// Include the basics
#include <ktexteditor/plugin.h>
#include <ktexteditor/view.h>
#include <ktexteditor/document.h>
#include <ktexteditor/markinterface.h>
#include <kxmlguiclient.h>
#include <klocalizedstring.h>
#include <ktexteditor/sessionconfiginterface.h>
#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <KConfigGroup>


class BookmarkMap
{
public:
  BookmarkMap();
  ~BookmarkMap();
  void addDocument(KTextEditor::Document* doc);
  void removeDocument(KTextEditor::Document* doc);
  void addBookmark(KTextEditor::Document* doc,QString name,int line);
  void removeBookmark(KTextEditor::Document* doc,QString name);
  void refresh(KTextEditor::Document* doc);
  void serialize(KTextEditor::Document* doc);
  int getLineInDocument(KTextEditor::Document* doc,uint code);
  int getLineInDocument(KTextEditor::Document* doc,QString name);
  QList<QString> getBookmarkNames(KTextEditor::Document *doc);
  class DocBookmarkMap
  {
  public:
    DocBookmarkMap(KTextEditor::Document* doc)
    :m_doc(doc)
    {
      max=1;
    }
    void addBookmark(QString name,int line)
    {
      KTextEditor::MarkInterface* mi=qobject_cast
      <KTextEditor::MarkInterface*>(m_doc);
      uint code=((max)<<9)+1;
      std::cout<<"\ncode:"<<code<<std::endl;
      //check if it already exists
      int mark=mi->mark(line);
      if(!mark)
      {
        code=((++max)<<9)+1;
        mi->addMark(line,code);
      }
      else//if it has a bookmark
      {
        foreach(KTextEditor::Mark* a, mi->marks())
        {
          if(a->line==line)
          {
            if(a->type==1)
            {
              code=((++max)<<9)+1;
              mi->addMark(line,code);
            }
            else
            {
              code=a->type;
            }
            break;
          }
        }
      }
      m_map[name]=code;
      //debug
      QTextStream qout(stdout);
      foreach(QString s, m_map.keys())
      {
        qout<<"radim:"<<s<<" "<<m_map[s];
      }
//    update of max value
      uint tmax=1;
      foreach(uint i, m_map.values())
      {
        uint shifted=(i>>9);
        tmax=tmax<shifted?shifted:tmax;
      }
      max=tmax;
      qout<<"\n";
    }
    void removeBookmark(QString name)
    {
      m_map.remove(name);
    }
    void refresh()
    {
      KTextEditor::MarkInterface* mi=qobject_cast
        <KTextEditor::MarkInterface*>(m_doc);
      QList<QString> pastKeys=m_map.keys();
      QMap < QString, uint > futureMap;
      foreach(KTextEditor::Mark* mark,mi->marks())
      {
        foreach(QString name, pastKeys)
        {
          if(mark->type==m_map[name])
          {
             futureMap[name]=mark->type;
          }
        }
      }
      m_map=futureMap;
    }//refresh
    int getLineOfBookmark(uint code)
    {
      KTextEditor::MarkInterface* mi=qobject_cast
        <KTextEditor::MarkInterface*>(m_doc);
      foreach(KTextEditor::Mark* mark,mi->marks())
      {
        if(mark->type==code)
          return mark->line;
      }
      return -1;
    }
    int getLineOfBookmark(QString name)
    {
      uint code=m_map[name];
      if(code==0)
        return -2;
      
      KTextEditor::MarkInterface* mi=qobject_cast
        <KTextEditor::MarkInterface*>(m_doc);
      foreach(KTextEditor::Mark* mark,mi->marks())
      {
        if(mark->type==code)
          return mark->line;
      }
      return -1;
    }
    QList<QString> getBookmarkNames()
    {
      return m_map.keys();
    }
    QVariantList* serialize()
    {
      QVariantList* serializationStrings=new QVariantList();
      qDebug()<<m_doc->documentName();
      qDebug()<<"ovde";
      QList<QString> keys=m_map.keys();
      foreach(QString s, keys)
      {
        qDebug()<<m_doc->documentName();
      }
      foreach(QString s,keys)
      {
        qDebug()<<"neki text";
        serializationStrings->push_back(s);
        serializationStrings->push_back(m_map[s]);
        qDebug()<<*serializationStrings;
      }
      qDebug()<<"onde";
      QList<QVariant>::iterator iter=serializationStrings->begin();
      QList<QVariant>::iterator end=serializationStrings->end();
      for(;iter!=end;iter++)
      {
        qDebug()<<"\nstampam:"<<*iter;
      }
      return serializationStrings;
    }
  private:
    KTextEditor::Document * m_doc;
    QMap < QString, uint > m_map;
    //TODO:update maxa
    uint max;
  };
  QMap <KTextEditor::Document*,DocBookmarkMap*> m_docmap;
};
#endif // BOOKMARKMAP_H