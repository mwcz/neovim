#ifndef NVIM_EX_GETLN_H
#define NVIM_EX_GETLN_H

#include "nvim/eval/typval.h"
#include "nvim/ex_cmds.h"
#include "nvim/ex_cmds_defs.h"
#include "nvim/os/time.h"
#include "nvim/regexp_defs.h"

// Values for nextwild() and ExpandOne().  See ExpandOne() for meaning.
#define WILD_FREE               1
#define WILD_EXPAND_FREE        2
#define WILD_EXPAND_KEEP        3
#define WILD_NEXT               4
#define WILD_PREV               5
#define WILD_ALL                6
#define WILD_LONGEST            7
#define WILD_ALL_KEEP           8
#define WILD_CANCEL             9
#define WILD_APPLY              10

#define WILD_LIST_NOTFOUND      0x01
#define WILD_HOME_REPLACE       0x02
#define WILD_USE_NL             0x04
#define WILD_NO_BEEP            0x08
#define WILD_ADD_SLASH          0x10
#define WILD_KEEP_ALL           0x20
#define WILD_SILENT             0x40
#define WILD_ESCAPE             0x80
#define WILD_ICASE              0x100
#define WILD_ALLLINKS           0x200
#define WILD_IGNORE_COMPLETESLASH   0x400
#define WILD_NOERROR            0x800  // sets EW_NOERROR
#define WILD_BUFLASTUSED        0x1000
#define BUF_DIFF_FILTER         0x2000

/// Present history tables
typedef enum {
  HIST_DEFAULT = -2,  ///< Default (current) history.
  HIST_INVALID = -1,  ///< Unknown history.
  HIST_CMD = 0,       ///< Colon commands.
  HIST_SEARCH,        ///< Search commands.
  HIST_EXPR,          ///< Expressions (e.g. from entering = register).
  HIST_INPUT,         ///< input() lines.
  HIST_DEBUG,         ///< Debug commands.
} HistoryType;

/// Number of history tables
#define HIST_COUNT      (HIST_DEBUG + 1)

typedef char_u *(*CompleteListItemGetter)(expand_T *, int);

/// History entry definition
typedef struct hist_entry {
  int hisnum;           ///< Entry identifier number.
  char_u *hisstr;       ///< Actual entry, separator char after the NUL.
  Timestamp timestamp;  ///< Time when entry was added.
  list_T *additional_elements;  ///< Additional entries from ShaDa file.
} histentry_T;

/// Command-line colors: one chunk
///
/// Defines a region which has the same highlighting.
typedef struct {
  int start;  ///< Colored chunk start.
  int end;  ///< Colored chunk end (exclusive, > start).
  int attr;  ///< Highlight attr.
} CmdlineColorChunk;

/// Command-line colors
///
/// Holds data about all colors.
typedef kvec_t(CmdlineColorChunk) CmdlineColors;

/// Command-line coloring
///
/// Holds both what are the colors and what have been colored. Latter is used to
/// suppress unnecessary calls to coloring callbacks.
typedef struct {
  unsigned prompt_id;  ///< ID of the prompt which was colored last.
  char *cmdbuff;  ///< What exactly was colored last time or NULL.
  CmdlineColors colors;  ///< Last colors.
} ColoredCmdline;

/// Keeps track how much state must be sent to external ui.
typedef enum {
  kCmdRedrawNone,
  kCmdRedrawPos,
  kCmdRedrawAll,
} CmdRedraw;

// Variables shared between getcmdline(), redrawcmdline() and others.
// These need to be saved when using CTRL-R |, that's why they are in a
// structure.
struct cmdline_info {
  char_u *cmdbuff;         // pointer to command line buffer
  int cmdbufflen;               // length of cmdbuff
  int cmdlen;                   // number of chars in command line
  int cmdpos;                   // current cursor position
  int cmdspos;                  // cursor column on screen
  int cmdfirstc;                // ':', '/', '?', '=', '>' or NUL
  int cmdindent;                // number of spaces before cmdline
  char_u *cmdprompt;       // message in front of cmdline
  int cmdattr;                  // attributes for prompt
  int overstrike;               // Typing mode on the command line.  Shared by
                                // getcmdline() and put_on_cmdline().
  expand_T *xpc;             // struct being used for expansion, xp_pattern
                             // may point into cmdbuff
  int xp_context;               // type of expansion
  char_u *xp_arg;          // user-defined expansion arg
  int input_fn;                 // when TRUE Invoked for input() function
  unsigned prompt_id;  ///< Prompt number, used to disable coloring on errors.
  Callback highlight_callback;  ///< Callback used for coloring user input.
  ColoredCmdline last_colors;   ///< Last cmdline colors
  int level;                    // current cmdline level
  struct cmdline_info *prev_ccline;  ///< pointer to saved cmdline state
  char special_char;            ///< last putcmdline char (used for redraws)
  bool special_shift;           ///< shift of last putcmdline char
  CmdRedraw redraw_state;       ///< needed redraw for external cmdline
};

typedef struct cmdline_info CmdlineInfo;

#ifdef INCLUDE_GENERATED_DECLARATIONS
# include "ex_getln.h.generated.h"
#endif
#endif  // NVIM_EX_GETLN_H
