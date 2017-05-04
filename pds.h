/********************************************************************
* %W%
* Copyrights (c) Nimbus Software AS.
********************************************************************/

#include <stdlib.h>
#include <time.h>
#include<stdarg.h>
#include<stdio.h>

#ifndef __pds_h
#define __pds_h

#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif

#define	pdsError(pds)		pds->lasterror
#define	pdsData(pds)		pds->data
#define	pdsDataSize(pds)	pds->size

#define pdsStrLen(v)		strlen(v)
#define	pdsStr(v)			v,pdsStrLen(v)
#define	pdsSize(v)			v,sizeof(v)
#define	pdsArray(v)			v,(sizeof(v)/sizeof(v[0]))
#define pdsErrReturn(p,e)	{if (p) p->lasterror=e;return(1);}
#define	pdsAssert(t)		if (!(t)) \
  printf("pdsAssert: %s.%d, error: %s\n", __FILE__,__LINE__,sys_errlist[errno]);

enum PDS_TYPE {
	PDS_I = 1,
	PDS_PI,
	PDS_PPI,
	PDS_RGI,
	PDS_RGPI,
	PDS_CH,
	PDS_PCH,
	PDS_PPCH,
	PDS_RGCH,
	PDS_RGPCH,
	PDS_DUM_L,
	PDS_DUM_PL,
	PDS_DUM_PPL,
	PDS_DUM_RGL,
	PDS_DUM_RGPL,
	PDS_F,
	PDS_PF,
	PDS_PPF,
	PDS_RGF,
	PDS_RGPF,
	PDS_PDS,
	PDS_VOID,
	PDS_SEP,
	PDS_PPDS,
	PDS_CPCH,
	PDS_CPDS,
	PDS_INT64
};

enum PDS_ERR_CODE {
	PDS_ERR_NONE,
	PDS_ERR,
	PDS_ERR_ILLCONV,
	PDS_ERR_ILLTYPE,
	PDS_ERR_ILLARG,
	PDS_ERR_NOMATCH,
	PDS_ERR_BOUNDS,
	PDS_ERR_MALLOC,
	PDS_ERR_WRITE,
	PDS_ERR_READ
};


#define PDStype				enum PDS_TYPE
#define PDSerr				enum PDS_ERR_CODE
#define PDS_INT				PDS_I
#define PDS_SZ				PDS_PCH
#define PDS_FLOAT			PDS_F
#define	PDS_TMP_BUFFSIZE	100

/* STRUCTURE FOR PDS-KEYINDEX */
typedef struct PDSI {
	char    	*key;
	struct PDSI *next;
}PDSI;

/* STRUCTURE FOR PDS-CACHE */
typedef struct PDSC {
	char    *key;
	char    *ofs;
	int		pos;
}PDSC;

/* STRUCTURE FOR PDS-HANDLE */
typedef struct PDS {
	size_t	total;
	size_t	size;
	size_t	osize;			/* original size - pdsCreateSize or default: 1024 */
	char	*get,*put;
	char	*data;
	PDSI	*keys;
	PDSerr	lasterror;
	int	flags;
	PDSC	putcache;
	PDSC	getcache;
}PDS;

typedef struct PDSINT64 {
	union {
		struct {
			unsigned long Hi32;
			unsigned long Lo32;
		}i64;
#ifdef NT
		UINT64 value;
#endif
	}as;
} PDSINT64;


#ifdef WIN32
#define mylseek    _lseeki64
#define mytell     _telli64
#else
#define mylseek    lseek
#define myseekpos  off_t
#define mytell     tell
#endif

typedef struct _pds_header {
	long read_pos;
	unsigned long records;
} PDSHEADER;

typedef struct _pds_record {
	unsigned long flags;
	unsigned long  size;
} PDSRECORD;

typedef struct _pds_file {
	int       fd;
	char      *fname;
	PDSHEADER hdr;
} PDSFILE;


