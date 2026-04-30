// #define ansi_fg(x) x+30
// #define ansi_bg(x) x+40
namespace colors {
  const char color_reset=0;//chars as integers not chars
  constexpr color_t col(enum color fore,enum color back){return ((fore)|(back<<4));}
  constexpr color_t default_color=col(white,black);
  constexpr char* ansi_fg(char c,char* buf){
    if(c&0x08){snprintf(buf,8,"\x1b""[%.2d;1m",(c&0x07)+30);return buf;}
    else{snprintf(buf,6,"\x1b""[%.2dm",  (c&0x07)+30);return buf;}//holy memleak
  }
  constexpr char* ansi_bg(char c,char* buf){
    if(c&0x80){snprintf(buf,8,"\x1b""[%.2d;1m",(c>>4)&0x07+40);return buf;}
    else{snprintf(buf,6,"\x1b""[%.2dm",  (c>>4)&0x07+40);return buf;}
  }
}