#pragma once

#define IX(i,j,k) ((i)+(N+2)*(j) + (N+2)*(N+2)*(k))
#define SWAP(x0,x) {float * tmp=x0;x0=x;x=tmp;}
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "solver.h"

Solver::Solver()
{
}
 
Solver::~Solver()
{
    delete  velocityU;
    delete  velocityV;
    delete  velocityW;
       
    delete  previousVelocityU;
    delete  previousVelocityV;
    delete  previousVelocityW;
       
    delete  density;
    delete  previousDensity;
}

void Solver::init(int n)
{
    N = n;

    timeStep = 0.1f;
    diffuseCoef = 0.0f;
    viscocity = 0.0f;
    force = 90.0f;
    source = 800.0f;
    addDensity = false;

    const int eachGridCount = N + 2;
    const int size = eachGridCount * eachGridCount * eachGridCount;

    velocityU = (float*)malloc(size * sizeof(float));
    velocityV = (float*)malloc(size * sizeof(float));
    velocityW = (float*)malloc(size * sizeof(float));

    previousVelocityU = (float*)malloc(size * sizeof(float));
    previousVelocityV = (float*)malloc(size * sizeof(float));
    previousVelocityW = (float*)malloc(size * sizeof(float));

    density = (float*)malloc(size * sizeof(float));
    previousDensity = (float*)malloc(size * sizeof(float));

    memset(velocityU, 0, sizeof(float) * size);
    memset(velocityV, 0, sizeof(float) * size);
    memset(velocityW, 0, sizeof(float) * size);

    memset(previousVelocityU, 0, sizeof(float) * size);
    memset(previousVelocityV, 0, sizeof(float) * size);
    memset(previousVelocityW, 0, sizeof(float) * size);

    memset(density, 0, sizeof(float) * size);
    memset(previousDensity, 0, sizeof(float) * size);
}

void Solver::update()
{
    wuSource();
    get_density(density, previousDensity, velocityU, velocityV, velocityW);
    get_velocity(velocityU, velocityV, velocityW, previousVelocityU, previousVelocityV, previousVelocityW);
}

void Solver::addSource()
{
    addDensity = true;
}

void Solver::reset()
{
    const int size = (N + 2) * (N + 2) * (N + 2);

    memset(velocityU, 0, sizeof(float) * size);
    memset(velocityV, 0, sizeof(float) * size);
    memset(velocityW, 0, sizeof(float) * size);

    memset(previousVelocityU, 0, sizeof(float) * size);
    memset(previousVelocityV, 0, sizeof(float) * size);
    memset(previousVelocityW, 0, sizeof(float) * size);

    memset(density, 0, sizeof(float) * size);
    memset(previousDensity, 0, sizeof(float) * size);
}

const float* Solver::getDensity()
{
    return density;
}

float Solver::getDensity(int x, int y, int z)
{
    return density[wuIndex(x, y, z)];
}

const float* Solver::getVelocityU()
{
    return velocityU;
}

const float* Solver::getVelocityV()
{
    return velocityV;
}

const float* Solver::getVelocityW()
{
    return velocityW;
}

float Solver::getVelocityU(int x, int y, int z)
{
    return velocityU[wuIndex(x, y, z)];
}

float Solver::getVelocityV(int x, int y, int z)
{
    return velocityV[wuIndex(x, y, z)];
}

float Solver::getVelocityW(int x, int y, int z)
{
    return velocityW[wuIndex(x, y, z)];
}

void Solver::add_source(float* value, float* value_prev)
{
    int count;
    int size = (N + 2) * (N + 2) * (N + 2);

    for (count = 0; count < size; count++)
    {
        value[count] = value[count] + timeStep * value_prev[count];
    }
}

