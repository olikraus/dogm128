/*

  bdf2dogm.c

  (c) 2010 Oliver Kraus (olikraus@gmail.com)

  Utility program to convert fonts from the BDF file format to the internal DOGM format

  

  This file is part of the dogm128 library.

  The dogm128 library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.



  Notes:
  - Fonts are avilable from X11 
  - my personal favorits from X11: 4x6, 5x7, 6x12, 9x18
  - there are some limitations (e.g versus font size) and bdf2dogm will abort if these limitations are broken
  - there a some simplifications with overlapping chars (e.g. 'f' from helvR08) DWIDTH is increased to BBX width + BBX x

  ChangeLog
  
  2010-09-25 Oliver Kraus
    * changes due to the new prog-mem abstraction layer in the dogm128 library
    
*/
#define DEBUG								

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define fnt_set_bbox_A(buf,x) (((buf)[0]=((x)&255)),((buf)[1]=((x)>>8)))
#define fnt_set_bbox_a(buf,x) (((buf)[2]=((x)&255)),((buf)[3]=((x)>>8)))
#define fnt_set_bbox_width(buf,x) ((buf)[4]=(x))
#define fnt_set_bbox_height(buf,x) ((buf)[5]=(x))
#define fnt_set_bbox_descent(buf,x) ((buf)[6]=(x))

#define fnt_get_bbox_width(buf) ((buf)[4])
#define fnt_get_bbox_height(buf) ((buf)[5])
#define fnt_get_bbox_descent(buf) ((buf)[6])
#define FNT_DATA 7


#define chr_set_descent_len_hw(buf,d,l,h,w) (((buf)[1] = ((d)<<5) | (w)),((buf)[0] = (l)),((buf)[2] = (h)))

#define chr_get_len(buf) ((buf)[0])
#define CHR_MAX_LEN 255
#define chr_get_descent(buf) ((buf)[1]  >>5)
#define chr_get_width(buf) ((buf)[1] & 0x1f)
#define CHR_MAX_WIDTH 31
#define chr_get_height(buf) ((buf)[2])
#define CHR_MAX_HEIGHT 255
#define CHR_DATA 3

unsigned char font_buf[1024*24];
unsigned short font_buf_len = FNT_DATA;
int font_max_bbox_width;
int font_max_bbox_height;
int font_max_bbox_descent;

int font_ascii;

unsigned short chr_get_skip_delta(unsigned char *cbuf, unsigned char skip_cnt)
{
  unsigned short pos = 0;
  unsigned char len;
  while( skip_cnt > 0 )
  {
    len = chr_get_len(cbuf+pos);
    if ( len == 0 )				
      return pos;			/* should never happen */
    pos += len;
    skip_cnt--;
  }
  return pos;
}

unsigned char *fnt_get_chr(unsigned char *fbuf, unsigned char code)
{
  return fbuf + FNT_DATA + chr_get_skip_delta(fbuf+FNT_DATA, code - ' ');
}

unsigned char chr_get_pixel(unsigned char *buf, unsigned char x, unsigned char y )
{
  unsigned char bytes_per_line, index;
  /* bytes_per_line = ((unsigned char)chr_get_width(buf)+(unsigned char)7) >> 3; */
  bytes_per_line = chr_get_width(buf);
  bytes_per_line += (unsigned char)7;
  bytes_per_line >>= (unsigned char)3;
  index = y;
  index *= bytes_per_line;
  index += (unsigned char)(x>>3);
  index += CHR_DATA;
  
  
  if ( buf[index] & ( (unsigned char)1 << (x&(unsigned char)7)) ) 
    return 1;
  return 0;
}

void chr_show(unsigned char *buf)
{
  unsigned char x,y;
  
  if ( chr_get_len(buf) == 0 )
    return;
  
  for( y = 0; y < chr_get_height(buf) ; y++ )
  {
    printf("%4d ", y);
    for( x = 0; x < chr_get_width(buf) ; x++ )
    {
      if ( chr_get_pixel(buf, x,chr_get_height(buf)-y-1) == 0 )
	printf(". ");
      else
	printf("* ");
    }
  printf("\n");
  }
}

void fnt_show_char(unsigned char *fbuf, unsigned char code)
{
  chr_show(fnt_get_chr(fbuf, code));  
}

