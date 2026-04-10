#define RATATOUILLE_NCURSES
#define PRINT_TRI3(B,T,F) fprintf(B,"triangle((%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F "),(%" #F ",%" #F ",%" #F ")),",T.a.x,T.a.y,T.a.z,T.b.x,T.b.y,T.b.z,T.c.x,T.c.y,T.c.z)
#define PRINT_TRI2(B,T,F) fprintf(B,"polygon((%" #F ",%" #F "),(%" #F ",%" #F "),(%" #F ",%" #F ")),",T.a.x,T.a.y,T.b.x,T.b.y,T.c.x,T.c.y)
#define PRINT_TRI21(B,F)   fprintf(B,"polygon((%" #F ",%" #F "),(%" #F ",%" #F "),(%" #F ",%" #F ")),",x0,y0,x1,y1,x2,y2)
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

const int COLLISION_TESTS = 5;
float Pspeed = 0.1f;

int main() {
  puts("\rRAT GAME 16");
  puts("LOADING MODELS");
  assets::asset3d_t model=assets::readAsset3d("assets/cube.rgmdl");//ari i'm going to ear you
  puts("LOADING COLLISION");
  mesh::cshape_t cshape(mesh::CUBE);
  cshape.pos={0,0,0};
  cshape.rot={0,0,0};
  cshape.size={2,2,2};
  mesh::cshape_t Pshape(mesh::CUBE);
  Pshape.pos={0,0,0};
  Pshape.rot={0,0,0};
  Pshape.size={0.5f,0.5f,0.5f};
  
  assets::readCollisonMap3d("assets/test.kr");
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
      case 'q' :gui::stop();exit(0);break;
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
      mesh::vec3<mesh::mesh_size> camMove {0,0,0};
      //cshape.colliding({2,2,2},2)
      switch(c){
        case 'w':camMove.x+=Pspeed*cos(rottrck);camMove.y+=Pspeed*sin(rottrck);break;//ari i just looked at these
        case 's':camMove.x-=Pspeed*cos(rottrck);camMove.y-=Pspeed*sin(rottrck);break;//,,, not a big fan
        case 'd':camMove.x-=Pspeed*sin(rottrck);camMove.y+=Pspeed*cos(rottrck);break;//either put everything on radians
        case 'a':camMove.x+=Pspeed*sin(rottrck);camMove.y-=Pspeed*cos(rottrck);break;//or dont PICK ONE
        case ',':camMove.z+=Pspeed;break;
        case '.':camMove.z-=Pspeed;break;
        case 'e':logmisc=!logmisc;break;
      }
      Pshape.pos = mesh::camera_position+camMove;
      auto [res, norm, proj] = cshape.checkCollision(Pshape);
      if (!res){
        mesh::camera_position = mesh::camera_position + camMove;
      }else{
        mesh::camera_position = mesh::camera_position + camMove+(norm*(proj+0.01f)); // i hate precision
      }
    }
    if(c){

      printf("position: (%f,%f,%f)",mesh::camera_position.x,mesh::camera_position.y,mesh::camera_position.z);
      gui::clear_scr();
      for(short unsigned int i=0;i<model.mesh.tricount;i++){
        gui::drawMTri(model.mesh.tris[i],model.texture);
      }
      gui::drawFrame();
      escapes=0;
    }
  }
  fclose(debug);
  return 0;
}//WHAT ARE YOU DOING