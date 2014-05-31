// Avoid multiple header inclusion
#ifndef BOOKMARKPP_H
#define BOOKMARKPP_H
 
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
 
/**
  * This is the plugin class. There will be only one instance of this class.
  * We always want to inherit KTextEditor::Plugin here.
  */
class BookmarkMap;
class BookmarkPlusPlus
  : public KTextEditor::Plugin
{
  Q_OBJECT
  public:
    // Constructor
    explicit BookmarkPlusPlus(QObject *parent,
                            const QVariantList &args);
    // Destructor
    virtual ~BookmarkPlusPlus();
 
    // Overriden methods
    // This method is called when a plugin has to be added to a view. As there
    // is only one instance of this plugin, but it is possible for plugins to
    // behave in different ways in different opened views where it is loaded, in
    // Kate plugins are added to views. For that reason we have the plugin itself
    // (this class) and then the plugin view class.
    // In this methods we have to create/remove BookmarkPlusPlusView classes.
    void addView (KTextEditor::View *view);
    void removeView (KTextEditor::View *view);
 
    void addDocument(KTextEditor::Document* doc);
    void removeDocument(KTextEditor::Document* doc);
    void readConfig();
    void writeConfig();
    
 
  private:
    QList<class BookmarkPlusPlusView*> m_views;
    QList<class KTextEditor::Document*> m_docs;
    BookmarkMap* m_bookmarks;
};
 
/**
  * This is the plugin view class. There can be as much instances as views exist.
  */
class BookmarkPlusPlusView
   : public QObject, public KXMLGUIClient
{
  Q_OBJECT
 
  public:
    explicit BookmarkPlusPlusView(KTextEditor::View *view = 0,BookmarkMap* bookmap=0);
    ~BookmarkPlusPlusView();
 
  private Q_SLOTS:
    void slotInsertTimeDate();
    void slotSetBookmark();
  private:
    KTextEditor::View *m_view;
    BookmarkMap* m_books;
};

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
    void serialize()
    {
      
      QString serializationString("");
      foreach(QString s,m_map.keys())
      {
        qDebug()<<"neki text";
        serializationString+=QString(s)+QString(" ")+QString::number(m_map[s])+QString(" ");
        qDebug()<<serializationString;
      }
      
      QTextStream qout(stdout);
      qout<<"\nbookmarkci:"<<serializationString<<"x\n";
      qobject_cast<KTextEditor::MarkInterface*>(m_doc)->setMarkDescription(
      KTextEditor::MarkInterface::markType21,QString("djokica"));
    }
  private:
    KTextEditor::Document * m_doc;
    QMap < QString, uint > m_map;
    //TODO:update maxa
    uint max;
  };
  QMap <KTextEditor::Document*,DocBookmarkMap*> m_docmap;
};
#endif // BOOKMARKPP_H