void Solver::wuSource()
{
    float* density = previousDensity;
    float* u = previousVelocityU;
    float* v = previousVelocityV;
    float* w = previousVelocityW;

    int indexX, indexY, indexZ;
    const int eachGridSize = N + 2;
    const int size = eachGridSize * eachGridSize * eachGridSize;

    memset(u, 0, sizeof(float) * size);
    memset(v, 0, sizeof(float) * size);
    memset(w, 0, sizeof(float) * size);
    memset(density, 0, sizeof(float) * size);

    if (addDensity == true)
    {
        indexX = N / 2;
        indexY = N / 4 * 3;
        indexZ = N / 2;

        density[wuIndex(indexX, indexY, indexZ)] = source;
        addDensity = false;
    }

    for (int x = 1; x < N; x++)
    {
        for (int y = 1; y < N; y++)
        {
            for (int z = 1; z < N; z++)
            {
                if (density[wuIndex(x, y, z)] > 0)
                {
                    v[wuIndex(x, y, z)] = v[wuIndex(x, y, z)] - density[wuIndex(x, y, z)] * 0.2 * (y + 1) / N;
                }
            }
        }
    }
}

void Solver::boundary_condition(float* value, int flag)
{
    int count_a;
    int count_b;

    for (count_a = 1; count_a <= N; count_a++)
    {
        for (count_b = 1; count_b <= N; count_b++)
        {
            if (flag == 1)
            {
                value[IX(0, count_a, count_b)] = -value[IX(1, count_a, count_b)];
                value[IX(N + 1, count_a, count_b)] = -value[IX(N, count_a, count_b)];
            }
            else
            {
                value[IX(0, count_a, count_b)] = value[IX(1, count_a, count_b)];
                value[IX(N + 1, count_a, count_b)] = value[IX(N, count_a, count_b)];
            }

            if (flag == 2)
            {
                value[IX(count_a, 0, count_b)] = -value[IX(count_a, 1, count_b)];
                value[IX(count_a, N + 1, count_b)] = -value[IX(count_a, N, count_b)];
            }
            else
            {
                value[IX(count_a, 0, count_b)] = value[IX(count_a, 1, count_b)];
                value[IX(count_a, N + 1, count_b)] = value[IX(count_a, N, count_b)];
            }

            if (flag == 3)
            {
                value[IX(count_a, count_b, 0)] = -value[IX(count_a, count_b, 1)];
                value[IX(count_a, count_b, N + 1)] = -value[IX(count_a, count_b, N)];
            }
            else
            {
                value[IX(count_a, count_b, 0)] = value[IX(count_a, count_b, 1)];
                value[IX(count_a, count_b, N + 1)] = value[IX(count_a, count_b, N)];
            }
        }
    }

    value[IX(0, 0, 0)] = (float)(1.0 / 3.0 * (value[IX(1, 0, 0)] + value[IX(0, 1, 0)] + value[IX(0, 0, 1)]));
    value[IX(0, N + 1, 0)] = (float)(1.0 / 3.0 * (value[IX(1, N + 1, 0)] + value[IX(0, N, 0)] + value[IX(0, N + 1, 1)]));

    value[IX(N + 1, 0, 0)] = (float)(1.0 / 3.0 * (value[IX(N, 0, 0)] + value[IX(N + 1, 1, 0)] + value[IX(N + 1, 0, 1)]));
    value[IX(N + 1, N + 1, 0)] = (float)(1.0 / 3.0 * (value[IX(N, N + 1, 0)] + value[IX(N + 1, N, 0)] + value[IX(N + 1, N + 1, 1)]));

    value[IX(0, 0, N + 1)] = (float)(1.0 / 3.0 * (value[IX(1, 0, N + 1)] + value[IX(0, 1, N + 1)] + value[IX(0, 0, N)]));
    value[IX(0, N + 1, N + 1)] = (float)(1.0 / 3.0 * (value[IX(1, N + 1, N + 1)] + value[IX(0, N, N + 1)] + value[IX(0, N + 1, N)]));

    value[IX(N + 1, 0, N + 1)] = (float)(1.0 / 3.0 * (value[IX(N, 0, N + 1)] + value[IX(N + 1, 1, N + 1)] + value[IX(N + 1, 0, N)]));
    value[IX(N + 1, N + 1, N + 1)] = (float)(1.0 / 3.0 * (value[IX(N, N + 1, N + 1)] + value[IX(N + 1, N, N + 1)] + value[IX(N + 1, N + 1, N)]));
}

