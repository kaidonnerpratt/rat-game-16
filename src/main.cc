#define RATATOUILLE_NCURSES
#define PRINT_TRI3(B,T,F) fprintf(B,"triangle((%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F ")),",T.a.x,T.a.y,T.a.z,T.b.x,T.b.y,T.b.z,T.c.x,T.c.y,T.c.z)
#define PRINT_TRI2(B,T,F) fprintf(B,"polygon((%" #F ",%" #F "),(%" #F ",%" #F "),(%" #F ",%" #F ")),",T.a.x,T.a.y,T.b.x,T.b.y,T.c.x,T.c.y)
#define PRINT_TRI23(B,T,F) fprintf(B,"triangle((%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F ")),",T.a.x,T.a.y,0,T.b.x,T.b.y,0,T.c.x,T.c.y,0)
#include <stdio.h>
#include <stdlib.h>
#include <type_traits>
#include <cmath>
#include <string>
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
#include <portal.hpp>
int main() {
  puts("\rRAT GAME 16");
  puts("LOADING MODELS");
  assets::asset3d_t model=assets::readAsset3d("assets/cube.rgmdl");//ari i'm going to ear you
  mesh::Plane prt_plane(0,0,0,0,0,0,1,2);
  portal::Portal p(2);

  // ptri1.a.x =  -1; ptri2.a.x =  -1;
  // ptri1.a.y =   1; ptri2.a.y =  -1;
  // ptri1.a.z =   1; ptri2.a.z =  -1;
  // ptri1.b.x =  -1; ptri2.b.x =  -1;
  // ptri1.b.y =  -1; ptri2.b.y =   1;
  // ptri1.b.z =  -1; ptri2.b.z =   1;
  // ptri1.c.x =  -1; ptri2.c.x =  -1;
  // ptri1.c.y =  -1; ptri2.c.y =   1;
  // ptri1.c.z =   1; ptri2.c.z =  -1;


  p.set_tri(0, prt_plane.e);
  p.set_tri(1, prt_plane.f);

  // for(unsigned int x=0;x<model.texture.width;x++){
  //   for(unsigned int y=0;y<model.texture.height;y++){
  //     fprintf(debug,"(%u,%u,%u,%i),",x,y,(y*model.texture.width+x)*3,model.texture.pixels[(y*model.texture.width+x)*3]);
  //   }
  // }
  // fputc('\n',debug);
  // fflush(debug);
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
      // mesh::vec3<mesh::mesh_size> cstorage = mesh::camera_position;
      // mesh::camera_position = 
      
      // for(short unsigned int i=0;i<model.mesh.tricount;i++){
      //   gui::drawMTri(model.mesh.tris[i],model.texture);
      // }
      // gui::drawMTri(     model.mesh.tris[9],model.texture);
      // printf("out: %f\n",model.mesh.tris[9].a.x);
      // printf("out: %f\n",model.mesh.tris[9].a.y);
      // printf("out: %f\n",model.mesh.tris[9].a.z);
      // printf("out: %f\n",model.mesh.tris[9].b.x);
      // printf("out: %f\n",model.mesh.tris[9].b.y);
      // printf("out: %f\n",model.mesh.tris[9].b.z);
      // printf("out: %f\n",model.mesh.tris[9].c.x);
      // printf("out: %f\n",model.mesh.tris[9].c.y);
      // printf("out: %f\n",model.mesh.tris[9].c.z);

      gui::drawPMTri(p.tris[0], colors::white);
      gui::drawPMTri(p.tris[1], colors::white);

      gui::drawFrame();
      
      escapes=0;
    }
  }
  fclose(debug);
  return 0;
}//WHAT ARE YOU DOING