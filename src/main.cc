#define RATATOUILLE_NCURSES
#define PRINT_TRI3(B,T,F) fprintf(B,"triangle((%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F ")),",T.a.x,T.a.y,T.a.z,T.b.x,T.b.y,T.b.z,T.c.x,T.c.y,T.c.z)
#define PRINT_TRI2(B,T,F) fprintf(B,"polygon((%" #F ",%" #F "),(%" #F ",%" #F "),(%" #F ",%" #F ")),",T.a.x,T.a.y,T.b.x,T.b.y,T.c.x,T.c.y)
#define PRINT_TRI21(B,F)   fprintf(B,"polygon((%" #F ",%" #F "),(%" #F ",%" #F "),(%" #F ",%" #F ")),",x0,y0,x1,y1,x2,y2)
#define PRINT_TRI23(B,T,F) fprintf(B,"triangle((%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F ")),",T.a.x,T.a.y,0,T.b.x,T.b.y,0,T.c.x,T.c.y,0)
#include <stdio.h>
#include <stdlib.h>
#include <type_traits>
#include <cmath>
#include <time.h>
FILE* debug;
bool logmisc=false;
template<typename T> concept arith=std::is_arithmetic_v<T>;
template<typename T> concept comp =requires(T a,T b){a<b;a>b;a==b;};
template<comp T,comp U> T constexpr const min(T a,U b){return a<b?a:b;}
template<comp T,comp U> T constexpr const max(T a,U b){return a<b?b:a;}
template<comp T,comp...U> T constexpr const min(T t, U...a){
  T b=min(a...);
  return t<b?t:b;
}
template<comp T,comp...U> T constexpr const max(T t, U...a){
  T b=max(a...);
  return t<b?b:t;
}
#include <types.hpp>
#include <assets.hpp>
#include <r@@2e.hpp>
#include <3rats.hpp>
gui::text_t* text=new gui::text_t[3];//who cares
gui::menu_t menu{};
void a(){
  text[0]={&gui::default_font,"thank you that's what i've been saying",38,gui::LEFT};
  menu.textcount=1;
  menu.btncount=0;
}
void b(){
  text[0]={&gui::default_font,"you're wrong :thumbsdown:",25,gui::LEFT};
  menu.textcount=1;
  menu.btncount=0;
}
void c(){
  text[0]={&gui::default_font,"you were supposed to pick yes or no...",38,gui::LEFT};
  menu.textcount=1;
  menu.btncount=0;
}
int main() {
  puts("\033[?1049h");
  puts("\rRAT GAME 16");
  debug=fopen("./debug/debug.log","w");
  if(ferror(debug)||errno||!debug){perror("couldn't open debug log file :(");exit(1);}
  puts("LOADING MODELS");
  assets::asset3d_t model0=assets::readAsset3d("./assets/poster.rgmdl");//ari i'm going to ear you
  assets::asset3d_t model1=assets::readAsset3d("./assets/base.rgmdl");
  puts("LOADING FONT");
  assets::font_t font=assets::readFont("./assets/font/6x5.rgft");
  assets::font_t font1=assets::readFont("./assets/font/3x3.rgft");
  gui::default_font=assets::readFont("./assets/font/1x1.rgft");
  gui::init();
  unsigned char escapes=0;
  unsigned char rotamnt=16;
  float rotamntrad = (rotamnt/128.0f)*M_PI;
  float rottrck=0;
  text[0]={&font,"Rat!!!!!!!!! Game, 16!!",23,gui::LEFT},
  text[1]={&gui::default_font,"The quick brown fox jumps over the lazy dog. Sphinx of black quartz, hear my vow.",81,gui::LEFT};
  text[2]={&gui::default_font,"there are menus now. they even have fancy fonts and text wrapping. does that count as a game mechanic?",102,gui::LEFT};
  gui::text_t buttons[3]={{&font1,"yes",3,gui::CENTER},{&gui::default_font,"no",2,gui::CENTER},{&gui::default_font,"use left and right brackets to navigate up and down dialog options and enter to select",86,gui::RIGHT}};
  void (*funcs[3])()={&a,&b,&c};
  menu={
    60,24,{'-','-','|','|','+'},3,text,3,buttons,funcs
  };
  gui::selected_menu=&menu;
  gui::selected_btn=0;
  while(true){
    char c=gui::readInput();
    switch(c){//escapey bits. add more later probably. note that tmux is doing strange things to us
      case '\e':escapes|='\x01';continue;
      case '[' :if((escapes&'\x03')=='\x01'){escapes|='\x02';continue;}else{//am i getting ear'd for this one
        gui::selected_btn=(gui::selected_btn+gui::selected_menu->btncount-1)%gui::selected_menu->btncount;
        break;
      }
      case 'q':puts("\x1b[c");puts("\033[?1049l");puts("closed rat game screen");gui::stop();exit(0);break;
    }
    if((escapes&'\x03')=='\x03'){
      switch(c){
        case 'A':mesh::farplanex++;break;
        case 'B':mesh::farplanex--;break;
        case 'C':mesh::camera_rotation.z-=rotamnt;rottrck+=rotamntrad;break;//left
        case 'D':mesh::camera_rotation.z+=rotamnt;rottrck-=rotamntrad;break;//right
      }
      if(rottrck > 2*M_PI){rottrck-=2*M_PI;}
      if(rottrck < 2*M_PI){rottrck+=2*M_PI;}
      escapes=0;
    }else{
      switch(c){
        case 'w':mesh::camera_position.x+=cos(rottrck);mesh::camera_position.y+=sin(rottrck);break;//ari i just looked at these
        case 's':mesh::camera_position.x-=cos(rottrck);mesh::camera_position.y-=sin(rottrck);break;//,,, not a big fan
        case 'd':mesh::camera_position.x-=sin(rottrck);mesh::camera_position.y+=cos(rottrck);break;//either put everything on radians
        case 'a':mesh::camera_position.x+=sin(rottrck);mesh::camera_position.y-=cos(rottrck);break;//or dont, PICK ONE
        case ',':mesh::camera_position.z++;break;
        case '.':mesh::camera_position.z--;break;
        case 'e':logmisc=!logmisc;break;
        case ']':gui::selected_btn=(gui::selected_btn+1)%gui::selected_menu->btncount;break;
        case '\r':if(gui::selected_menu->funcs[gui::selected_btn]){gui::selected_menu->funcs[gui::selected_btn]();}break;
      }
    }
    if(c){
      clock_t t=clock();
      gui::clear_scr();
      for(short unsigned int i=0;i<model1.mesh.tricount;i++){
        gui::drawMTri(model1.mesh.tris[i],model1.texture);
      }
      for(short unsigned int i=0;i<model0.mesh.tricount;i++){
        gui::drawMTri(model0.mesh.tris[i],model0.texture);
      }
      gui::putMenu(&menu,1,5);
      gui::drawFrame();
      clock_t t1=clock()-t;
      float s=t1/(float)CLOCKS_PER_SEC;
      if(s>0.1){
        fprintf(debug,"took %.3fs to draw a frame!!!\n",s);
      }
      escapes=0;
    }
  }
  fclose(debug);
  return 0;
}//WHAT ARE YOU DOING
