﻿#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL\glew.h"
#include "GL\freeglut.h"

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku
#include "tekstura.h"

//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective


//Wymiary okna
int screen_width = 640;
int screen_height = 480;

GLint uniformTex0 , uniformTex1;
int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX = 10.0;
double kameraZ = 20.0;
double kameraD = -3;
double kameraPredkosc;
double kameraKat = -20;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;
double blend = 0.5;
double blend2 = 0;
//macierze
glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 MV1; //macierz ruchu ramki
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy


float vertices [ ] = {//floor
	-3,1,1,
	3,1,1,
	3,1,-9,
	-3,1,-9,

};
float vertices2 [ ] = { //green
	-1, 1, -2,
	1, 1, -2,
	1, 3, -2,
	-1, 3, -2,
};

float vertices3 [ ] = { //blue
	-3, 3, -4,
3, 3, -4,
3, 1, -4,
-3, 1, -4,

};

float verticesFigura[] = {
	0, 0, -127,
5, 25, -125,
10, 24, -125,
15, 21, -125,
19, 17, -125,
22, 13, -125,
25, 8, -125,
26, 2, -125,
26, -3, -125,
25, -9, -125,
22, -14, -125,
19, -18, -125,
15, -22, -125,
10, -25, -125,
5, -26, -125,
0, -27, -125,
-6, -26, -125,
-11, -25, -125,
-16, -22, -125,
-20, -18, -125,
-23, -14, -125,
-26, -9, -125,
-27, -3, -125,
-27, 2, -125,
-26, 8, -125,
-23, 13, -125,
-20, 17, -125,
-16, 21, -125,
-11, 24, -125,
-6, 25, -125,
-1, 26, -125,
10, 50, -117,
21, 47, -117,
30, 41, -117,
38, 34, -117,
44, 25, -117,
49, 15, -117,
51, 5, -117,
51, -6, -117,
49, -16, -117,
44, -26, -117,
38, -35, -117,
30, -42, -117,
21, -48, -117,
10, -51, -117,
0, -52, -117,
-11, -51, -117,
-22, -48, -117,
-31, -42, -117,
-39, -35, -117,
-45, -26, -117,
-50, -16, -117,
-52, -6, -117,
-52, 5, -117,
-50, 15, -117,
-45, 25, -117,
-39, 34, -117,
-31, 41, -117,
-22, 47, -117,
-11, 50, -117,
-1, 51, -117,
15, 73, -103,
30, 68, -103,
43, 60, -103,
55, 49, -103,
64, 37, -103,
70, 23, -103,
74, 7, -103,
74, -8, -103,
70, -24, -103,
64, -38, -103,
55, -50, -103,
43, -61, -103,
30, -69, -103,
15, -74, -103,
0, -75, -103,
-16, -74, -103,
-31, -69, -103,
-44, -61, -103,
-56, -50, -103,
-65, -38, -103,
-71, -24, -103,
-75, -8, -103,
-75, 7, -103,
-71, 23, -103,
-65, 37, -103,
-56, 49, -103,
-44, 60, -103,
-31, 68, -103,
-16, 73, -103,
-1, 74, -103,
19, 92, -85,
38, 86, -85,
55, 76, -85,
70, 63, -85,
81, 47, -85,
89, 29, -85,
93, 9, -85,
93, -10, -85,
89, -30, -85,
81, -48, -85,
70, -64, -85,
55, -77, -85,
38, -87, -85,
19, -93, -85,
0, -95, -85,
-20, -93, -85,
-39, -87, -85,
-56, -77, -85,
-71, -64, -85,
-82, -48, -85,
-90, -30, -85,
-94, -10, -85,
-94, 9, -85,
-90, 29, -85,
-82, 47, -85,
-71, 63, -85,
-56, 76, -85,
-39, 86, -85,
-20, 92, -85,
-1, 94, -85,
22, 107, -64,
44, 100, -64,
64, 88, -64,
81, 73, -64,
95, 54, -64,
104, 33, -64,
109, 11, -64,
109, -12, -64,
104, -34, -64,
95, -55, -64,
81, -74, -64,
64, -89, -64,
44, -101, -64,
22, -108, -64,
0, -110, -64,
-23, -108, -64,
-45, -101, -64,
-65, -89, -64,
-82, -74, -64,
-96, -55, -64,
-105, -34, -64,
-110, -12, -64,
-110, 11, -64,
-105, 33, -64,
-96, 54, -64,
-82, 73, -64,
-65, 88, -64,
-45, 100, -64,
-23, 107, -64,
-1, 109, -64,
25, 118, -40,
49, 110, -40,
70, 97, -40,
89, 80, -40,
104, 60, -40,
114, 37, -40,
120, 12, -40,
120, -13, -40,
114, -38, -40,
104, -61, -40,
89, -81, -40,
70, -98, -40,
49, -111, -40,
25, -119, -40,
0, -121, -40,
-26, -119, -40,
-50, -111, -40,
-71, -98, -40,
-90, -81, -40,
-105, -61, -40,
-115, -38, -40,
-121, -13, -40,
-121, 12, -40,
-115, 37, -40,
-105, 60, -40,
-90, 80, -40,
-71, 97, -40,
-50, 110, -40,
-26, 118, -40,
-1, 120, -40,
26, 123, -14,
51, 115, -14,
74, 102, -14,
93, 84, -14,
109, 63, -14,
120, 39, -14,
125, 13, -14,
125, -14, -14,
120, -40, -14,
109, -64, -14,
93, -85, -14,
74, -103, -14,
51, -116, -14,
26, -124, -14,
0, -127, -14,
-27, -124, -14,
-52, -116, -14,
-75, -103, -14,
-94, -85, -14,
-110, -64, -14,
-121, -40, -14,
-126, -14, -14,
-126, 13, -14,
-121, 39, -14,
-110, 63, -14,
-94, 84, -14,
-75, 102, -14,
-52, 115, -14,
-27, 123, -14,
-1, 126, -14,
26, 123, 13,
51, 115, 13,
74, 102, 13,
93, 84, 13,
109, 63, 13,
120, 39, 13,
125, 13, 13,
125, -14, 13,
120, -40, 13,
109, -64, 13,
93, -85, 13,
74, -103, 13,
51, -116, 13,
26, -124, 13,
0, -127, 13,
-27, -124, 13,
-52, -116, 13,
-75, -103, 13,
-94, -85, 13,
-110, -64, 13,
-121, -40, 13,
-126, -14, 13,
-126, 13, 13,
-121, 39, 13,
-110, 63, 13,
-94, 84, 13,
-75, 102, 13,
-52, 115, 13,
-27, 123, 13,
-1, 126, 13,
25, 118, 39,
49, 110, 39,
70, 97, 39,
89, 80, 39,
104, 60, 39,
114, 37, 39,
120, 12, 39,
120, -13, 39,
114, -38, 39,
104, -61, 39,
89, -81, 39,
70, -98, 39,
49, -111, 39,
25, -119, 39,
0, -121, 39,
-26, -119, 39,
-50, -111, 39,
-71, -98, 39,
-90, -81, 39,
-105, -61, 39,
-115, -38, 39,
-121, -13, 39,
-121, 12, 39,
-115, 37, 39,
-105, 60, 39,
-90, 80, 39,
-71, 97, 39,
-50, 110, 39,
-26, 118, 39,
-1, 120, 39,
22, 107, 63,
44, 100, 63,
64, 88, 63,
81, 73, 63,
95, 54, 63,
104, 33, 63,
109, 11, 63,
109, -12, 63,
104, -34, 63,
95, -55, 63,
81, -74, 63,
64, -89, 63,
44, -101, 63,
22, -108, 63,
0, -110, 63,
-23, -108, 63,
-45, -101, 63,
-65, -89, 63,
-82, -74, 63,
-96, -55, 63,
-105, -34, 63,
-110, -12, 63,
-110, 11, 63,
-105, 33, 63,
-96, 54, 63,
-82, 73, 63,
-65, 88, 63,
-45, 100, 63,
-23, 107, 63,
-1, 109, 63,
19, 92, 84,
38, 86, 84,
55, 76, 84,
70, 63, 84,
81, 47, 84,
89, 29, 84,
93, 9, 84,
93, -10, 84,
89, -30, 84,
81, -48, 84,
70, -64, 84,
55, -77, 84,
38, -87, 84,
19, -93, 84,
0, -95, 84,
-20, -93, 84,
-39, -87, 84,
-56, -77, 84,
-71, -64, 84,
-82, -48, 84,
-90, -30, 84,
-94, -10, 84,
-94, 9, 84,
-90, 29, 84,
-82, 47, 84,
-71, 63, 84,
-56, 76, 84,
-39, 86, 84,
-20, 92, 84,
-1, 94, 84,
15, 73, 102,
30, 68, 102,
43, 60, 102,
55, 49, 102,
64, 37, 102,
70, 23, 102,
74, 7, 102,
74, -8, 102,
70, -24, 102,
64, -38, 102,
55, -50, 102,
43, -61, 102,
30, -69, 102,
15, -74, 102,
0, -75, 102,
-16, -74, 102,
-31, -69, 102,
-44, -61, 102,
-56, -50, 102,
-65, -38, 102,
-71, -24, 102,
-75, -8, 102,
-75, 7, 102,
-71, 23, 102,
-65, 37, 102,
-56, 49, 102,
-44, 60, 102,
-31, 68, 102,
-16, 73, 102,
-1, 74, 102,
10, 50, 116,
21, 47, 116,
30, 41, 116,
38, 34, 116,
44, 25, 116,
49, 15, 116,
51, 5, 116,
51, -6, 116,
49, -16, 116,
44, -26, 116,
38, -35, 116,
30, -42, 116,
21, -48, 116,
10, -51, 116,
0, -52, 116,
-11, -51, 116,
-22, -48, 116,
-31, -42, 116,
-39, -35, 116,
-45, -26, 116,
-50, -16, 116,
-52, -6, 116,
-52, 5, 116,
-50, 15, 116,
-45, 25, 116,
-39, 34, 116,
-31, 41, 116,
-22, 47, 116,
-11, 50, 116,
-1, 51, 116,
5, 25, 124,
10, 24, 124,
15, 21, 124,
19, 17, 124,
22, 13, 124,
25, 8, 124,
26, 2, 124,
26, -3, 124,
25, -9, 124,
22, -14, 124,
19, -18, 124,
15, -22, 124,
10, -25, 124,
5, -26, 124,
0, -27, 124,
-6, -26, 124,
-11, -25, 124,
-16, -22, 124,
-20, -18, 124,
-23, -14, 124,
-26, -9, 124,
-27, -3, 124,
-27, 2, 124,
-26, 8, 124,
-23, 13, 124,
-20, 17, 124,
-16, 21, 124,
-11, 24, 124,
-6, 25, 124,
-1, 26, 124,
0, 0, 127,
};




