xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 26;
 -201.96275;193.49716;102.97319;,
 -201.96275;193.49716;-102.89878;,
 -201.96275;-150.13831;-102.89878;,
 -201.96275;-150.13831;102.97319;,
 -201.96275;193.49716;-102.89878;,
 200.88592;190.99501;-100.76979;,
 200.88592;-152.64043;-100.76979;,
 -201.96275;-150.13831;-102.89878;,
 200.88592;190.99501;-100.76979;,
 200.88592;190.99501;105.10232;,
 200.88592;-152.64043;105.10232;,
 200.88592;-152.64043;-100.76979;,
 200.88592;190.99501;105.10232;,
 -201.96275;193.49716;102.97319;,
 -201.96275;-150.13831;102.97319;,
 200.88592;-152.64043;105.10232;,
 -201.96275;-150.13831;102.97319;,
 -201.96275;-150.13831;-102.89878;,
 -201.62954;263.07696;0.03716;,
 201.21912;260.57481;2.16622;,
 -201.96275;193.49716;-102.89878;,
 201.21912;260.57481;2.16622;,
 -201.62954;263.07696;0.03716;,
 -201.96275;193.49716;102.97319;,
 -201.62954;263.07696;0.03716;,
 201.21912;260.57481;2.16622;;
 
 9;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,6,15;,
 4;18,19,5,20;,
 4;21,22,23,12;,
 3;24,20,23;,
 3;25,12,5;;
 
 MeshMaterialList {
  1;
  9;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.269600;0.269600;0.269600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game308\\Desktop\\TeamC_Data\\テクスチャ\\Brick02.jpg";
   }
  }
 }
 MeshNormals {
  13;
  -0.999997;0.002394;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.007580;0.469006;-0.883162;,
  0.005285;0.000000;-0.999986;,
  0.999997;-0.002394;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.008105;0.828443;-0.560014;,
  -0.001754;0.469033;0.883179;,
  -0.005285;0.000000;0.999986;,
  -0.006211;-0.999981;0.000000;,
  0.002186;0.828478;0.560018;,
  -0.999988;0.004789;0.000000;,
  0.999989;-0.004789;-0.000000;;
  9;
  4;0,0,1,1;,
  4;2,2,3,3;,
  4;4,4,5,5;,
  4;7,7,8,8;,
  4;9,9,9,9;,
  4;6,6,2,2;,
  4;10,10,7,7;,
  3;11,0,0;,
  3;12,4,4;;
 }
 MeshTextureCoords {
  26;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;;
 }
}
