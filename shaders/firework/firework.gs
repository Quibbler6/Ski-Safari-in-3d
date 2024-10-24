#version 330 core
#define GRAVITY 0.98

// 输入点的信息
layout (points) in;
// 输出三角形带的信息，每个点扩展为一个四面体
layout (triangle_strip, max_vertices = 6) out;

// 输入的粒子半径
in float r[];
// 当前时间
uniform float t_cur;
// 粒子出生时间
in float t_birth[];
// 模型、视图、投影矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// 粒子速度信息
in vec3 v[];

void main() {
    // 计算粒子经过的时间
    float delta_t = t_cur - t_birth[0];

    // 只处理已经出生的粒子
    if (delta_t >= 0) {
        // 获取粒子的当前位置
        vec4 center = gl_in[0].gl_Position;

        // 根据速度和时间计算位移
        vec3 velocity = v[0];
        float _x = velocity.x * delta_t;
        float _y = velocity.y * delta_t - 0.5 * GRAVITY * delta_t * delta_t;
        float _z = velocity.z * delta_t;
        vec4 translate = vec4(_x, _y, _z, 0);
        // 循环创建三角形带的顶点
        for (int i = 0; i < 6; ++i) {
            vec4 offset = vec4(0);
            // 根据循环索引选择不同的顶点偏移量
            if (i == 1) offset = vec4(0, 0, 0.1 * r[0], 0);
            else if (i == 2) offset = vec4(0.1 * r[0], 0, 0, 0);
            else if (i == 3) offset = vec4(0, 0.1 * r[0], 0, 0);
            else if (i == 4) offset = vec4(-0.1* r[0], -0.1 * r[0], -0.1 * r[0], 0);
            else if (i == 5) offset = vec4(0, 0, 0.1 * r[0], 0);

            // 计算最终的顶点位置并发射
            gl_Position = projection * view * model * (center + translate + offset);
            EmitVertex();
        }

        // 结束当前的三角形带
        EndPrimitive();
    }
}