GLuint elements [ ] = {
	0,1,2,3

};
GLuint elements2 [ ] = {
	3,2,1,0
};

GLuint elements3[] = {
	0, 1, 2,
0, 2, 3,
0, 3, 4,
0, 4, 5,
0, 5, 6,
0, 6, 7,
0, 7, 8,
0, 8, 9,
0, 9, 10,
0, 10, 11,
0, 11, 12,
0, 12, 13,
0, 13, 14,
0, 14, 15,
0, 15, 16,
0, 16, 17,
0, 17, 18,
0, 18, 19,
0, 19, 20,
0, 20, 21,
0, 21, 22,
0, 22, 23,
0, 23, 24,
0, 24, 25,
0, 25, 26,
0, 26, 27,
0, 27, 28,
0, 28, 29,
0, 29, 30,
0, 30, 1,
1, 32, 2,
32, 1, 31,
2, 33, 3,
33, 2, 32,
3, 34, 4,
34, 3, 33,
4, 35, 5,
35, 4, 34,
5, 36, 6,
36, 5, 35,
6, 37, 7,
37, 6, 36,
7, 38, 8,
38, 7, 37,
8, 39, 9,
39, 8, 38,
9, 40, 10,
40, 9, 39,
10, 41, 11,
41, 10, 40,
11, 42, 12,
42, 11, 41,
12, 43, 13,
43, 12, 42,
13, 44, 14,
44, 13, 43,
14, 45, 15,
45, 14, 44,
15, 46, 16,
46, 15, 45,
16, 47, 17,
47, 16, 46,
17, 48, 18,
48, 17, 47,
18, 49, 19,
49, 18, 48,
19, 50, 20,
50, 19, 49,
20, 51, 21,
51, 20, 50,
21, 52, 22,
52, 21, 51,
22, 53, 23,
53, 22, 52,
23, 54, 24,
54, 23, 53,
24, 55, 25,
55, 24, 54,
25, 56, 26,
56, 25, 55,
26, 57, 27,
57, 26, 56,
27, 58, 28,
58, 27, 57,
28, 59, 29,
59, 28, 58,
29, 60, 30,
60, 29, 59,
30, 31, 1,
31, 30, 60,
31, 62, 32,
62, 31, 61,
32, 63, 33,
63, 32, 62,
33, 64, 34,
64, 33, 63,
34, 65, 35,
65, 34, 64,
35, 66, 36,
66, 35, 65,
36, 67, 37,
67, 36, 66,
37, 68, 38,
68, 37, 67,
38, 69, 39,
69, 38, 68,
39, 70, 40,
70, 39, 69,
40, 71, 41,
71, 40, 70,
41, 72, 42,
72, 41, 71,
42, 73, 43,
73, 42, 72,
43, 74, 44,
74, 43, 73,
44, 75, 45,
75, 44, 74,
45, 76, 46,
76, 45, 75,
46, 77, 47,
77, 46, 76,
47, 78, 48,
78, 47, 77,
48, 79, 49,
79, 48, 78,
49, 80, 50,
80, 49, 79,
50, 81, 51,
81, 50, 80,
51, 82, 52,
82, 51, 81,
52, 83, 53,
83, 52, 82,
53, 84, 54,
84, 53, 83,
54, 85, 55,
85, 54, 84,
55, 86, 56,
86, 55, 85,
56, 87, 57,
87, 56, 86,
57, 88, 58,
88, 57, 87,
58, 89, 59,
89, 58, 88,
59, 90, 60,
90, 59, 89,
60, 61, 31,
61, 60, 90,
61, 92, 62,
92, 61, 91,
62, 93, 63,
93, 62, 92,
63, 94, 64,
94, 63, 93,
64, 95, 65,
95, 64, 94,
65, 96, 66,
96, 65, 95,
66, 97, 67,
97, 66, 96,
67, 98, 68,
98, 67, 97,
68, 99, 69,
99, 68, 98,
69, 100, 70,
100, 69, 99,
70, 101, 71,
101, 70, 100,
71, 102, 72,
102, 71, 101,
72, 103, 73,
103, 72, 102,
73, 104, 74,
104, 73, 103,
74, 105, 75,
105, 74, 104,
75, 106, 76,
106, 75, 105,
76, 107, 77,
107, 76, 106,
77, 108, 78,
108, 77, 107,
78, 109, 79,
109, 78, 108,
79, 110, 80,
110, 79, 109,
80, 111, 81,
111, 80, 110,
81, 112, 82,
112, 81, 111,
82, 113, 83,
113, 82, 112,
83, 114, 84,
114, 83, 113,
84, 115, 85,
115, 84, 114,
85, 116, 86,
116, 85, 115,
86, 117, 87,
117, 86, 116,
87, 118, 88,
118, 87, 117,
88, 119, 89,
119, 88, 118,
89, 120, 90,
120, 89, 119,
90, 91, 61,
91, 90, 120,
91, 122, 92,
122, 91, 121,
92, 123, 93,
123, 92, 122,
93, 124, 94,
124, 93, 123,
94, 125, 95,
125, 94, 124,
95, 126, 96,
126, 95, 125,
96, 127, 97,
127, 96, 126,
97, 128, 98,
128, 97, 127,
98, 129, 99,
129, 98, 128,
99, 130, 100,
130, 99, 129,
100, 131, 101,
131, 100, 130,
101, 132, 102,
132, 101, 131,
102, 133, 103,
133, 102, 132,
103, 134, 104,
134, 103, 133,
104, 135, 105,
135, 104, 134,
105, 136, 106,
136, 105, 135,
106, 137, 107,
137, 106, 136,
107, 138, 108,
138, 107, 137,
108, 139, 109,
139, 108, 138,
109, 140, 110,
140, 109, 139,
110, 141, 111,
141, 110, 140,
111, 142, 112,
142, 111, 141,
112, 143, 113,
143, 112, 142,
113, 144, 114,
144, 113, 143,
114, 145, 115,
145, 114, 144,
115, 146, 116,
146, 115, 145,
116, 147, 117,
147, 116, 146,
117, 148, 118,
148, 117, 147,
118, 149, 119,
149, 118, 148,
119, 150, 120,
150, 119, 149,
120, 121, 91,
121, 120, 150,
152, 121, 151,
122, 153, 123,
153, 122, 152,
123, 154, 124,
154, 123, 153,
124, 155, 125,
155, 124, 154,
125, 156, 126,
156, 125, 155,
126, 157, 127,
157, 126, 156,
127, 158, 128,
158, 127, 157,
128, 159, 129,
159, 128, 158,
129, 160, 130,
160, 129, 159,
130, 161, 131,
161, 130, 160,
131, 162, 132,
162, 131, 161,
132, 163, 133,
163, 132, 162,
133, 164, 134,
164, 133, 163,
134, 165, 135,
165, 134, 164,
135, 166, 136,
166, 135, 165,
136, 167, 137,
167, 136, 166,
137, 168, 138,
168, 137, 167,
138, 169, 139,
169, 138, 168,
139, 170, 140,
170, 139, 169,
140, 171, 141,
171, 140, 170,
141, 172, 142,
172, 141, 171,
142, 173, 143,
173, 142, 172,
143, 174, 144,
174, 143, 173,
144, 175, 145,
175, 144, 174,
145, 176, 146,
176, 145, 175,
146, 177, 147,
177, 146, 176,
147, 178, 148,
178, 147, 177,
148, 179, 149,
179, 148, 178,
149, 180, 150,
180, 149, 179,
150, 151, 121,
151, 150, 180,
151, 182, 152,
182, 151, 181,
152, 183, 153,
183, 152, 182,
153, 184, 154,
184, 153, 183,
154, 185, 155,
185, 154, 184,
155, 186, 156,
186, 155, 185,
156, 187, 157,
187, 156, 186,
157, 188, 158,
188, 157, 187,
158, 189, 159,
189, 158, 188,
159, 190, 160,
190, 159, 189,
160, 191, 161,
191, 160, 190,
161, 192, 162,
192, 161, 191,
162, 193, 163,
193, 162, 192,
163, 194, 164,
194, 163, 193,
164, 195, 165,
195, 164, 194,
165, 196, 166,
196, 165, 195,
166, 197, 167,
197, 166, 196,
167, 198, 168,
198, 167, 197,
168, 199, 169,
199, 168, 198,
169, 200, 170,
200, 169, 199,
170, 201, 171,
201, 170, 200,
171, 202, 172,
202, 171, 201,
172, 203, 173,
203, 172, 202,
173, 204, 174,
204, 173, 203,
174, 205, 175,
205, 174, 204,
175, 206, 176,
206, 175, 205,
176, 207, 177,
207, 176, 206,
177, 208, 178,
208, 177, 207,
178, 209, 179,
209, 178, 208,
179, 210, 180,
210, 179, 209,
180, 181, 151,
181, 180, 210,
181, 212, 182,
212, 181, 211,
182, 213, 183,
213, 182, 212,
183, 214, 184,
214, 183, 213,
184, 215, 185,
215, 184, 214,
185, 216, 186,
216, 185, 215,
186, 217, 187,
217, 186, 216,
187, 218, 188,
218, 187, 217,
188, 219, 189,
219, 188, 218,
189, 220, 190,
220, 189, 219,
190, 221, 191,
221, 190, 220,
191, 222, 192,
222, 191, 221,
192, 223, 193,
223, 192, 222,
193, 224, 194,
224, 193, 223,
194, 225, 195,
225, 194, 224,
195, 226, 196,
226, 195, 225,
196, 227, 197,
227, 196, 226,
197, 228, 198,
228, 197, 227,
198, 229, 199,
229, 198, 228,
199, 230, 200,
230, 199, 229,
200, 231, 201,
231, 200, 230,
201, 232, 202,
232, 201, 231,
202, 233, 203,
233, 202, 232,
203, 234, 204,
234, 203, 233,
204, 235, 205,
235, 204, 234,
205, 236, 206,
236, 205, 235,
206, 237, 207,
237, 206, 236,
207, 238, 208,
238, 207, 237,
208, 239, 209,
239, 208, 238,
209, 240, 210,
240, 209, 239,
210, 211, 181,
211, 210, 240,
211, 242, 212,
242, 211, 241,
212, 243, 213,
243, 212, 242,
213, 244, 214,
244, 213, 243,
214, 245, 215,
245, 214, 244,
215, 246, 216,
246, 215, 245,
216, 247, 217,
247, 216, 246,
217, 248, 218,
248, 217, 247,
218, 249, 219,
249, 218, 248,
219, 250, 220,
250, 219, 249,
220, 251, 221,
251, 220, 250,
221, 252, 222,
252, 221, 251,
222, 253, 223,
253, 222, 252,
223, 254, 224,
254, 223, 253,
224, 255, 225,
255, 224, 254,
225, 256, 226,
256, 225, 255,
226, 257, 227,
257, 226, 256,
227, 258, 228,
258, 227, 257,
228, 259, 229,
259, 228, 258,
229, 260, 230,
260, 229, 259,
230, 261, 231,
261, 230, 260,
231, 262, 232,
262, 231, 261,
232, 263, 233,
263, 232, 262,
233, 264, 234,
264, 233, 263,
234, 265, 235,
265, 234, 264,
235, 266, 236,
266, 235, 265,
236, 267, 237,
267, 236, 266,
237, 268, 238,
268, 237, 267,
238, 269, 239,
269, 238, 268,
239, 270, 240,
270, 239, 269,
240, 241, 211,
241, 240, 270,
241, 272, 242,
272, 241, 271,
242, 273, 243,
273, 242, 272,
243, 274, 244,
274, 243, 273,
244, 275, 245,
275, 244, 274,
245, 276, 246,
276, 245, 275,
246, 277, 247,
277, 246, 276,
247, 278, 248,
278, 247, 277,
248, 279, 249,
279, 248, 278,
249, 280, 250,
280, 249, 279,
250, 281, 251,
281, 250, 280,
251, 282, 252,
282, 251, 281,
252, 283, 253,
283, 252, 282,
253, 284, 254,
284, 253, 283,
254, 285, 255,
285, 254, 284,
255, 286, 256,
286, 255, 285,
256, 287, 257,
287, 256, 286,
257, 288, 258,
288, 257, 287,
258, 289, 259,
289, 258, 288,
259, 290, 260,
290, 259, 289,
260, 291, 261,
291, 260, 290,
261, 292, 262,
292, 261, 291,
262, 293, 263,
293, 262, 292,
263, 294, 264,
294, 263, 293,
264, 295, 265,
295, 264, 294,
265, 296, 266,
296, 265, 295,
266, 297, 267,
297, 266, 296,
267, 298, 268,
298, 267, 297,
268, 299, 269,
299, 268, 298,
269, 300, 270,
300, 269, 299,
270, 271, 241,
271, 270, 300,
271, 302, 272,
302, 271, 301,
272, 303, 273,
303, 272, 302,
273, 304, 274,
304, 273, 303,
274, 305, 275,
305, 274, 304,
275, 306, 276,
306, 275, 305,
276, 307, 277,
307, 276, 306,
277, 308, 278,
308, 277, 307,
278, 309, 279,
309, 278, 308,
279, 310, 280,
310, 279, 309,
280, 311, 281,
311, 280, 310,
281, 312, 282,
312, 281, 311,
282, 313, 283,
313, 282, 312,
283, 314, 284,
314, 283, 313,
284, 315, 285,
315, 284, 314,
285, 316, 286,
316, 285, 315,
286, 317, 287,
317, 286, 316,
287, 318, 288,
318, 287, 317,
288, 319, 289,
319, 288, 318,
289, 320, 290,
320, 289, 319,
290, 321, 291,
321, 290, 320,
291, 322, 292,
322, 291, 321,
292, 323, 293,
323, 292, 322,
293, 324, 294,
324, 293, 323,
294, 325, 295,
325, 294, 324,
295, 326, 296,
326, 295, 325,
296, 327, 297,
327, 296, 326,
297, 328, 298,
328, 297, 327,
298, 329, 299,
329, 298, 328,
299, 330, 300,
330, 299, 329,
300, 301, 271,
301, 300, 330,
301, 332, 302,
332, 301, 331,
302, 333, 303,
333, 302, 332,
303, 334, 304,
334, 303, 333,
304, 335, 305,
335, 304, 334,
305, 336, 306,
336, 305, 335,
306, 337, 307,
337, 306, 336,
307, 338, 308,
338, 307, 337,
308, 339, 309,
339, 308, 338,
309, 340, 310,
340, 309, 339,
310, 341, 311,
341, 310, 340,
311, 342, 312,
342, 311, 341,
312, 343, 313,
343, 312, 342,
313, 344, 314,
344, 313, 343,
314, 345, 315,
345, 314, 344,
315, 346, 316,
346, 315, 345,
316, 347, 317,
347, 316, 346,
317, 348, 318,
348, 317, 347,
318, 349, 319,
349, 318, 348,
319, 350, 320,
350, 319, 349,
320, 351, 321,
351, 320, 350,
321, 352, 322,
352, 321, 351,
322, 353, 323,
353, 322, 352,
323, 354, 324,
354, 323, 353,
324, 355, 325,
355, 324, 354,
325, 356, 326,
356, 325, 355,
326, 357, 327,
357, 326, 356,
327, 358, 328,
358, 327, 357,
328, 359, 329,
359, 328, 358,
329, 360, 330,
360, 329, 359,
330, 331, 301,
331, 330, 360,
331, 362, 332,
362, 331, 361,
332, 363, 333,
363, 332, 362,
333, 364, 334,
364, 333, 363,
334, 365, 335,
365, 334, 364,
335, 366, 336,
366, 335, 365,
336, 367, 337,
367, 336, 366,
337, 368, 338,
368, 337, 367,
338, 369, 339,
369, 338, 368,
339, 370, 340,
370, 339, 369,
340, 371, 341,
371, 340, 370,
341, 372, 342,
372, 341, 371,
342, 373, 343,
373, 342, 372,
343, 374, 344,
374, 343, 373,
344, 375, 345,
375, 344, 374,
345, 376, 346,
376, 345, 375,
346, 377, 347,
377, 346, 376,
347, 378, 348,
378, 347, 377,
348, 379, 349,
379, 348, 378,
349, 380, 350,
380, 349, 379,
350, 381, 351,
381, 350, 380,
351, 382, 352,
382, 351, 381,
352, 383, 353,
383, 352, 382,
353, 384, 354,
384, 353, 383,
354, 385, 355,
385, 354, 384,
355, 386, 356,
386, 355, 385,
356, 387, 357,
387, 356, 386,
357, 388, 358,
388, 357, 387,
358, 389, 359,
389, 358, 388,
359, 390, 360,
390, 359, 389,
360, 361, 331,
361, 360, 390,
361, 392, 362,
392, 361, 391,
362, 393, 363,
393, 362, 392,
363, 394, 364,
394, 363, 393,
364, 395, 365,
395, 364, 394,
365, 396, 366,
396, 365, 395,
366, 397, 367,
397, 366, 396,
367, 398, 368,
398, 367, 397,
368, 399, 369,
399, 368, 398,
369, 400, 370,
400, 369, 399,
370, 401, 371,
401, 370, 400,
371, 402, 372,
402, 371, 401,
372, 403, 373,
403, 372, 402,
373, 404, 374,
404, 373, 403,
374, 405, 375,
405, 374, 404,
375, 406, 376,
406, 375, 405,
376, 407, 377,
407, 376, 406,
377, 408, 378,
408, 377, 407,
378, 409, 379,
409, 378, 408,
379, 410, 380,
410, 379, 409,
380, 411, 381,
411, 380, 410,
381, 412, 382,
412, 381, 411,
382, 413, 383,
413, 382, 412,
383, 414, 384,
414, 383, 413,
384, 415, 385,
415, 384, 414,
385, 416, 386,
416, 385, 415,
386, 417, 387,
417, 386, 416,
387, 418, 388,
418, 387, 417,
388, 419, 389,
419, 388, 418,
389, 420, 390,
420, 389, 419,
390, 391, 361,
391, 390, 420,
391, 421, 392,
392, 421, 393,
393, 421, 394,
394, 421, 395,
395, 421, 396,
396, 421, 397,
397, 421, 398,
398, 421, 399,
399, 421, 400,
400, 421, 401,
401, 421, 402,
402, 421, 403,
403, 421, 404,
404, 421, 405,
405, 421, 406,
406, 421, 407,
407, 421, 408,
408, 421, 409,
409, 421, 410,
410, 421, 411,
411, 421, 412,
412, 421, 413,
413, 421, 414,
414, 421, 415,
415, 421, 416,
416, 421, 417,
417, 421, 418,
418, 421, 419,
419, 421, 420,
420, 421, 391,
};


