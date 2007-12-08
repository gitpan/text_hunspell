#ifndef _HASHMGR_HXX_
#define _HASHMGR_HXX_

#ifndef _HTYPES_HXX_
#define _HTYPES_HXX_

#include <cstdlib>
#include <cstring>
#include <cstdio>


#define MAXDELEN    256

#define ROTATE_LEN   5

#define ROTATE(v,q) \
   (v) = ((v) << (q)) | (((v) >> (32 - q)) & ((1 << (q))-1));

// approx. number  of user defined words
#define USERWORD 1000

#ifdef HUNSPELL_STATIC
	#define DLLEXPORT
#else
	#ifdef HUNSPELL_EXPORTS
		#define DLLEXPORT  __declspec( dllexport )
	#else
		#define DLLEXPORT  __declspec( dllimport )
	#endif
#endif


typedef struct {
    unsigned char l;
    unsigned char h;
} w_char;


struct hentry
{
  short    wlen;
  short    alen;
  char *   word;
  unsigned short * astr;
  struct   hentry * next;
  struct   hentry * next_homonym;
  char *   description;
};

#endif


enum flag { FLAG_CHAR, FLAG_LONG, FLAG_NUM };

class HashMgr
{
  int             tablesize;
  struct hentry * tableptr;
  int	          userword;
  flag            flag_mode;
  int             complexprefixes;
  int             utf8;

public:
  HashMgr(const char * tpath, const char * apath);
  ~HashMgr();

  struct hentry * lookup(const char *) const;
  int hash(const char *) const;
  struct hentry * walk_hashtable(int & col, struct hentry * hp) const;

  int put_word(const char * word, int wl, char * ap);
  int put_word_pattern(const char * word, int wl, const char * pattern);
  int decode_flags(unsigned short ** result, char * flags);
  unsigned short        decode_flag(const char * flag);
  char *                encode_flag(unsigned short flag);
  
private:
  int load_tables(const char * tpath);
  int add_word(const char * word, int wl, unsigned short * ap, int al, const char * desc);
  int load_config(const char * affpath);

};

#endif 

#ifndef _AFFIXMGR_HXX_
#define _AFFIXMGR_HXX_

#ifndef _ATYPES_HXX_
#define _ATYPES_HXX_

// HUNSTEM def.
#define HUNSTEM

#define SETSIZE         256
#define CONTSIZE        65536
#define MAXWORDLEN      100
#define MAXWORDUTF8LEN  (MAXWORDLEN * 4)
#define XPRODUCT        (1 << 0)

enum {IN_CPD_NOT, IN_CPD_BEGIN, IN_CPD_END};

#define MAXLNLEN        8192 * 4

#define MAXCOMPOUND	10

#define MAXACC          1000

#define MISSING_DESCRIPTION "[<missing_description>]"

#define FLAG unsigned short
#define FLAG_NULL 0x00
#define FREE_FLAG(a) a = 0

#define TESTAFF( a, b , c ) flag_bsearch((unsigned short *) a, (unsigned short) b, c)

struct affentry
{
   char * strip;
   char * appnd;
   short  stripl;
   short  appndl;
   short  numconds;
   short  xpflg;
   unsigned short aflag;
   char   utf8;
   union {
   	char   base[SETSIZE];
	struct {
		char  ascii[SETSIZE/2];
                char neg[8];
                char all[8];
                w_char * wchars[8];
		int wlen[8];
	} utf8;
   } conds;
   char *       morphcode;
   unsigned short * contclass;
   short        contclasslen;
};

struct replentry {
  char * pattern;
  char * pattern2;
};

struct mapentry {
  char * set;
  w_char * set_utf16;
  int len;
};

struct guessword {
  char * word;
  bool allow;
};

#endif
#ifndef _BASEAFF_HXX_
#define _BASEAFF_HXX_

