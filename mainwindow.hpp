#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtWidgets>
#include <QMap>
#include <QHeaderView>
#include <memory>
#include <mutex>
#include "searchdialog.hpp"
#include "filterfunctions.hpp"
#include "extractor.hpp"
#include "downloadmanager.hpp"
#include "videodetails.hpp"

namespace Purple
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        enum ToolbarIconDisplayFormat
        {
            IconOnly = 0,
            TextOnly,
            TextBesideIcon,
            TextBelowIcon,
            Hide
        };

    public:
        explicit    MainWindow( QWidget *parent = nullptr );
        ~MainWindow();

        static std::map< QString, filter_function_t > filterSearchResultsByDate;
        static std::map< QString, filter_function_t > filterSearchResultsByType;
        static QString YOUTUBE_URL;
        static QString API_KEY;
        static unsigned int MAX_RESULT;
        static void download( QString const & url, QString const & title,
                              QMainWindow *parent = nullptr );
    private slots:
        void        handleAllErrors( QString const & );
        void        basicSearch();
        void        advancedSearch();
        void        directDownload();
        void        findDownloadLink();
        void        closeEvent( QCloseEvent * );
        void        aboutPurple();
        void        viewDetails( QModelIndex const & );

        void        thumbnailMapped(int quality);
        void        iconDisplayMapped( int t );
        void        filterUploadHandler();
        void        filterTypeHandler();
        void        filterController(QString const &current_filter);
    private:
        void        displayVideoInfo( QByteArray const & response );
        void        universalDownload( QString const & videoID );
        void        sendRequest( QString const & request );
        void        setupMenubar();
        void        setupToolbar();
        void        setupActions();
        void        connectAllActions();
        void        setupWindowButtons();
        bool        isSafeToExit();
    private:
        QMenu       *m_fileMenu;
        QMenu       *m_preferenceMenu;

        QToolBar    *m_toolbar;

        QAction     *m_basicSearchAction;
        QAction     *m_advancedSearchAction;
        QAction     *m_directDownloadAction;
        QAction     *m_downloadAction;
        QAction     *m_exitAction;
        QAction     *m_aboutQt;
        QAction     *m_aboutPurple;

        QSignalMapper *m_thumbnailViewmapper;
        QSignalMapper *m_iconDisplayMapper;

        QTableView  *m_videoDisplayTable;
        VideoDetails *m_videoDetailsList;

        QListWidget *m_filterList;

        QPushButton *m_filterByUploadDateButton;
        QPushButton *m_filterByTypeButton;

        SyncNetworkAccessManager      *m_networkManager;
        CustomVideoFilterProxyModel   *m_proxyModel;
        VideoTableModel               *m_underlyingProxyTableModel;
        ThumbnailViewQuality           m_thumbnailQuality;
    signals:

    public slots:

    };
}
#endif // MAINWINDOW_HPP