float textureCoords [ ] = {
	0,1,
	1,1,
	1,0,
	0,0, };

float textureCoords2 [ ] = {
	0,0,
	0,2,
	3,2,
	3,0 };


//shaders
GLuint programID = 0, programID2=0;

unsigned int VBO , VBO2 , VBO3 , VBO4, ebo, ebo2 , ebo3 , ebo4, vtex , vtex2;
GLuint  tex_id0 , tex_id1;
unsigned int VAO [ 4 ];

double dx = -5;
double alpha = 1.0;
/*###############################################################*/
void mysz ( int button , int state , int x , int y )
{
	mbutton = button;
	switch (state)
	{
	case GLUT_UP:
		break;
	case GLUT_DOWN:
		pozycjaMyszyX = x;
		pozycjaMyszyY = y;
		poprzednie_kameraX = kameraX;
		poprzednie_kameraZ = kameraZ;
		poprzednie_kameraD = kameraD;
		break;

	}
}
/*******************************************/
void mysz_ruch ( int x , int y )
{
	if (mbutton == GLUT_LEFT_BUTTON)
	{
		kameraX = poprzednie_kameraX - ( pozycjaMyszyX - x ) * 0.1;
		kameraZ = poprzednie_kameraZ - ( pozycjaMyszyY - y ) * 0.1;
	}
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
		kameraD = poprzednie_kameraD + ( pozycjaMyszyY - y ) * 0.1;
	}

}
/******************************************/