class AffEntry
{
public:

protected:
       char *       appnd;
       char *       strip;
       short        appndl;
       short        stripl;
       short        numconds;
       short        xpflg;
       unsigned short aflag;
       char         utf8;
       union {
   	 char   base[SETSIZE];
	 struct {
		char  ascii[SETSIZE/2];
                char neg[8];
                char all[8];
                w_char * wchars[8];
		int wlen[8];
	 } utf8;
       } conds;
       char *       morphcode;
       unsigned short * contclass;
       short        contclasslen;
};

#endif


#ifndef _HASHMGR_HXX_
#define _HASHMGR_HXX_

enum flag { FLAG_CHAR, FLAG_LONG, FLAG_NUM };

class HashMgr
{
  int             tablesize;
  struct hentry * tableptr;
  int	          userword;
  flag            flag_mode;
  int             complexprefixes;
  int             utf8;

public:
  HashMgr(const char * tpath, const char * apath);
  ~HashMgr();

  struct hentry * lookup(const char *) const;
  int hash(const char *) const;
  struct hentry * walk_hashtable(int & col, struct hentry * hp) const;

  int put_word(const char * word, int wl, char * ap);
  int put_word_pattern(const char * word, int wl, const char * pattern);
  int decode_flags(unsigned short ** result, char * flags);
  unsigned short        decode_flag(const char * flag);
  char *                encode_flag(unsigned short flag);
  
private:
  int load_tables(const char * tpath);
  int add_word(const char * word, int wl, unsigned short * ap, int al, const char * desc);
  int load_config(const char * affpath);

};

#endif


class AffixMgr
{

  AffEntry *          pStart[SETSIZE];
  AffEntry *          sStart[SETSIZE];
  AffEntry *          pFlag[CONTSIZE];
  AffEntry *          sFlag[CONTSIZE];
  HashMgr *           pHMgr;
  char *              trystring;
  char *              encoding;
  int                 utf8;
  struct unicode_info2 * utf_tbl;
  int                 complexprefixes;
  FLAG                compoundflag;  
  FLAG                compoundbegin;
  FLAG                compoundmiddle;
  FLAG                compoundend;
  FLAG                compoundroot;
  FLAG                compoundforbidflag;
  FLAG                compoundpermitflag;
  int                 checkcompounddup;
  int                 checkcompoundrep;
  int                 checkcompoundtriple;
  FLAG                forbiddenword;
  FLAG                pseudoroot;
  int                 cpdmin;
  int                 numrep;
  replentry *         reptable;
  int                 nummap;
  mapentry *          maptable;
  int                 numcheckcpd;
  replentry *         checkcpdtable;
  int                 maxngramsugs;
  int                 nosplitsugs;
  int                 sugswithdots;
  int                 cpdwordmax;
  int                 cpdmaxsyllable;
  char *              cpdvowels;
  w_char *            cpdvowels_utf16;
  int                 cpdvowels_utf16_len;
  char *              cpdsyllablenum;
  const char *        pfxappnd; // BUG: not stateless
  const char *        sfxappnd; // BUG: not stateless
  FLAG                sfxflag;  // BUG: not stateless
  char *              derived;  // BUG: not stateless
  AffEntry *          sfx;      // BUG: not stateless
  AffEntry *          pfx;      // BUG: not stateless
  int                 checknum;
  char *              wordchars;
  char *              version;
  char *              lang;
  int                 langnum;
  FLAG                lemma_present;
  FLAG                circumfix;
  FLAG                onlyincompound;

  int                 havecontclass; // boolean variable
#ifndef NOT_SHORT_2BYTE
  short               contclasses[CONTSIZE]; // flags of possible continuing classes (twofold affix)
#endif
  flag                flag_mode;

public:
 
  AffixMgr(const char * affpath, HashMgr * ptr);
  ~AffixMgr();
  struct hentry *     affix_check(const char * word, int len,
            const unsigned short needflag = (unsigned short) 0, char in_compound = IN_CPD_NOT);
  struct hentry *     prefix_check(const char * word, int len,
            char in_compound, const FLAG needflag = FLAG_NULL);
  struct hentry *     prefix_check_twosfx(const char * word, int len,
            char in_compound, const FLAG needflag = FLAG_NULL);
  struct hentry *     suffix_check(const char * word, int len, int sfxopts, AffEntry* ppfx,
			char ** wlst, int maxSug, int * ns, const FLAG cclass = FLAG_NULL,
                        const FLAG needflag = FLAG_NULL, char in_compound = IN_CPD_NOT);
  struct hentry *     suffix_check_twosfx(const char * word, int len,
            int sfxopts, AffEntry* ppfx, const FLAG needflag = FLAG_NULL);