void chr_set(unsigned char *buf, unsigned char x, unsigned char y, unsigned char val )
{
  unsigned char bytes_per_line;
  unsigned char offset;
  unsigned char data;
  bytes_per_line = (chr_get_width(buf)+7) >> 3;
  offset = y*bytes_per_line+(x>>3)+CHR_DATA;
  data = buf[offset];
  if ( val == 0 )
    data &= ~(1 << (x&7));
  else
    data |= (1 << (x&7));
  buf[offset] = data;
}


#define BDF_TOK_CHAR_MAX 256
#define BDF_TOK_CNT_MAX 32
#define BDF_DATA_MAX 1024

#define BDF_LINE_MAX (BDF_TOK_CHAR_MAX*BDF_TOK_CNT_MAX+BDF_TOK_CHAR_MAX)


FILE *bdf_fp;
char bdf_line[BDF_LINE_MAX];
char bdf_tok_list[BDF_TOK_CNT_MAX][BDF_TOK_CHAR_MAX];
int bdf_tok_cnt;

char bdf_FONT[BDF_TOK_CHAR_MAX];
char bdf_COPYRIGHT[BDF_TOK_CHAR_MAX];
char bdf_STARTCHAR[BDF_TOK_CHAR_MAX];
int bdf_ENCODING;
int bdf_DWIDTH;
int bdf_BBX_w;
int bdf_BBX_h;
int bdf_BBX_x;
int bdf_BBX_y;
int bdf_is_bitmap = 0;
unsigned char bdf_data[BDF_DATA_MAX];
int bdf_data_cnt;

void bdf_char_available(void);

int bdf_get_pix(int x, int y)
{
  unsigned int b = bdf_data[y*((bdf_BBX_w+7)/8) + (x/8)];
  if ( b & 1<<(7-(x&7)) )
    return 1;
  return 0;
}

void bdf_show_char(void)
{
  int x, y;
  for( y = 0; y < bdf_BBX_h; y++)
  {
    for( x = 0; x < bdf_BBX_w; x++)
    {
      if ( bdf_get_pix(x,y) )
	printf("#");
      else
	printf(".");
    }
    printf("\n");
  }
}

int bdf_read_line(void)
{
  if ( fgets(bdf_line, BDF_LINE_MAX, bdf_fp) == NULL )
    return 0;
  return 1;
}

void bdf_extract_tokens(void)
{
  char *delim = " \"\t\r\n";
  char *d;
  char *s;
  bdf_tok_cnt = 0;
  d = delim;
  s = strtok(bdf_line, d);
  for(;;)
  {
    if ( s == NULL )
      break;
    strcpy(bdf_tok_list[bdf_tok_cnt], s);
    bdf_tok_cnt++;
    if ( bdf_tok_cnt >= BDF_TOK_CNT_MAX )
      break;
    s = strtok(NULL, d);
  }
}

int bdf_ascii_to_nibble(char ascii)
{
  if ( ascii >= '0' && ascii <= '9' )
    return ascii - '0';
  if ( ascii >= 'a' && ascii <= 'z' )
    return ascii-'a'+10;
  return ascii-'A'+10;
}

void bdf_store_byte(char upper, char lower)
{
  bdf_data[bdf_data_cnt++] =  (bdf_ascii_to_nibble(upper)<<4) + bdf_ascii_to_nibble(lower);  
}

void bdf_store_data(void)
{
  int l ;
  int i = 0;
  char upper;
  char lower;

  if ( bdf_tok_cnt == 0 )
    return;

  l = strlen(bdf_tok_list[0]);
  if ( strcmp( bdf_tok_list[0], "ENDCHAR" ) == 0 )
  {
    bdf_is_bitmap = 0;
    bdf_char_available();
  }
  else
  {
    for(;;)
    {
      upper = bdf_tok_list[0][i]; 
      lower = bdf_tok_list[0][i+1]; 
      bdf_store_byte(upper, lower);
      i+= 2;
      if ( i >= l )
	break;
    }
  }
}