void klawisz ( GLubyte key , int x , int y )
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit ( 1 );
		break;

	case 'x':
	
		break;
	case 'c':
	
		break;


	case '1':

		break;
	case '2':

		break;
	}

	
}

GLfloat offset = 0.0f;

/*###############################################################*/
void rysuj ( void )
{

	//glClearColor(0.8f, 0.8f, 1.0f, 1.0f); //jasnoniebieskie tło
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //białe tło
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Kasowanie ekranu
	glUseProgram ( programID2); //program do kolorow
	GLuint MVP_id = glGetUniformLocation ( programID , "MVP" );
	MV = glm::mat4 ( 1.0f );  //macierz jednostkowa 
	MV = glm::translate ( MV , glm::vec3 ( 1, -1, kameraD + dx) );
	MV = glm::rotate ( MV , ( float ) glm::radians ( kameraZ ) , glm::vec3 ( 1 , 0 , 0 ) );
	MV = glm::rotate ( MV , ( float ) glm::radians ( kameraX ) , glm::vec3 ( 0 , 1 , 0 ) );
	glm::mat4 MVP = P * MV;

	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
	//glUseProgram(programID2); //program do kolorow

	glBindVertexArray(VAO[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glActiveTexture ( GL_TEXTURE0 );
	//glBindTexture ( GL_TEXTURE_2D , tex_id1 );
	//glUniform1i ( uniformTex1 , 0 );
	glVertexAttrib4f(2, 1.0, 1.0, 0.0, 1.0); //kolor żółty
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);

	// Narysowanie zielonej ramki 
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glVertexAttrib4f(2, 0.5, 0.5, 0.5, 1.0); // Czarny zielony
	glVertexAttrib4f(2, 0.0, 1.0, 0.0, 1.0); // Czarny zielony
	glLineWidth(1.0f); // Grubość linii
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);

	//blue
	glUseProgram(programID); //program do tekstury


	/*MV1 = glm::mat4(1.0f);
	MV1 = glm::translate(MV, glm::vec3(0.0f, 0.0f, -0.5f));
	MV1 = glm::translate(MV, glm::vec3(0.0f, 0.0f, offset)); // Przesunięcie
	MVP = P * MV1;*/


	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);



	glBindVertexArray(VAO[2]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id0);
	glUniform1i(uniformTex1, 0);

	//glVertexAttrib4f(2, 0.0, 0.0, 1.0, 0.3);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);


	// Narysowanie zielonej ramki wokół podstawy
	/*glUseProgram(programID2); //program do kolorow
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glVertexAttrib4f(2, 0.0, 1.0, 0.0, 1.0); // Czarny zielony
	glLineWidth(4.0f); // Grubość linii
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);*/

	//Figura
	glUseProgram(programID2); //program do kolorow
	//MV = glm::translate(MV, glm::vec3(0.0f, 1.0f, -2.0f)); // Przesunięcie
	MV = glm::translate(MV, glm::vec3(0.0f, 1.8f, offset)); // Przesunięcie
	MV = glm::scale(MV, glm::vec3(0.006f)); // Skalowanie 
	MVP = P * MV;
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

	glBindVertexArray(VAO[3]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glVertexAttrib4f(2, 1.0, 1.0, 0.0, 1.0); //kolor żółty
	//glVertexAttrib4f(2, 1.0, 0.0, 0.0, 1.0); //kolor czerwony
	glVertexAttrib4f(2, 0.0, 0.0, 1.0, 1.0); // kolor niebieski
	glLineWidth(1.0f); // Grubość linii
	glDrawElements(GL_TRIANGLES, 2500, GL_UNSIGNED_INT, 0);


	/*//zielony kwadrat
	
	glBindVertexArray ( VAO [ 1 ] );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id1);
	glUniform1i(uniformTex0, 0);
	//glVertexAttrib4f(2, 0.0, 1.0, 0.0, 1.0);
	glDrawElements ( GL_QUADS , 4 , GL_UNSIGNED_INT , 0 );

	// Narysowanie czarnej nieprzeźroczystej ramki wokół zielonego kwadratu
	glUseProgram(programID2); //program do kolorow
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glVertexAttrib4f(2, 0.0, 0.0, 0.0, 1.0); // Czarny kolor
	glLineWidth(2.0f); // Grubość linii
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);*/

	
	//blue
	//glUseProgram(programID); //program do tekstury


	/*MV1 = glm::mat4(1.0f);
	MV1 = glm::translate(MV, glm::vec3(0.0f, 0.0f, -0.5f));
	MV1 = glm::translate(MV, glm::vec3(0.0f, 0.0f, offset)); // Przesunięcie
	MVP = P * MV1;*/


	/*glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);

	

	glBindVertexArray ( VAO [ 2 ] );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glActiveTexture ( GL_TEXTURE0 );
	glBindTexture ( GL_TEXTURE_2D , tex_id0 );
	glUniform1i ( uniformTex1 , 0 );
	
	//glVertexAttrib4f(2, 0.0, 0.0, 1.0, 0.3);
	glDrawElements ( GL_QUADS , 4 , GL_UNSIGNED_INT , 0 );

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);*/

	/*// Narysowanie czarnej nieprzeźroczystej ramki wokół niebieskiego kwadratu
	glUseProgram(programID2); //program do kolorow
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glVertexAttrib4f(2, 0.0, 0.0, 0.0, 1.0); // Czarny kolor
	glLineWidth(2.0f); // Grubość linii
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);*/
	


	glFlush ( );
	glutSwapBuffers ( );

}
/*###############################################################*/
void rozmiar ( int width , int height )
{
	screen_width = width;
	screen_height = height;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport ( 0 , 0 , screen_width , screen_height );

	P = glm::perspective ( glm::radians ( 60.0f ) , ( GLfloat ) screen_width / ( GLfloat ) screen_height , 1.0f , 1000.0f );

	glutPostRedisplay ( ); // Przerysowanie sceny
}

