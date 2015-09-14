#include "synchronizednetworkaccessmanager.hpp"

ProgressBar::ProgressBar(QWidget *parent):
    QDialog{ parent }, m_progressBar{ new QProgressBar }
{
    m_progressBar->setRange( 1, 100 );
    m_progressBar->setTextVisible( true );

    QVBoxLayout *layout = new QVBoxLayout( this );
    layout->addWidget( new QLabel( "Loading, please wait..." ) );
    layout->addWidget( m_progressBar );
    setLayout( layout );
}

void ProgressBar::setProgressStatus( qint64 done )
{
    m_progressBar->setValue( done );
    if( done == 100 ){
        reset();
        accept();
    }
}

void ProgressBar::reset()
{
    m_progressBar->reset();
}

SyncNetworkAccessManager::SyncNetworkAccessManager( QObject *parent ):
    QObject( parent ), m_progressBar{ new ProgressBar }
{
    m_progressBar->setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint );
    m_progressBar->setWindowModality( Qt::ApplicationModal );
}

SyncNetworkAccessManager::~SyncNetworkAccessManager(){}

void SyncNetworkAccessManager::getRequest( QUrl const & url )
{
    QNetworkRequest request{ url };
    request.setRawHeader( "USER-AGENT", "Purple" );
    QNetworkReply *reply = m_networkManager.get( request );

    QObject::connect( reply, SIGNAL(finished()), this, SLOT(onFinished()) );
    QObject::connect( reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
    QObject::connect( reply, SIGNAL( error( QNetworkReply::NetworkError )),
                      this, SLOT( onNetworkError( QNetworkReply::NetworkError )) );
    QObject::connect( reply, SIGNAL(finished()), &m_eventLoop, SLOT(quit()) );
    m_progressBar->show();
    m_eventLoop.exec();
}

void SyncNetworkAccessManager::cleanup()
{
    m_progressBar->accept();
    if( m_eventLoop.isRunning() ) m_eventLoop.quit();
}

void SyncNetworkAccessManager::onFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>( sender() );
    if( !reply ) return;

    if( reply->error() != QNetworkReply::NoError ){
        QMessageBox::critical( nullptr, "Error", reply->errorString(), QMessageBox::Ok );
        return;
    }
    cleanup();
    result = reply->readAll();
}

void SyncNetworkAccessManager::onNetworkError( QNetworkReply::NetworkError error )
{
    cleanup();
    switch( error )
    {
    case QNetworkReply::NetworkError::RemoteHostClosedError:
        QMessageBox::critical( nullptr, "Error", "Remote Host Connection Closed.", QMessageBox::Ok );
        break;
    case QNetworkReply::NetworkError::HostNotFoundError:
        QMessageBox::critical( nullptr, "Error", "Host Not Found", QMessageBox::Ok );
        break;
    case QNetworkReply::NetworkError::TimeoutError:
        QMessageBox::critical( nullptr, "Error", "Network Timed Out", QMessageBox::Ok );
        break;
    case QNetworkReply::NetworkError::OperationCanceledError:
        QMessageBox::critical( nullptr, "Error", "Operation cancelled from the server side.", QMessageBox::Ok );
        break;
    case QNetworkReply::NetworkError::SslHandshakeFailedError:
        QMessageBox::critical( nullptr, "Error", "SSL Handshake Failed Error, try again!", QMessageBox::Ok );
        break;
    case QNetworkReply::NetworkError::TemporaryNetworkFailureError:
        QMessageBox::critical( nullptr, "Error", "Temporary Network Failure", QMessageBox::Ok );
        break;
    case QNetworkReply::NetworkError::UnknownNetworkError: default:
        break;
    }
}

void SyncNetworkAccessManager::onDownloadProgress(qint64 received, qint64 total )
{
    if( total == 0 ) return;
    qint64 progressStatus = ( received * 100 )/ total;
    m_progressBar->setProgressStatus( progressStatus );
}
