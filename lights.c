//// This is the lightmap (darkmap) code for
//// the dynamic lighting for glDoom.
//
//#include <windows.h>
//#include <math.h>
//#include <stdio.h>
//
//#include "sys_sdl.h"
//#include "doomdata.h"
//
////#define DegreesToRadians((d/360.0f)*M_PI*2.0f)
//
//void lfprintf(char *message, ... );
//
//float AngleOff( float x1, float z1, float x2, float z2 );
//
//typedef struct
//   {
//    float  x;
//    float  y;
//    float  z;
//   }fpoint_t;
//
//DW_Polygon  TestPoly;
//DW_Vertex3D TestPoint;
//DW_Polygon  LightMap;
//
//float CheckXZAltitude(DW_Polygon *poly, DW_Vertex3D *light, DW_Polygon *LightMap, float radius );
//
//void TestAlt()
//   {
//    fpoint_t p1, p2, p3, p4;
//    float    radius, altitude;
//
//    p1.x = 4.0f;
//    p1.z = 6.0f;
//
//    p2.x = 1.0f;
//    p2.z = 1.0f;
//
//    p3.x = 7.0f;
//    p3.z = 1.0f;
//
//    radius = 6.5f;
//
//    altitude = CheckXZAltitude( &TestPoly, &TestPoint, &LightMap, radius);
//
//    lfprintf("Altitude = %f, p4 = %f,%f\n", altitude, p4.x, p4.z);
//   }
//
//
//// This will work better with passing the polygon pointer to it.
//// All vertices are needed in order to check altitude, then find projection
//// point.  Then texture coordinates need to be made (based on ratio of
//// distances from light source to vertices)
//// Whether the light source is above, below or "contained" in the quad is
//// needed for vertical texture coordinates.
//// Whether the light source is left, right or "contained" in the quad is
//// needed for horizontal texture coordinates.
//// Two different "systems" will apply.
//// Vertical height is 'Y' so it can be measured directly against the 'Y'
//// coordinate of verts 0 and 1.
//// Horizontal coordinates may be "variant" so the distance between the
//// ends of the quad must be tested against the distance from the light
//// source to each of the ends of the quad (verts 0 & 3 or 1 & 2).
//
//float CheckXZAltitude(DW_Polygon *poly, DW_Vertex3D *light, DW_Polygon *lmap, float radius )
//   {
//    float      angle; // angle in radians
//    float      xd, zd, a, as, b, bs, cs, c, d;
//    float      l, ls, s, os, alt, r2, vt, vb;
//    float      vx, vz, px, pz, ppx, ppz, diam;
//    int        i;
//
//    xd = (light->x-poly->Point[3].v[0]);
//    zd = (light->z-poly->Point[3].v[2]);
//    as = (xd*xd)+(zd*zd); // squared segment length
//    a  = sqrt(as);        // actual segment length
//
//    xd = (poly->Point[3].v[0]-poly->Point[0].v[0]);
//    zd = (poly->Point[3].v[2]-poly->Point[0].v[2]);
//    bs = (xd*xd)+(zd*zd); // squared segment length
//    b  = sqrt(bs);        // actual segment length
//
//    xd = (poly->Point[0].v[0]-light->x);
//    zd = (poly->Point[0].v[2]-light->z);
//    cs = (xd*xd)+(zd*zd); // squared segment length
//    c  = sqrt(cs);        // actual segment length
//
//    // use the "longest" of the two legs to make sure
//    // it's an "inside" angle
//    if (a < c)
//       {
//        ls = as;
//        s = a;
//        os = cs;
//       }
//    else
//       {
//        ls = cs;
//        s = c;
//        os = as;
//       }
//
//    // Law of Cosines
//    // 2ab cos Y = a*a + b*b - c*c
//    // cos Y = (a*a + b*b - c*c) / 2ab
//
//    angle = acos(((os+bs)-ls)/(2*s*b));
//
//    // find length of adjacent side of right triangle
//    d = s * cos(angle);
//
//    // Pythagorean Theorem
//    // hyp squared minus adj squared yields square of opp
//    alt = sqrt(as-(d*d));  // this is the altitude of the point over the plane
//
//    if (alt >= radius)
//       {
//        return -1.0f;
//       }
//
//    // project point onto plane
//    // line vector with start point and distance yields projected point.
//
//    // find unit vector of "base" line...
//    // get the directional vector of line segment
//    // p2-p3 (the only REAL line in this)
//    if (a > c)
//       {
//        px = poly->Point[3].v[0];
//        pz = poly->Point[3].v[2];
//        xd = (poly->Point[0].v[0] - poly->Point[3].v[0]);
//        zd = (poly->Point[0].v[2] - poly->Point[3].v[2]);
//       }
//    else
//       {
//        px = poly->Point[0].v[0];
//        pz = poly->Point[0].v[2];
//        xd = (poly->Point[3].v[0] - poly->Point[0].v[0]);
//        zd = (poly->Point[3].v[2] - poly->Point[0].v[2]);
//       }
//    // divide the vector "parts" by the length
//    // to make the vector a "unit" vector
//    vx = xd / b;
//    vz = zd / b;
//
//    // calculate the new "point" by adding the endpoint coords to
//    // the unit vector "parts" multiplied by the length "d"
//    for (i = 0; i < 4; i++)
//       {
//        lmap->Point[i].v[0] = poly->Point[i].v[0];
//        lmap->Point[i].v[1] = poly->Point[i].v[1];
//        lmap->Point[i].v[2] = poly->Point[i].v[2];
//       }
//
//    ppx = px + (vx * d);
//    ppz = pz + (vz * d);
//
//    diam = radius * 2;
//    r2 = radius * radius;
//
//
//    // Calculate the horizontal texture coordinates
//
//    if ((a <= b) && (c <= b))
//       { // light is "within" the wall polygon projection
//        if (c >= radius)
//            lmap->Point[0].tu = 0.0f - (c / diam);
//        else
//            lmap->Point[0].tu = 1.0f - ((c + radius) / diam);
//        if (a >= radius)
//            lmap->Point[3].tu = (a - radius) / diam;
//        else
//            lmap->Point[3].tu = 1.0f - ((a + radius) / diam);
//       }
//    else
//    if (a < c)  // light is to the right of the wall poly
//       {
//        lmap->Point[3].tu = (diam - a) / diam;
//        lmap->Point[0].tu = (b / diam) - lmap->Point[3].tu;
//       }
//    else // c < a - light is to the left of the wall poly
//       {
//        lmap->Point[0].tu = (diam - c) / diam;
//        lmap->Point[3].tu = (b / diam) - lmap->Point[0].tu;
//       }
//
//    lmap->Point[1].tu = lmap->Point[0].tu;
//    lmap->Point[2].tu = lmap->Point[3].tu;
//
//    // Calculate the vertical polygon coordinates
//
//    if ((light->y + radius) < poly->Point[0].v[1])
//       {
//        vt = light->y + radius;
//       }
//    else
//       {
//        vt = poly->Point[0].v[1];
//       }
//
//    if (light->y > poly->Point[1].v[1])
//        vb = poly->Point[1].v[1];
//    else
//        vb = poly->Point[1].v[1];
//
//    if (light->y > poly->Point[0].v[1])
//       { // light is "above" the polygon
//        lmap->Point[0].tv = (diam-(vt+radius))/diam;
//        lmap->Point[1].tv = vt/diam;
//       }
//    else
//    if (light->y < poly->Point[1].v[1])
//       { // light is "below" the polygon
//       }
//    else
//       { // light is "within" the projected polygon
//       }
//
//    return alt;
//   }
//
////lightmap (walls) pseudo-code
//
///*
//for (side = 0; side < sidedefs; side++)
//   {
//    if (drawside[side] == true)
//       {
//        find distance to endpoints
//        find angle off the wall plane of longest line
//            (will always be an "inside" angle)
//        angle and length of two sides will yield base
//            of Isoceles then distance to base from apex
//            through Pythagorean Theorem
//
//        find nearest point to wall plane from emitter. (done with altitude)
//
//        if (distance to wall plane from emitter > light radius)
//           {
//            continue with next wall
//           }
//        if (nearest point of wall plane to emitter is "off the wall")
//           {
//            find nearest endpoint on wall and get distance there
//            if (distance to wall endpoint from emitter > light radius)
//               {
//                continue with next wall
//               }
//           }
//        distance to wall plane determines amount of light to add
//        (light is, after all, an additive phenomenon)
//        // rules
//        // 1 Light must not fall off at edges below ambient light level.
//        // 2 Light must be centered on point in plane where normal points
//        //   directly to the emitter.
//        // 3 Light color is determined by the color of the emitter.
//        // 4 Dynamic lights cast no shadows.
//        // 5 Walls may be lit from behind. (unavoidable - because of light
//        //   shining on floors)
//       }
//   }
//
//*/
//
//float AngleTo( float x1, float z1, float x2, float z2 )
//   {
//    int       quad, a, b;
//    float     angle, af, bf;
//
//    quad = -1;
//
//    angle = 0.0f;
//
//    a = x2 - x1;
//    b = z1 - z2;
//    af = x2 - x1;
//    bf = z2 - z1;
//
//    if (( af > 0) && (bf > 0))
//        quad = 0;
//    else
//    if (( af > 0) && (bf < 0))
//        quad = 1;
//    else
//    if (( af < 0) && (bf < 0))
//        quad = 2;
//    else
//    if (( af < 0) && (bf > 0))
//        quad = 3;
//    if (af == 0)
//       {
//        if (bf > 0)
//           angle = 0.0f;
//        else
//        if (bf < 0)
//           angle = 180.0f;
//       }
//    else
//    if (bf == 0)
//       {
//        if (af > 0)
//           angle = 90.0f;
//        else
//        if (af < 0)
//           angle = 270.0f;
//       }
//
//    //con_printf("af = %.0f : bf = %.0f\n", af, bf);
//    //con_printf("quad = %d\n", quad);
//    switch(quad)
//       {
//        case 0:
//             if (af == bf)
//                 angle = 45.0f;
//             else
//             if (af < bf)
//                 angle = (af / bf) * 45.0f;
//             else
//                 angle = 90.0f - ((bf / af) * 45.0f);
//                 
//             break;
//        case 1:
//             bf *= -1.0f;
//             if (af == bf)
//                angle = 135.0f;
//             else
//             if (bf < af)
//                 angle =  90.0f + ((bf / af) * 45.0f);
//             else
//                 angle = 180.0f - ((af / bf) * 45.0f);
//             break;
//        case 2:
//             af *= -1.0f;
//             bf *= -1.0f;
//             if (af == bf)
//                angle = 225.0f;
//             else
//             if (bf < af)
//                 angle = 270.0f - ((bf / af) * 45.0f);
//             else
//                 angle = 180.0f + ((af / bf) * 45.0f);
//             break;
//        case 3:
//             af *= -1.0f;
//             if (af == bf)
//                angle = 315.0f;
//             else
//             if (bf < af)
//                 angle = 270.0f + ((bf / af) * 45.0f);
//             else
//                 angle = 360.0f - ((af / bf) * 45.0f);
//             break;
//       }
//    return angle;
//   }
