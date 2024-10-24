# include "Firework.h"

Particle::Particle(glm::vec3 initPosition, glm::vec3 initColor, float initRadius)
    : position(initPosition), color(initColor), radius(initRadius), birthTime(glfwGetTime()), velocity(glm::vec3(0.0f)), isActive(true) {
    // 构造函数初始化列表：使用成员初始化列表设置类成员的初始值
    // 初始化粒子位置、颜色、半径，形成时间为当前时间，初始速度为零，粒子处于活跃状态
}

Firework::Firework(glm::vec3 initPosition, glm::vec3 initColor, glm::vec3 initFlowDirection, float initFlowForce, float initFlowLastTime)
    : isActive(true), validity(0), flowDirection(glm::normalize(initFlowDirection)), flowForce(initFlowForce),
      birthTime(glfwGetTime()), deltaTime(-1), flowLastTime(initFlowLastTime), position(initPosition), color(initColor) {
    // 构造函数初始化列表：使用成员初始化列表设置类成员的初始值
    // 初始化粒子流的位置、颜色、流动方向、流动力度、形成时间、时间间隔、是否活跃和有效性标志

    for (int i = 0; i < PARTICLE_NUM; ++i) {
        particles[i] = Particle(position, color, PARTICLE_SIZE);
        // 使用构造函数初始化每个粒子，位置和颜色从流的属性中继承，半径为全局定义的 PARTICLE_SIZE

        particles[i].velocity = glm::vec3(
            flowForce * (flowDirection.x + NOISE * (rand() % 100) / 100.0),
            flowForce * (flowDirection.y + NOISE * (rand() % 100) / 100.0),
            flowForce * (flowDirection.z + NOISE * (rand() % 100) / 100.0)
        );
        // 为每个粒子生成具有流动方向的随机速度，考虑了噪声

        particles[i].birthTime = glfwGetTime() + (rand() % 100) / 100.0 * flowLastTime / 2;
        // 为每个粒子生成具有随机偏移的形成时间
    }

    for (int i = 0; i < PARTICLE_NUM; ++i) {
        positionRadiusVelocityTime[8 * i] = particles[i].position.x;
        positionRadiusVelocityTime[8 * i + 1] = particles[i].position.y;
        positionRadiusVelocityTime[8 * i + 2] = particles[i].position.z;
        positionRadiusVelocityTime[8 * i + 3] = particles[i].radius;
        positionRadiusVelocityTime[8 * i + 4] = particles[i].velocity.x;
        positionRadiusVelocityTime[8 * i + 5] = particles[i].velocity.y;
        positionRadiusVelocityTime[8 * i + 6] = particles[i].velocity.z;
        positionRadiusVelocityTime[8 * i + 7] = particles[i].birthTime;
        // 为粒子流的每个粒子填充存储位置、半径、速度和形成时间的数组
    }
}

void Firework::update(){
    // 重新设置各个粒子的birthTime
    for (int i = 0; i < PARTICLE_NUM; ++i) {
        particles[i].birthTime = glfwGetTime() + (rand() % 100) / 100.0 * flowLastTime / 2;
    }
    for(int i = 0; i < PARTICLE_NUM; ++i){
        positionRadiusVelocityTime[8 * i + 7] = particles[i].birthTime;
    }
}

void Firework::drawFlow(Shader &shader_, glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model) {
    shader_.use();
    shader_.set_3trans_matrix(glm::value_ptr(projection), glm::value_ptr(view), glm::value_ptr(model));
    shader_.setVec3("fireworkColor", this->color);
    shader_.setFloat("t_cur", glfwGetTime());
    // 设置着色器的投影矩阵、视图矩阵和模型矩阵，并传递当前时间给着色器

    unsigned int VAO_part, VBO_part;
    glGenVertexArrays(1, &VAO_part);
    glBindVertexArray(VAO_part);

    glGenBuffers(1, &VBO_part);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_part);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * PARTICLE_NUM, positionRadiusVelocityTime, GL_STATIC_DRAW);
    // 创建并绑定顶点数组对象（VAO）和顶点缓冲对象（VBO），将粒子属性数据传递给 GPU

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(4 * sizeof(float)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(7 * sizeof(float)));
    // 配置顶点属性指针，指定顶点属性数据在缓冲中的布局

    glDrawArrays(GL_POINTS, 0, PARTICLE_NUM);
    // 使用绘制点的方式渲染粒子
}
