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
 16;
 -19.79070;231.14854;-119.77127;,
 52.71216;182.53833;-119.77127;,
 -92.29353;182.53833;-119.77127;,
 52.71216;182.53833;-119.77127;,
 52.71216;0.23189;-119.77127;,
 -92.29353;0.23189;-119.77127;,
 -92.29353;182.53833;-119.77127;,
 -92.29353;182.53833;-119.77127;,
 -92.29353;0.23189;-119.77127;,
 -92.29353;0.23189;115.50222;,
 -92.29353;182.53833;115.50222;,
 -19.79070;231.14854;115.50222;,
 52.71216;182.53833;115.50222;,
 -19.79070;231.14854;-119.77127;,
 -19.79070;231.14854;-119.77127;,
 -19.79070;231.14854;115.50222;;
 
 9;
 3;0,1,2;,
 3;3,4,5;,
 3;3,5,6;,
 3;7,8,9;,
 3;7,9,10;,
 3;11,12,1;,
 3;11,1,13;,
 3;14,2,10;,
 3;14,10,15;;
 
 MeshMaterialList {
  2;
  9;
  0,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.075200;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.661600;0.520800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  7;
  0.000000;0.000000;-1.000000;,
  0.556879;0.830594;0.000000;,
  -0.786250;0.617908;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.556879;0.830594;0.000000;,
  -0.951077;0.308954;0.000000;,
  -0.556879;0.830594;0.000000;;
  9;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;5,3,3;,
  3;5,3,2;,
  3;4,1,4;,
  3;4,4,1;,
  3;6,5,2;,
  3;6,2,6;;
 }
 MeshTextureCoords {
  16;
  0.500000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;;
 }
}
