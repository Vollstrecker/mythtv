#ifndef THUMBFINDER_H_
#define THUMBFINDER_H_

// qt
#include <QString>
#include <QStringList>
#include <QScopedPointer>

// mythtv
#include <mythscreentype.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include "programtypes.h"

// mytharchive
#include "archiveutil.h"
#include "remoteavformatcontext.h"
#include "mythavutil.h"

typedef struct SeekAmount
{
    QString name;
    int amount;
} SeekAmount;

extern struct SeekAmount SeekAmounts[];
extern int SeekAmountsCount;

class MythUIButton;
class MythUItext;
class MythUIImage;
class MythUIButtonList;
class MythUIButtonListItem;
class MythImage;

class ThumbFinder : public MythScreenType
{

  Q_OBJECT

  public:

      ThumbFinder(MythScreenStack *parent, ArchiveItem *archiveItem,
                  const QString &menuTheme);
    ~ThumbFinder();

    bool Create(void) override; // MythScreenType
    bool keyPressEvent(QKeyEvent *) override; // MythScreenType


  private slots:
    void gridItemChanged(MythUIButtonListItem *item);
    void ShowMenu(void) override; // MythScreenType
    void cancelPressed(void);
    void savePressed(void);
    void updateThumb(void);

  private:
    void Init(void) override; // MythScreenType
    bool getThumbImages(void);
    int  getChapterCount(const QString &menuTheme);
    void changeSeekAmount(bool up);
    void updateCurrentPos(void);
    bool seekToFrame(int frame, bool checkPos = true);
    QString createThumbDir(void);
    QString frameToTime(int64_t frame, bool addFrame = false);

    // avcodec stuff
    bool initAVCodec(const QString &inFile);
    void closeAVCodec();
    bool seekForward();
    bool seekBackward();
    bool getFrameImage(bool needKeyFrame = true, int64_t requiredPTS = -1);
    int  checkFramePosition(int frameNumber);
    void loadCutList(void);
    void updatePositionBar(int64_t frame);
    int  calcFinalDuration(void);

    RemoteAVFormatContext m_inputFC      {nullptr};
    AVCodecContext  *m_codecCtx          {nullptr};
    AVCodec         *m_codec             {nullptr};
    MythAVFrame      m_frame;
    MythAVCopy       m_copy;
    QScopedPointer<MythPictureDeinterlacer> m_deinterlacer;

    float            m_fps               {0.0F};
    unsigned char   *m_outputbuf         {nullptr};
    QString          m_frameFile;
    int              m_frameWidth        { 0};
    int              m_frameHeight       { 0};
    int              m_videostream       { 0};
    int              m_currentSeek       { 0};
    int64_t          m_startTime         {-1}; // in time_base units
    int64_t          m_startPTS          {-1}; // in time_base units
    int64_t          m_currentPTS        {-1}; // in time_base units
    int64_t          m_firstIFramePTS    {-1};
    int              m_frameTime         { 0};   // in time_base units
    bool             m_updateFrame       {false};
    frm_dir_map_t    m_deleteMap;
    int              m_finalDuration     { 0};
    int              m_offset            { 0};

    ArchiveItem        *m_archiveItem    {nullptr};
    int                 m_thumbCount;
    QList<ThumbImage *> m_thumbList;
    QString             m_thumbDir;

    // GUI stuff
    MythUIButton       *m_frameButton    {nullptr};
    MythUIButton       *m_saveButton     {nullptr};
    MythUIButton       *m_cancelButton   {nullptr};
    MythUIImage        *m_frameImage     {nullptr};
    MythUIImage        *m_positionImage  {nullptr};
    MythUIButtonList   *m_imageGrid      {nullptr};
    MythUIText         *m_seekAmountText {nullptr};
    MythUIText         *m_currentPosText {nullptr};
};

#endif
