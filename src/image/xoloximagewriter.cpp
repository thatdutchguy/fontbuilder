#include "xoloximagewriter.h"
#include "builtinimagewriter.h"
#include "layoutdata.h"
#include <QImage>


XoloxImageWriter::XoloxImageWriter(QString ext, QObject *parent) :
    AbstractImageWriter(parent)
{
    setExtension(ext);
    setReloadSupport(false);
}


typedef unsigned char u8;
typedef char c8;
typedef unsigned short u16;
typedef unsigned int u32;

static const char HeaderTag[] = "XFI0";
static const u16 HeaderTagSize = sizeof(HeaderTag) - 1;


// byte-align structures
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	define PACK_STRUCT
#endif


struct xolox_bitmap_font_image_header
{
  u32 TotalSizeInBytes;
  u32 ImageSizeInBytes;
  u16 Width;
  u16 Height;
}PACK_STRUCT;

// Default alignment
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT

bool XoloxImageWriter::Export(QFile& file)
{
    QImage pixmap = buildImage();

    xolox_bitmap_font_image_header Header = {};
    Header.Width = pixmap.width();
    Header.Height = pixmap.height();
    Header.ImageSizeInBytes = Header.Width * Header.Height;
    Header.TotalSizeInBytes = Header.ImageSizeInBytes + sizeof(Header) + HeaderTagSize;

    file.write((const char *) &HeaderTag, HeaderTagSize);
    file.write((const char *) &Header, sizeof(Header));

    for (int y = 0; y < pixmap.height(); ++y)
    {
      for (int x = 0; x < pixmap.width(); x += 4)
      {
        // x + 3 -> alpha channel only
        file.write(((const char *) pixmap.scanLine(y)) + x + 3, 1);
      }
    }

    return true;
}
