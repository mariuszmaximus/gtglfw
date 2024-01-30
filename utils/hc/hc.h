#ifndef HC_H_
#define HC_H_

const double DOUBLE_CLICK_TIME = 0.5;

const char *const hc_logo[] =
{
   "................",
   "................",
   "................",
   "................",
   "................",
   "..0...0...0000..",
   "..0...0...0.....",
   "..00000...0.....",
   "..0...0...0.....",
   "..0...0...0000..",
   "................",
   "................",
   "................",
   "................",
   "................",
   "................"
};

const unsigned char icon_colors[ 2 ][ 4 ] =
{
   { 0,   0, 0, 255 },
   { 0, 255, 0, 255 },
};

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
typedef struct _Panel Panel;

struct _Panel
{
   App        *pApp;
   int         col;
   int         row;
   int         maxCol;
   int         maxRow;
   char        currentDir[ 512 ];

   FileInfo   *pFiles;
   int         nFilesCount;

   int         rowBar;
   int         rowNo;

   const char *cmdLine;
   int         cmdCol;
   int         cmdColNo;

   bool        isFirstDirectory;
   bool        isHiddenDirectory;
   bool        isFirstFile;
   bool        isHiddenFile;
};

static Panel *PanelInit( App *pApp );
static void   PanelFree( Panel *pPanel );
static void   PanelFetchList( Panel *pPanel, const char *currentDir );
static int    CompareFiles( const void *a, const void *b );
static void   UpdatePanelFetchList( Panel *pPanel, const char *newDir );
static void   Autosize( App *pApp );
static void   Resize( Panel *pPanel, int col, int row, int maxCol, int maxRow );
static void   DrawPanel( Panel *pPanel );
static const char *PaddedString( Panel *pPanel, int longestName, int longestSize, const char *name, const char *size, const char *date, const char *time, const char *attr );
static const char *SelectColor( const char *attr, bool state );
static void   SafeStrCopy( char *dest, const char *src, size_t destSize );
static void   DrawComdLine( App *pApp, Panel *pPanel );
static void   ChangeDir( Panel *pPanel );
static int    DirIndexName( Panel *pPanel, const char *tmpDir );
static int    FindLongestName( Panel *pPanel );
static int    FindLongestSize( Panel *pPanel );
static void   SetIcon( GLFWwindow *window );
static void   PrintPanelStructure( const Panel *pPanel );

#define IIF( condition, trueValue, falseValue ) ( ( condition ) ? ( trueValue ) : ( falseValue ) )

#endif /* End HC_H_ */
