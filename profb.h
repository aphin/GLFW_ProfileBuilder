
// shortcuts for common colors
#define glBlack glColor3f(0.0,0.0,0.0)
#define glWhite glColor3f(1.0,1.0,1.0)
#define glGray(i) glColor3f(1.0*i,1.0*i,1.0*i)
#define glRed(i) glColor3f(1.0*i,0.0,0.0)
#define glGreen(i) glColor3f(0.0,1.0*i,0.0)
#define glBlue(i) glColor3f(0.0,0.0,1.0*i)
#define glYellow(i) glColor3f(1.0*i,1.0*i,0.0)
#define glMagenta(i) glColor3f(1.0*i,0.0,1.0*i)
#define glCyan(i) glColor3f(0.0,1.0*i,1.0*i)

int screen_width;
int screen_height;

float arat;	//aspect ratio

unsigned char Redisplay=1;

const float hscale = 0.15F;
const float vscale = 0.06F;

float mx;
float my;

int mix;
int miy;

int keycode;

struct ChrFont
{
    unsigned short hSize;
    unsigned short fSize;
    unsigned short nChar;
    unsigned char fChar;
    signed char uMargin;
    signed char lMargin;
    unsigned short mHeight;
    unsigned short cOff[256];
    unsigned char cWidth[256];
    unsigned char mWidth;
    char *fData;
    unsigned int dSize;
};

struct ChrFont Font;

// coordinates of screen division (window borders)
float wd_x1=0.0;	// center of screen
float wd_y1=-0.5;	// bottom of screen (for time window)
// ---

// --- common routines declaration ---

void load_chr_font(char *fname) // Borland .chr font loader (simple and brutal)
{
    FILE *f;

    f = fopen(fname,"rb");

    unsigned char buf[20];
    fread(buf,1,4,f);   // PK#8#8
    while (buf[0] != 0x1A) { fread(&buf[0],1,1,f); } // Copyrights

    fread(&Font.hSize,2,1,f);    // header size
    fread(buf,1,4,f);   // font name
    fread(&Font.fSize,2,1,f);    // font size
    fseek(f,Font.hSize+1,SEEK_SET);

    fread(&Font.nChar,2,1,f);
    fread(&buf[0],1,1,f);
    fread(&Font.fChar,1,1,f);

    unsigned short fofs;
    fread(&fofs,2,1,f);
    fread(&buf[0],1,1,f);

    fread(&Font.uMargin,1,1,f);
    fread(buf,1,1,f);
    fread(&Font.lMargin,1,1,f);
    fread(buf,1,5,f);

    unsigned int i;
    for (i=0; i<Font.nChar; i++)
    {
        fread(&Font.cOff[i],2,1,f);
    }
    for (i=0; i<Font.nChar; i++)
    {
        fread(&Font.cWidth[i],1,1,f);
    }

    long int c1;
    c1 = ftell(f);
    fseek(f,0L,SEEK_END);
    long int c2;
    c2 = ftell(f);
    fseek(f,c1,SEEK_SET);

    Font.fData=malloc(c2-c1);
    Font.dSize = c2-c1;
    fread(Font.fData,1,Font.dSize,f);
    fclose(f);

    Font.mWidth = Font.cWidth[0];
    Font.mHeight = Font.uMargin-Font.lMargin;
    for (i=0; i<Font.nChar; i++)
    {
        if (Font.mWidth < Font.cWidth[i]) { Font.mWidth = Font.cWidth[i]; }
    }
}

void DrawChar(unsigned char code, struct ChrFont fnt, float x, float y, float sx, float sy)
{
    float px;
    float py;
    float ix;
    float iy;
    unsigned char dend=0;

    unsigned char b1;
    unsigned char b2;

    signed char k1;
    signed char k2;

    unsigned short Off=fnt.cOff[code];

    while (dend==0)
    {
        b1 = fnt.fData[Off];
        b2 = fnt.fData[Off+1];
        if (((b1 & 0x80) == 0)&&((b2 & 0x80) == 0))
        {
            dend = 1;
            break;
        }
        if (((b1 & 0x80) == 0x80)&&((b2 & 0x80) == 0))
        {
            k1 = b1 & 0x7F; k2 = b2 & 0x7F;
            if ((b1 & 0x40) == 0x40) { k1 = b1 | 0x80; }
            if ((b2 & 0x40) == 0x40) { k2 = b2 | 0x80; }

            px = ((float)k1 / fnt.mWidth)*sx;
            py = ((float)k2 / fnt.mHeight)*sy;
        }
        if (((b1 & 0x80) == 0x80)&&((b2 & 0x80) == 0x80))
        {
            k1 = b1 & 0x7F; k2 = b2 & 0x7F;
            if ((b1 & 0x40) == 0x40) { k1 = k1 | 0x80; }
            if ((b2 & 0x40) == 0x40) { k2 = k2 | 0x80; }

            ix = ((float)k1 / fnt.mWidth)*sx;
            iy = ((float)k2 / fnt.mHeight)*sy;
            glBegin(GL_LINES);
            glVertex2f(x+px,y+py*arat);
            glVertex2f(x+ix,y+iy*arat);
            glEnd();
            px = ix; py = iy;
        }
        Off += 2;
    }
}

void DrawText(char *tx, struct ChrFont fnt, float x, float y, float sx, float sy)
{
    float dx;
    float dy;
    unsigned char t;
    int i=0;

    dx = x; dy = y;

    while (i < strlen(tx))
    {
        if ((unsigned char)tx[i] >= fnt.fChar)
        {
            t = (unsigned char)tx[i] - fnt.fChar;
            DrawChar(t,fnt,dx,dy,sx,sy);
            dx += ((float)fnt.cWidth[t] / (float)fnt.mWidth) * sx;
        }
        i++;
    }
}
