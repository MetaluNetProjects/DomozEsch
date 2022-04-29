
void rfSetup();
void rfService();

void bnoGetQuat(void* dest); // 4 ints = 8 chars
void bnoGetAcc(void* dest); // 3 ints = 6 chars

typedef struct _outbuf_ {
  unsigned char id;
  unsigned char vbatt;
  unsigned char outData[15];
} t_outbuf;

extern t_outbuf outBuf;
struct _myData_;
typedef struct s_myData t_myData;
#define myData ((t_myData *)&(outBuf.outData))
