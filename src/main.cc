#define RATATOUILLE_NCURSES
#define PRINT_TRI3(B,T,F) fprintf(B,"triangle((%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F ")),",T.a.x,T.a.y,T.a.z,T.b.x,T.b.y,T.b.z,T.c.x,T.c.y,T.c.z)
#define PRINT_TRI2(B,T,F) fprintf(B,"polygon((%" #F ",%" #F "),(%" #F ",%" #F "),(%" #F ",%" #F ")),",T.a.x,T.a.y,T.b.x,T.b.y,T.c.x,T.c.y)
#define PRINT_TRI23(B,T,F) fprintf(B,"triangle((%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F ")),",T.a.x,T.a.y,0,T.b.x,T.b.y,0,T.c.x,T.c.y,0)
#include <stdio.h>
#include <stdlib.h>
#include <type_traits>
#include <cmath>
FILE* debug=fopen("./debug/debug.log","w");
bool logmisc=false;
template<typename T> concept arith=std::is_arithmetic_v<T>;
template<typename T> concept comp =requires(T a,T b){a<b;a>b;a==b;};
template<comp T,comp U> T constexpr min(T a,U b){return a<b?a:b;}
template<comp T,comp U> T constexpr max(T a,U b){return a<b?b:a;}
template<comp T,comp...U> T constexpr min(T t, U...a){
  T b=min(a...);
  return t<b?t:b;
}
template<comp T,comp...U> T constexpr max(T t, U...a){
  T b=max(a...);
  return t<b?b:t;
}
#include <types.hpp>
#include <r@@2e.hpp>
#include <3rats.hpp>
#include <assets.hpp>
int main() {
  puts("\rRAT GAME 16");
  puts("LOADING MODELS");
  assets::asset3d_t model=assets::readAsset3d("assets/cube.rgmdl");//ari i'm going to ear you
  puts("LOADING FONT");
  assets::font_t font=assets::readFont("assets/font/2x2");
  gui::init();
  unsigned char escapes=0;
  unsigned char rotamnt=16;
  float rotamntrad = (rotamnt/128.0f)*M_PI;
  float rottrck=0;
  while(true){
    char c=gui::readInput();
    switch(c){//escapey bits. add more later probably. note that tmux is doing strange things to us
      case '\e':escapes|='\x01';continue;
      case '[' :if(escapes&'\x03'=='\x01'){escapes|='\x02';}continue;
      case 'q':gui::stop();exit(0);break;
    }
    if(escapes&'\x03'=='\x03'){
      switch(c){
        case 'A':mesh::farplanex++;break;
        case 'B':mesh::farplanex--;break;
        case 'C':mesh::camera_rotation.z-=rotamnt;rottrck+=rotamntrad;break;//left
        case 'D':mesh::camera_rotation.z+=rotamnt;rottrck-=rotamntrad;break;//right
        if(rottrck > 2*M_PI){rottrck-=2*M_PI;}
        if(rottrck < 2*M_PI){rottrck+=2*M_PI;}
      }
      escapes=0;
      // continue;
    }else{
      switch(c){
        case 'w':mesh::camera_position.x+=cos(rottrck);mesh::camera_position.y+=sin(rottrck);break;
        case 's':mesh::camera_position.x-=cos(rottrck);mesh::camera_position.y-=sin(rottrck);break;
        case 'd':mesh::camera_position.x-=sin(rottrck);mesh::camera_position.y+=cos(rottrck);break;
        case 'a':mesh::camera_position.x+=sin(rottrck);mesh::camera_position.y-=cos(rottrck);break;
        case ',':mesh::camera_position.z++;break;
        case '.':mesh::camera_position.z--;break;
        case 'e':logmisc=!logmisc;break;
      }
    }
    if(c){
      gui::clear_scr();
      for(short unsigned int i=0;i<model.mesh.tricount;i++){
        gui::drawMTri(model.mesh.tris[i],model.texture);
      }
      gui::putFText(&font,"According to all known laws of aviation",39,1,1,40);
      gui::drawFrame();
      escapes=0;
    }
  }
  fclose(debug);
  return 0;
}//WHAT ARE YOU DOING