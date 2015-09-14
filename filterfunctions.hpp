#ifndef FILTER_FUNCTIONS_HPP
#define FILTER_FUNCTIONS_HPP

#include <QDateTime>
#include <functional>
#include "videotablemodel.hpp"

namespace Purple
{
    using filter_function_t = std::function<bool( int, QModelIndex const &, VideoTableModel * )>;
    static QDate todaysDate = QDate::currentDate();

    inline namespace FilterFunctionImplementations
    {
        static inline bool filterDataByTodaysDate( int current_row, QModelIndex const &, VideoTableModel * model )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet().publishedDate().date()
                    == todaysDate;
        }

        static inline bool filterDataFromLastSevenDays( int current_row, QModelIndex const &, VideoTableModel *model  )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet().publishedDate().date().addDays( -7 ) >= todaysDate;
        }

        static inline bool filterDataFromThisMonth( int current_row, QModelIndex const &, VideoTableModel * model )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet().publishedDate().date()
                    >= QDate( todaysDate.year(), todaysDate.month(), 1 );
        }

        static inline bool filterDataFromLastYear(int current_row, QModelIndex const &, VideoTableModel * model )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet().publishedDate().date().year()
                    == ( todaysDate.year() - 1 );
        }
        
        static inline bool filterDataFromThisYear(int current_row, QModelIndex const &, VideoTableModel * model )
        {
            return model->videoStructure().getSearchResponse( current_row ).snippet().publishedDate().date()
                    >= QDate( todaysDate.year(), 1, 1 );
        }

        static inline bool filterDataByVideoTypeImpl( int current_row, QModelIndex const &, VideoTableModel *model, QString const & type )
        {
            return model->videoStructure().getSearchResponse( current_row ).id().kind() == type;
        }

        static inline bool showAllResults( int, QModelIndex const &, VideoTableModel *)
        {
            return true;
        }

        static auto filterDataByVideo = std::bind( filterDataByVideoTypeImpl, std::placeholders::_1, std::placeholders::_2,
                                            std::placeholders::_3, "youtube#video" );
        static auto filterDataByChannel = std::bind( filterDataByVideoTypeImpl, std::placeholders::_1, std::placeholders::_2,
                                            std::placeholders::_3, "youtube#channel" );
        static auto filterDataByPlaylist = std::bind( filterDataByVideoTypeImpl, std::placeholders::_1, std::placeholders::_2,
                                            std::placeholders::_3, "youtube#playlist" );
        static auto filterDataByMovie = std::bind( filterDataByVideoTypeImpl, std::placeholders::_1, std::placeholders::_2,
                                            std::placeholders::_3, "youtube#movie" );
        static auto filterDataByShow = std::bind( filterDataByVideoTypeImpl, std::placeholders::_1, std::placeholders::_2,
                                            std::placeholders::_3, "youtube#show" );
    }
}
#endif // FILTER_FUNCTIONS_HPP