void bdf_store_cmd(void)
{
  int i;
  if ( bdf_tok_cnt == 0 )
    return;
  /* puts(bdf_tok_list[0]); */
  if ( strcmp(bdf_tok_list[0], "STARTCHAR") == 0 )
  {
    if ( bdf_tok_cnt == 1 )
      return;
    strcpy(bdf_STARTCHAR, bdf_tok_list[1] );
  } 
  else if ( strcmp(bdf_tok_list[0], "FONT") == 0 )
  {
    if ( bdf_tok_cnt == 1 )
      return;
    strcpy(bdf_FONT, bdf_tok_list[1] );
  } 
  else if ( strcmp(bdf_tok_list[0], "COPYRIGHT") == 0 )
  {
    if ( bdf_tok_cnt == 1 )
      return;
    bdf_COPYRIGHT[0] = '\0';
    for( i = 1; i < bdf_tok_cnt; i++ )
    {
      strcpy(bdf_COPYRIGHT+strlen(bdf_COPYRIGHT), bdf_tok_list[i] );
      strcpy(bdf_COPYRIGHT+strlen(bdf_COPYRIGHT), " " );
    }
  } 
  else if ( strcmp(bdf_tok_list[0], "ENCODING") == 0 )
  {
    if ( bdf_tok_cnt == 1 )
      return;
    bdf_ENCODING = atoi(bdf_tok_list[1]);
  }
  else if ( strcmp(bdf_tok_list[0], "DWIDTH") == 0 )
  {
    if ( bdf_tok_cnt == 1 )
      return;
    bdf_DWIDTH = atoi(bdf_tok_list[1]);
  }
  else if ( strcmp(bdf_tok_list[0], "BBX") == 0 )
  {
    if ( bdf_tok_cnt < 5 )
      return;
    bdf_BBX_w = atoi(bdf_tok_list[1]);
    bdf_BBX_h = atoi(bdf_tok_list[2]);
    bdf_BBX_x = atoi(bdf_tok_list[3]);
    bdf_BBX_y = atoi(bdf_tok_list[4]);
  }
  else if ( strcmp(bdf_tok_list[0], "BITMAP") == 0 )
  {
    bdf_is_bitmap = 1;
    bdf_data_cnt = 0;
  }
}

void bdf_store(void)
{
  if ( bdf_is_bitmap != 0 )
    bdf_store_data();
  else
    bdf_store_cmd();
}

void bdf_read_file(void)
{
  fseek(bdf_fp, 0, SEEK_SET);
  for(;;)
  {
    if ( bdf_read_line() == 0 )
      break;
    bdf_extract_tokens();
    bdf_store();
  }
}

void write_font_file(const char *filename, const char *fontname)
{
  FILE *fp = fopen(filename, "w");
  int i;
  if ( fp != NULL )
  {
    fprintf(fp, "/*\n");
    fprintf(fp, "  FONT %s\n", bdf_FONT);
    fprintf(fp, "  COPYRIGHT %s\n", bdf_COPYRIGHT);
    fprintf(fp, "  Converted by bdf2dogm\n");
    fprintf(fp, "  Use\n");
    fprintf(fp, "      extern const dog_pgm_uint8_t %s[];\n", fontname);
    fprintf(fp, "  to declare the font.\n");
    fprintf(fp, "*/\n");
    
    /*
    fprintf(fp, "#include <avr/pgmspace.h>\n");
    fprintf(fp, "#ifndef PROGMEM\n");
    fprintf(fp, "#define PROGMEM\n");
    fprintf(fp, "#endif\n");
    fprintf(fp, "const char %s[%d] PROGMEM = {\n  ", fontname, font_buf_len);
    */

    fprintf(fp, "#include \"dogmpgm.h\"\n");
    fprintf(fp, "const dog_pgm_uint8_t %s[%d] = {\n  ", fontname, font_buf_len);
    
    for( i = 0;i < font_buf_len; i++ )
    {
      fprintf(fp, "0x%02x", font_buf[i]);
      if ( i+1 < font_buf_len )
	fprintf(fp, ", ");
      if ( i % 16 == 15 )
	fprintf(fp, "\n  ");
    }
    fprintf(fp, "\n};\n");
    fclose(fp);
  }
  else
  {
    perror(filename);
  }
}

int bdf_is_zero_line(int y)
{
  int x;
    for( x = 0; x < bdf_BBX_w; x++ )
    {
      if ( bdf_get_pix(x,y) != 0 )
	return 0;
    }
    return 1;
}


