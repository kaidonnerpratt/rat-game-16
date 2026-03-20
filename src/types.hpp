#ifndef TYPES_H
#define TYPES_H
#include <cstring>
#include <cstdlib>
#include <cmath>
namespace colors {
  enum color:char{//3 bit color 1 bit intensity
    black=0,red,green,brown,blue,purple,cyan,white,
    gray,bright_red,bright_green,yellow,bright_blue,bright_purple,bright_cyan,bright_white
  };
  typedef unsigned char color_t;//4 bits for fg and bg. bg lshift 4
}
namespace gui {
  typedef unsigned short int scoord;//coordinate on the screen, in characters
  typedef unsigned char sfrac;//represents the fraction of width/this
}
namespace assets {
  struct texture_t {
      unsigned char* pixels;
      int width, height;
      texture_t() = default;
      texture_t(const texture_t& o) {
          width=o.width; height=o.height;
          pixels=(unsigned char*)malloc(width*height*3);
          memcpy(pixels,o.pixels,width*height*3);
      }
      // ~texture_t() noexcept {free(pixels);}
  };
}

namespace mesh {
  typedef float mesh_size;
  template<typename T> requires arith<T>&&comp<T> struct vec2 {
    T x,y;//these operators are fuckin wild
    template<typename U> auto constexpr operator+(const vec2<U>& v)const{return (vec2<decltype(std::declval<T>()+std::declval<U>())>){x+v.x,y+v.y};}
    template<typename U> auto constexpr operator+(const U& v)const{return (vec2<decltype(std::declval<T>()+std::declval<U>())>){x+v,y+v};}
    template<typename U> auto constexpr operator-(const vec2<T>& v)const{return (vec2<decltype(std::declval<T>()-std::declval<U>())>){x-v.x,y-v.y};}
    template<typename U> auto constexpr operator-(const U& v)const{return (vec2<decltype(std::declval<T>()-std::declval<U>())>){x-v,y-v};}
    template<typename U> auto constexpr operator*(const vec2<U>& v)const{return (vec2<decltype(std::declval<T>()*std::declval<U>())>){x*v.x,y*v.y};}
    template<typename U> auto constexpr operator*(const U& v)const{return (vec2<decltype(std::declval<T>()*std::declval<U>())>){x*v,y*v};}
    template<typename U> auto constexpr operator/(const vec2<U>& v)const{return (vec2<decltype(std::declval<T>()/std::declval<U>())>){x/v.x,y/v.y};}
    template<typename U> auto constexpr operator/(const U& v)const{return (vec2<decltype(std::declval<T>()/std::declval<U>())>){x/v,y/v};}
  };//all of these should implement https://cplusplus.com/reference/type_traits/is_nothrow_move_constructible/
  template<typename T> requires arith<T>&&comp<T> struct vec3 {
    T x,y,z;
    template<typename U> auto constexpr operator+(const vec3<U>& v)const{return (vec3<decltype(std::declval<T>()+std::declval<U>())>){x+v.x,y+v.y,z+v.z};}
    template<typename U> auto constexpr operator+(const U& v)const{return (vec3<decltype(std::declval<T>()+std::declval<U>())>){x+v,y+v,z+v};}
    template<typename U> auto constexpr operator-(const vec3<U>& v)const{return (vec3<decltype(std::declval<T>()-std::declval<U>())>){x-v.x,y-v.y,z-v.z};}
    template<typename U> auto constexpr operator-(const U& v)const{return (vec3<decltype(std::declval<T>()-std::declval<U>())>){x-v,y-v,z-v};}
    template<typename U> auto constexpr operator*(const vec3<U>& v)const{return (vec3<decltype(std::declval<T>()*std::declval<U>())>){x*v.x,y*v.y,z*v.z};}
    template<typename U> auto constexpr operator*(const U& v)const{return (vec3<decltype(std::declval<T>()*std::declval<U>())>){x*v,y*v,z*v};}
    template<typename U> auto constexpr operator/(const vec3<U>& v)const{return (vec3<decltype(std::declval<T>()/std::declval<U>())>){x/v.x,y/v.y,z/v.z};}
    template<typename U> auto constexpr operator/(const U& v)const{return (vec3<decltype(std::declval<T>()/std::declval<U>())>){x/v,y/v,z/v};}
    template<typename U> auto constexpr cross(const vec3<U>& v)const{return (vec3<decltype(std::declval<T>()*std::declval<U>()-std::declval<T>()*std::declval<U>())>){y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x};}
    template<typename U>
    T dist(vec3<U> o){
      return std::sqrt(std::pow(x-o.x,2)+std::pow(y-o.y,2)+std::pow(z-o.z,2));
    }
    T sdist(){
      return std::sqrt(std::pow(x,2)+std::pow(y,2)+std::pow(z,2));
    }
    template<typename U>
    vec3<T> rotate(vec3<U> o){
      vec3<T> n = vec3<T>{x,y,z};
      T d = sdist();
      float x_rot = atan2(n.y,n.z);//+o.x;
      n.y = sin(x_rot);
      n.z = cos(x_rot);
      float y_rot = atan2(n.x,n.z);//+o.y;
      n.x = sin(y_rot);
      n.z = cos(y_rot);
      float z_rot = atan2(n.y,n.x);//+o.z;
      n.y = sin(z_rot);
      n.x = cos(z_rot);
      n=n*d;
      return n;
    }

  };
  template<typename T> struct vec_inner;//partial template specialization
  template<typename T> struct vec_inner<vec2<T>>{using type=T;};
  template<typename T> struct vec_inner<vec3<T>>{using type=T;};
  template<typename T> using  vec_inner_t=typename vec_inner<T>::type;
  template<typename T> requires arith<T>&&comp<T> struct tri2 {
    vec2<T> a,b,c;
    template<typename U> auto constexpr operator+(const tri2<U>& t)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()+std::declval<vec2<U>>())>>){a+t.a,b+t.b,c+t.c};}
    template<typename U> auto constexpr operator+(const vec2<U>& v)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()+std::declval<vec2<U>>())>>){a+v,b+v,c+v};}
    template<typename U> auto constexpr operator+(const U& v)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()+std::declval<U>())>>){a+v,b+v,c+v};}
    template<typename U> auto constexpr operator-(const tri2<U>& t)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()-std::declval<vec2<U>>())>>){a-t.a,b-t.b,c-t.c};}
    template<typename U> auto constexpr operator-(const vec2<U>& v)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()-std::declval<vec2<U>>())>>){a-v,b-v,c-v};}
    template<typename U> auto constexpr operator-(const U& v)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()-std::declval<U>())>>){a-v,b-v,c-v};}
    template<typename U> auto constexpr operator*(const tri2<U>& t)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()*std::declval<vec2<U>>())>>){a*t.a,b*t.b,c*t.c};}
    template<typename U> auto constexpr operator*(const vec2<U>& v)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()*std::declval<vec2<U>>())>>){a*v,b*v,c*v};}
    template<typename U> auto constexpr operator*(const U& v)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()*std::declval<U>())>>){a*v,b*v,c*v};}
    template<typename U> auto constexpr operator/(const tri2<U>& t)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()/std::declval<vec2<U>>())>>){a/t.a,b/t.b,c/t.c};}
    template<typename U> auto constexpr operator/(const vec2<U>& v)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()/std::declval<vec2<U>>())>>){a/v,b/v,c/v};}
    template<typename U> auto constexpr operator/(const U& v)const{return (tri2<vec_inner_t<decltype(std::declval<vec2<T>>()/std::declval<U>())>>){a/v,b/v,c/v};}
    
  };
  template<typename T> requires arith<T>&&comp<T> struct tri3 {
    vec3<T> a,b,c;//my compiler is going to blow its brains out
    template<typename U> auto constexpr operator+(const tri3<U>& t)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()+std::declval<vec3<U>>())>>){a+t.a,b+t.b,c+t.c};}
    template<typename U> auto constexpr operator+(const vec3<U>& v)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()+std::declval<vec3<U>>())>>){a+v,b+v,c+v};}
    template<typename U> auto constexpr operator+(const U& v)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()+std::declval<U>())>>){a+v,b+v,c+v};}
    template<typename U> auto constexpr operator-(const tri3<U>& t)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()-std::declval<vec3<U>>())>>){a-t.a,b-t.b,c-t.c};}
    template<typename U> auto constexpr operator-(const vec3<U>& v)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()-std::declval<vec3<U>>())>>){a-v,b-v,c-v};}
    template<typename U> auto constexpr operator-(const U& v)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()-std::declval<U>())>>){a-v,b-v,c-v};}
    template<typename U> auto constexpr operator*(const tri3<U>& t)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()*std::declval<vec3<U>>())>>){a*t.a,b*t.b,c*t.c};}
    template<typename U> auto constexpr operator*(const vec3<U>& v)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()*std::declval<vec3<U>>())>>){a*v,b*v,c*v};}
    template<typename U> auto constexpr operator*(const U& v)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()*std::declval<U>())>>){a*v,b*v,c*v};}
    template<typename U> auto constexpr operator/(const tri3<U>& t)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()/std::declval<vec3<U>>())>>){a/t.a,b/t.b,c/t.c};}
    template<typename U> auto constexpr operator/(const vec3<U>& v)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()/std::declval<vec3<U>>())>>){a/v,b/v,c/v};}
    template<typename U> auto constexpr operator/(const U& v)const{return (tri3<vec_inner_t<decltype(std::declval<vec3<T>>()/std::declval<U>())>>){a/v,b/v,c/v};}
    
  };
  struct meshtri:tri3<mesh_size>{
    // unsigned char flags=255;
    vec2<float> uv0, uv1, uv2;
  };
  struct model_t {
    short unsigned int tricount;
    meshtri* tris;
   ~model_t() noexcept {free(tris);}
  };
  class Plane{
    public:
      meshtri e;
      meshtri f;
      vec3<mesh_size> a;
      vec3<mesh_size> b;
      vec3<mesh_size> c;
      vec3<mesh_size> d;
      vec3<float> pos;
      vec3<float> rot;
      vec2<float> scl;
      void apply_rotation(){
        a.rotate(rot);
        b.rotate(rot);
        c.rotate(rot);
        d.rotate(rot);
      }
      void create_tris(){
        e.a = b; f.a = c;
        e.b = c; f.b = b;
        e.c = d; f.c = a;
      }
      void reconstruct(){
        a.y=+scl.y/2;
        a.z=-scl.x/2;
        b.y=+scl.y/2;  
        b.z=+scl.x/2;  
        c.y=-scl.y/2;  
        c.z=-scl.x/2; 
        d.y=-scl.y/2;  
        d.z=+scl.x/2;
        apply_rotation();
        a.x=a.x+pos.x;
        a.y=a.y+pos.y;
        a.z=a.z+pos.z;
        b.x=b.x+pos.x;
        b.y=b.y+pos.y;
        b.z=b.z+pos.z;
        c.x=c.x+pos.x;
        c.y=c.y+pos.y;
        c.z=c.z+pos.z;
        d.x=d.x+pos.x;
        d.y=d.y+pos.y;
        d.z=d.z+pos.z;
        create_tris();
      }
      Plane(float x,float y,float z, float rx, float ry, float rz, float sx, float sy){
        pos=vec3<float>{x,y,z};
        rot=vec3<float>{rx,ry,rz};
        scl=vec2<float>{sy,sx};
        reconstruct();
      }
      Plane(vec3<float> planepos, vec3<float> planerot, vec2<float> planescl){
        pos=planepos;
        rot=planerot;
        scl=planescl;
        reconstruct();
      }

  };
}
namespace assets{
  struct asset3d_t{
    mesh::model_t mesh;
    assets::texture_t texture;
   
  };
}
#endif