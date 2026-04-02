#ifndef RATS3D_H
#define RATS3D_H
#include <type_traits>
#include <cstdlib>
#include <r@@2e.hpp>
#include <cmath>
#define SCAST(t,v) static_cast<t>(v)
#define MESHTRI_OUTLN_01 0b00000001
#define MESHTRI_OUTLN_12 0b00000010
#define MESHTRI_OUTLN_20 0b00000100
template<arith T> inline auto constexpr triarea(T x0,T y0,T x1,T y1,T x2,T y2){
  if constexpr(std::is_integral_v<T>){
    using sT=std::make_signed_t<T>;
    return -((x0 * ((sT)y1-y2)) + (x1 * ((sT)y2-y0)) + (x2 * ((sT)y0-y1)));
  }else{return -((x0 * (y1-y2)) + (x1 * (y2-y0)) + (x2 * (y0-y1)));}
}
namespace mesh {
  unsigned int farplanex=8;
  const char* charsbyopacity="$@MN%&E0K?UO^!;:,.";
  int opacitylength=18;
  template<arith T> inline void rotate(T& axis_0,T& axis_1,char d){
    float r1=cos(d/128.0*M_PI),r2=sin(d/128.0*M_PI);
    float axis_0_t=(axis_0*r1)-(axis_1*r2);
    axis_1=axis_1*r1+axis_0*r2;
    axis_0=axis_0_t;
  }
  template<typename T> inline tri3<T> rotateT(tri3<T>& v,char d){
    rotate(v.a.x,v.a.y,d);rotate(v.b.x,v.b.y,d);rotate(v.c.x,v.c.y,d);
    return v;
  }
  template<typename T> requires std::is_signed_v<T> vec3<T>* clipTriX(const tri3<T>& t,T x){//012,230
    vec3<T>* out=(vec3<T>*)malloc(sizeof(vec3<T>)*4);
    #define p0 t.a
    #define p1 t.b
    #define p2 t.c
    #define o0 out[0]
    #define o1 out[1]
    #define o2 out[2]
    #define o3 out[3]
    o3={0,0,0};
    char v=(p0.x>=x)+(p1.x>=x)+(p2.x>=x);
    switch(v){
      case 0:break;
      case 1:{
        if(p0.x>=x){
          o0=p0;
          o1={
            x,
            (p0.y-p1.y)/(p0.x-p1.x)*(x-p1.x)+p1.y,
            (p0.z-p1.z)/(p0.x-p1.x)*(x-p1.x)+p1.z
          };
          o2={
            x,
            (p0.y-p2.y)/(p0.x-p2.x)*(x-p2.x)+p2.y,
            (p0.z-p2.z)/(p0.x-p2.x)*(x-p2.x)+p2.z
          };
        }else if(p1.x>=x){
          o0={
            x,
            (p1.y-p0.y)/(p1.x-p0.x)*(x-p0.x)+p0.y,
            (p1.z-p0.z)/(p1.x-p0.x)*(x-p0.x)+p0.z
          };
          o1=p1;
          o2={
            x,
            (p1.y-p2.y)/(p1.x-p2.x)*(x-p2.x)+p2.y,
            (p1.z-p2.z)/(p1.x-p2.x)*(x-p2.x)+p2.z
          };
        }else{
          o0={
            x,
            (p2.y-p0.y)/(p2.x-p0.x)*(x-p0.x)+p0.y,
            (p2.z-p0.z)/(p2.x-p0.x)*(x-p0.x)+p0.z
          };
          o1={
            x,
            (p2.y-p1.y)/(p2.x-p1.x)*(x-p1.x)+p1.y,
            (p2.z-p1.z)/(p2.x-p1.x)*(x-p1.x)+p1.z
          };
          o2=p2;
        }
        break;
      }
      case 2:{
        if(p0.x<x){
          o0={
            x,
            (p1.y-p0.y)/(p1.x-p0.x)*(x-p0.x)+p0.y,
            (p1.z-p0.z)/(p1.x-p0.x)*(x-p0.x)+p0.z
          };
          o1=p1;
          o2=p2;
          o3={
            x,
            (p0.y-p2.y)/(p0.x-p2.x)*(x-p2.x)+p2.y,
            (p0.z-p2.z)/(p0.x-p2.x)*(x-p2.x)+p2.z
          };
        }else if(p1.x<x){
          o0=p0;
          o1={
            x,
            (p0.y-p1.y)/(p0.x-p1.x)*(x-p1.x)+p1.y,
            (p0.z-p1.z)/(p0.x-p1.x)*(x-p1.x)+p1.z
          };
          o2={
            x,
            (p1.y-p2.y)/(p1.x-p2.x)*(x-p2.x)+p2.y,
            (p1.z-p2.z)/(p1.x-p2.x)*(x-p2.x)+p2.z
          };
          o3=p2;
        }else{
          o0=p0;
          o1=p1;
          o2={
            x,
            (p1.y-p2.y)/(p1.x-p2.x)*(x-p2.x)+p2.y,
            (p1.z-p2.z)/(p1.x-p2.x)*(x-p2.x)+p2.z
          };
          o3={
            x,
            (p0.y-p2.y)/(p0.x-p2.x)*(x-p2.x)+p2.y,
            (p0.z-p2.z)/(p0.x-p2.x)*(x-p2.x)+p2.z
          };
        }
        break;
      }
      case 3:{
        return_original:
        o0=t.a;o1=t.b;o2=t.c;
      }
    }
    return out;
    #undef p0
    #undef p1
    #undef p2
    #undef o0
    #undef o1
    #undef o2
    #undef o3
  }
  vec3<mesh_size> camera_position{-5.0f,0.0f,0.0f};
  vec3<char>      camera_rotation{0,0,0};//roll pitch yaw
}
namespace gui {
  using namespace mesh;
  inline vec2<scoord> toSSPV(vec3<mesh_size> v){
    return (vec2<scoord>){toSSPX(v.y,v.x),toSSPY(v.z,v.x)};
  }
  inline tri2<scoord> toSSPT(tri3<mesh_size> t){
    return (tri2<scoord>){toSSPV(t.a),toSSPV(t.b),toSSPV(t.c)};
  }
  void drawTri(const tri3<mesh_size>& t1, vec2<float> uv0, vec2<float> uv1, vec2<float> uv2, assets::texture_t& tex){
    mesh_size z0=t1.a.x,z1=t1.b.x,z2=t1.c.x;
    signed short int
      x0=toSSPX(t1.a.y,z0),y0=toSSPY(t1.a.z,z0),
      x1=toSSPX(t1.b.y,z1),y1=toSSPY(t1.b.z,z1),
      x2=toSSPX(t1.c.y,z2),y2=toSSPY(t1.c.z,z2);
    scoord minx=max(min(x0,x1,x2),0),
           miny=max(min(y0,y1,y2),0),
           maxx=min(max(x0,x1,x2),gui::term_dims.ws_col),
           maxy=min(max(y0,y1,y2),gui::term_dims.ws_row);
    float area=triarea(
      SCAST(float,x0),SCAST(float,y0),
      SCAST(float,x1),SCAST(float,y1),
      SCAST(float,x2),SCAST(float,y2));
    if(area<0.1){return;}
    if(logmisc){
      // PRINT_TRI3(debug,t1,f);
      // fprintf(debug,"polygon((%i,%i),(%i,%i),(%i,%i)),",x0,y0,x1,y1,x2,y2);
      // fprintf(debug,"polygon((%i,%i),(%i,%i),(%i,%i),(%i,%i)),",minx,miny,maxx,miny,maxx,maxy,minx,maxy);
    }
    for(scoord x=minx;x<maxx;x++){
      for(scoord y=miny;y<maxy;y++){
        vec3<float> barycentric;
        if(((barycentric.x=triarea(
          SCAST(float,x), SCAST(float,y),
          SCAST(float,x1),SCAST(float,y1),
          SCAST(float,x2),SCAST(float,y2)
        ))>=0)&&((barycentric.y=triarea(
          SCAST(float,x0), SCAST(float,y0),
          SCAST(float,x),  SCAST(float,y),
          SCAST(float,x2), SCAST(float,y2)
        ))>=0)&&((barycentric.z=triarea(
          SCAST(float,x0), SCAST(float,y0),
          SCAST(float,x1), SCAST(float,y1),
          SCAST(float,x),  SCAST(float,y)
        ))>=0)){
          barycentric=barycentric/area;
          float depth=(barycentric.x*z0+barycentric.y*z1+barycentric.z*z2);
          float d=(depth/farplanex);
          if((depth_buffer[toSSPI(x,y)]) > (d*255)){
            depth_buffer[toSSPI(x,y)]=(unsigned char)(d*255);
            if((0<depth)&&(depth<farplanex)){
              float u=uv0.x*barycentric.x+uv1.x*barycentric.y+uv2.x*barycentric.z;
              float v=uv0.y*barycentric.x+uv1.y*barycentric.y+uv2.y*barycentric.z;
              u*=tex.width; 
              v*=tex.height;
              int iu=(((int)u%tex.width+tex.width)%tex.width);
              int iv=(((int)v%tex.height+tex.height)%tex.height);
              int idx=(iv*tex.width+iu)*3;
              unsigned char r=tex.pixels[idx],g=tex.pixels[idx+1],b=tex.pixels[idx+2];
              char colorIdx = (r>128)|((g>128)<<1)|((b>128)<<2)|(((r+g+b)>(255.0f*3/2))<<3);//don't need to store brightness just calculate it as bool earlier
              char c = charsbyopacity[(int)(d*opacitylength)];
              putChar(x,y,c);
              putColor(x,y,colors::col((colors::color)colorIdx,colors::black));
            }
            if(logmisc){
              // fprintf(debug,"(%u,%u,%f),",x,y,depth);
              // fprintf(debug,"(%u,%u,%u),",x,y,depth_buffer[toSSPI(x,y)]);
            }
          }
        }
      }
    }
  }
  void drawMTri(const meshtri& t, assets::texture_t& tex){
    tri3<mesh_size> t1=t-camera_position;
    rotateT(t1,camera_rotation.z);
    char v=(t1.a.x<1)+(t1.b.x<1)+(t1.c.x<1);
    if(v==3){return;}
    if(v!=0){
      vec3<mesh_size>* clipped=clipTriX(t1,1.0f);//optimize to reuse
      t1.a=clipped[0];
      t1.b=clipped[1];
      t1.c=clipped[2];
      if(clipped[3].x!=0.0f){
        meshtri t2=t;
        t2.a=clipped[2];
        t2.b=clipped[3];
        t2.c=clipped[0];
        drawTri(t2, t.uv0, t.uv1, t.uv2, tex);
        }
      free(clipped);
    }
    drawTri(t1, t.uv0, t.uv1, t.uv2, tex);//merge uvs into tri2<float>
  }
}
#endif