  char * affix_check_morph(const char * word, int len,
                    const FLAG needflag = FLAG_NULL, char in_compound = IN_CPD_NOT);
  char * prefix_check_morph(const char * word, int len,
                    char in_compound, const FLAG needflag = FLAG_NULL);
  char * suffix_check_morph (const char * word, int len, int sfxopts, AffEntry * ppfx,
            const FLAG cclass = FLAG_NULL, const FLAG needflag = FLAG_NULL, char in_compound = IN_CPD_NOT);

  char * prefix_check_twosfx_morph(const char * word, int len,
            char in_compound, const FLAG needflag = FLAG_NULL);
  char * suffix_check_twosfx_morph(const char * word, int len,
            int sfxopts, AffEntry * ppfx, const FLAG needflag = FLAG_NULL);

  int                 expand_rootword(struct guessword * wlst, int maxn, const char * ts,
                        int wl, const unsigned short * ap, unsigned short al, char * bad, int);

  int                 get_syllable (const char * word, int wlen);
  int                 repl_check(const char * word, int len);
  int                 checkcpd_check(const char * word, int len);
  int                 candidate_check(const char * word, int len);
  struct hentry *     compound_check(const char * word, int len,
                              int wordnum, int numsyllable, int maxwordnum, int hu_mov_rule,
			      int * cmpdstemnum, int * cmpdstem);

  int compound_check_morph(const char * word, int len,
                              int wordnum, int numsyllable, int maxwordnum, int hu_mov_rule,
                              char ** result, char * partresult);

  struct hentry *     lookup(const char * word);
  int                 get_numrep();
  struct replentry *  get_reptable();
  int                 get_nummap();
  struct mapentry *   get_maptable();
  char *              get_encoding();
  int                 get_langnum();
  struct unicode_info2 * get_utf_conv();
  char *              get_try_string();
  const char *        get_wordchars();
  FLAG                get_compoundflag();
  FLAG                get_compoundbegin();
  FLAG                get_forbiddenword();
  FLAG                get_pseudoroot();
  FLAG                get_onlyincompound();
  FLAG                get_compoundroot();
  FLAG                get_lemma_present();
  int                 get_checknum();
  char *              get_possible_root();
  const char *        get_prefix();
  const char *        get_suffix();
  const char *        get_derived();
  const char *        get_version();
  const int           have_contclass();
  int                 get_utf8();
  int                 get_complexprefixes();
  char *              get_suffixed(char );
  int                 get_maxngramsugs();
  int                 get_nosplitsugs();
  int                 get_sugswithdots(void);

private:
  int  parse_file(const char * affpath);
  int  parse_try(char * line);
  int  parse_set(char * line);
  int  parse_flag(char * line, unsigned short * out, char * name);
  int  parse_num(char * line, int * out, char * name);
  int  parse_cpdflag(char * line);
  int  parse_cpdforbid(char * line);
  int  parse_forbid(char * line);
  int  parse_cpdsyllable(char * line);
  int  parse_syllablenum(char * line);
  int  parse_reptable(char * line, FILE * af);
  int  parse_maptable(char * line, FILE * af);
  int  parse_checkcpdtable(char * line, FILE * af);
  int  parse_affix(char * line, const char at, FILE * af);
  int  parse_wordchars(char * line);
  int  parse_lang(char * line);
  int  parse_version(char * line);

