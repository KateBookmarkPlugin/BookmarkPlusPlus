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

#include "BookmarkMap.h"
/**
  * This is the plugin class. There will be only one instance of this class.
  * We always want to inherit KTextEditor::Plugin here.
  */
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
    void readConfig(KTextEditor::Document* doc);
    void writeConfig(KTextEditor::Document* doc);
    
 
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
#endif // BOOKMARKPP_H