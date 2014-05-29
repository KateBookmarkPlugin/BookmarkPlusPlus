#ifndef KATE_BOOKMARK_TREE_VIEW_H
#define KATE_BOOKMARK_TREE_VIEW_H

#include <QTreeView>
#include <QContextMenuEvent>


/**
 * A tree like view of project content.
 */
class KateBookmarkTreeView : public QTreeView
{
  Q_OBJECT
  
  public:
    
      KateBookmarkTreeView (BookmarkPlusPlusPluginView *pluginView);

    /**
     * deconstruct project
     */
    ~KateBookmarkTreeView ();
    
    void selectFile (const QString &file);

    /**
     * Open the selected document, if any.
     */
    void openSelectedDocument ();
    
private Q_SLOTS:
    /**
     * item got clicked, do stuff, like open document
     * @param index model index of clicked item
     */
    void slotClicked (const QModelIndex &index);

    /**
     * Triggered on model changes.
     * This includes the files list, itemForFile mapping!
     */
    void slotModelChanged ();

  protected:
    /**
     * Create matching context menu.
     * @param event context menu event
     */
    void contextMenuEvent (QContextMenuEvent *event);

  private:
    /**
     * our plugin view
     */
    BookmarkPlusPlusPluginView *m_pluginView;

};

#endif