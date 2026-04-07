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
#define ORTHENDIE(c,s) {c;perror(s);ABORT}
#define FEXPECTL(EXP_STR,EXP_STR_LEN)\
fgets(tmp,128,file);\
DO(ferror(file))ORDIE("ferror");i=0;\
while(tmp[i]==' ' || tmp[i]=='\n'){i++;};\
debug_bad_stl=memcmp(&tmp[i],EXP_STR,EXP_STR_LEN);\
if(debug_bad_stl){printf("'%.*s'!='%.*s'\n",EXP_STR_LEN,&tmp[i],EXP_STR_LEN,EXP_STR);}\
DO(debug_bad_stl)
#define FEXPECTS(EXP_STR,EXP_STR_LEN)\
DO(fread(tmp,1,EXP_STR_LEN,file)<1){perror("expected to be able to read more");ABORT}\
DO(memcmp(tmp,EXP_STR,EXP_STR_LEN))
#define FGETI(VAR)\
VAR=0;\
do{tmp[VAR]=fgetc(file);VAR++;}while(('0'<=tmp[VAR-1])&&(tmp[VAR-1]<='9'));\
VAR--;ungetc(tmp[VAR],file);\
tmp[VAR]='\0';VAR=atoi(tmp);
#define WSPACEL(VAR) while((tmp[VAR]==' ')||(tmp[VAR]=='\n')||(tmp[VAR]=='#')){if(tmp[VAR]=='#'){while(tmp[VAR]!='\n'){VAR++;}}VAR++;}
#define NSPACEL(VAR) while((tmp[VAR]!=' ')&&(tmp[VAR]!='\n')){VAR++;}
#define NSPACE_TOKENS ' ','\n','#','=',',',')','('
#define nspace(F,B) readUntil(F,B,NSPACE_TOKENS)
namespace assets {
  static int debug_bad_stl;
  static int wspace(FILE* file,char* tmp){//read until not whitespace
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
  static int readUntil(FILE* file,char* tmp,T... c){
    int o=0;
    do{
      tmp[o]=fgetc(file);
      o++;
    }while(o<128&&((tmp[o-1]!=c)&&...));
    o--;ungetc(tmp[o],file);
    return o;
  }
  static std::vector<mesh::meshtri> readSTL(const char* filename) {
    unsigned int i;
    FILE* file=fopen(filename,"r");
    char* tmp=(char*)malloc(128);
    DO(!tmp)ORDIE("couldn't alloc temporary space")
    DO(!file)ORDIE("couldn't open file")
    DO(ferror(file))ORDIE("ferror 1");
    std::vector<mesh::meshtri> tris;
    FEXPECTL("solid ",6){
      fseek(file,0,SEEK_SET);
      unsigned int trinum;
      float data[12];//first 3 are normal
      fread(tmp,80,1,file);
      fread(&trinum,4,1,file);
      for(i=0;(i<trinum)&&!feof(file);i++){
        fread(data,12,4,file);
        fseek(file,2,SEEK_CUR);//extra flags we don't really care for
        mesh::meshtri t{
          data[ 3],data[ 4],data[ 5],
          data[ 6],data[ 7],data[ 8],
          data[ 9],data[10],data[11]};
        float area=(t.a-t.b).cross(t.a-t.c).magnitude();
        if(area>0.1){
          tris.push_back(t);
        }else{
          printf("warning: tiny triangle at index %i,area=%f\n",i,area);
        }
      }
    }else{//ascii stl
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
        mesh::meshtri t{x0,y0,z0,x1,y1,z1,x2,y2,z2};
        float area=(t.a-t.b).cross(t.a-t.c).magnitude();
        if(area>0.1){
          tris.push_back(t);
        }else{
          printf("warning: tiny triangle at index %i,area=%f\n",i,area);
        }
      }
    }
    free(tmp);
    tmp=NULL;
    fclose(file);
    file=NULL;
    printf("%li triangles\n",tris.size());
    return tris;
  }
  static texture_t readPPM(const char* filename){
    texture_t out;
    FILE* file=fopen(filename, "r");
    char* tmp = (char*)malloc(128);
    DO(!file){memcpy(tmp,"couldn't open file for read: ",30);strcat(tmp,filename);perror(tmp);ABORT};
    int i=0;
    int width, height, maxVal;
    char format=0;
    FEXPECTS("P3",2){//segfaults i guess
      DO(memcmp(tmp,"P6",2))ORDIE("unsupported file format: not PPM3 or PPM6")else{
        format=2;
      }
    }else{
      format=1;
    }
    wspace(file,tmp);FGETI(width);
    wspace(file,tmp);FGETI(height);
    wspace(file,tmp);FGETI(maxVal);
    printf("%ix%i,%i\n",width,height,maxVal);
    out.width=width;out.height=height;out.pixels=(unsigned char*)malloc(3*width*height);
    if(format==1){//ppm3
      unsigned int r=0,g=0,b=0,j=0;
      while(!feof(file)&&(j<width*height)){
        wspace(file,tmp);FGETI(r);
        wspace(file,tmp);FGETI(g);
        wspace(file,tmp);FGETI(b);
        out.pixels[j*3]  =r*255/maxVal;
        out.pixels[j*3+1]=g*255/maxVal;
        out.pixels[j*3+2]=b*255/maxVal;
        j++;
      }
    }else if(format==2){//ppm6
      wspace(file,tmp);
      short unsigned int r,g,b;
      unsigned j;
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
  mesh::vec2<float> readV2f(FILE* file,char* tmp){
    mesh::vec2<float> out;
    FEXPECTS("(",1)ORDIE("expected '(' to start vector");wspace(file,tmp);
    tmp[nspace(file,tmp)]='\0';
    out.x=atof(tmp);
    FEXPECTS(",",1)ORDIE("expected ',' to separate vector coordinates");wspace(file,tmp);
    tmp[nspace(file,tmp)]='\0';
    out.y=atof(tmp);
    wspace(file,tmp);FEXPECTS(")",1)ORDIE("expected ')' to end vector");
    return out;
  }
  std::vector<mesh::vec2<float>> readV2FVec(FILE* file,char* tmp){
    std::vector<mesh::vec2<float>> out={};
    wspace(file,tmp);
    FEXPECTS("[",1)ORDIE("expected '[' to start array");
    while(!feof(file)){
      wspace(file,tmp);
      out.push_back(readV2f(file,tmp));
      char c=fgetc(file);
      if(c!=','){
        ungetc(c,file);break;
      }
    }
    wspace(file,tmp);
    FEXPECTS("]",1)ORDIE("expected ']' to end array");
    return out;
  }
  // void readCollisonMap3d(const char* name){
  //   bool writeingObjects = false;
  //   DO(strlen(name)>=128){perror("file name too long");exit(1);}
  //   printf("loading collision map %s:\n",name);
  //   FILE* file=fopen(name,"r");
  //   char* tmp=(char*)malloc(128);
  //   DO(!file)ORDIE("couldn't open collision map file for read :(")
  //   DO(!tmp)ORDIE("couldn't alloc memory for tmp buffer")
  //   wspace(file,tmp);
  //   FEXPECTS("VERSION",7)ORDIE("expected version decleration first")
  //   int CUR_VERSION_MAJOR;
  //   int CUR_VERSION_MINOR;
  //   wspace(file,tmp);
  //   FEXPECTS("(",1)ORDIE("version decleration formatted incorrectly")
  //   wspace(file,tmp);
  //   tmp[nspace(file,tmp)]='\0';
  //   CUR_VERSION_MAJOR=atoi(tmp);
  //   wspace(file,tmp);
  //   FEXPECTS(",",1)ORDIE("expected ',' to separate VERSION parameters")
  //   wspace(file,tmp);
  //   tmp[nspace(file,tmp)]='\0';
  //   CUR_VERSION_MINOR=atoi(tmp);
  //   wspace(file,tmp);
  //   FEXPECTS(")",1)ORDIE("expected ')' to end version decleration")
  //   printf("version : %i.%i\n", CUR_VERSION_MAJOR, CUR_VERSION_MINOR);
  //   readUntil(file,tmp,';');
  //   bool use_stls = false;

  //   while(!feof(file)){
  //     wspace(file,tmp);
  //     unsigned int token_length=nspace(file,tmp);
  //     DO(!token_length)ORDIE("bad tokens in collision map")
  //     DO((token_length==8)&&!(memcmp(tmp,"USE_STLS",8))){
  //       DO(writeingObjects)ORDIE("file information found inside data")
  //       readUntil(file,tmp,'=');
  //       FEXPECTS("=",1)ORDIE("expected '=' to assign USE_STLS value")
  //       wspace(file,tmp);
  //       token_length=readUntil(file,tmp,';');
  //       DO(!token_length)ORDIE("bad value in USE_STLS")
  //       DO((15+token_length+1)>=128)ORDIE("value too long")
  //       tmp[token_length]='\0';
  //       use_stls = !memcmp(tmp, "true", 4);
  //       DO(!use_stls)DO(memcmp(tmp, "false", 5))ORDIE("expected bool for USE_STLS ")
  //     }else if ((token_length==12)&&!(memcmp(tmp,"LOADFROM_STL",12))){
  //       ORDIE("LOADFROM_STL currently in development... come back later")
  //     }else if ((token_length>=2)&&!(memcmp(tmp,"--",2))){
  //       writeingObjects=true;
  //       char* shapename = (char*)malloc(token_length-2);
  //       shapename[token_length-2]='\0';
  //       memcpy(shapename,tmp+2,token_length-2);
  //       switch (shapename){
  //         case("CUBE"):

  //         break;
  //         case("HOTDOG"):
  //           printf("current buffer : '%s'\n",shapename);  

  //         break;
  //         case("TRIANGLE"):

  //         break;
  //         case("ORANGE"):

  //         break;
  //       }
  //       // memcpy(tmp,"--",2)
  //       // FEXPECTS("--",2)ORDIE("expected '--' to assign object shape")
  //       // tmp[2]='';
  //       free(shapename);
  //       shapename=NULL;
  //     }
  //   }

  //   free(tmp);tmp=NULL;
  //   fclose(file);file=NULL;
  // }
  assets::asset3d_t readAsset3d(const char* name) {
#define ORDIE1(S) {if(mesh_fp){free(mesh_fp);mesh_fp=NULL;}if(text_fp){free(text_fp);text_fp=NULL;}if(out.mesh.tris){free(out.mesh.tris);out.mesh.tris=NULL;}perror(S);if(file){fclose(file);file=NULL;}if(tmp){free(tmp);tmp=NULL;};exit(1);}
    DO(strlen(name)>=128){perror("file name too long");exit(1);}
    printf("loading asset %s:\n", name);
    asset3d_t out{};
    FILE* file=fopen(name,"r");
    char* tmp=(char*)malloc(128);
    DO(!file)ORDIE("couldn't open asset file for read :(")
    DO(!tmp)ORDIE("couldn't alloc memory for tmp buffer")
    char* mesh_fp=(char*)calloc(128,1);
    DO(!mesh_fp)ORDIE("couldn't alloc memory for mesh file path for asset")
    char* text_fp=(char*)calloc(128,1);
    DO(!text_fp)ORTHENDIE(free(mesh_fp),"couldn't alloc memory for texture file path for asset")
    mesh::vec2<float>* tex_binds;
    unsigned short int uvassignedtris=0;
    while(!feof(file)){
      wspace(file,tmp);
      unsigned int token_length=nspace(file,tmp);
      DO(!token_length)ORDIE1("bad tokens in asset")
      if((token_length==5)&&!(memcmp(tmp,"model",5))){
        DO(mesh_fp[0])ORDIE1("duplicate models in asset")
        wspace(file,tmp);FEXPECTS("=",1)ORDIE("expected '=' to assign model path");wspace(file,tmp);
        token_length=readUntil(file,tmp,';');
        printf("reading model file assets/model/%.*s:",token_length,tmp);
        DO(!token_length)ORDIE1("bad model filepath in asset")
        DO((13+token_length+1)>=128)ORDIE1("model filepath too long")
        tmp[token_length]='\0';
        memcpy(mesh_fp,"assets/model/",13);
        memcpy(&mesh_fp[13],tmp,token_length+1);
        std::vector<mesh::meshtri> tris=readSTL(mesh_fp);//go out of scope and free yourself NOW
        out.mesh.tricount=tris.size();
        size_t s=out.mesh.tricount*sizeof(mesh::meshtri);
        out.mesh.tris=(mesh::meshtri*)calloc(s,1);
        memcpy(out.mesh.tris,&tris[0],s);
      }else if((token_length==7)&&!(memcmp(tmp,"texture",7))){
        DO(text_fp[0])ORDIE1("duplicate textures in asset")
        wspace(file,tmp);FEXPECTS("=",1)ORDIE("expected '=' to assign texture path");wspace(file,tmp);
        token_length=readUntil(file,tmp,';');
        DO(!token_length)ORDIE1("bad texture filepath in asset")
        DO((15+token_length+1)>=128)ORDIE1("texture filepath too long")
        tmp[token_length]='\0';
        memcpy(text_fp,"assets/texture/",15);
        memcpy(&text_fp[15],tmp,token_length+1);
        printf("reading texture file: %s, ",text_fp);
        out.texture=readPPM(text_fp);
      }else if((token_length==6)&&!(memcmp(tmp,"texmap",6))){
        DO(!mesh_fp[0])ORDIE1("expected model before texmap")
        wspace(file,tmp);FEXPECTS("=",1)ORDIE1("expected '=' to assign texmap");wspace(file,tmp);
        std::vector<mesh::vec2<float>> v2fv=readV2FVec(file,tmp);
        uvassignedtris=v2fv.size()/3;
        DO(uvassignedtris>out.mesh.tricount)ORDIE1("too many uv coordinates")
        for(short unsigned int i=0;i<uvassignedtris;i++){
          out.mesh.tris[i].uv0=v2fv[i*3];
          out.mesh.tris[i].uv1=v2fv[i*3+1];
          out.mesh.tris[i].uv2=v2fv[i*3+2];
        }
      }else if((token_length==18)&&!memcmp(tmp,"texmap_filldefault",18)){
        FEXPECTS("(",1)ORDIE1("expected '(' to start texmap_filldefault parameters")
        wspace(file,tmp);
        switch(fgetc(file)){
          case 'x':
            for(unsigned int i=uvassignedtris;i<out.mesh.tricount;i++){
              out.mesh.tris[i].uv0.x=out.mesh.tris[i].a.x;
              out.mesh.tris[i].uv1.x=out.mesh.tris[i].b.x;
              out.mesh.tris[i].uv2.x=out.mesh.tris[i].c.x;
            }break;
          case 'y':
            for(unsigned int i=uvassignedtris;i<out.mesh.tricount;i++){
              out.mesh.tris[i].uv0.x=out.mesh.tris[i].a.y;
              out.mesh.tris[i].uv1.x=out.mesh.tris[i].b.y;
              out.mesh.tris[i].uv2.x=out.mesh.tris[i].c.y;
            }break;
          case 'z':
            for(unsigned int i=uvassignedtris;i<out.mesh.tricount;i++){
              out.mesh.tris[i].uv0.x=out.mesh.tris[i].a.z;
              out.mesh.tris[i].uv1.x=out.mesh.tris[i].b.z;
              out.mesh.tris[i].uv2.x=out.mesh.tris[i].c.z;
            }break;
          default:
            DO(1)ORDIE1("unrecognized texmap_filldefault parameter")
        }
        wspace(file,tmp);FEXPECTS(",",1)ORDIE1("expected ',' to separate texmap_filldefault parameters")
        switch(fgetc(file)){
          case 'x':
            for(unsigned int i=uvassignedtris;i<out.mesh.tricount;i++){
              out.mesh.tris[i].uv0.y=out.mesh.tris[i].a.x;
              out.mesh.tris[i].uv1.y=out.mesh.tris[i].b.x;
              out.mesh.tris[i].uv2.y=out.mesh.tris[i].c.x;
            }break;
          case 'y':
            for(unsigned int i=uvassignedtris;i<out.mesh.tricount;i++){
              out.mesh.tris[i].uv0.y=out.mesh.tris[i].a.y;
              out.mesh.tris[i].uv1.y=out.mesh.tris[i].b.y;
              out.mesh.tris[i].uv2.y=out.mesh.tris[i].c.y;
            }break;
          case 'z':
            for(unsigned int i=uvassignedtris;i<out.mesh.tricount;i++){
              out.mesh.tris[i].uv0.y=out.mesh.tris[i].a.z;
              out.mesh.tris[i].uv1.y=out.mesh.tris[i].b.z;
              out.mesh.tris[i].uv2.y=out.mesh.tris[i].c.z;
            }break;
          default:
            DO(1)ORDIE1("unrecognized texmap_filldefault parameter")
        }
        wspace(file,tmp);FEXPECTS(")",1)ORDIE1("expected ')' to end texmap_filldefault")
        uvassignedtris=out.mesh.tricount;
      }else if((token_length==13)&&!memcmp(tmp,"texmap_offset",13)){
        FEXPECTS("(",1)ORDIE1("expected '(' to start texmap_offset parameters")
        tmp[nspace(file,tmp)]='\0';
        float x=atof(tmp);
        FEXPECTS(",",1)ORDIE1("expected ',' to separate texmap_offset parameters")
        tmp[nspace(file,tmp)]='\0';
        float y=atof(tmp);
        FEXPECTS(")",1)ORDIE1("expected ')' to end texmap_offset parameters")
        for(unsigned int i=0;i<uvassignedtris;i++){
          out.mesh.tris[i].uv0.x+=x;
          out.mesh.tris[i].uv1.x+=x;
          out.mesh.tris[i].uv2.x+=x;
          out.mesh.tris[i].uv0.y+=y;
          out.mesh.tris[i].uv1.y+=y;
          out.mesh.tris[i].uv2.y+=y;
        }
      }else if((token_length==12)&&!memcmp(tmp,"texmap_scale",12)){
        FEXPECTS("(",1)ORDIE1("expected '(' to start texmap_scale parameters")
        tmp[nspace(file,tmp)]='\0';
        float x=atof(tmp);
        FEXPECTS(",",1)ORDIE1("expected ',' to separate texmap_scale parameters")
        tmp[nspace(file,tmp)]='\0';
        float y=atof(tmp);
        FEXPECTS(")",1)ORDIE1("expected ')' to end texmap_scale parameters")
        for(unsigned int i=0;i<uvassignedtris;i++){
          out.mesh.tris[i].uv0.x*=x;
          out.mesh.tris[i].uv1.x*=x;
          out.mesh.tris[i].uv2.x*=x;
          out.mesh.tris[i].uv0.y*=y;
          out.mesh.tris[i].uv1.y*=y;
          out.mesh.tris[i].uv2.y*=y;
        }
      }
    }
    DO(uvassignedtris!=out.mesh.tricount)ORDIE1("didn't assign enough uv coordinates")
    free(tmp);tmp=NULL;
    free(mesh_fp);free(text_fp);
    fclose(file);file=NULL;
    return out;
#undef ORDIE1
  }
  void writeGrayScaleToPPM(const char* name,const unsigned char* buf,size_t width,size_t height){
    FILE* file=fopen(name,"w");
    fprintf(file,"P3 %lu %lu 255\n",width,height*3);
    for(unsigned int i=0;i<height;i++){
      for(unsigned int j=0;j<width;j++){
        fprintf(file,"%3u %3u %3u ",buf[i*width+j],buf[i*width+j],buf[i*width+j]);
      }
      fputs("\n",file);
      for(unsigned int j=0;j<width;j++){
        fprintf(file,"%3u %3u %3u ",buf[i*width+j],buf[i*width+j],buf[i*width+j]);
      }
      fputs("\n",file);
      for(unsigned int j=0;j<width;j++){
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