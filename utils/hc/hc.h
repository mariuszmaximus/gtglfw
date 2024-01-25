#ifndef HC_H_
#define HC_H_

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
static const char *PaddedString( const char *name );
static void   DrawComdLine( App *pApp, Panel *pPanel );
static void   ChangeDir( Panel *pPanel );
static int    DirIndexName( Panel *pPanel, const char *tmpDir );
static void   PrintPanelStructure( const Panel *pPanel );

#define IIF( condition, trueValue, falseValue ) ( ( condition ) ? ( trueValue ) : ( falseValue ) )

#endif /* End HC_H_ */