void Solver::lin_solve(int b, float* value, float* value_prev, float a, float c)
{
    int count;
    int count_x;
    int count_y;
    int count_z;

    for (count = 0; count < 10; count++)
    {
        for (count_x = 1; count_x <= N; count_x++)
        {
            for (count_y = 1; count_y <= N; count_y++)
            {
                for (count_z = 1; count_z <= N; count_z++)
                {
                    value[IX(count_x, count_y, count_z)] = (value_prev[IX(count_x, count_y, count_z)] + a * (value[IX(count_x - 1, count_y, count_z)] + value[IX(count_x + 1, count_y, count_z)] + value[IX(count_x, count_y - 1, count_z)] + value[IX(count_x, count_y + 1, count_z)] + value[IX(count_x, count_y, count_z - 1)] + value[IX(count_x, count_y, count_z + 1)])) / c;
                }
            }
        }

        boundary_condition(value, b);
    }
}

void Solver::diffuse(int b, float* value, float* value_prev)
{
    float alpha = timeStep * diffuseCoef * N * N * N;

    lin_solve(b, value, value_prev, alpha, 1 + 6 * alpha);
}

void Solver::advect(int b, float* density, float* density_prev, float* velocity_u, float* velocity_v, float* velocity_w)
{
    int count_x;
    int count_y;
    int count_z;

    int i0;
    int j0;
    int k0;

    int i1;
    int j1;
    int k1;

    float x;
    float y;
    float z;

    float s0;
    float t0;
    float s1;
    float t1;
    float u1;
    float u0;

    float dh;

    dh = timeStep * N;

    for (count_x = 1; count_x <= N; count_x++)
    {
        for (count_y = 1; count_y <= N; count_y++)
        {
            for (count_z = 1; count_z <= N; count_z++)
            {
                x = count_x - dh * velocity_u[IX(count_x, count_y, count_z)];
                y = count_y - dh * velocity_v[IX(count_x, count_y, count_z)];
                z = count_z - dh * velocity_w[IX(count_x, count_y, count_z)];

                if (x < 0.5)
                {
                    x = 0.5;
                }

                if (x > N + 0.5)
                {
                    x = (float)(N + 0.5);
                }

                if (y < 0.5)
                {
                    y = 0.5;
                }

                if (y > N + 0.5)
                {
                    y = (float)(N + 0.5);
                }

                if (z < 0.5)
                {
                    z = 0.5;
                }

                if (z > N + 0.5)
                {
                    z = (float)(N + 0.5);
                }

                i0 = (int)x;
                i1 = i0 + 1;
                j0 = (int)y;
                j1 = j0 + 1;
                k0 = (int)z;
                k1 = k0 + 1;

                s1 = x - i0;
                s0 = 1 - s1;
                t1 = y - j0;
                t0 = 1 - t1;
                u1 = z - k0;
                u0 = 1 - u1;

                density[IX(count_x, count_y, count_z)] = s0 * (t0 * u0 * density_prev[IX(i0, j0, k0)] + t1 * u0 * density_prev[IX(i0, j1, k0)] + t0 * u1 * density_prev[IX(i0, j0, k1)] + t1 * u1 * density_prev[IX(i0, j1, k1)]) +
                    s1 * (t0 * u0 * density_prev[IX(i1, j0, k0)] + t1 * u0 * density_prev[IX(i1, j1, k0)] + t0 * u1 * density_prev[IX(i1, j0, k1)] + t1 * u1 * density_prev[IX(i1, j1, k1)]);
            }
        }
    }

    boundary_condition(density, b);
}

