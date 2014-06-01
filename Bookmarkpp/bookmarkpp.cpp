// Own includes
#include "bookmarkpp.h"
 
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
// This macro defines a KPluginFactory subclass named BookmarkPlusPlusFactory. The second
// argument to the macro is code that is inserted into the constructor of the class.
// I our case all we need to do is register one plugin. If you want to have more
// than one plugin in the same library then you can register multiple plugin classes
// here. The registerPlugin function takes an optional QString parameter which is a
// keyword to uniquely identify the plugin then (it maps to X-KDE-PluginKeyword in the
// .desktop file).
K_PLUGIN_FACTORY(BookmarkPlusPlusFactory,
                 registerPlugin<BookmarkPlusPlus>();
                )
 
// With the next macro call, the library exports version information about the
// Qt and KDE libraries being used and (most important) the entry symbol to get at
// the factory we defined above.
// The argument this macro takes is the constructor call of the above factory which
// provides two constructors. One which takes a KAboutData* and another one
// that takes two (optional) const char* parameters (Same as for KComponentData
// constructors).
// We put there the X-KDE-LibraryName.
// Is important to provide as last parameter "ktexteditor_plugins".
K_EXPORT_PLUGIN(BookmarkPlusPlusFactory("ktexteditor_timedate", "ktexteditor_plugins"))
 
// Constructor
BookmarkPlusPlus::BookmarkPlusPlus(QObject *parent, const QVariantList &args)
    : KTextEditor::Plugin(parent)
{
    // Avoid warning on compile time because of unused argument
    Q_UNUSED(args);
    m_bookmarks=new BookmarkMap();
}


// Destructor
BookmarkPlusPlus::~BookmarkPlusPlus()
{
  delete m_bookmarks;
}
 
// Create the plugin view class and add it to the views list
void BookmarkPlusPlus::addView(KTextEditor::View *view)
{
  qDebug()<<"dodat je view za dokument:"<<view->document()->url()<<"\n";
    BookmarkPlusPlusView *nview = new BookmarkPlusPlusView(view,m_bookmarks);
    m_views.append(nview);
}
 
// Find the view where we want to remove the plugin from, and remove it.
// Do not forget to free the memory.
void BookmarkPlusPlus::removeView(KTextEditor::View *view)
{
    for (int z = 0; z < m_views.size(); z++)
    {
        if (m_views.at(z)->parentClient() == view)
        {
            BookmarkPlusPlusView *nview = m_views.at(z);
            m_views.removeAll(nview);
            delete nview;
        }
    }
}
// Add the document to documents
void BookmarkPlusPlus::addDocument(KTextEditor::Document *doc)
{
  qDebug()<<"otvaram dokument: "<<doc->documentName()<<" "<<doc->url()<<"\n";
    m_bookmarks->addDocument(doc);
    m_docs.append(doc);
    readConfig(doc);
}
 
// Remove a document from documents list
void BookmarkPlusPlus::removeDocument(KTextEditor::Document *doc)
{
    writeConfig(doc);
    m_bookmarks->removeDocument(doc);
    for (int z = 0; z < m_docs.size(); z++)
    {
        if (m_docs[z] == doc)
        {
            m_docs.removeAt(z);
        }
    }
}

void BookmarkPlusPlus::readConfig(KTextEditor::Document* doc)
{
  qDebug()<<"\nreadConfig je pozvan\n";
  KConfigGroup cg(KGlobal::config(), "BookmarkPlusPlus");
  qDebug()<<"readEntry:"<<cg.readEntry(doc->url().prettyUrl(), QString("Default"));
}

void BookmarkPlusPlus::writeConfig(KTextEditor::Document* doc)
{
  qDebug()<<"\nwriteConfig je pozvan\n";
  KConfigGroup cg(KGlobal::config(), "BookmarkPlusPlus" );
  cg.writeEntry(doc->url().prettyUrl(), m_bookmarks->m_docmap[doc]->serialize() );
}

// Plugin view class
BookmarkPlusPlusView::BookmarkPlusPlusView(KTextEditor::View *view,BookmarkMap* books)
  : QObject(view)
  , KXMLGUIClient(view)
  , m_view(view)
  , m_books(books)
{
    setComponentData(BookmarkPlusPlusFactory::componentData());
 
    KAction *action = new KAction(i18n("Dummyyyyy"), this);
    // Here we need as first parameter the same we declared at the resource
    // contents file (timedateui.rc). We named the action "tools_insert_timedate".
    // Here is where we connect it to an actual KDE action.
    actionCollection()->addAction("tools_insert_timedate", action);
    action->setShortcut(Qt::CTRL + Qt::Key_D);
    // As usual, we connect the signal triggered() to a slot here. When the menu
    // element is clicked, we go to the slot slotInsertTimeDate().
    connect(action, SIGNAL(triggered()), this, SLOT(slotInsertTimeDate()));
    std::cout<<std::endl<<std::endl<<"view!:"<<typeid(view->document()).name()<<std::endl;
    connect(view->document(),SIGNAL(marksChanged(KTextEditor::Document*)),
            this,SLOT(slotInsertTimeDate()));
    // This is always needed, tell the KDE XML GUI client that we are using
    // that file for reading actions from.
    KAction *setBookmarkAction = new KAction(i18n("Set Bookmark"), this);
    actionCollection()->addAction("tools_set_bookmark",setBookmarkAction);
    setBookmarkAction->setShortcut(Qt::CTRL +Qt::ALT+Qt::Key_B);
    connect(setBookmarkAction,SIGNAL(triggered()),this,SLOT(slotSetBookmark()));
    
    setXMLFile("ui.rc");
}
 
// Destructor
BookmarkPlusPlusView::~BookmarkPlusPlusView()
{
}


void BookmarkPlusPlusView::slotSetBookmark()
{
  QTextStream qout(stdout);
  bool ok;
  QString text=QInputDialog::getText(NULL,QString("Set Bookmark"),
      QString("Bookmark name:"),QLineEdit::Normal,
      QString("name"),&ok,NULL);
  if(ok)
    qout<<text<<QString("\n");
  else
    qout<<QString("Input string not OK!\n");
  m_books->addBookmark(m_view->document(),text,
                       m_view->cursorPosition().line()
           );
  
  
}
// The slot that will be called when the menu element "Insert Time & Date" is
// clicked.
void BookmarkPlusPlusView::slotInsertTimeDate()
{
    
    m_books->refresh(m_view->document());
//     std::cout<<typeid(m_view->document()).name()<<std::endl;
//     KTextEditor::MarkInterface* mi=qobject_cast
//       <KTextEditor::MarkInterface*>(m_view->document());
    QTextStream qout(stdout);
    qout<<"\n\nmarkdescription\n";
    KTextEditor::MarkInterface* marki= qobject_cast<KTextEditor::MarkInterface*>
    (m_view->document());
    qout<<marki->markDescription( KTextEditor::MarkInterface::markType21)<<"\n\n";
//     foreach(KTextEditor::Mark* a,mi->marks())
//     {
//       qout<<a->line<<" "<<a->type<<"\n";
//     }
    QList<QString> bukmarci=m_books->getBookmarkNames(m_view->document());
    foreach(QString s,bukmarci)
    {
      qout<<s<<"\n";
    }
    /// We insert the information in the document at the current cursor position
    // with the default string declared on the header.
    //int n=m_view->document()->lines();
    //QString stringa=QString(std::string(n,'*'));
    //std::cout<<n<<std::endl;
    //m_view->document()->insertText(m_view->cursorPosition(), stringa);
}

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
#include "bookmarkpp.moc"