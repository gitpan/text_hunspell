#include "hunspell.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#ifdef __cplusplus
}
#endif


using namespace std;
/*using namespace Hunspell;*/

/* $Id: Hunspell.xs,v 1.5 2002/08/29 20:28:00 moseley Exp $ */

MODULE = Text::Hunspell        PACKAGE = Text::Hunspell

PROTOTYPES: ENABLE

# Make sure that we have at least xsubpp version 1.922.
REQUIRE: 1.922

Hunspell *
Hunspell::new(aff,dic )
    char *aff;
    char *dic;
    CODE:
        RETVAL = new Hunspell(aff, dic);

    OUTPUT:
        RETVAL


void
Hunspell::delete(pMS)
    Hunspell *pMS;
    CODE:
       delete pMS;

int
Hunspell::check(buf)
    char *buf;
    CODE:
        RETVAL = THIS->spell(buf);

    OUTPUT:
        RETVAL

void 
Hunspell::suggest( buf)
    char *buf;
    PREINIT:
        char **wlsti;
	int i, val;
    PPCODE:
        val = THIS->suggest(&wlsti, buf);
	for (int i = 0; i < val; i++) {
            PUSHs(sv_2mortal(newSVpv( wlsti[i] ,0 )));
	    free(wlsti[i]);
	}