  void encodeit(struct affentry * ptr, char * cs);
  int build_pfxtree(AffEntry* pfxptr);
  int build_sfxtree(AffEntry* sfxptr);
  int process_pfx_order();
  int process_sfx_order();
  AffEntry * process_pfx_in_order(AffEntry * ptr, AffEntry * nptr);
  AffEntry * process_sfx_in_order(AffEntry * ptr, AffEntry * nptr);
  int process_pfx_tree_to_list();
  int process_sfx_tree_to_list();
  void set_spec_utf8_encoding();
  int redundant_condition(char, char * strip, int stripl, const char * cond, char *);
};

#endif

#ifndef _SUGGESTMGR_HXX_
#define _SUGGESTMGR_HXX_

#define MAXSWL 100
#define MAXSWUTF8L (MAXSWL * 4)
#define MAX_ROOTS 50
#define MAX_WORDS 200
#define MAX_GUESS 200
#define MAXNGRAMSUGS 3

#define MINTIMER 500
#define MAXPLUSTIMER 500

#define NGRAM_IGNORE_LENGTH 0
#define NGRAM_LONGER_WORSE  1
#define NGRAM_ANY_MISMATCH  2

enum { LCS_UP, LCS_LEFT, LCS_UPLEFT };

class SuggestMgr
{
  char *          ctry;
  int             ctryl;
  w_char *        ctry_utf;

  AffixMgr*       pAMgr;
  int             maxSug;
  struct cs_info * csconv;
  struct unicode_info2 * utfconv;
  int             utf8;
  int             nosplitsugs;
  int             maxngramsugs;
  int             complexprefixes;


public:
  SuggestMgr(const char * tryme, int maxn, AffixMgr *aptr);
  ~SuggestMgr();

  int suggest(char*** slst, const char * word, int nsug);
  int ngsuggest(char ** wlst, char * word, HashMgr* pHMgr);
  int suggest_auto(char*** slst, const char * word, int nsug);
  int suggest_stems(char*** slst, const char * word, int nsug);
  int suggest_pos_stems(char*** slst, const char * word, int nsug);

  char * suggest_morph(const char * word);
  char * suggest_morph_for_spelling_error(const char * word);

private:
   int check(const char *, int, int, int *, time_t *);
   int check_forbidden(const char *, int);

   int replchars(char**, const char *, int, int);
   int doubledsyllable(char**, const char *, int, int);
   int forgotchar(char **, const char *, int, int);
   int swapchar(char **, const char *, int, int);
   int extrachar(char **, const char *, int, int);
   int badchar(char **, const char *, int, int);
   int twowords(char **, const char *, int, int);
   int fixstems(char **, const char *, int);

   int forgotchar_utf(char**, const w_char *, int wl, int, int);
   int extrachar_utf(char**, const w_char *, int wl, int, int);
   int badchar_utf(char **, const w_char *, int wl, int, int);
   int swapchar_utf(char **, const w_char *, int wl, int, int);

   int mapchars(char**, const char *, int, int);
   int map_related(const char *, int, char ** wlst, int, const mapentry*, int, int *, time_t *);
   int map_related_utf(w_char *, int, int, char ** wlst, int, const mapentry*, int, int *, time_t *);
   int ngram(int n, char * s1, const char * s2, int uselen);
   int mystrlen(const char * word);
   int equalfirstletter(char * s1, const char * s2);
   int commoncharacterpositions(char * s1, const char * s2, int * is_swap);
   void bubblesort( char ** rwd, int * rsc, int n);
   void lcs(const char * s, const char * s2, int * l1, int * l2, char ** result);
   int lcslen(const char * s, const char* s2);

};

#endif

#ifndef __CSUTILHXX__
#define __CSUTILHXX__

// First some base level utility routines


// convert UTF-16 characters to UTF-8
char * u16_u8(char * dest, int size, const w_char * src, int srclen);

// convert UTF-8 characters to UTF-16
int u8_u16(w_char * dest, int size, const char * src);

// sort 2-byte vector
void flag_qsort(unsigned short flags[], short begin, short end);

// binary search in 2-byte vector
int flag_bsearch(unsigned short flags[], unsigned short flag, short right);

// remove end of line char(s)
void   mychomp(char * s);

// duplicate string
char * mystrdup(const char * s);

// duplicate reverse of string
char * myrevstrdup(const char * s);