void bdf_copy_to_chr(unsigned char *buf)
{
  int x, y, dogm_x, upshift;
  unsigned char val;
  unsigned len;
  int y_start;
  int y_end;
  int w, h;
  int descent;

  /* assert( bdf_BBX_y <= 0 ); */
  /*assert( bdf_BBX_x == 0 );*/
  
  assert( bdf_BBX_w <= CHR_MAX_WIDTH );
  assert( bdf_BBX_h <= CHR_MAX_HEIGHT );

  assert( bdf_DWIDTH > 0 );
  
  w = bdf_BBX_w;
  h = bdf_BBX_h;
  
  /* internal format does not support x vectors... a hack... */
  if ( bdf_DWIDTH < bdf_BBX_w+bdf_BBX_x )
    bdf_DWIDTH = bdf_BBX_w+bdf_BBX_x;
  
  y_start = 0;
  y_end = y_start + bdf_BBX_h - 1;
  if ( bdf_BBX_y <= 0 )
  {
    descent = -bdf_BBX_y;
    upshift = 0;
  }
  else
  {
    descent = 0;
    upshift = bdf_BBX_y;
  }

  for(;;)
  {
    if ( descent == 0 )
      break;
    if ( y_end == 0 )
      break;
    if ( y_end == y_start )
      break;
    
    if ( bdf_is_zero_line(y_end) == 0 )
      break;
    descent--;
    y_end--;
  }

  for(;;)
  {
    if ( y_start < 0 )
      break;
    if ( y_end <= y_start )
      break;
    
    if ( bdf_is_zero_line(y_start) == 0 )
      break;
    y_start++;
  }

  if ( font_max_bbox_descent < descent )
    font_max_bbox_descent = descent;

  if ( font_max_bbox_width < w )
    font_max_bbox_width = w;
  
  if ( font_max_bbox_height < h+descent )
    font_max_bbox_height = h+descent;
  
  
  len = ((bdf_DWIDTH+7)/8) * (y_end-y_start+1+upshift) + CHR_DATA;
  assert( len <= CHR_MAX_LEN );

  chr_set_descent_len_hw(buf,descent,len,y_end-y_start+1+upshift, bdf_DWIDTH);

  /*
  for( i = 0; i < len ; i++ )
    buf[CHR_DATA+i] = 0;
  */
  
  for( y = y_start; y <= y_end; y++ )
  {
    for( x = 0; x < bdf_BBX_w; x++ )
    {
      dogm_x = x + bdf_BBX_x;
      val = bdf_get_pix(x,y);
      if ( dogm_x >= 0 && dogm_x < bdf_DWIDTH )
	chr_set(buf, dogm_x, y_end-y+upshift, val);
    }
  }
}

void bdf_char_available(void)
{
  if ( font_ascii == bdf_ENCODING )
  {
#ifdef DEBUG
    printf("ENCODING %d\n", bdf_ENCODING) ;
    bdf_show_char();
#endif
    bdf_copy_to_chr(font_buf + font_buf_len);
#ifdef DEBUG
    chr_show(font_buf + font_buf_len);
#endif

    font_buf_len += chr_get_len(font_buf + font_buf_len);
#ifdef DEBUG
    printf("BUF LEN %d\n", font_buf_len);
#endif
  }
}

int main(int argc, char **argv)
{
  int pos;
  if ( argc <= 3 )
  {
    printf("%s infile outfile fontname\n", argv[0]);
    return 1;
  }
  font_buf_len = FNT_DATA;
  bdf_fp = fopen(argv[1], "r");
  if ( bdf_fp == NULL )
  {
    perror(argv[1]);
    return 1;
  }
  
  font_ascii = ' ';
  while( font_ascii < 128 )
  {
    fseek(bdf_fp, 0, SEEK_SET);
    bdf_read_file();
    font_ascii++;
  }
  fclose(bdf_fp);
  
  font_buf[font_buf_len++] = 0;
  
  pos = fnt_get_chr(font_buf, 'A') - font_buf;
  fnt_set_bbox_A(font_buf,pos);
  pos = fnt_get_chr(font_buf, 'a') - font_buf;
  fnt_set_bbox_a(font_buf,pos);

  
  fnt_set_bbox_width(font_buf,font_max_bbox_width);
  fnt_set_bbox_height(font_buf,font_max_bbox_height);
  fnt_set_bbox_descent(font_buf,font_max_bbox_descent);
  
#ifdef DEBUG
  printf("max_bbox_width %d\n", font_max_bbox_width);
  printf("max_bbox_height %d\n", font_max_bbox_height);
  printf("max_bbox_descent %d\n", font_max_bbox_descent);
#endif
 
  write_font_file(argv[2], argv[3]);
  
  return 0;
}

