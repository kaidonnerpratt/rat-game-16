#ifndef TYPES_H
#define TYPES_H
#include <cstring>
#include <cstdlib>
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
  enum bshape {TRIANGLE, CUBE};
  enum face {back=0,front,top,bottom,left,right};
  template <typename T> int sgn(T n) {return (T(0) < n) - (n < T(0));}

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
    auto constexpr magnitude()const{return sqrt((x*x)+(y*y)+(z*z));}
    template<typename U> T dot(vec3<U> o){return x*o.x+y*o.y+z*o.z;}
    template<typename U>
    void rotate(vec3<U> o){
      T nx = 0;
      T ny = 0;
      T nz = 0;
      ny = y*cos(o.x)-z*sin(o.x);
      nz = y*sin(o.x)+z*cos(o.x);
      nx = z*sin(o.y)+x*cos(o.y);
      nz = z*cos(o.y)-x*sin(o.y);
      nx = x*cos(o.z)-y*sin(o.z);
      ny = x*sin(o.z)+y*cos(o.z);
      x=nx;      
      y=ny;
      z=nz;
    }
  };
  const vec3<float> normals[6]{
    {-1, 0, 0},
    { 1, 0, 0},
    { 0, 0, 1},
    { 0, 0,-1},
    { 0,-1, 0},
    { 0, 1, 0}
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

  struct cshape_t{
    bshape baseshape;
    bool faces[6];
    vec3<float> pos;
    vec3<float> rot;
    vec3<float> size;
    // vec3<float> va;
    // vec3<float> vb;
    // vec3<float> vc;
    // vec3<float> vd;
    // vec3<float> ve;
    // vec3<float> vf;
    // vec3<float> vg;
    // vec3<float> vh;
    tri3<float> tripoints;
    
    // void makepoints(){
    //   va = {pos.x+size.x,pos.y+size.y,pos.z+size.z};
    //   vb = {pos.x-size.x,pos.y+size.y,pos.z+size.z};
    //   vc = {pos.x+size.x,pos.y+size.y,pos.z-size.z};
    //   vd = {pos.x-size.x,pos.y+size.y,pos.z-size.z};
    //   ve = {pos.x+size.x,pos.y-size.y,pos.z+size.z};
    //   vf = {pos.x-size.x,pos.y-size.y,pos.z+size.z};
    //   vg = {pos.x+size.x,pos.y-size.y,pos.z-size.z};
    //   vh = {pos.x-size.x,pos.y-size.y,pos.z-size.z};
    //   va.rotate(rot);
    //   vb.rotate(rot);
    //   vc.rotate(rot);
    //   vd.rotate(rot);
    //   ve.rotate(rot);
    //   vf.rotate(rot);
    //   vg.rotate(rot);      
    //   vh.rotate(rot);
    
    // }
    bool colliding(vec3<float> p,float r){
      vec3<float> np = {p.x,p.y,p.z};
      np.rotate(rot*-1);
      np = np - pos;
      for(int i = 0; i < 6; i++){
        vec3<float> np2 = np+(normals[i]*r);
        vec3<float> np3 = np-(normals[i]*r);

        if (sgn<float>(np.dot(normals[i]*size)) != 0 && sgn<float>(np2.dot(normals[i]*size)) != 0 
        && sgn<float>(np.dot(normals[i]*size)) != 0 && sgn<float>(np3.dot(normals[i]*size)) != 0){
          if (sgn<float>(np2.dot(normals[i]*size))!=sgn<float>(np.dot(normals[i]*size))){
            printf("dot: %f, %f\n",np2.dot(normals[i]*size),np.dot(normals[i]*size));
            return true;
          }else if (sgn<float>(np3.dot(normals[i]*size))!=sgn<float>(np.dot(normals[i]*size))){
            return true;
          }
        }

        
      }
      return false;



    }
    void settri(tri3<float> t){
      if(baseshape==CUBE)return;
      tripoints = t;
    }
    void setface(face f, bool v){
      if(baseshape==TRIANGLE)return;
      if(f > 5)return;
      if(f < 0)return;
      faces[f] = v;
    }
    cshape_t(bshape s){
      baseshape = s;
      for (int i = 0; i < 6; i++){
        faces[i]==true;
      }
    }


  };
  
}
namespace assets{
  struct asset3d_t{
    mesh::model_t mesh;
    assets::texture_t texture;
  };
  template<typename T> struct collider_t{
    
  };
}
#endif