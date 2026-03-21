#ifndef PORTAL_H
#define PORTAL_H
#include <types.hpp>
#include <r@@2e.hpp>
#include <3rats.hpp>
#include <assets.hpp>
#include <stdio.h>
#include <stdlib.h>
namespace portal{
  class Portal{
  public:
    mesh::vec3<mesh::mesh_size> portal_cam {0,0,0};
    mesh::meshtri* tris;
    mesh::vec3<mesh::mesh_size> pos;
    mesh::vec3<mesh::mesh_size> out_pos;
    short unsigned int tricount;
    char key;
    Portal(mesh::vec3<mesh::mesh_size> p,  mesh::vec3<mesh::mesh_size> op, short unsigned int tc, char k){
      tricount = tc;
      tris = new mesh::meshtri[tricount];
      pos = p;
      out_pos = op;
      key=k;
    }
    void set_tri(int i, mesh::meshtri t){
      tris[i] = t;
    }
    void draw(){
      for (int i = 0; i < tricount; i++){
        tris[i];
      }
    }
    void update_cam(mesh::vec3<mesh::mesh_size> cam){
      portal_cam = out_pos + (cam-pos);
    }
    void pop_buffers(){
      for(gui::scoord i=0;i<gui::max_chars;i++){
        if(gui::term_buffer[i]!=key){
          gui::depth_buffer[i]=0;
        }else{
          gui::depth_buffer[i]=255;
        }
      }
    }
    void cleanup_view(){
      for(gui::scoord i=0;i<gui::max_chars;i++){
        if(gui::term_buffer[i]==key){
          gui::depth_buffer[i]=0;
          gui::term_buffer[i] = ' ';
        }


      }
    }
  };
}

#endif