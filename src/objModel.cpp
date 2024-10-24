#include "objModel.h"

Model::Model(const char* path) {
    // 使用 ifstream 打开 OBJ 文件
    std::ifstream objfile(path);
    // 检查文件是否成功打开
    if (!objfile.is_open()) {
        std::cerr << "错误：无法打开文件 " << path << std::endl;
        return;
    }
    // 用于存储行和标记的变量
    std::string line, token;

    // 用于存储顶点、纹理坐标和法线数据的变量
    std::vector<glm::vec3> vertices_vector;
    std::vector<glm::vec2> uvs_vector;
    std::vector<glm::vec3> normals_vector;

    // 用于存储面索引的变量
    std::vector<unsigned int> vertice_indice;
    std::vector<unsigned int> uvs_indice;
    std::vector<unsigned int> normal_indice;

    // 逐行读取文件
    while (std::getline(objfile, line)) {
        // 创建一个字符串流以解析每一行
        std::istringstream linestream(line);

        // 读取行的第一个标记
        linestream >> token;

        // 根据标记检查数据类型
        if (token == "v") {
            // 读取顶点数据
            glm::vec3 vertex_crood;
            linestream >> vertex_crood.x >> vertex_crood.y >> vertex_crood.z;
            vertices_vector.push_back(vertex_crood);
        } 
        else if (token == "vt") {
            // 读取纹理坐标数据
            glm::vec2 texture_crood;
            linestream >> texture_crood.x >> texture_crood.y;
            uvs_vector.push_back(texture_crood);
        } 
        else if (token == "vn") {
            // 读取法线数据
            glm::vec3 norm_vec;
            linestream >> norm_vec.x >> norm_vec.y >> norm_vec.z;
            normals_vector.push_back(norm_vec);
        } 
        else if (token == "f") {
            // 读取面索引数据
            unsigned int vertex_ind, texture_ind, norm_ind;
            char t;
            for (int i = 0; i < 3; i++) {
                linestream >> vertex_ind >> t >> texture_ind >> t >> norm_ind;
                vertice_indice.push_back(vertex_ind - 1);
                uvs_indice.push_back(texture_ind - 1);
                normal_indice.push_back(norm_ind - 1);
            }
        }
        else{
            // 忽略其他数据
            continue;
        }
    }
    // 关闭文件
    objfile.close();

    // 将解析得到的顶点、纹理坐标、法线数据存储到 Model 类的成员变量中
    // 处理顶点数据
    for (unsigned int index : vertice_indice) {
        vertex.push_back(vertices_vector[index]);
    }
    // 处理纹理坐标数据
    for (unsigned int index : uvs_indice) {
        texture.push_back(uvs_vector[index]);
    }
    // 处理法线数据
    for (unsigned int index : normal_indice) {
        norm.push_back(normals_vector[index]);
    }

    // 生成顶点缓冲区对象
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &normalBuffer);

    // 生成顶点数组对象
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 将顶点数据绑定到顶点缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);

    // 将法线数据绑定到法线缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm[0], GL_STATIC_DRAW);

    // 启用顶点属性数组，设置指针
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // 启用法线属性数组，设置指针
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

Model::~Model() {
    glDeleteBuffers(1,&vertexBuffer);
    glDeleteBuffers(1,&normalBuffer);
    glDeleteVertexArrays(1,&VAO);
}

void Model::draw(Shader &shader_ ,glm::mat4 & projection, glm::mat4 &view, glm::mat4 &model) {
    shader_.use();
    shader_.set_3trans_matrix(glm::value_ptr(projection),glm::value_ptr(view),glm::value_ptr(model));
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES,0,vertex.size());
}

void Model::use() {
    glBindVertexArray(VAO);
}