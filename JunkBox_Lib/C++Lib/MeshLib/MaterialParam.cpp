
#include "MaterialParam.h"


using namespace jbxl;


///////////////////////////////////////////////////////////////////////////////////////////////////
// TextureParam クラス
//

void  TextureParam::init(void)
{
    name = init_Buffer();

    for (int i=0; i<4; i++) color[i] = 1.0;

    alphaChannel = false;
    alphaMode    = MATERIAL_ALPHA_NONE;
    alphaCutoff  = 0.0;

    shiftU       = 0.0;
    shiftV       = 0.0;
    scaleU       = 1.0;
    scaleV       = 1.0;
    rotate       = 0.0;

    flipU        = false;
    flipV        = false;

    return;
}



void  TextureParam::free (void)
{
    free_Buffer(&name);

    return;
}



void  TextureParam::dup(TextureParam t)
{
    *this = t;
    
    name = dup_Buffer(t.name);
}



void  TextureParam::execShift(UVMap<double>* uv, int num)
{
    for (int i=0; i<num; i++) {
        uv[i].u += shiftU;
        uv[i].v += shiftV;
    }
}



void  TextureParam::execInvShift(UVMap<double>* uv, int num)
{
    for (int i=0; i<num; i++) {
        uv[i].u -= shiftU;
        uv[i].v -= shiftV;
    }
}



void  TextureParam::execScale(UVMap<double>* uv, int num)
{
    double uu, vv;

    for (int i=0; i<num; i++) {
        uu = (uv[i].u - 0.5)*scaleU;
        vv = (uv[i].v - 0.5)*scaleV;
        uv[i].u = uu + 0.5;
        uv[i].v = vv + 0.5;
    }
}



void  TextureParam::execInvScale(UVMap<double>* uv, int num)
{
    double uu, vv;

    for (int i=0; i<num; i++) {
        uu = (uv[i].u - 0.5)/scaleU;
        vv = (uv[i].v - 0.5)/scaleV;
        uv[i].u = uu + 0.5;
        uv[i].v = vv + 0.5;
    }
}



void  TextureParam::execRotate(UVMap<double>* uv, int num)
{
    double uu, vv;
    double cs = cos(rotate);
    double sn = sin(rotate);

    for (int i=0; i<num; i++) {
        uu = uv[i].u - 0.5;
        vv = uv[i].v - 0.5;
        uv[i].u =  uu*cs + vv*sn + 0.5;
        uv[i].v = -uu*sn + vv*cs + 0.5;
    }
}



void  TextureParam::execInvRotate(UVMap<double>* uv, int num)
{
    double uu, vv;
    double cs =  cos(rotate);
    double sn = -sin(rotate);

    for (int i=0; i<num; i++) {
        uu = uv[i].u - 0.5;
        vv = uv[i].v - 0.5;
        uv[i].u =  uu*cs + vv*sn + 0.5;
        uv[i].v = -uu*sn + vv*cs + 0.5;
    }
}



void  TextureParam::execTrans(UVMap<double>* uv, int num)
{
    //DEBUG_MODE PRINT_MESG("%f %f, %f %f, %f\n", shiftU, shiftV, scaleU, scaleV, rotate);
    if (flipU) { execFlipU(uv, num); flipU = false;}    
    if (flipV) { execFlipV(uv, num); flipV = false;}    
    if (isSetRotate()) execRotate(uv, num);
    if (isSetScale())  execScale (uv, num);
    if (isSetShift())  execShift (uv, num);

    return;
}



void  TextureParam::execInvTrans(UVMap<double>* uv, int num)
{
    if (flipU) { execFlipU(uv, num); flipU = false;}    
    if (flipV) { execFlipV(uv, num); flipV = false;}    
    if (isSetShift())  execInvShift (uv, num);
    if (isSetScale())  execInvScale (uv, num);
    if (isSetRotate()) execInvRotate(uv, num);

    return;
}



