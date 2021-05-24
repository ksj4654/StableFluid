#pragma once


#include <stdio.h>

class Solver
{
public : 
    Solver();
     ~Solver();
        
    void init(int n);
    void update();
    void addSource();
    void reset();

    const float* getDensity();
    float getDensity(int x, int y, int z);

    const float* getVelocityU();
    const float* getVelocityV();
    const float* getVelocityW();
    float getVelocityU(int x, int y, int z);
    float getVelocityV(int x, int y, int z);
    float getVelocityW(int x, int y, int z);
 
 private:
 
    void add_source(float* value, float* value_prev);

    void wuSource();
    
    void boundary_condition(float* value, int flag);
    
    void lin_solve(int b, float* value, float* value_prev, float a, float c);
    
    void diffuse(int b, float* value, float* value_prev);

    void advect(int b, float* density, float* density_prev, float* velocity_u, float* velocity_v, float* velocity_w);

    void project(float* velocity_u, float* velocity_v, float* velocity_w, float* p, float* div);

    void get_density(float* density, float* density_prev, float* velocity_u, float* velocity_v, float* velocity_w);

    void get_velocity(float* velocity_u, float* velocity_v, float* velocity_w, float* velocity_u_prev, float* velocity_v_prev, float* velocity_w_prev);

    int wuIndex(int i, int j, int k);

private :

    int N;

   float timeStep;
   float diffuseCoef;
   float viscocity;
   float force;
   float source;

   float* velocityU;
   float* velocityV;
   float* velocityW;

   float* previousVelocityU;
   float* previousVelocityV;
   float* previousVelocityW;

   float* density;
   float* previousDensity;

   bool addDensity;
};