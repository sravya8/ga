#ifndef  _GLOBALP_H_
#define _GLOBALP_H_

#include "config.h"
#include "global.h"

#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif
#ifdef CRAY
#include <fortran.h>
#endif
#ifdef CRAY_YMP
#define FALSE _btol(0)
#define TRUE  _btol(1)
#else
#define FALSE (logical) 0
#define TRUE  (logical) 1
#endif

#if defined(WIN32)
#   include "winutil.h"
#endif
#include "macdecls.h"

#define GA_OFFSET   1000           /* offset for handle numbering */

#ifndef MAX_NPROC                  /* default max number of processors  */
#   ifdef PARAGON
#     define MAX_NPROC    1024
#   elif defined(DELTA)
#     define MAX_NPROC     512
#   elif defined(SP1) || defined(SP)
#     define MAX_NPROC     400
#   elif defined(LAPI)
#     define MAX_NPROC     512
#   elif defined(CRAY_T3D)
#     define MAX_NPROC     256
#   elif defined(KSR)
#     define MAX_NPROC      80
#   else
#     define MAX_NPROC     128     /* default for everything else */
#   endif
#endif


/* types/tags of messages used internally by GA */
#define     GA_TYPE_SYN   GA_MSG_OFFSET + 1
#define     GA_TYPE_GSM   GA_MSG_OFFSET + 5
#define     GA_TYPE_GOP   GA_MSG_OFFSET + 15
#define     GA_TYPE_BRD   GA_MSG_OFFSET + 16

/* GA operation ids */
#define     GA_OP_GET 1          /* Get                         */
#define     GA_OP_END 2          /* Terminate                   */
#define     GA_OP_CRE 3          /* Create                      */
#define     GA_OP_PUT 4          /* Put                         */
#define     GA_OP_ACC 5          /* Accumulate                  */
#define     GA_OP_DES 6          /* Destroy                     */
#define     GA_OP_DUP 7          /* Duplicate                   */
#define     GA_OP_ZER 8          /* Zero                        */
#define     GA_OP_DDT 9          /* dot product                 */
#define     GA_OP_SCT 10         /* scatter                     */
#define     GA_OP_GAT 11         /* gather                      */
#define     GA_OP_RDI 15         /* Integer read and increment  */
#define     GA_OP_ACK 16         /* acknowledgment              */
#define     GA_OP_LCK 17         /* acquire lock                */
#define     GA_OP_UNL 18         /* release lock                */


#ifdef GA_TRACE
  static Integer     op_code;
#endif


#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define ABS(a)   (((a) >= 0) ? (a) : (-(a)))

#define GAsizeofM(type)  ( (type)==MT_F_DBL? sizeof(DoublePrecision): \
                           (type)==MT_F_INT? sizeof(Integer): \
                           (type)==MT_F_DCPL? sizeof(DoubleComplex): \
                           (type)==MT_F_REAL? sizeof(float):0)

#define NAME_STACK_LEN 10
#define PAGE_SIZE  4096

struct ga_stat_t {
         long   numcre; 
         long   numdes;
         long   numget;
         long   numput;
         long   numacc;
         long   numsca;
         long   numgat;
         long   numrdi;
         long   numser;
         long   curmem; 
         long   maxmem; 
};

struct ga_bytes_t{ 
         double acctot;
         double accloc;
         double gettot;
         double getloc;
         double puttot;
         double putloc;
         double rditot;
         double rdiloc;
         double gattot;
         double gatloc;
         double scatot;
         double scaloc;
};

#define STAT_AR_SZ sizeof(ga_stat_t)/sizeof(long)

extern long *GAstat_arr;  
extern struct ga_stat_t GAstat;
extern struct ga_bytes_t GAbytes;
extern char *GA_name_stack[NAME_STACK_LEN];    /* stack for names of GA ops */ 
extern int GA_stack_size;

#define  GA_PUSH_NAME(name) (GA_name_stack[GA_stack_size++] = (name)) 
#define  GA_POP_NAME        (GA_stack_size--)


extern void f2cstring(char*, Integer, char*, Integer);
extern void c2fstring( char*, char*, Integer);
extern void ga_clean_resources( void);

/* periodic operations */
#define PERIODIC_GET 1
#define PERIODIC_PUT 2
#define PERIODIC_ACC 3

extern void ngai_periodic_(Integer *g_a, Integer *lo, Integer *hi, void *buf,
                           Integer *ld, void *alpha, Integer op_code);

#define FNAM        31              /* length of array names   */
typedef struct {
       int  ndim;               /* number of dimensions                 */
       int  dims[MAXDIM];       /* global array dimensions              */
       int  chunk[MAXDIM];      /* chunking                             */
       int  nblock[MAXDIM];     /* number of blocks per dimension       */
       int  width[MAXDIM];      /* boundary cells per dimension         */
       double scale[MAXDIM];    /* nblock/dim (precomputed)             */
       char **ptr;              /* arrays of pointers to remote data    */
       int  *mapc;              /* block distribution map               */
       int type;                /* type of array                        */
       int  actv;               /* activity status                      */
       Integer lo[MAXDIM];      /* top/left corner in local patch       */
       Integer size;            /* size of local data in bytes          */
       int elemsize;            /* sizeof(datatype)                     */
       int ghosts;              /* flag indicating presence of ghosts   */
       long lock;               /* lock                                 */
       long id;                 /* ID of shmem region / MA handle       */
       char name[FNAM+1];       /* array name                           */
} global_array_t;


#define FLUSH_CACHE
#ifdef  CRAY_T3D
#       define ALLIGN_SIZE      32
#else
#       define ALLIGN_SIZE      128
#endif

#define allign__(n, SIZE) (((n)%SIZE) ? (n)+SIZE - (n)%SIZE: (n))
#define allign_size(n) allign__((long)(n), ALLIGN_SIZE)
#define allign_page(n) allign__((long)(n), PAGE_SIZE)

extern void gai_print_subscript(char *pre,int ndim, Integer subscript[], char* post);

#endif