#ifdef NIMEXTERNC
extern "C" {
#endif
	extern PDS*		pdsCopy	  	(PDS *src, PDS **dest);
	extern int		pdsDelete 	(PDS *pds);
	extern int		pdsCount  	(PDS *pds);
	extern PDS*		pdsCreate 	(void);
	extern PDS*		pdsCreateSize   (size_t size);
	extern PDS*		pdsMap	  	(PDS *pds,char *szDta, size_t iSize);
	extern PDS*		pdsSet	  	(char *szDta, size_t iSize);
	extern PDS      *pdsSetBuffer(char *pchData,size_t iSize, int bFreeBuffer);
	extern int		pdsRemove   (PDS *pds, char *matchKey);
	extern int		pdsRewind   (PDS *pds);
	extern int		pdsReset    (PDS *pds);
	extern int		pdsTrim	    (PDS *pds);
	extern int		pdsScanf 	(PDS *pds,char *fmt,...);
	extern int		pdsPrintf 	(PDS *pds,char *fmt,...);
	extern int		pdsVaScanf 	(PDS *pds,char *fmt,va_list ap);
	extern int		pdsVaPrintf	(PDS *pds,char *fmt,va_list ap);
	extern void		pdsFreeTable(PDStype t, void **val);
	extern int 		pdsGetTable (PDS *pds,PDStype t,char *key,int i,void *val);
	extern int 		pdsPutTable (PDS *pds,PDStype t,char *key,void *val);
	extern int		pdsPut	  	(PDS *pds,PDStype t,char *key,void *d,size_t l);
	extern int		pdsGet	  	(PDS *pds,PDStype t,char *key,void *d);
	extern void*	pdsGetData 	(PDS *pds,char *key,PDStype t);
	extern int		pdsGetNext  (PDS *pds,char **pKey,PDStype *pT,size_t *pS,void **pD);
	extern int		pdsDump		(PDS *pds);
	extern char*	pdsExpand   (PDS *pds,char *format,...);	/* optional par: char *missing_value */
	extern int		pdsCfgRead  (char *fname,PDS **out);

	extern PDSFILE *pdsFileOpen(const char *pchFile);
	extern void     pdsFileClose(PDSFILE *pdsfp);
	extern void     pdsFileReset(PDSFILE *pdsfp);
	extern int      pdsFileWrite(PDSFILE *pdsfp, PDS *pds);
	extern int      pdsFileRead(PDSFILE *pdsfp, PDS **pds, unsigned int *len, int bMarkAsRead);
	extern int      pdsFileMarkRead(PDSFILE *pdsfp);
	extern void     pdsFileDump(PDSFILE *pdsfp);


	extern int 		pdsGet_PPDS (PDS *pds, char *key, PDS ***pppPds);
	extern int		pdsGet_PDS	(PDS *pds, char *key, PDS **ppds);
	extern int      pdsGet_CPDS (PDS *pds, char *key, PDS **ppPds);
	extern int		pdsGet_VOID	(PDS *pds, char *key, void **ppd, size_t *lsz);
	extern int		pdsGet_RGCH	(PDS *pds, char *key, char *pch,size_t size);
	extern int		pdsGet_CPCH (PDS *pds, char *key, char **ppch);
	extern int		pdsGet_PCH	(PDS *pds, char *key, char **ppch);
	extern int		pdsGet_PPCH	(PDS *pds, char *key, char ***pppch);
	extern int		pdsGet_INT	(PDS *pds, char *key, int *pi);
	extern int		pdsGet_INT64(PDS *pds, char *key, PDSINT64 *pi64);
	extern int		pdsGet_PI	(PDS *pds, char *key, int **ppi);
	extern int		pdsGet_LONG	(PDS *pds, char *key, long *pl);
	extern int		pdsGet_PL	(PDS *pds, char *key, long **ppl);
	extern int		pdsGet_F	(PDS *pds, char *key, double *pd);
	extern int		pdsGet_TIME	(PDS *pds, char *key, time_t *pt);
	extern int		pdsGet_SIZE	(PDS *pds, char *key, size_t *ps);

	extern int 		pdsPut_PDS 	(PDS *pds, char *key, PDS *dpds);
	extern int 		pdsPut_PPDS (PDS *pds, char *key, PDS *dpds);
	extern int 		pdsPut_VOID	(PDS *pds, char *key, void *d, size_t size);
	extern int 		pdsPut_RGCH	(PDS *pds, char *key, char *pch, size_t size);
	extern int 		pdsPut_PCH 	(PDS *pds, char *key, char *pch);
	extern int 		pdsPut_PPCH	(PDS *pds, char *key, char **ppch);
	extern int 		pdsPut_INT 	(PDS *pds, char *key, int i);
	extern int 		pdsPut_INT64(PDS *pds, char *key, PDSINT64 i64);
	extern int 		pdsPut_PPI 	(PDS *pds, char *key, int *pi);
	extern int 		pdsPut_LONG	(PDS *pds, char *key, long l);
	extern int 		pdsPut_PPL 	(PDS *pds, char *key, long *ppl);
	extern int 		pdsPut_F   	(PDS *pds, char *key, double d);
	extern int 		pdsPut_TIME   	(PDS *pds, char *key, time_t t);
	extern int 		pdsPut_SIZE   	(PDS *pds, char *key, size_t s);

#ifdef NIMEXTERNC
};
#endif

#endif
