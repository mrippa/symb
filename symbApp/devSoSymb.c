/* base/src/dev $Id$ */

/* @(#)devSoSymb.c	1.1	6/4/93
 *      Device Support for VxWorks Global Symbol String Output Records
 *
 *
 *      Author:         Carl Lionberger
 *      Date:           060893
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 1991, the Regents of the University of California,
 *      and the University of Chicago Board of Governors.
 *
 *      This software was produced under  U.S. Government contracts:
 *      (W-7405-ENG-36) at the Los Alamos National Laboratory,
 *      and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *      Initial development by:
 *              The Controls and Automation Group (AT-8)
 *              Ground Test Accelerator
 *              Accelerator Technology Division
 *              Los Alamos National Laboratory
 *
 *      Co-developed with
 *              The Controls and Computing Group
 *              Accelerator Systems Division
 *              Advanced Photon Source
 *              Argonne National Laboratory
 *
 *              The Control Systems Group
 *              Systems Engineering Department
 *              Lawrence Berkeley Laboratory
 *
 *      NOTES:
 * Derived from soft record device support.
 * Modification Log:
 * -----------------
 * wfl  06-Jun-96       call devSymbFind() to parse PARM field and
 *                      look up symbol
 * anj  14-Oct-96	Changed devSymbFind() parameters.
 */


#include	<vxWorks.h>
#include	<sysSymTbl.h>
#include	<types.h>
#include	<stdioLib.h>
#include	<string.h>
#include	<intLib.h>

#include	<alarm.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include	<recSup.h>
#include	<recGbl.h>
#include	<devSup.h>
#include	<module_types.h>
#include	<stringoutRecord.h>
#include	<devSymb.h>

static long init_record();
static long write_stringout();

/* Create the dset for devSoSymb */

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_stringout;
}devSoSymb={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	write_stringout};
 

static long init_record(pstringout)
    struct stringoutRecord	*pstringout;
{
    /* determine address of record value */
    if (devSymbFind(pstringout->name, &pstringout->out, &pstringout->dpvt))
    {
        recGblRecordError(S_db_badField,(void *)pstringout,
            "devSoSymb (init_record) Illegal NAME or OUT field");
        return(S_db_badField);
    }

    return(0);
}

static long write_stringout(pstringout)
    struct stringoutRecord	*pstringout;
{
	int lockKey;
    struct vxSym *private = (struct vxSym *) pstringout->dpvt;

    if (private)
    {
        pstringout->val[39] = '\0';
	    lockKey = intLock();
        strcpy((char *)(*private->ppvar) + private->index, pstringout->val);
        intUnlock(lockKey);
    }
    else 
        return(1);

    pstringout->udf = FALSE;

    return(0);
}