// parse into tokens with char delimiter
char * mystrsep(char ** sptr, const char delim);

// parse into tokens with char delimiter
char * mystrrep(char *, const char *, const char *);

// is one string a leading subset of another
int    isSubset(const char * s1, const char * s2);

// is one reverse string a leading subset of the end of another   
int    isRevSubset(const char * s1, const char * s2, int len);

// append s to ends of every lines in text
void strlinecat(char * lines, const char * s);

// tokenize into lines with new line
   int line_tok(const char * text, char *** lines);

// tokenize into lines with new line and uniq in place
   char * line_uniq(char * text);

// change \n to c in place
   char * line_join(char * text, char c);

// leave only last {[^}]*} pattern in string
   char * delete_zeros(char * morphout);

// reverse word
   void reverseword(char *);

// reverse word
   void reverseword_utf(char *);

// character encoding information
struct cs_info {
  unsigned char ccase;
  unsigned char clower;
  unsigned char cupper;
};

// Unicode character encoding information
struct unicode_info {
  unsigned short ccase;
  unsigned short cupper;
  unsigned short clower;
};

struct unicode_info2 {
  char ccase;
  unsigned short cupper;
  unsigned short clower;
};

struct enc_entry {
  const char * enc_name;
  struct cs_info * cs_table;
};

// language to encoding default map

struct lang_map {
  const char * lang;
  const char * def_enc;
  int num;
};

struct cs_info * get_current_cs(const char * es);

struct unicode_info * get_utf_cs();

int get_utf_cs_len();

const char * get_default_enc(const char * lang);

int get_lang_num(const char * lang);

// convert null terminated string to all caps using encoding
void enmkallcap(char * d, const char * p, const char * encoding);

// convert null terminated string to all little using encoding
void enmkallsmall(char * d, const char * p, const char * encoding);

// convert null terminated string to have intial capital using encoding
void enmkinitcap(char * d, const char * p, const char * encoding);

// convert null terminated string to all caps
void mkallcap(char * p, const struct cs_info * csconv);

// convert null terminated string to all little
void mkallsmall(char * p, const struct cs_info * csconv);

// convert null terminated string to have intial capital
void mkinitcap(char * p, const struct cs_info * csconv);

// convert first nc characters of UTF-8 string to little
void mkallsmall_utf(w_char * u, int nc, struct unicode_info2 * utfconv);

#endif

#ifndef _LANGNUM_HXX_
#define _LANGNUM_HXX_

/*
 language numbers for language specific codes
 see http://l10n.openoffice.org/languages.html
*/

enum {
LANG_az=100, // custom number
LANG_bg=41,
LANG_ca=37,
LANG_cs=42,
LANG_da=45,
LANG_de=49,
LANG_el=30,
LANG_en=01,
LANG_es=34,
LANG_eu=10,
LANG_fr=02,
LANG_gl=38,
LANG_hr=78,
LANG_hu=36,
LANG_it=39,
LANG_la=99, // custom number
LANG_lv=101, // custom number
LANG_nl=31,
LANG_pl=48,
LANG_pt=03,
LANG_ru=07,
LANG_sv=50,
LANG_tr=90,
LANG_uk=80,
LANG_xx=999
};

#endif

#define NOCAP   0
#define INITCAP 1
#define ALLCAP  2
#define HUHCAP  3

#define MAXSUGGESTION 15
#define MAXSHARPS 5

/*#ifdef W32
#define DLLTEST2_API __declspec(dllexport)
#endif*/

#ifndef _MYSPELLMGR_HXX_
#define _MYSPELLMGR_HXX_

