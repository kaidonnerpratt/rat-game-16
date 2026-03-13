#ifndef ASSETS_H
#define ASSETS_H
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <type_traits>
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#define DO(x) if(x)
#define ABORT if(file){fclose(file);file=NULL;}if(tmp){free(tmp);tmp=NULL;};exit(1);
#define ORDIE(s) {perror(s);ABORT}
#define ORTHENDIE(c,s) {s;perror(s);ABORT}
#define FEXPECTL(EXP_STR,EXP_STR_LEN)\
fgets(tmp,128,file);\
DO(ferror(file))ORDIE("ferror");i=0;\
while(tmp[i]==' ' || tmp[i]=='\n'){i++;};\
debug_bad_stl=memcmp(&tmp[i],EXP_STR,EXP_STR_LEN);\
if(debug_bad_stl){printf("'%.*s'!='%.*s'\n",EXP_STR_LEN,&tmp[i],EXP_STR_LEN,EXP_STR);}\
DO(debug_bad_stl)
#define FEXPECTS(EXP_STR,EXP_STR_LEN)\
fread(tmp,1,EXP_STR_LEN,file);\
DO(memcmp(tmp,EXP_STR,EXP_STR_LEN))
#define FGETI(VAR)\
VAR=0;\
do{tmp[VAR]=fgetc(file);VAR++;}while(('0'<=tmp[VAR-1])&&(tmp[VAR-1]<='9'));\
VAR--;ungetc(tmp[VAR],file);\
tmp[VAR]='\0';VAR=atoi(tmp);
#define WSPACEL(VAR) while((tmp[VAR]==' ')||(tmp[VAR]=='\n')||(tmp[VAR]=='#')){if(tmp[VAR]=='#'){while(tmp[VAR]!='\n'){VAR++;}}VAR++;}
#define NSPACEL(VAR) while((tmp[VAR]!=' ')&&(tmp[VAR]!='\n')){VAR++;}
#define NSPACE_TOKENS ' ','\n','#','='
#define nspace() readUntil(NSPACE_TOKENS)
namespace assets {
  static int debug_bad_stl;
  static char* tmp;
  static FILE* file;
  static int wspace(){//read until not whitespace
    int o=0;
    do{
      tmp[o]=fgetc(file);
      if(tmp[o]=='#'){
        while(tmp[o]!='\n'){o++;tmp[o]=fgetc(file);}
      }
      o++;
    }while((tmp[o-1]==' ')||(tmp[o-1]=='\n'));
    o--;ungetc(tmp[o],file);
    return o;
  }
  template<typename... T> requires (std::is_convertible_v<T,char>&&...)
  static int readUntil(T... c){
    int o=0;
    do{tmp[o]=fgetc(file);o++;}while(o<128&&((tmp[o-1]!=c)&&...));
    o--;ungetc(tmp[o],file);
    return o;
  }
  static std::vector<mesh::meshtri> readSTL(const char* filename) {
    unsigned short int i;
    file=fopen(filename,"r");
    std::vector<mesh::meshtri> tris;
    tmp=(char*)malloc(512);
    DO(!tmp)ORDIE("couldn't alloc name");
    DO(ferror(file))ORDIE("ferror 1");
    FEXPECTL("solid ",5);
    unsigned short int l=strlen(&tmp[i]-5);
    char* name=(char*)malloc(l+1);
    memcpy(name,&tmp[i+5],l);name[l]='\0';
    while(!feof(file)){
      fgets(tmp,128,file);
      DO(ferror(file))ORDIE("ferror 2");
      i=0;
      WSPACEL(i);
      if(!memcmp(&tmp[i],"endsolid",8)){
        FEXPECTL(name,l)ORDIE("couldn't read stl file: bad ending");
        break;
      }
      FEXPECTL("outer loop",10)ORDIE("couldn't read stl file: expected outer loop")
      FEXPECTL("vertex",6)ORDIE("couldn't read stl file: expected vertex");
      i+=6;
      WSPACEL(i);
      float x0=atof(&tmp[i]);
      NSPACEL(i);WSPACEL(i);
      float y0=atof(&tmp[i]);
      NSPACEL(i);WSPACEL(i);
      float z0=atof(&tmp[i]);
      NSPACEL(i);WSPACEL(i);
      FEXPECTL("vertex",6)ORDIE("couldn't read stl file: expected vertex");
      i+=6;
      WSPACEL(i);
      float x1=atof(&tmp[i]);
      NSPACEL(i);WSPACEL(i);
      float y1=atof(&tmp[i]);
      NSPACEL(i);WSPACEL(i);
      float z1=atof(&tmp[i]);
      NSPACEL(i);WSPACEL(i);
      FEXPECTL("vertex",6)ORDIE("couldn't read stl file: expected vertex");
      i+=6;
      WSPACEL(i);
      float x2=atof(&tmp[i]);
      NSPACEL(i);WSPACEL(i);
      float y2=atof(&tmp[i]);
      NSPACEL(i);WSPACEL(i);
      float z2=atof(&tmp[i]);
      NSPACEL(i);WSPACEL(i);
      FEXPECTL("endloop",7)ORDIE("expected endloop");
      FEXPECTL("endfacet",8)ORDIE("expected endfacet");
      tris.push_back((mesh::meshtri){x0,y0,z0,x1,y1,z1,x2,y2,z2});
    }
    free(tmp);
    tmp=NULL;
    fclose(file);
    file=NULL;
    return tris;
  }
  static texture_t readPPM(const char* filename){
    texture_t out;
    file=fopen(filename, "r");
    tmp = (char*)malloc(128);
    DO(!file){memcpy(tmp,"couldn't open file for read: ",30);strcat(tmp,filename);perror(tmp);ABORT};
    int i=0;
    int width, height, maxVal;
    char format=0;
    printf("lading texture %s:",filename);
    FEXPECTS("P3",2){//segfaults i guess
      DO(memcmp(tmp,"P6",2))ORDIE("unsupported file format: not PPM3 or PPM6")else{
        format=2;
        puts("ppm6");
      }
    }else{
      format=1;
      puts("ppm3");
    }
    wspace();FGETI(width);
    wspace();FGETI(height);
    wspace();FGETI(maxVal);
    printf("%ix%i:%i\n",width,height,maxVal);
    out.width=width;out.height=height;out.pixels=(unsigned char*)malloc(3*width*height);
    if(format==1){//ppm3
      unsigned int r=0,g=0,b=0,j=0;
      while(!feof(file)&&(j<width*height)){
        wspace();FGETI(r);
        wspace();FGETI(g);
        wspace();FGETI(b);
        out.pixels[j*3]  =r*255/maxVal;
        out.pixels[j*3+1]=g*255/maxVal;
        out.pixels[j*3+2]=b*255/maxVal;
        j++;
      }
    }else if(format==2){//ppm6
      wspace();
      short int r,g,b,j;
      while(!feof(file)&&(j<width*height)){
        fread(&r,1+(maxVal>255),1,file);
        fread(&g,1+(maxVal>255),1,file);
        fread(&b,1+(maxVal>255),1,file);
        out.pixels[j*3]  =r*255/maxVal;
        out.pixels[j*3+1]=g*255/maxVal;
        out.pixels[j*3+2]=b*255/maxVal;
        j++;
      }
    }
    free(tmp);
    tmp=NULL;
    fclose(file);
    file=NULL;
    return out;
  }
  assets::asset3d_t readAsset3d(const char* name) {
    DO(strlen(name)>=128)ORDIE("filename to long")
    printf("loading asset %s:\n");
    asset3d_t out{0};
    file=fopen(name,"r");
    DO(!file)ORDIE("couldn't open asset file for read :(")
    tmp=(char*)malloc(128);
    DO(!tmp)ORDIE("couldn't alloc memory for tmp buffer")
    char* mesh_fp=(char*)malloc(128);
    DO(!mesh_fp)ORDIE("couldn't alloc memory for mesh file path for asset")
    char* text_fp=(char*)malloc(128);
    DO(!text_fp)ORTHENDIE(free(mesh_fp),"couldn't alloc memory for texture file path for asset")
    mesh::vec2<float>* tex_binds;
    unsigned int token_length=nspace();
    if((token_length==5)&&!(memcmp(tmp,"model",5))){
      DO(mesh_fp[0])ORTHENDIE(free(mesh_fp);free(text_fp),"duplicate models in asset")
      wspace();FEXPECTS("=",1);wspace();
      token_length=readUntil(';');
      DO(token_length)ORTHENDIE(free(mesh_fp);free(text_fp),"bad model filepath in asset")
      memcpy(mesh_fp,tmp,token_length);
      mesh_fp[token_length]='\0';
      std::vector<mesh::meshtri> tris=readSTL(mesh_fp);//go out of scope and free yourself NOW
      out.mesh.tricount=tris.size();
      size_t s=out.mesh.tricount*sizeof(mesh::meshtri);
      out.mesh.tris=(mesh::meshtri*)malloc(s);
      memcpy(out.mesh.tris,&tris[0],s);
    }else if((token_length==7)&&!(memcmp(tmp,"texture",7))){
      //do this one later
    }
    free(tmp);tmp=NULL;
    free(mesh_fp);free(text_fp);
    fclose(file);file=NULL;
    return out;
  }
  void writeGrayScaleToPPM(const char* name,const unsigned char* buf,size_t width,size_t height){
    file=fopen(name,"w");
    fprintf(file,"P3 %lu %lu 255\n",width,height*3);
    for(int i=0;i<height;i++){
      for(int j=0;j<width;j++){
        fprintf(file,"%3u %3u %3u ",buf[i*width+j],buf[i*width+j],buf[i*width+j]);
      }
      fputs("\n",file);
      for(int j=0;j<width;j++){
        fprintf(file,"%3u %3u %3u ",buf[i*width+j],buf[i*width+j],buf[i*width+j]);
      }
      fputs("\n",file);
      for(int j=0;j<width;j++){
        fprintf(file,"%3u %3u %3u ",buf[i*width+j],buf[i*width+j],buf[i*width+j]);
      }
      fputs("\n",file);
    }
    fclose(file);
  }
}
#undef FEXPECTL
#undef NOFERR
#undef WSPACEL
#undef NSPACEL
#undef DO
#undef ORDIE
#pragma GCC diagnostic warning "-Wint-to-pointer-cast"
#endif