void Solver::project(float* velocity_u, float* velocity_v, float* velocity_w, float* p, float* div)
{
    int count_x;
    int count_y;
    int count_z;

    for (count_x = 1; count_x <= N; count_x++)
    {
        for (count_y = 1; count_y <= N; count_y++)
        {
            for (count_z = 1; count_z <= N; count_z++)
            {
                div[IX(count_x, count_y, count_z)] = (float)(-1.0 / 3.0 * ((velocity_u[IX(count_x + 1, count_y, count_z)] - velocity_u[IX(count_x - 1, count_y, count_z)]) / N + (velocity_v[IX(count_x, count_y + 1, count_z)] - velocity_v[IX(count_x, count_y - 1, count_z)]) / N + (velocity_w[IX(count_x, count_y, count_z + 1)] - velocity_w[IX(count_x, count_y, count_z - 1)]) / N));
                p[IX(count_x, count_y, count_z)] = 0;
            }
        }
    }

    boundary_condition(div, 0);
    boundary_condition(p, 0);

    lin_solve(0, p, div, 1, 6);

    for (count_x = 1; count_x <= N; count_x++)
    {
        for (count_y = 1; count_y <= N; count_y++)
        {
            for (count_z = 1; count_z <= N; count_z++)
            {
                velocity_u[IX(count_x, count_y, count_z)] -= 0.5f * N * (p[IX(count_x + 1, count_y, count_z)] - p[IX(count_x - 1, count_y, count_z)]);
                velocity_v[IX(count_x, count_y, count_z)] -= 0.5f * N * (p[IX(count_x, count_y + 1, count_z)] - p[IX(count_x, count_y - 1, count_z)]);
                velocity_w[IX(count_x, count_y, count_z)] -= 0.5f * N * (p[IX(count_x, count_y, count_z + 1)] - p[IX(count_x, count_y, count_z - 1)]);
            }
        }
    }

    boundary_condition(velocity_u, 1);
    boundary_condition(velocity_v, 2);
    boundary_condition(velocity_w, 3);
}

void Solver::get_density(float* density, float* density_prev, float* velocity_u, float* velocity_v, float* velocity_w)
{
    add_source(density, density_prev);

    SWAP(density_prev, density);
    diffuse(0, density, density_prev);

    SWAP(density_prev, density);
    advect(0, density, density_prev, velocity_u, velocity_v, velocity_w);
}

void Solver::get_velocity(float* velocity_u, float* velocity_v, float* velocity_w, float* velocity_u_prev, float* velocity_v_prev, float* velocity_w_prev)
{
    add_source(velocity_u, velocity_u_prev);
    add_source(velocity_v, velocity_v_prev);
    add_source(velocity_w, velocity_w_prev);

    SWAP(velocity_u_prev, velocity_u);
    SWAP(velocity_v_prev, velocity_v);
    SWAP(velocity_w_prev, velocity_w);

    diffuse(1, velocity_u, velocity_u_prev);
    diffuse(2, velocity_v, velocity_v_prev);
    diffuse(3, velocity_w, velocity_w_prev);

    project(velocity_u, velocity_v, velocity_w, velocity_u_prev, velocity_v_prev);


    SWAP(velocity_u_prev, velocity_u);
    SWAP(velocity_v_prev, velocity_v);
    SWAP(velocity_w_prev, velocity_w);

    advect(1, velocity_u, velocity_u_prev, velocity_u_prev, velocity_v_prev, velocity_w_prev);
    advect(2, velocity_v, velocity_v_prev, velocity_u_prev, velocity_v_prev, velocity_w_prev);
    advect(3, velocity_w, velocity_w_prev, velocity_u_prev, velocity_v_prev, velocity_w_prev);

    project(velocity_u, velocity_v, velocity_w, velocity_u_prev, velocity_v_prev);
}

int Solver::wuIndex(int i, int j, int k)
{
    const int eachGridSize = N + 2;
    return i + eachGridSize * j + eachGridSize * eachGridSize * k;
}