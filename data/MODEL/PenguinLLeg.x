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
 308;
 -0.87061;-0.82689;0.01853;,
 -0.87489;-0.58233;-0.57198;,
 0.73686;-0.55419;-0.57198;,
 0.74114;-0.79875;0.01853;,
 -0.88519;0.00810;-0.81658;,
 0.72656;0.03623;-0.81658;,
 -0.89549;0.59852;-0.57198;,
 0.71625;0.62666;-0.57198;,
 -0.89976;0.84307;0.01853;,
 0.71199;0.87122;0.01853;,
 -0.89549;0.59852;0.60906;,
 0.71625;0.62666;0.60906;,
 -0.88519;0.00810;0.85365;,
 0.72656;0.03623;0.85365;,
 -0.87489;-0.58233;0.60905;,
 0.73686;-0.55419;0.60905;,
 -0.87061;-0.82689;0.01853;,
 0.74114;-0.79875;0.01853;,
 1.84866;-0.56722;-0.57198;,
 1.83586;-0.81149;0.01853;,
 1.87956;0.02248;-0.81658;,
 1.91047;0.61219;-0.57198;,
 1.92327;0.85645;0.01853;,
 1.91047;0.61219;0.60906;,
 1.87956;0.02248;0.85365;,
 1.84866;-0.56722;0.60905;,
 1.83586;-0.81149;0.01853;,
 2.54918;-0.55229;-0.52397;,
 2.54527;-0.77697;0.01853;,
 2.55866;-0.00986;-0.74869;,
 2.56812;0.53257;-0.52397;,
 2.57204;0.75724;0.01853;,
 2.56812;0.53257;0.56104;,
 2.55866;-0.00986;0.78576;,
 2.54918;-0.55229;0.56104;,
 2.54527;-0.77697;0.01853;,
 3.05784;-0.57403;-0.48487;,
 3.04512;-0.78215;0.01853;,
 3.08857;-0.07156;-0.69338;,
 3.11930;0.43091;-0.48487;,
 3.13204;0.63903;0.01853;,
 3.11930;0.43091;0.52194;,
 3.08857;-0.07156;0.73046;,
 3.05784;-0.57403;0.52194;,
 3.04512;-0.78215;0.01853;,
 4.33054;-0.51299;-0.83558;,
 4.31781;-0.68179;0.01853;,
 4.36127;-0.10548;-1.18938;,
 4.39200;0.30202;-0.83558;,
 4.40473;0.47082;0.01853;,
 4.39200;0.30202;0.87266;,
 4.36127;-0.10548;1.22645;,
 4.33054;-0.51299;0.87266;,
 4.31781;-0.68179;0.01853;,
 5.80170;-0.56210;-1.28004;,
 5.78897;-0.70527;0.01854;,
 5.83243;-0.21643;-1.71302;,
 5.86315;0.12923;-1.28004;,
 5.87589;0.27241;0.01854;,
 5.86315;0.12923;1.31712;,
 5.83243;-0.21643;1.73775;,
 5.80170;-0.56210;1.31712;,
 5.78897;-0.70527;0.01854;,
 6.04983;-0.53680;-1.61277;,
 6.12099;-0.58696;-1.17510;,
 5.79788;-0.60505;-0.89039;,
 6.15242;-0.48029;-1.02176;,
 5.81128;-0.45425;-0.87489;,
 6.15242;-0.48029;-1.02176;,
 6.18327;-0.27066;-0.95840;,
 5.83243;-0.21643;-0.85047;,
 5.81128;-0.45425;-0.87489;,
 6.09648;-0.24823;-1.81927;,
 6.18327;-0.27066;-0.95840;,
 6.19805;-0.00974;-1.01438;,
 5.85245;0.00872;-0.82734;,
 5.83243;-0.21643;-0.85047;,
 6.11106;-0.03700;-1.63843;,
 6.16979;0.21175;-1.27021;,
 5.86778;0.18110;-0.80963;,
 6.19805;-0.00974;-1.01438;,
 5.85245;0.00872;-0.82734;,
 5.84771;-0.04460;0.66406;,
 5.86968;0.20250;0.65265;,
 5.86894;0.19429;0.72706;,
 5.84919;0.06916;0.72696;,
 5.83243;-0.21643;0.67199;,
 5.84771;-0.04460;0.66406;,
 5.84919;0.06916;0.72696;,
 5.83243;-0.21643;0.72688;,
 5.81677;-0.39253;0.68012;,
 5.83243;-0.21643;0.67199;,
 5.83243;-0.21643;0.72688;,
 5.81566;-0.40495;0.72679;,
 5.79556;-0.63111;0.69113;,
 5.81677;-0.39253;0.68012;,
 5.81566;-0.40495;0.72679;,
 5.79590;-0.62719;0.72669;,
 5.81375;-0.42647;-0.77056;,
 5.79678;-0.61739;-0.77851;,
 5.83243;-0.21643;-0.76183;,
 5.81375;-0.42647;-0.77056;,
 5.85069;-0.01098;-0.75329;,
 5.83243;-0.21643;-0.76183;,
 5.86840;0.18822;-0.74500;,
 5.85069;-0.01098;-0.75329;,
 6.19411;0.17365;0.35650;,
 6.20032;0.31379;0.01854;,
 6.17214;-0.07345;0.53289;,
 6.15686;-0.24526;0.53936;,
 6.17214;-0.07345;0.53289;,
 6.14121;-0.42138;0.49371;,
 6.15686;-0.24526;0.53936;,
 6.11341;-0.73411;0.01854;,
 6.12000;-0.65996;0.34885;,
 6.14121;-0.42138;0.49371;,
 6.12122;-0.64624;-0.37636;,
 6.11341;-0.73411;0.01854;,
 6.13818;-0.45532;-0.56625;,
 6.15686;-0.24526;-0.62931;,
 6.13818;-0.45532;-0.56625;,
 6.17513;-0.03983;-0.56985;,
 6.15686;-0.24526;-0.62931;,
 6.19284;0.15939;-0.37999;,
 6.17513;-0.03983;-0.56985;,
 6.15897;0.10293;1.49016;,
 6.16476;0.22950;1.20072;,
 6.14500;0.02550;0.92088;,
 6.14500;0.02550;0.92088;,
 6.12823;-0.24272;0.79559;,
 6.12823;-0.24272;1.78919;,
 6.12823;-0.24272;0.79559;,
 6.11147;-0.43125;0.87441;,
 6.09750;-0.58839;1.49016;,
 6.09171;-0.65349;1.02450;,
 6.11147;-0.43125;0.87441;,
 6.65761;-0.56029;-1.66094;,
 6.72056;-0.59038;-1.37986;,
 6.74859;-0.51988;-1.21413;,
 6.74859;-0.51988;-1.21413;,
 6.78501;-0.34835;-1.12503;,
 6.64949;-0.32961;-1.82591;,
 6.78501;-0.34835;-1.12503;,
 6.78837;-0.11452;-1.20623;,
 6.70506;0.01473;-1.65357;,
 6.76436;0.10840;-1.42529;,
 6.78837;-0.11452;-1.20623;,
 6.76617;0.04988;0.28718;,
 6.77132;0.18748;0.00757;,
 6.74800;-0.15454;0.43312;,
 6.73536;-0.29671;0.43847;,
 6.74800;-0.15454;0.43312;,
 6.72241;-0.51226;0.40071;,
 6.73536;-0.29671;0.43847;,
 6.69940;-0.73918;0.00757;,
 6.70485;-0.67859;0.28085;,
 6.72241;-0.51226;0.40071;,
 6.70586;-0.66807;-0.31914;,
 6.69940;-0.73918;0.00757;,
 6.71990;-0.47049;-0.47625;,
 6.73536;-0.29671;-0.52842;,
 6.71990;-0.47049;-0.47625;,
 6.75047;-0.12673;-0.47924;,
 6.73536;-0.29671;-0.52842;,
 6.76512;0.03808;-0.32215;,
 6.75047;-0.12673;-0.47924;,
 6.64332;0.01153;1.56947;,
 6.69474;0.12432;1.30846;,
 6.72183;0.00748;1.09663;,
 6.72183;0.00748;1.09663;,
 6.70667;-0.27541;0.96867;,
 6.58383;-0.27541;1.74424;,
 6.70667;-0.27541;0.96867;,
 6.69151;-0.47858;1.04482;,
 6.59712;-0.58089;1.50620;,
 6.63450;-0.67477;1.26367;,
 6.69151;-0.47858;1.04482;,
 8.12792;-0.61046;0.04272;,
 8.12926;-0.52465;-0.03110;,
 7.35549;-0.05031;-0.01075;,
 7.35214;-0.23498;0.17132;,
 8.12311;-0.66892;0.08125;,
 7.34031;-0.38504;0.26636;,
 8.11977;-0.70957;0.08267;,
 8.12311;-0.66892;0.08125;,
 7.34031;-0.38504;0.26636;,
 7.33208;-0.48938;0.26984;,
 8.11634;-0.75124;0.07270;,
 8.11977;-0.70957;0.08267;,
 7.33208;-0.48938;0.26984;,
 7.32364;-0.59634;0.24526;,
 8.11027;-0.82524;-0.03110;,
 8.11171;-0.80770;0.04106;,
 7.31221;-0.74122;0.16721;,
 7.30866;-0.78627;-0.01075;,
 8.11634;-0.75124;0.07270;,
 7.32364;-0.59634;0.24526;,
 8.11198;-0.80444;-0.11737;,
 8.11027;-0.82524;-0.03110;,
 7.30866;-0.78627;-0.01075;,
 7.31286;-0.73289;-0.22352;,
 8.11569;-0.75928;-0.15885;,
 7.32201;-0.61695;-0.32583;,
 8.11977;-0.70958;-0.17262;,
 8.11569;-0.75928;-0.15885;,
 7.32201;-0.61695;-0.32583;,
 7.33208;-0.48940;-0.35979;,
 8.12376;-0.66097;-0.15964;,
 8.11977;-0.70958;-0.17262;,
 7.33208;-0.48940;-0.35979;,
 7.34191;-0.36463;-0.32776;,
 8.12763;-0.61383;-0.11816;,
 7.35145;-0.24366;-0.27138;,
 8.12376;-0.66097;-0.15964;,
 7.34191;-0.36463;-0.32776;,
 8.45406;-0.75713;-0.01953;,
 8.45446;-0.75232;-0.04096;,
 8.45267;-0.77410;-0.00834;,
 8.45170;-0.78590;-0.04096;,
 8.45170;-0.78590;-0.00793;,
 8.45267;-0.77410;-0.00834;,
 8.45170;-0.78590;-0.04096;,
 8.45071;-0.79799;-0.01083;,
 8.45170;-0.78590;-0.00793;,
 8.45170;-0.78590;-0.04096;,
 8.44894;-0.81947;-0.04096;,
 8.44937;-0.81438;-0.02002;,
 8.45071;-0.79799;-0.01083;,
 8.45170;-0.78590;-0.04096;,
 8.44944;-0.81343;-0.06599;,
 8.44894;-0.81947;-0.04096;,
 8.45051;-0.80032;-0.07804;,
 8.45170;-0.78590;-0.04096;,
 8.45170;-0.78590;-0.08204;,
 8.45051;-0.80032;-0.07804;,
 8.45170;-0.78590;-0.04096;,
 8.45286;-0.77178;-0.07828;,
 8.45170;-0.78590;-0.08204;,
 8.45170;-0.78590;-0.04096;,
 8.45397;-0.75811;-0.06623;,
 8.45286;-0.77178;-0.07828;,
 8.45170;-0.78590;-0.04096;,
 7.71687;-0.38982;1.59598;,
 7.73812;-0.29542;1.49495;,
 7.20784;-0.01801;1.40483;,
 7.17118;-0.17485;1.58573;,
 7.74694;-0.32109;1.44360;,
 7.22645;-0.11609;1.26696;,
 7.74694;-0.32109;1.44360;,
 7.74054;-0.51372;1.32184;,
 7.21896;-0.38383;1.11441;,
 7.22645;-0.11609;1.26696;,
 7.69768;-0.51372;1.63273;,
 7.14020;-0.38383;1.70121;,
 7.74054;-0.51372;1.32184;,
 7.72904;-0.58073;1.42910;,
 7.19910;-0.53439;1.25015;,
 7.21896;-0.38383;1.11441;,
 7.69560;-0.59958;1.59073;,
 7.13763;-0.57044;1.54719;,
 7.70966;-0.64000;1.50949;,
 7.16293;-0.67441;1.39086;,
 7.72904;-0.58073;1.42910;,
 7.19910;-0.53439;1.25015;,
 8.15699;-0.57344;1.56768;,
 8.16066;-0.56350;1.55203;,
 8.16166;-0.57531;1.53572;,
 8.16166;-0.57531;1.53572;,
 8.16089;-0.59022;1.52981;,
 8.15323;-0.59180;1.57325;,
 8.16089;-0.59022;1.52981;,
 8.15935;-0.60626;1.53503;,
 8.15232;-0.60634;1.56950;,
 8.15564;-0.61632;1.55233;,
 8.15935;-0.60626;1.53503;,
 7.60124;-0.53739;-1.70854;,
 7.63232;-0.55186;-1.71006;,
 7.30356;-0.56053;-1.54360;,
 7.26481;-0.53866;-1.70145;,
 7.63009;-0.51361;-1.62513;,
 7.31944;-0.53393;-1.40898;,
 7.63009;-0.51361;-1.62513;,
 7.65430;-0.40762;-1.45333;,
 7.34745;-0.45431;-1.31495;,
 7.31944;-0.53393;-1.40898;,
 7.60789;-0.38537;-1.78387;,
 7.25944;-0.36941;-1.79012;,
 7.65430;-0.40762;-1.45333;,
 7.66170;-0.26221;-1.49725;,
 7.34788;-0.27989;-1.37374;,
 7.34745;-0.45431;-1.31495;,
 7.63211;-0.27618;-1.70928;,
 7.29423;-0.08699;-1.69990;,
 7.65610;-0.22730;-1.61716;,
 7.33309;-0.06105;-1.53275;,
 7.66170;-0.26221;-1.49725;,
 7.34788;-0.27989;-1.37374;,
 8.03032;-0.48513;-1.67848;,
 8.03657;-0.49534;-1.66476;,
 8.03619;-0.48972;-1.65355;,
 8.03619;-0.48972;-1.65355;,
 8.03885;-0.47508;-1.64323;,
 8.02970;-0.47217;-1.68697;,
 8.03885;-0.47508;-1.64323;,
 8.03972;-0.46130;-1.64949;,
 8.03458;-0.45786;-1.67647;,
 8.03790;-0.45156;-1.66318;,
 8.03972;-0.46130;-1.64949;;
 
 212;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;18,19,3,2;,
 4;20,18,2,5;,
 4;21,20,5,7;,
 4;22,21,7,9;,
 4;23,22,9,11;,
 4;24,23,11,13;,
 4;25,24,13,15;,
 4;26,25,15,17;,
 4;27,28,19,18;,
 4;29,27,18,20;,
 4;30,29,20,21;,
 4;31,30,21,22;,
 4;32,31,22,23;,
 4;33,32,23,24;,
 4;34,33,24,25;,
 4;35,34,25,26;,
 4;36,37,28,27;,
 4;38,36,27,29;,
 4;39,38,29,30;,
 4;40,39,30,31;,
 4;41,40,31,32;,
 4;42,41,32,33;,
 4;43,42,33,34;,
 4;44,43,34,35;,
 4;45,46,37,36;,
 4;47,45,36,38;,
 4;48,47,38,39;,
 4;49,48,39,40;,
 4;50,49,40,41;,
 4;51,50,41,42;,
 4;52,51,42,43;,
 4;53,52,43,44;,
 4;54,55,46,45;,
 4;56,54,45,47;,
 4;57,56,47,48;,
 4;58,57,48,49;,
 4;59,58,49,50;,
 4;60,59,50,51;,
 4;61,60,51,52;,
 4;62,61,52,53;,
 4;63,64,65,54;,
 4;64,66,67,65;,
 4;68,69,70,71;,
 4;72,63,54,56;,
 4;73,74,75,76;,
 4;77,72,56,57;,
 4;78,77,57,79;,
 4;80,78,79,81;,
 4;82,83,84,85;,
 4;86,87,88,89;,
 4;90,91,92,93;,
 4;94,95,96,97;,
 4;98,99,65,67;,
 4;100,101,71,70;,
 4;102,103,76,75;,
 4;104,105,81,79;,
 4;106,107,58,83;,
 4;108,106,83,82;,
 4;109,110,87,86;,
 4;111,112,91,90;,
 4;113,114,94,62;,
 4;114,115,95,94;,
 4;116,117,55,99;,
 4;118,116,99,98;,
 4;119,120,101,100;,
 4;121,122,103,102;,
 4;107,123,104,58;,
 4;123,124,105,104;,
 4;125,126,84,59;,
 4;126,127,85,84;,
 4;128,129,89,88;,
 4;130,125,59,60;,
 4;131,132,93,92;,
 4;133,130,60,61;,
 4;134,133,61,97;,
 4;135,134,97,96;,
 4;136,137,64,63;,
 4;137,138,66,64;,
 4;139,140,69,68;,
 4;141,136,63,72;,
 4;142,143,74,73;,
 4;144,141,72,77;,
 4;145,144,77,78;,
 4;146,145,78,80;,
 4;147,148,107,106;,
 4;149,147,106,108;,
 4;150,151,110,109;,
 4;152,153,112,111;,
 4;154,155,114,113;,
 4;155,156,115,114;,
 4;157,158,117,116;,
 4;159,157,116,118;,
 4;160,161,120,119;,
 4;162,163,122,121;,
 4;148,164,123,107;,
 4;164,165,124,123;,
 4;166,167,126,125;,
 4;167,168,127,126;,
 4;169,170,129,128;,
 4;171,166,125,130;,
 4;172,173,132,131;,
 4;174,171,130,133;,
 4;175,174,133,134;,
 4;176,175,134,135;,
 4;177,178,179,180;,
 4;181,177,180,182;,
 4;183,184,185,186;,
 4;187,188,189,190;,
 4;191,192,193,194;,
 4;192,195,196,193;,
 4;197,198,199,200;,
 4;201,197,200,202;,
 4;203,204,205,206;,
 4;207,208,209,210;,
 4;178,211,212,179;,
 4;211,213,214,212;,
 4;215,216,178,177;,
 4;217,215,177,181;,
 4;217,218,216,215;,
 4;219,220,184,183;,
 3;219,221,220;,
 4;222,223,188,187;,
 3;222,224,223;,
 4;225,226,192,191;,
 4;226,227,195,192;,
 4;226,225,228,227;,
 4;229,230,198,197;,
 4;231,229,197,201;,
 4;231,232,230,229;,
 4;233,234,204,203;,
 3;233,235,234;,
 4;236,237,208,207;,
 3;236,238,237;,
 4;216,239,211,178;,
 4;239,240,213,211;,
 4;239,216,241,240;,
 4;242,243,244,245;,
 4;243,246,247,244;,
 4;248,249,250,251;,
 4;252,242,245,253;,
 4;254,255,256,257;,
 4;258,252,253,259;,
 4;260,258,259,261;,
 4;262,260,261,263;,
 4;264,265,243,242;,
 4;265,266,246,243;,
 3;265,264,266;,
 4;267,268,249,248;,
 4;269,264,242,252;,
 4;268,267,264,269;,
 4;270,271,255,254;,
 4;272,269,252,258;,
 4;271,270,269,272;,
 4;273,272,258,260;,
 4;274,273,260,262;,
 3;274,272,273;,
 4;275,276,277,278;,
 4;276,279,280,277;,
 4;281,282,283,284;,
 4;285,275,278,286;,
 4;287,288,289,290;,
 4;291,285,286,292;,
 4;293,291,292,294;,
 4;295,293,294,296;,
 4;297,298,276,275;,
 4;298,299,279,276;,
 3;298,297,299;,
 4;300,301,282,281;,
 4;302,297,275,285;,
 4;301,300,297,302;,
 4;303,304,288,287;,
 4;305,302,285,291;,
 4;304,303,302,305;,
 4;306,305,291,293;,
 4;307,306,293,295;,
 3;307,305,306;,
 4;137,136,278,277;,
 4;138,137,277,280;,
 4;140,139,284,283;,
 4;136,141,286,278;,
 4;143,142,290,289;,
 4;141,144,292,286;,
 4;144,145,294,292;,
 4;145,146,296,294;,
 4;148,147,180,179;,
 4;147,149,182,180;,
 4;151,150,186,185;,
 4;153,152,190,189;,
 4;155,154,194,193;,
 4;156,155,193,196;,
 4;158,157,200,199;,
 4;157,159,202,200;,
 4;161,160,206,205;,
 4;163,162,210,209;,
 4;164,148,179,212;,
 4;165,164,212,214;,
 4;167,166,245,244;,
 4;168,167,244,247;,
 4;170,169,251,250;,
 4;166,171,253,245;,
 4;173,172,257,256;,
 4;171,174,259,253;,
 4;174,175,261,259;,
 4;175,176,263,261;;
 
 MeshMaterialList {
  8;
  212;
  4,
  4,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  3,
  3,
  4,
  4,
  3,
  4,
  4,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  3,
  4,
  4,
  3,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  4,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  1,
  1,
  0,
  1,
  0,
  1,
  0,
  4,
  1,
  0,
  4,
  4,
  0,
  1,
  0,
  1,
  0,
  1,
  1,
  0,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  1,
  1,
  0,
  1,
  1,
  0,
  1,
  1,
  0,
  1,
  1,
  0,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  1,
  1,
  0,
  1,
  1,
  0,
  1,
  1,
  0,
  1,
  1,
  0,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4;;
  Material {
   0.204000;0.533600;0.708800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.316800;0.304000;0.244800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.558400;0.545600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.496000;0.166400;0.156800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.075200;0.075200;0.075200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  288;
  0.017457;-0.999848;-0.000003;,
  0.012343;-0.706999;-0.707107;,
  -0.000000;-0.000000;-1.000000;,
  -0.012345;0.707002;-0.707104;,
  -0.017460;0.999848;-0.000006;,
  -0.012345;0.707001;0.707105;,
  0.000000;-0.000007;1.000000;,
  0.012343;-0.707001;0.707104;,
  0.002889;-0.999996;-0.000003;,
  0.002027;-0.707144;-0.707067;,
  0.000020;-0.000001;-1.000000;,
  -0.001888;0.707145;-0.707066;,
  -0.002652;0.999996;-0.000006;,
  -0.001888;0.707144;0.707067;,
  0.000021;-0.000009;1.000000;,
  0.002027;-0.707147;0.707064;,
  0.018709;-0.999825;-0.000003;,
  0.027741;-0.707636;-0.706032;,
  0.049780;-0.001867;-0.998758;,
  0.072150;0.704198;-0.706328;,
  0.081515;0.996672;-0.000005;,
  0.072153;0.704198;0.706328;,
  0.049781;-0.001874;0.998758;,
  0.027741;-0.707639;0.706030;,
  0.019233;-0.999815;-0.000003;,
  0.043556;-0.707715;-0.705154;,
  0.101032;-0.004434;-0.994873;,
  0.156738;0.695280;-0.701441;,
  0.179289;0.983797;-0.000004;,
  0.156738;0.695283;0.701438;,
  0.101030;-0.004435;0.994874;,
  0.043554;-0.707719;0.705150;,
  0.012508;-0.999922;-0.000003;,
  -0.052600;-0.750582;-0.658680;,
  -0.091071;0.005561;-0.995829;,
  0.047656;0.752678;-0.656662;,
  0.147070;0.989126;-0.000003;,
  0.047654;0.752681;0.656658;,
  -0.091076;0.005561;0.995828;,
  -0.052602;-0.750586;0.658676;,
  -0.003957;-0.999992;-0.000002;,
  -0.144578;-0.839664;-0.523509;,
  -0.267031;-0.593798;-0.759012;,
  -0.053932;0.852258;-0.520334;,
  0.098233;0.995164;-0.000002;,
  -0.053837;0.851312;0.521890;,
  -0.197394;0.622713;0.757142;,
  -0.144325;-0.838734;0.525068;,
  -0.033364;-0.999443;0.000245;,
  -0.230971;-0.871415;-0.432768;,
  -0.389247;-0.625733;-0.675977;,
  -0.145831;0.868404;-0.473927;,
  0.075762;0.997122;0.002856;,
  -0.115692;0.878572;0.463387;,
  -0.165938;0.712423;0.681849;,
  -0.207481;-0.876570;0.434254;,
  0.869465;-0.203649;-0.450064;,
  0.927785;0.081500;-0.364106;,
  0.894229;0.068204;-0.442383;,
  0.996493;-0.081098;0.020610;,
  0.940716;-0.166601;-0.295462;,
  -0.041696;-0.993229;-0.108429;,
  0.389036;-0.363259;0.846577;,
  0.359651;-0.098731;0.927849;,
  0.514674;0.149513;0.844249;,
  -0.236336;0.916619;-0.322419;,
  -0.190298;-0.863855;-0.466414;,
  -0.308025;-0.578691;-0.755140;,
  -0.133561;0.793319;-0.593976;,
  0.012793;-0.996049;0.087879;,
  0.295141;-0.526936;0.797013;,
  0.324043;-0.090808;0.941674;,
  0.428414;0.327761;0.842042;,
  -0.126622;0.907091;-0.401437;,
  0.842300;0.009036;0.538933;,
  0.863113;0.056518;0.501838;,
  0.946481;-0.149968;0.285801;,
  0.882784;-0.178544;0.434528;,
  0.902414;0.088248;-0.421736;,
  0.867974;-0.182327;-0.461929;,
  0.886483;-0.224196;-0.404826;,
  0.996070;-0.088570;0.000028;,
  0.960489;0.025339;-0.277163;,
  -0.017443;-0.999847;0.000925;,
  0.137732;0.990426;0.009280;,
  0.335886;0.207762;0.918703;,
  0.166921;0.843434;0.510644;,
  0.281891;-0.081073;0.956015;,
  0.314979;-0.294785;0.902159;,
  0.010743;-0.900957;0.433775;,
  0.162464;0.872767;-0.460308;,
  0.329599;-0.373524;-0.867090;,
  0.013323;-0.919712;-0.392367;,
  0.312561;-0.020642;-0.949673;,
  0.379521;0.331800;-0.863639;,
  -0.066362;0.824686;0.561684;,
  -0.069865;0.655895;0.751612;,
  -0.140151;-0.862616;0.486057;,
  -0.116204;-0.975105;0.188857;,
  0.043723;0.997639;0.052956;,
  0.418161;0.435107;-0.797385;,
  0.324367;-0.008697;-0.945891;,
  0.336770;-0.389728;-0.857145;,
  -0.036157;-0.851942;-0.522386;,
  -0.028504;-0.270105;-0.962409;,
  0.056800;0.732597;-0.678289;,
  0.052276;-0.989702;0.133259;,
  0.171753;-0.760292;0.626463;,
  0.300118;-0.137058;0.944004;,
  0.342889;0.482777;0.805825;,
  0.086287;0.915427;-0.393125;,
  -0.040704;-0.999170;0.001425;,
  0.340267;0.940001;0.024845;,
  0.278121;0.280234;0.918759;,
  0.319915;0.688760;0.650588;,
  0.200966;-0.099527;0.974529;,
  0.135512;-0.386102;0.912448;,
  0.031172;-0.820166;0.571277;,
  0.299278;0.737294;-0.605666;,
  0.109065;-0.482964;-0.868822;,
  0.007062;-0.857910;-0.513751;,
  0.214823;-0.025276;-0.976326;,
  0.281347;0.422238;-0.861719;,
  0.128645;0.721170;0.680708;,
  0.096809;0.533936;0.839964;,
  -0.039182;-0.809572;0.585711;,
  -0.052271;-0.933608;0.354462;,
  0.252132;0.967656;-0.008452;,
  0.315189;0.636480;-0.703953;,
  0.299367;-0.026834;-0.953761;,
  0.210201;-0.583087;-0.784745;,
  -0.060450;-0.998170;0.001221;,
  0.403789;0.681844;0.609954;,
  0.330443;0.258534;0.907727;,
  0.396725;0.589059;0.704002;,
  0.222908;-0.105912;0.969069;,
  0.149395;-0.371195;0.916458;,
  0.029980;-0.805002;0.592515;,
  0.390257;0.627794;-0.673479;,
  0.103825;-0.478507;-0.871924;,
  -0.012399;-0.854951;-0.518561;,
  0.241988;-0.027886;-0.969878;,
  0.340969;0.363562;-0.866927;,
  -0.015967;-0.999872;0.000837;,
  0.431556;0.694754;0.575393;,
  0.319996;0.258955;0.911342;,
  0.404256;0.597573;0.692447;,
  0.218261;-0.114894;0.969103;,
  0.152967;-0.368403;0.916995;,
  0.060975;-0.796590;0.601437;,
  0.408011;0.644798;-0.646345;,
  0.122171;-0.487356;-0.864614;,
  0.022687;-0.860651;-0.508689;,
  0.241065;-0.026828;-0.970138;,
  0.338854;0.388981;-0.856663;,
  0.017059;-0.999854;0.000719;,
  0.498157;0.867021;0.010684;,
  0.996649;-0.081794;-0.000047;,
  0.329626;0.259670;0.907699;,
  0.420413;0.640949;0.642213;,
  0.236337;-0.117070;0.964593;,
  0.175700;-0.371756;0.911552;,
  0.091969;-0.797830;0.595827;,
  0.411840;0.697610;-0.586284;,
  0.147158;-0.490729;-0.858795;,
  0.053705;-0.862256;-0.503618;,
  0.257108;-0.028320;-0.965968;,
  0.348347;0.418641;-0.838686;,
  0.200847;0.601317;0.773355;,
  0.057638;-0.096425;0.993670;,
  -0.062534;-0.750548;0.657850;,
  -0.058714;-0.879418;0.472415;,
  0.229735;0.754948;0.614228;,
  0.411150;0.642564;-0.646582;,
  0.371867;0.129769;-0.919171;,
  0.164699;-0.705937;-0.688859;,
  0.241163;0.520332;0.819204;,
  0.083787;-0.074423;0.993701;,
  -0.024126;-0.692751;0.720773;,
  -0.030006;-0.874821;0.483516;,
  0.296998;0.699863;0.649603;,
  0.490540;0.617216;-0.615155;,
  0.447152;0.420872;-0.789254;,
  0.133639;-0.755296;-0.641614;,
  0.264529;0.502586;0.823062;,
  0.110221;-0.048573;0.992720;,
  0.023683;-0.656179;0.754234;,
  0.002605;-0.880209;0.474579;,
  0.337980;0.706166;0.622173;,
  0.520864;0.589345;-0.617554;,
  0.461959;0.386395;-0.798306;,
  0.126245;-0.741414;-0.659066;,
  -0.050726;-0.775133;-0.629759;,
  0.025275;-0.049921;-0.998433;,
  0.194658;0.701572;-0.685496;,
  0.040270;-0.999189;-0.000624;,
  0.223948;-0.856275;0.465447;,
  0.283107;-0.725916;0.626814;,
  0.378182;0.503976;0.776522;,
  0.391152;0.906921;0.156505;,
  0.015003;-0.564369;-0.825386;,
  0.138813;-0.006427;-0.990298;,
  0.309965;0.666148;-0.678357;,
  0.040563;-0.994084;0.100759;,
  0.191459;-0.855516;0.481078;,
  0.234162;-0.771506;0.591563;,
  0.466197;0.556765;0.687512;,
  0.484699;0.874675;-0.003217;,
  0.123871;-0.408470;-0.904328;,
  0.118467;-0.493088;-0.861876;,
  0.350187;0.653886;-0.670673;,
  0.128161;-0.320180;-0.938648;,
  0.091623;-0.825259;0.557273;,
  0.098919;-0.733557;0.672390;,
  0.535818;0.570029;0.622870;,
  0.510392;0.855314;-0.089094;,
  -0.197689;0.625346;-0.754891;,
  -0.266347;-0.591225;0.761257;,
  -0.214514;0.711132;-0.669534;,
  -0.280414;-0.674902;0.682550;,
  0.449561;-0.493680;0.744430;,
  -0.133789;0.638474;-0.757925;,
  0.480293;0.508519;0.714652;,
  0.903675;0.100711;0.416208;,
  0.996074;-0.088522;0.000005;,
  0.996071;-0.088553;0.000003;,
  0.996070;-0.088569;0.000107;,
  0.035573;0.967770;0.249311;,
  0.525129;0.218637;0.822458;,
  0.402488;-0.061131;0.913382;,
  0.425855;-0.127221;0.895803;,
  -0.029809;-0.985826;0.165104;,
  0.584478;-0.231117;0.777798;,
  -0.019863;-0.986238;-0.164135;,
  0.604643;-0.316307;-0.730997;,
  0.444117;-0.020803;-0.895727;,
  0.440301;0.103435;-0.891872;,
  0.046508;0.970119;-0.238130;,
  0.612002;0.245837;-0.751677;,
  -0.216597;0.943341;0.251382;,
  0.657153;0.264141;-0.705961;,
  0.363272;-0.023294;-0.931392;,
  0.311514;-0.206876;-0.927449;,
  -0.149884;-0.660331;0.735865;,
  -0.182291;-0.975398;0.123968;,
  0.392634;-0.483725;-0.782208;,
  0.011851;0.554813;-0.831891;,
  0.333579;0.666987;0.666223;,
  -0.023480;-0.630764;0.775619;,
  0.193059;-0.701731;-0.685785;,
  0.434716;0.732103;-0.524449;,
  0.401279;0.740811;-0.538677;,
  0.996648;-0.081808;0.000039;,
  0.996662;-0.081641;-0.000031;,
  0.996650;-0.081779;0.000078;,
  0.996654;-0.081737;-0.000000;,
  0.996659;-0.081681;-0.000126;,
  0.996655;-0.081724;-0.000252;,
  0.996654;-0.081741;-0.000155;,
  0.996632;-0.082008;-0.000029;,
  0.996652;-0.081758;-0.000059;,
  0.996628;-0.082056;-0.000000;,
  0.996659;-0.081678;-0.000070;,
  0.996673;-0.081503;-0.000140;,
  0.499221;0.769944;-0.397448;,
  0.399609;0.805570;-0.437458;,
  0.139699;-0.698240;-0.702100;,
  0.112762;-0.768256;-0.630133;,
  0.126671;-0.806957;-0.576867;,
  0.168384;-0.753330;-0.635721;,
  0.539491;0.746721;-0.389047;,
  0.980685;-0.124832;0.150578;,
  0.979243;-0.130238;0.155312;,
  0.979278;-0.105581;0.172823;,
  0.165126;-0.620678;-0.766480;,
  0.980151;-0.046626;0.192694;,
  0.083876;-0.841666;-0.533444;,
  0.979676;-0.017799;0.199795;,
  0.407876;0.283420;0.867934;,
  0.351442;0.288484;0.890655;,
  0.082338;-0.899040;0.430055;,
  0.920176;0.361593;-0.150089;,
  0.968994;0.158277;-0.189732;,
  0.970342;-0.109914;-0.215304;,
  0.465921;0.288677;0.836411;,
  0.290932;0.497140;-0.817442;,
  0.970113;-0.131778;-0.203752;,
  0.975450;-0.096130;-0.198132;;
  212;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;17,16,8,9;,
  4;18,17,9,10;,
  4;19,18,10,11;,
  4;20,19,11,12;,
  4;21,20,12,13;,
  4;22,21,13,14;,
  4;23,22,14,15;,
  4;16,23,15,8;,
  4;25,24,16,17;,
  4;26,25,17,18;,
  4;27,26,18,19;,
  4;28,27,19,20;,
  4;29,28,20,21;,
  4;30,29,21,22;,
  4;31,30,22,23;,
  4;24,31,23,16;,
  4;33,32,24,25;,
  4;34,33,25,26;,
  4;35,34,26,27;,
  4;36,35,27,28;,
  4;37,36,28,29;,
  4;38,37,29,30;,
  4;39,38,30,31;,
  4;32,39,31,24;,
  4;41,40,32,33;,
  4;42,41,33,34;,
  4;43,216,34,35;,
  4;44,43,35,36;,
  4;45,44,36,37;,
  4;46,45,37,38;,
  4;47,217,38,39;,
  4;40,47,39,32;,
  4;49,48,40,41;,
  4;50,49,41,42;,
  4;51,218,216,43;,
  4;52,51,43,44;,
  4;53,52,44,45;,
  4;54,53,45,46;,
  4;55,219,217,47;,
  4;48,55,47,40;,
  4;66,69,61,49;,
  4;220,70,62,220;,
  4;70,71,63,62;,
  4;67,66,49,50;,
  4;71,72,64,63;,
  4;68,221,218,51;,
  4;73,68,51,65;,
  4;72,222,223,64;,
  4;74,75,57,58;,
  4;59,74,58,59;,
  4;76,59,59,60;,
  4;77,76,60,56;,
  4;79,80,224,225;,
  4;81,79,225,81;,
  4;82,81,81,226;,
  4;78,82,226,223;,
  4;86,84,52,227;,
  4;85,228,75,74;,
  4;87,85,74,229;,
  4;88,87,229,230;,
  4;83,89,231,48;,
  4;232,88,76,77;,
  4;92,83,48,233;,
  4;91,234,80,79;,
  4;93,91,79,235;,
  4;94,93,235,236;,
  4;84,90,237,52;,
  4;238,94,82,78;,
  4;95,99,239,53;,
  4;240,100,58,57;,
  4;100,101,241,58;,
  4;96,95,53,54;,
  4;101,102,242,241;,
  4;97,243,219,55;,
  4;98,97,55,244;,
  4;102,245,56,60;,
  4;103,106,69,66;,
  4;106,107,70,69;,
  4;107,108,71,70;,
  4;104,103,66,67;,
  4;108,109,72,71;,
  4;105,246,221,68;,
  4;110,105,68,73;,
  4;109,247,222,72;,
  4;114,112,84,86;,
  4;113,114,86,85;,
  4;115,113,85,87;,
  4;116,115,87,88;,
  4;111,117,89,83;,
  4;117,116,88,89;,
  4;120,111,83,92;,
  4;119,120,92,91;,
  4;121,119,91,93;,
  4;122,121,93,94;,
  4;112,118,90,84;,
  4;118,122,94,90;,
  4;123,127,99,95;,
  4;127,128,100,99;,
  4;128,129,101,100;,
  4;124,123,95,96;,
  4;129,130,102,101;,
  4;125,248,243,97;,
  4;126,125,97,98;,
  4;130,249,245,102;,
  4;146,144,132,134;,
  4;145,146,134,133;,
  4;147,145,133,135;,
  4;148,147,135,136;,
  4;143,149,137,131;,
  4;149,148,136,137;,
  4;152,143,131,140;,
  4;151,152,140,139;,
  4;153,151,139,141;,
  4;154,153,141,142;,
  4;250,150,138,251;,
  4;150,154,142,138;,
  4;159,156,144,146;,
  4;158,159,146,145;,
  4;252,157,253,254;,
  4;160,158,145,147;,
  3;255,157,252;,
  4;161,160,147,148;,
  3;256,157,255;,
  4;155,162,149,143;,
  4;162,161,148,149;,
  4;257,258,157,256;,
  4;165,155,143,152;,
  4;164,165,152,151;,
  4;259,157,258,260;,
  4;166,164,151,153;,
  3;261,157,259;,
  4;167,166,153,154;,
  3;262,157,261;,
  4;156,163,150,250;,
  4;163,167,154,150;,
  4;263,253,157,262;,
  4;176,180,172,168;,
  4;264,181,173,265;,
  4;181,182,174,173;,
  4;177,176,168,169;,
  4;266,183,175,267;,
  4;178,177,169,170;,
  4;179,178,170,171;,
  4;183,268,269,175;,
  4;184,188,180,176;,
  4;270,189,181,264;,
  3;271,272,272;,
  4;189,190,182,181;,
  4;185,184,176,177;,
  4;273,272,272,273;,
  4;274,191,183,266;,
  4;186,185,177,178;,
  4;275,273,273,275;,
  4;187,186,178,179;,
  4;191,276,268,183;,
  3;275,275,277;,
  4;200,203,195,192;,
  4;203,204,196,195;,
  4;204,205,197,196;,
  4;201,200,192,193;,
  4;278,206,198,279;,
  4;202,201,193,194;,
  4;207,202,194,199;,
  4;206,207,199,198;,
  4;208,211,211,200;,
  4;280,212,204,203;,
  3;281,282,282;,
  4;212,213,205,204;,
  4;209,208,200,201;,
  4;283,282,282,283;,
  4;284,214,206,278;,
  4;210,285,201,202;,
  4;286,283,283,286;,
  4;215,210,202,207;,
  4;214,215,207,206;,
  3;286,286,287;,
  4;106,103,192,195;,
  4;107,106,195,196;,
  4;108,107,196,197;,
  4;103,104,193,192;,
  4;109,108,279,198;,
  4;104,105,194,193;,
  4;105,110,199,194;,
  4;247,109,198,199;,
  4;112,114,134,132;,
  4;114,113,133,134;,
  4;113,115,135,133;,
  4;115,116,136,135;,
  4;117,111,131,137;,
  4;116,117,137,136;,
  4;111,120,140,131;,
  4;120,119,139,140;,
  4;119,121,141,139;,
  4;121,122,142,141;,
  4;118,112,251,138;,
  4;122,118,138,142;,
  4;127,123,168,172;,
  4;128,127,265,173;,
  4;129,128,173,174;,
  4;123,124,169,168;,
  4;130,129,174,175;,
  4;248,125,170,169;,
  4;125,126,171,170;,
  4;249,130,175,269;;
 }
 MeshTextureCoords {
  308;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.125000;1.000000;,
  0.087490;1.000000;,
  0.087490;1.000000;,
  0.105500;1.000000;,
  0.105500;1.000000;,
  0.144500;1.000000;,
  0.217070;1.000000;,
  0.217070;1.000000;,
  0.144500;1.000000;,
  0.250000;1.000000;,
  0.282930;1.000000;,
  0.353210;1.000000;,
  0.353210;1.000000;,
  0.282930;1.000000;,
  0.375000;1.000000;,
  0.420280;1.000000;,
  0.420280;1.000000;,
  0.396790;1.000000;,
  0.396790;1.000000;,
  0.593570;1.000000;,
  0.561040;1.000000;,
  0.568200;1.000000;,
  0.596600;1.000000;,
  0.709740;1.000000;,
  0.656430;1.000000;,
  0.653400;1.000000;,
  0.711610;1.000000;,
  0.844340;1.000000;,
  0.790260;1.000000;,
  0.788390;1.000000;,
  0.846590;1.000000;,
  0.935260;1.000000;,
  0.905660;1.000000;,
  0.903410;1.000000;,
  0.931830;1.000000;,
  0.100480;1.000000;,
  0.076720;1.000000;,
  0.214060;1.000000;,
  0.149520;1.000000;,
  0.349650;1.000000;,
  0.285940;1.000000;,
  0.426500;1.000000;,
  0.400350;1.000000;,
  0.561040;1.000000;,
  0.500000;1.000000;,
  0.593570;1.000000;,
  0.709740;1.000000;,
  0.656430;1.000000;,
  0.844340;1.000000;,
  0.790260;1.000000;,
  1.000000;1.000000;,
  0.935260;1.000000;,
  0.905660;1.000000;,
  0.076720;1.000000;,
  0.000000;1.000000;,
  0.100480;1.000000;,
  0.214060;1.000000;,
  0.149520;1.000000;,
  0.349650;1.000000;,
  0.285940;1.000000;,
  0.426500;1.000000;,
  0.400350;1.000000;,
  0.625000;1.000000;,
  0.568200;1.000000;,
  0.596600;1.000000;,
  0.653400;1.000000;,
  0.711610;1.000000;,
  0.750000;1.000000;,
  0.788390;1.000000;,
  0.846590;1.000000;,
  0.875000;1.000000;,
  0.931830;1.000000;,
  0.903410;1.000000;,
  0.125000;1.000000;,
  0.087490;1.000000;,
  0.105500;1.000000;,
  0.144500;1.000000;,
  0.217070;1.000000;,
  0.250000;1.000000;,
  0.282930;1.000000;,
  0.353210;1.000000;,
  0.375000;1.000000;,
  0.420280;1.000000;,
  0.396790;1.000000;,
  0.561040;1.000000;,
  0.500000;1.000000;,
  0.593570;1.000000;,
  0.709740;1.000000;,
  0.656430;1.000000;,
  0.844340;1.000000;,
  0.790260;1.000000;,
  1.000000;1.000000;,
  0.935260;1.000000;,
  0.905660;1.000000;,
  0.076720;1.000000;,
  0.000000;1.000000;,
  0.100480;1.000000;,
  0.214060;1.000000;,
  0.149520;1.000000;,
  0.349650;1.000000;,
  0.285940;1.000000;,
  0.426500;1.000000;,
  0.400350;1.000000;,
  0.625000;1.000000;,
  0.568200;1.000000;,
  0.596600;1.000000;,
  0.653400;1.000000;,
  0.711610;1.000000;,
  0.750000;1.000000;,
  0.788390;1.000000;,
  0.846590;1.000000;,
  0.875000;1.000000;,
  0.931830;1.000000;,
  0.903410;1.000000;,
  0.561040;1.000000;,
  0.500000;1.000000;,
  0.500000;1.000000;,
  0.561040;1.000000;,
  0.593570;1.000000;,
  0.593570;1.000000;,
  0.709740;1.000000;,
  0.656430;1.000000;,
  0.656430;1.000000;,
  0.709740;1.000000;,
  0.844340;1.000000;,
  0.790260;1.000000;,
  0.790260;1.000000;,
  0.844340;1.000000;,
  1.000000;1.000000;,
  0.935260;1.000000;,
  0.935260;1.000000;,
  1.000000;1.000000;,
  0.905660;1.000000;,
  0.905660;1.000000;,
  0.076720;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.076720;1.000000;,
  0.100480;1.000000;,
  0.100480;1.000000;,
  0.214060;1.000000;,
  0.149520;1.000000;,
  0.149520;1.000000;,
  0.214060;1.000000;,
  0.349650;1.000000;,
  0.285940;1.000000;,
  0.285940;1.000000;,
  0.349650;1.000000;,
  0.426500;1.000000;,
  0.426500;1.000000;,
  0.400350;1.000000;,
  0.400350;1.000000;,
  0.561040;1.000000;,
  0.500000;1.000000;,
  0.593570;1.000000;,
  0.562500;1.000000;,
  0.709740;1.000000;,
  0.656430;1.000000;,
  0.687500;1.000000;,
  0.844340;1.000000;,
  0.790260;1.000000;,
  0.812500;1.000000;,
  1.000000;1.000000;,
  0.935260;1.000000;,
  0.905660;1.000000;,
  0.937500;1.000000;,
  0.076720;1.000000;,
  0.000000;1.000000;,
  0.100480;1.000000;,
  0.062500;1.000000;,
  0.214060;1.000000;,
  0.149520;1.000000;,
  0.187500;1.000000;,
  0.349650;1.000000;,
  0.285940;1.000000;,
  0.312500;1.000000;,
  0.426500;1.000000;,
  0.400350;1.000000;,
  0.437500;1.000000;,
  0.625000;1.000000;,
  0.568200;1.000000;,
  0.568200;1.000000;,
  0.625000;1.000000;,
  0.596600;1.000000;,
  0.596600;1.000000;,
  0.653400;1.000000;,
  0.711610;1.000000;,
  0.711610;1.000000;,
  0.653400;1.000000;,
  0.750000;1.000000;,
  0.750000;1.000000;,
  0.788390;1.000000;,
  0.846590;1.000000;,
  0.846590;1.000000;,
  0.788390;1.000000;,
  0.875000;1.000000;,
  0.875000;1.000000;,
  0.931830;1.000000;,
  0.931830;1.000000;,
  0.903410;1.000000;,
  0.903410;1.000000;,
  0.625000;1.000000;,
  0.568200;1.000000;,
  0.596600;1.000000;,
  0.653400;1.000000;,
  0.711610;1.000000;,
  0.750000;1.000000;,
  0.788390;1.000000;,
  0.846590;1.000000;,
  0.875000;1.000000;,
  0.931830;1.000000;,
  0.903410;1.000000;,
  0.125000;1.000000;,
  0.087490;1.000000;,
  0.087490;1.000000;,
  0.125000;1.000000;,
  0.105500;1.000000;,
  0.105500;1.000000;,
  0.144500;1.000000;,
  0.217070;1.000000;,
  0.217070;1.000000;,
  0.144500;1.000000;,
  0.250000;1.000000;,
  0.250000;1.000000;,
  0.282930;1.000000;,
  0.353210;1.000000;,
  0.353210;1.000000;,
  0.282930;1.000000;,
  0.375000;1.000000;,
  0.375000;1.000000;,
  0.420280;1.000000;,
  0.420280;1.000000;,
  0.396790;1.000000;,
  0.396790;1.000000;,
  0.125000;1.000000;,
  0.087490;1.000000;,
  0.105500;1.000000;,
  0.144500;1.000000;,
  0.217070;1.000000;,
  0.250000;1.000000;,
  0.282930;1.000000;,
  0.353210;1.000000;,
  0.375000;1.000000;,
  0.420280;1.000000;,
  0.396790;1.000000;;
 }
}