void  TextureParam::printParam(FILE* fp)
{
    fprintf(fp, "TextureParam.name         = %s\n", name.buf);
    fprintf(fp, "TextureParam.color        = (%f,%f,%f,%f) [RGBA]\n", color[0], color[1], color[2], color[3]);
    fprintf(fp, "TextureParam.shiftU       = %f\n", shiftU);
    fprintf(fp, "TextureParam.shiftV       = %f\n", shiftV);
    fprintf(fp, "TextureParam.scaleU       = %f\n", scaleU);
    fprintf(fp, "TextureParam.scaleV       = %f\n", scaleV);
    fprintf(fp, "TextureParam.rotate       = %f\n", rotate);
    
    if (alphaChannel) fprintf(fp, "TextureParam.alphaChannel = true\n");
    else              fprintf(fp, "TextureParam.alphaChannel = flase\n");

    if (flipU) fprintf(fp, "TextureParam.flipU        = true\n");
    else       fprintf(fp, "TextureParam.flipU        = false\n");
    if (flipV) fprintf(fp, "TextureParam.flipV        = true\n");
    else       fprintf(fp, "TextureParam.flipV        = false\n");

    fflush(fp);
    return;
}




////////////////////////////////////////////////////////////////////////////////

/**
@brief テクスチャ名，カラーを比べる

*/
bool  jbxl::isSameTexture(TextureParam a, TextureParam b)
{
    char* a_name = a.getName();
    char* b_name = b.getName();
    if (a_name!=NULL && b_name!=NULL) {
        if (strcmp(a_name, b_name)) return false;
    }
    else if (a_name!=NULL || b_name!=NULL) return false;

    //
    for (int i=0; i<4; i++) {
        if (a.getColor(i)!=b.getColor(i)) return false;
    }

    return true;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
// MaterialParam クラス
//

void  MaterialParam::init(void)
{
    enable = false;

    texture.init();
    bumpmap.init();
    specmap.init();

    addname = init_Buffer();

    transparent = 1.0;
    shininess   = 0.0;
    glow        = 0.0;
    bright      = 0.0;
    glossiness  = 0.0;  ///< 光沢
    environment = 0.0;  ///< 環境光
    light       = 0.0;

    mapping     = 0;
    mflags      = 0;
    others      = 0;

    return;
}



void  MaterialParam::free (void)
{
    texture.free();
    bumpmap.free();
    specmap.free();

    free_Buffer(&addname);

    return;
}



void  MaterialParam::dup(MaterialParam m)
{
    *this = m;
    
    texture.dup(m.texture);
    bumpmap.dup(m.bumpmap);
    specmap.dup(m.specmap);

    addname = dup_Buffer(m.addname);
}



/**
@param ext 拡張子
*/
void  MaterialParam::setupFullName(const char* ext)
{
    if (texture.isSetTexture()) {
        if (addname.buf!=NULL) {
            texture.addName("_");
            texture.addName((char*)addname.buf);
        }
        if (ext!=NULL) {
            if (ext[0]!='.') texture.addName(".");
            texture.addName(ext);
        }
    }
    if (bumpmap.isSetTexture()) {
        if (addname.buf!=NULL) {
            bumpmap.addName("_");
            bumpmap.addName((char*)addname.buf);
        }
        if (ext!=NULL) {
            if (ext[0]!='.') bumpmap.addName(".");
            bumpmap.addName(ext);
        }
    }
    if (specmap.isSetTexture()) {
        if (addname.buf!=NULL) {
            specmap.addName("_");
            specmap.addName((char*)addname.buf);
        }
        if (ext!=NULL) {
            if (ext[0]!='.') specmap.addName(".");
            specmap.addName(ext);
        }
    }
}



void  MaterialParam::printParam(FILE* fp)
{
    if (!enable) {
        fprintf(fp, "MaterialParam is disable\n");
        return;
    }

    texture.printParam(fp);
    if (bumpmap.isSetTexture()) bumpmap.printParam(fp);
    if (specmap.isSetTexture()) specmap.printParam(fp);

    fprintf(fp, "MaterialParam.addname     = %s\n", addname.buf);
    fprintf(fp, "MaterialParam.transparent = %f\n", transparent);
    fprintf(fp, "MaterialParam.shininess   = %f\n", shininess);
    fprintf(fp, "MaterialParam.glow        = %f\n", glow);
    fprintf(fp, "MaterialParam.bright      = %f\n", bright);
    fprintf(fp, "MaterialParam.mapping     = %d\n", mapping);
    fprintf(fp, "MaterialParam.mflags      = %d\n", mflags);
    fprintf(fp, "MaterialParam.others      = %d\n", others);
    
    fprintf(fp, "MaterialParam.glossiness  = %f\n", glossiness);
    fprintf(fp, "MaterialParam.environment = %f\n", environment);

    fflush(fp);
    return;
}



/**
マテリアルの各パラメータを Base64で文字列化する．ただし '/' はファイル名として使用できないので，'$' に変換される．@n
戻りポインタは free する必要がある．

@param obj オブジェクトの種類を示す任意の一文字
@return マテリアルの各パラメータをBase64で文字列化したデータへのポインタ．33Byte (32Byte+0x00). 要 free
*/
char*  MaterialParam::getBase64Params(char obj)
{
    uByte attr[MATERIAL_ATTR_LEN];

    double red    = getColor(0);
    double green  = getColor(1);
    double blue   = getColor(2);
    double alpha  = getColor(3);
    double cutoff = getAlphaCutoff();
    int   almode = getAlphaMode();
    //
    double transp = transparent;

    short int rotate = (short int)((int)(getRotate()*2000.)%32768); // 2Byte化
    short int shiftu = (short int)((int)(getShiftU()*2000.)%32768);
    short int shiftv = (short int)((int)(getShiftV()*2000.)%32768);
    short int scaleu = (short int)((int)(getScaleU()*100. )%32768);
    short int scalev = (short int)((int)(getScaleV()*100. )%32768);

    if (alpha<0.99) {
        almode = MATERIAL_ALPHA_BLENDING;   // Blending Mode
        cutoff = 0.0;
        transp = alpha;
    }
    //
    if (cutoff==0.0 && almode==MATERIAL_ALPHA_MASKING) transp = 1.0;    // cutoff==0.0 のときは Blending Modeとするため
    if (cutoff>0.9) cutoff = 0.9;         // for Unity3D

    memset(attr, 0, MATERIAL_ATTR_LEN);
    attr[MATERIAL_ATTR_COLOR_RED]   = (uByte)((1.0-red)*255);
    attr[MATERIAL_ATTR_COLOR_GREEN] = (uByte)((1.0-green)*255);
    attr[MATERIAL_ATTR_COLOR_BLUE]  = (uByte)((1.0-blue)*255);
    attr[MATERIAL_ATTR_TRANSPARENT] = (uByte)((1.0-transp)*255);
    attr[MATERIAL_ATTR_ALPHACUTOFF] = (uByte)(cutoff*255);
    attr[MATERIAL_ATTR_SHININESS]   = (uByte)(shininess*255);
    attr[MATERIAL_ATTR_GLOW]        = (uByte)(glow*255);
    attr[MATERIAL_ATTR_BRIGHT]      = (uByte)(bright*255);
    attr[MATERIAL_ATTR_LIGHT]       = (uByte)(light*255);
    attr[MATERIAL_ATTR_OBJECT]      = (uByte)obj;
    //
    memcpy(attr+MATERIAL_ATTR_SHIFT_U, &shiftu, 2);
    memcpy(attr+MATERIAL_ATTR_SHIFT_V, &shiftv, 2);
    memcpy(attr+MATERIAL_ATTR_SCALE_U, &scaleu, 2);
    memcpy(attr+MATERIAL_ATTR_SCALE_V, &scalev, 2);
    memcpy(attr+MATERIAL_ATTR_ROTATE,  &rotate, 2);

    char* params = (char*)encode_base64_filename(attr, MATERIAL_ATTR_LEN);  // 要 free   / -> $

    return params;
}




////////////////////////////////////////////////////////////////////////////////

/**
@brief マテリアルのパラメータを比べる．

*/
bool  jbxl::isSameMaterial(MaterialParam a, MaterialParam b)
{
    if (!isSameTexture(a.texture, b.texture)) return false;
    if (!isSameTexture(a.bumpmap, b.bumpmap)) return false;
    if (!isSameTexture(a.specmap, b.specmap)) return false;

    //
    bool ret = true;

    char* a_param = a.getBase64Params();
    char* b_param = b.getBase64Params();
    //
    if (a_param!=NULL && b_param!=NULL) {
        if (strcmp(a_param, b_param)) ret = false;
    }
    else if (a_param!=NULL || b_param!=NULL) {
        ret = false;
    }
    if (a_param!=NULL) ::free(a_param);
    if (b_param!=NULL) ::free(b_param);

    return ret;
}



