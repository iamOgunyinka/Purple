#ifndef VIDEOTABLEMODEL_HPP
#define VIDEOTABLEMODEL_HPP

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>
#include <QPixmap>
#include <QSize>
#include <functional>

#include "videostructure.hpp"

namespace Purple
{
    enum ThumbnailViewQuality
    {
        DefaultRes = 0,
        MediumRes = 1,
        HighRes = 2
    };
    QString qualityToString( ThumbnailViewQuality quality );

    class VideoTableModel : public QAbstractTableModel
    {
        Q_OBJECT
    public:
        explicit VideoTableModel( QByteArray const &data, QObject *parent = nullptr );

        Purple::VideoStructure const & videoStructure() const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        int rowCount( QModelIndex const &parent = QModelIndex{} ) const override;
        int columnCount(const QModelIndex &parent) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        void setThumbnailQuality( ThumbnailViewQuality quality );
    private:
        Purple::VideoStructure const m_videoStructure;
        QString resultType( QString const & result ) const;

        enum View
        {
            e_thumbnail = 0,
            e_title,
            e_description,
            e_channelTitle,
            e_publishDate,
            e_resultType,
            e_max_limit
        };
    };

    class CustomFilter
    {
    protected:
        std::function<bool(int, QModelIndex const &, VideoTableModel *)> m_proxy;
    public:
        CustomFilter();
        CustomFilter( std::function<bool(int, QModelIndex const &, VideoTableModel *)> proxy );
        bool operator()( int, QModelIndex const &, VideoTableModel * ) const;
    };

    struct CustomVideoFilterProxyModel: QSortFilterProxyModel
    {
        Q_OBJECT

        CustomFilter m_filter;
    protected:
        bool filterAcceptsRow(int source_row, const QModelIndex &source_parent ) const override;
    public:
        CustomVideoFilterProxyModel();
        using QSortFilterProxyModel::QSortFilterProxyModel;
        void setFilter( CustomFilter filter );
        static bool noFilters( int source_row, QModelIndex const &source_index, VideoTableModel * item );
    };
}
#endif // VIDEOTABLEMODEL_HPP
