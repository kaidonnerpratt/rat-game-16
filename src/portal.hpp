#ifndef PORTAL_H
#define PORTAL_H
#include <types.hpp>
#include <r@@2e.hpp>
#include <3rats.hpp>
#include <assets.hpp>
namespace portal{
  class Portal{
  public:
    char *term_buffer = NULL;
    unsigned char *depth_buffer = NULL;
    colors::color_t *color_buffer = NULL;
    mesh::vec3<mesh::mesh_size> portal_cam;
    mesh::meshtri *tris;
    short unsigned int tricount;
    Portal(short unsigned int tc){
      tricount = tc;
      tris = new mesh::meshtri[tricount];
    }
    void set_tri(int i, mesh::meshtri t){
      tris[i] = t;
    }
    void draw(){
      for (int i = 0; i < tricount; i++){
        tris[i];
      }
    }
  };
}

#endif