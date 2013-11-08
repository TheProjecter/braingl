/*
 * mainwindow.h
 *
 * Created on: May 03, 2012
 * @author Ralph Schurade
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../data/enums.h"

#include <QMainWindow>
#include <QSettings>
#include <QAbstractItemModel>
#include <QFileDialog>

class QAction;
class QListView;
class QMenu;
class QTabWidget;
class QTableView;

class Dataset;
class DatasetListWidget;
class ROIWidget;
class ShaderEditWidget;
class ScriptWidget;
class FNDockWidget;

class GLWidget;
class ToolBar;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( bool debug = false, bool resetSettings = false );

    void closeEvent( QCloseEvent *event );
    bool load( QString fileName );

    void saveScene( QString fileName );
    void loadScene( QString fileName );

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

    bool save( Dataset* ds );
    void saveDataset( Dataset* ds, QString filter = "" );

    void saveSettings();
    QModelIndex createIndex( int branch, int pos, int column );
    void loadSettings();
    void loadSetting( QSettings &settings, Fn::Property setting );
    void loadColormaps( bool resetSettings );

    void setCurrentFile( const QString &fileName );
    void updateRecentFileActions();
    QString strippedName( const QString &fullFileName );

    bool load( QString fileName, QList<QVariant> state );

    Dataset* selectedDataset();

    bool m_debug;

    QMainWindow* m_centralWidget;

    GLWidget* mainGLWidget;
    GLWidget* mainGLWidget2;
    DatasetListWidget* m_datasetWidget;
    ROIWidget* m_roiWidget;
    ShaderEditWidget* m_shaderEditWidget;
    ScriptWidget* m_scriptWidget;
    FNDockWidget* m_dockDSP;

    QMenu* fileMenu;
    QMenu* optionMenu;
    QMenu* viewMenu;
    QMenu* tabMenu;
    QMenu* helpMenu;
    QMenu *recentFilesMenu;

    QToolBar* fileToolBar;
    QToolBar* editToolBar;
    ToolBar* m_toolsToolBar;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QAction *separatorAct;
    QString curFile;

    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* quitAct;
    QAction* saveSceneAct;
    QAction* newMainGLAct;

    QAction* lockDockTitlesAct;
    QAction* renderCrosshairsAct;

    QAction* showAxialAct;
    QAction* showCoronalAct;
    QAction* showSagittalAct;

    QAction* screenshotAct;
    QAction* resetSettingsAct;
    QAction* aboutAct;

    QAction* toggleShaderEditAct;

    QAction* standardViewAxialAct;
    QAction* standardViewCoronalAct;
    QAction* standardViewSagittalAct;

    QAction* newSelectionBoxAct;
    QAction* continousRenderingAct;

    QAction* dilbertAct;

    QFileDialog* fd;

    static int countMainGL;

public slots:
    void screenshot();
    void runScript();

private slots:
    void slotNew();
    void open();
    void openRecentFile();
    void save();
    void saveScene();
    void about();
    void resetSettings();
    void newMainGL();
    void continousRendering();

    void slotToggleAxialSlice();
    void slotToggleCoronalSlice();
    void slotToggleSagittalSlice();

    void slotStandardAxialView();
    void slotStandardCoronalView();
    void slotStandardSagittalView();
    void slotToggleShaderEdit();

    void slotDilbert();

    void slotRenderCrosshairs( bool value );

    void slotDatasetSelectionChanged();

    void saveFilterChanged(QString filterString);
};

#endif