/*###############################################################*/
void idle ( )
{

	glutPostRedisplay ( );
}

/*###############################################################*/

GLfloat k = 0.1f;

void timer ( int value ) {

	/////////////////////////
	offset += k;

	if (offset > 0.5 || offset < -8)
		k = -k;

	glutPostRedisplay();
	glutTimerFunc ( 20 , timer , 0 );
}
/*###############################################################*/
int main ( int argc , char** argv )
{
	glutInit ( &argc , argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize ( screen_width , screen_height );
	glutInitWindowPosition ( 0 , 0 );
	glutCreateWindow ( "Szablon" );

	glewInit ( ); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc ( rysuj );			// def. funkcji rysuj¦cej
	glutIdleFunc ( idle );			// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	glutTimerFunc ( 20 , timer , 0 );
	glutReshapeFunc ( rozmiar ); // def. obs-ugi zdarzenia resize (GLUT)

	glutKeyboardFunc ( klawisz );		// def. obsługi klawiatury
	glutMouseFunc ( mysz ); 		// def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc ( mysz_ruch ); // def. obsługi zdarzenia ruchu myszy (GLUT)


	glEnable ( GL_DEPTH_TEST );

	/*//green
	glGenBuffers ( 1 , &VBO2 );
	glBindBuffer ( GL_ARRAY_BUFFER , VBO2 );
	glBufferData ( GL_ARRAY_BUFFER , sizeof ( vertices2 ) , vertices2 , GL_STATIC_DRAW );

	glGenBuffers ( 1 , &ebo2 );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , ebo2 );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER , sizeof ( elements ) , elements , GL_STATIC_DRAW );

	glGenBuffers(1, &vtex2);
	glBindBuffer ( GL_ARRAY_BUFFER , vtex2 );
	glBufferData ( GL_ARRAY_BUFFER , sizeof ( textureCoords2 ) , textureCoords2 , GL_STATIC_DRAW );*/

	//blue
	glGenBuffers ( 1 , &VBO3 );
	glBindBuffer ( GL_ARRAY_BUFFER , VBO3 );
	glBufferData ( GL_ARRAY_BUFFER , sizeof ( vertices3 ) , vertices3 , GL_STATIC_DRAW );

	glGenBuffers ( 1 , &ebo3 );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , ebo3 );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER , sizeof ( elements2 ) , elements2 , GL_STATIC_DRAW );

	glGenBuffers(1, &vtex);
	glBindBuffer ( GL_ARRAY_BUFFER , vtex );
	glBufferData ( GL_ARRAY_BUFFER , sizeof ( textureCoords ) , textureCoords , GL_STATIC_DRAW );



	//podloga
	glGenBuffers ( 1 , &VBO );
	glBindBuffer ( GL_ARRAY_BUFFER , VBO );
	glBufferData ( GL_ARRAY_BUFFER , sizeof ( vertices ) , vertices , GL_STATIC_DRAW );

	glGenBuffers ( 1 , &ebo );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , ebo );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER , sizeof ( elements2 ) , elements2, GL_STATIC_DRAW );

	/*glGenBuffers(1, &vtex);
	glBindBuffer ( GL_ARRAY_BUFFER , vtex );
	glBufferData ( GL_ARRAY_BUFFER , sizeof ( textureCoords ) , textureCoords , GL_STATIC_DRAW );*/

	
	glGenBuffers(1, &vtex2);
	glBindBuffer ( GL_ARRAY_BUFFER , vtex2 );
	glBufferData ( GL_ARRAY_BUFFER , sizeof ( textureCoords2 ) , textureCoords2 , GL_STATIC_DRAW );

	//KULA
	glGenBuffers(1, &VBO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFigura) , verticesFigura, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements3), elements3, GL_STATIC_DRAW);


	programID = loadShaders ( "vertex_shader.glsl" , "fragment_shader.glsl" );

	programID2 = loadShaders ( "vertex_shader.glsl" , "fragment_shader2.glsl" );

	tex_id0 = WczytajTeksture ( "zdjecie.bmp" );
	if (tex_id0 == -1)
	{
		MessageBox ( NULL , "Nie znaleziono pliku z teksturą" , "Problem" , MB_OK | MB_ICONERROR );
		exit ( 0 );
	}

	tex_id1 = WczytajTeksture ( "sand.bmp" );
	if (tex_id1 == -1)
	{
		MessageBox ( NULL , "Nie znaleziono pliku z teksturą" , "Problem" , MB_OK | MB_ICONERROR );
		exit ( 0 );
	}


	glGenVertexArrays ( 4 , VAO );
	
	//podloga
	glBindVertexArray ( VAO [ 0 ] );
	glBindBuffer(GL_ARRAY_BUFFER, vtex2);
	glEnableVertexAttribArray ( 1 );
	glVertexAttribPointer ( 1 , 2 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindBuffer ( GL_ARRAY_BUFFER , VBO );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , ebo );
	glEnableVertexAttribArray ( 0 );
	glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , ( void* ) 0 );

	//blue
	glBindVertexArray ( VAO [ 2 ] );
	glBindBuffer(GL_ARRAY_BUFFER, vtex);
	glEnableVertexAttribArray ( 1 );
	glVertexAttribPointer ( 1 , 2 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindBuffer ( GL_ARRAY_BUFFER , VBO3 );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , ebo3 );
	glEnableVertexAttribArray ( 0 );
	glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , ( void* ) 0 );

	//green
	/*glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vtex2);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer ( GL_ARRAY_BUFFER , VBO2 );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , ebo2 );
	glEnableVertexAttribArray ( 0 ); 
	glVertexAttribPointer (0 , 3 , GL_FLOAT ,GL_FALSE ,0 ,( void* ) 0 );*/

	//Figura
	glBindVertexArray(VAO[3]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo4);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glutMainLoop ( );					// start

	glDeleteBuffers ( 1 , &VBO );
	glDeleteBuffers ( 1 , &ebo );
	glDeleteBuffers ( 1 , &vtex );
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &ebo2);
	glDeleteBuffers(1, &vtex2);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &ebo3);
	glDeleteBuffers ( 3 , VAO );
	return( 0 );

}

