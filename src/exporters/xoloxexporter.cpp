#include "xoloxexporter.h"
#include "../fontconfig.h"


XoloxExporter::XoloxExporter(QObject *parent) :
    AbstractExporter(parent)
{
  setExtension("xfd");
}

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

static const char HeaderTag[] = "XFD0";
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

struct xolox_bitmap_font_symbols_header
{
  u32 TotalSizeInBytes;
  u32 SymbolCount;
  u16 FontSize;
  u16 LineHeight;
  u16 AtlasWidth;
  u16 AtlasHeight;
}PACK_STRUCT;

struct xolox_bitmap_font_symbol_description
{
  char CharCode;
  u16 X, Y, W, H;
  u16 OffsetX, OffsetY;
  u16 AdvanceX;
}PACK_STRUCT;

// Default alignment
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT


bool XoloxExporter::Export(QByteArray& out)
{
  const FontConfig* cfg = fontConfig();
  int height = metrics().height;

  xolox_bitmap_font_symbols_header Header = {};
  Header.TotalSizeInBytes = HeaderTagSize +
                            sizeof(xolox_bitmap_font_symbols_header) +
                            sizeof(xolox_bitmap_font_symbol_description) * symbols().size();
  Header.SymbolCount = symbols().size();
  Header.FontSize = cfg->size();
  Header.LineHeight = height;
  Header.AtlasWidth = texWidth();
  Header.AtlasHeight = texHeight();

  out.append((char *) &HeaderTag, HeaderTagSize);
  out.append((char *) &Header, sizeof(Header));


  xolox_bitmap_font_symbol_description Description = {};

  foreach(const Symbol& c , symbols())
  {
    Description.CharCode = c.id;
    Description.X = c.placeX;
    Description.Y = c.placeY;
    Description.W = c.placeW;
    Description.H = c.placeH;
    Description.OffsetX = c.offsetX;
    Description.OffsetY = c.offsetY;
    Description.AdvanceX = c.advance;

    out.append((char *) &Description, sizeof(Description));
  }



  return true;
}


AbstractExporter* XoloxExporterFactoryFunc (QObject* parent) {
    return new XoloxExporter(parent);
}