#ifdef W32
class DLLEXPORT Hunspell
#else
class Hunspell
#endif
{
  AffixMgr*       pAMgr;
  HashMgr*        pHMgr;
  SuggestMgr*     pSMgr;
  char *          encoding;
  struct cs_info * csconv;
  int             langnum;
  int             utf8;
  int             complexprefixes;
  char**          wordbreak;

public:

  /* Hunspell(aff, dic) - constructor of Hunspell class
   * input: path of affix file and dictionary file
   */
  
  Hunspell(const char * affpath, const char * dpath);

  ~Hunspell();

  /* spell(word) - spellcheck word
   * output: 0 = bad word, not 0 = good word
   *   
   * plus output:
   *   info: information bit array, fields:
   *     SPELL_COMPOUND  = a compound word 
   *     SPELL_FORBIDDEN = an explicit forbidden word
   *   root: root (stem), when input is a word with affix(es)
   */
   
  int spell(const char * word, int * info = NULL, char ** root = NULL);

  /* suggest(suggestions, word) - search suggestions
   * input: pointer to an array of strings pointer and the (bad) word
   *   array of strings pointer (here *slst) may not be initialized
   * output: number of suggestions in string array, and suggestions in
   *   a newly allocated array of strings (*slts will be NULL when number
   *   of suggestion equals 0.)
   */

  int suggest(char*** slst, const char * word);

  char * get_dic_encoding();

 /* morphological functions */

 /* analyze(result, word) - morphological analysis of the word */
 
  int analyze(char*** slst, const char * word);

 /* stem(result, word) - stemmer function */
  
  int stem(char*** slst, const char * word);
  
 /* stem(result, analysis, n) - get stems from a morph. analysis
  * example:
  * char ** result, result2;
  * int n1 = analyze(result, "words");
  * int n2 = stem(result2, result, n1);   
  */
 
  int stem(char*** slst, char ** morph, int n);

 /* generate(result, word, word2) - morphological generation by example(s) */

  int generate(char*** slst, const char * word, const char * word2);

 /* generate(result, word, desc, n) - generation by morph. description(s)
  * example:
  * char ** result;
  * char * affix = "is:plural"; // description depends from dictionaries, too
  * int n = generate(result, "word", &affix, 1);
  * for (int i = 0; i < n; i++) printf("%s\n", result[i]);
  */

  int generate(char*** slst, const char * word, char ** desc, int n);

  /* functions for run-time modification of the dictionary */

  /* add word to the run-time dictionary */
  
  int add(const char * word);

  /* add word to the run-time dictionary with affix flags of
   * the example (a dictionary word): Hunspell will recognize
   * affixed forms of the new word, too.
   */
  
  int add_with_affix(const char * word, const char * example);

  /* remove word from the run-time dictionary */
  /* NOTE: not implemented yet */

  int remove(const char * word);

  /* other */

  /* get extra word characters definied in affix file for tokenization */
  const char * get_wordchars();
  unsigned short * get_wordchars_utf16(int * len);

  struct cs_info * get_csconv();
  const char * get_version();
  
  /* experimental and deprecated functions */

#ifdef HUNSPELL_EXPERIMENTAL
  /* suffix is an affix flag string, similarly in dictionary files */  
  int put_word_suffix(const char * word, const char * suffix);
  char * morph_with_correction(const char * word);

  /* spec. suggestions */
  int suggest_auto(char*** slst, const char * word);
  int suggest_pos_stems(char*** slst, const char * word);
  char * get_possible_root();
#endif

private:
   int    cleanword(char *, const char *, int * pcaptype, int * pabbrev);
   int    cleanword2(char *, const char *, w_char *, int * w_len, int * pcaptype, int * pabbrev);
   void   mkinitcap(char *);
   int    mkinitcap2(char * p, w_char * u, int nc);
   int    mkinitsmall2(char * p, w_char * u, int nc);
   void   mkallcap(char *);
   int    mkallcap2(char * p, w_char * u, int nc);
   void   mkallsmall(char *);
   int    mkallsmall2(char * p, w_char * u, int nc);
   struct hentry * checkword(const char *, int * info, char **root);
   char * sharps_u8_l1(char * dest, char * source);
   hentry * spellsharps(char * base, char *, int, int, char * tmp, int * info, char **root);
   int    is_keepcase(const hentry * rv);
   int    insert_sug(char ***slst, char * word, int ns);
   char * cat_result(char * result, char * st);
   char * stem_description(const char * desc);

};

#endif
