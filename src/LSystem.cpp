#include "LSystem.h"
#include "MyRotate.h"

/*
 *函数名称：LSystem()
 *函数功能：初始化在将文法翻译成三维信息的一些基本参数
				包括：1、初始旋转角度dx、dy、dz
					 2、树枝长度length、
					 3、树枝半径radius、
					 4、树叶长度leafLength、
					 5、树叶半径leafRadius
					 6、长度迭代因子lengthFactor
					 7、半径迭代因子radiusFactor
					 8、树枝、树叶数量numTrunks,numLeafs
					 9、字符串格式的树result

 *传递参数：none
 * 返回值： none
 */
LSystem::LSystem() {
	dx = dz = 35.0f;
	dy = 30.0f;
	length = 3.0f;
	radius = 0.15f;
	leafRadius = 0.1f;
	leafLength = 0.1f;
	lengthFactor = 0.7;
	radiusFactor = 0.6;
	numTrunks = numLeafs = 0;
	result = grammar.getResult();
}

LSystem::~LSystem() {
	result.clear();
}

/*
 *函数名称：generateFractal()
 *函数功能：将文法翻译成三维信息并存储这些信息
			通俗的说：通过文法，让树从主干开始“生长”，第一级枝干、第二级枝干、……第level级枝干，最后长出叶子。
					翻译方法：
							1、初始化当前状态（初始状态）
							2、取出文法中的一个字符
								1、'F‘：代表树干
								2、'+‘：代表绕x轴顺时针环绕
								3、'-‘：代表绕x轴逆时针环绕
								4、'*‘：代表绕y轴顺时针环绕
								5、'/‘：代表绕y轴逆时针环绕
								6、'!‘：代表绕z轴顺时针环绕
								7、'@‘：代表绕z轴逆时针环绕
								8、'[‘：入栈操作，表示进入下一级枝干
								9、']‘：出栈操作，表示返回上一级枝干
							   10、'L'：代表叶子
 *传递参数：none
 * 返回值：none
 */
void LSystem::generateFractal(Shader& shader_, glm::mat4& projection, glm::mat4& view, glm::vec3& pos , unsigned int* texture1, unsigned int* texture2) {
	trunks.clear();
	leafs.clear();
	curState.pos = glm::vec3(0, 0, 0);
	curState.dir = glm::vec3(0, 1, 0);
	curState.length = length;
	curState.level = 1;
	curState.radius = radius;
	glm::vec3 trunkColor = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 leafColor = glm::vec3(0.0f, 1.0f, 0.0f);
	//状态栈
	//压入当前的状态，进入下一级枝干，将枝干按照因子系数进行缩小缩小
	//当出栈时代表返回上一级枝干的起点进行其他下一级枝干或继续返回上一级枝干
	stack<State> stacks;


	for (int i = 0; i < result.size(); i++) {
		char c = result[i];
		Trunk tmp;
		//树干
		if (c == 'F') {
			tmp.start = curState.pos;
			curState.pos += curState.dir * (float)curState.length;
			tmp.end = curState.pos;
			tmp.radius = curState.radius;
			trunks.push_back(tmp);

			//Cylinder trunk((float)curState.radius, (float)curState.length, trunkColor);
			Cylinder trunk(tmp.start, tmp.end, tmp.radius, trunkColor);
			glm::mat4 model = glm::mat4(1.0f); 
			model = glm::translate(model, pos);
			//model = calculateRotationMatrix(glm::vec3(0, 1, 0), curState.dir) * model;
			//model = glm::translate(model, tmp.start);
			
			trunk.draw(shader_, projection, view, model, texture1);

			//printf("Trunk's start is (%f, %f, %f), end is (%f, %f, %f), radius is %f\n", tmp.start[0], tmp.start[1], tmp.start[2], tmp.end[0], tmp.end[1], tmp.end[2], tmp.radius);
			continue;
		}
		//绕X轴转dx角度
		else if (c == '+') {
			curState.dir = MyRotate::RotateX(curState.dir, dx);
			continue;
		}
		//绕X轴转-dx角度
		else if (c == '-') {
			curState.dir = MyRotate::RotateX(curState.dir, -dx);
			continue;
		}
		//绕X轴转dy角度
		else if (c == '*') {
			curState.dir = MyRotate::RotateY(curState.dir, dy);
			continue;
		}
		//绕X轴转-dy角度
		else if (c == '/') {
			curState.dir = MyRotate::RotateY(curState.dir, -dy);
			continue;
		}
		//绕X轴转dz角度
		else if (c == '!') {
			curState.dir = MyRotate::RotateZ(curState.dir, dz);
			continue;
		}
		//绕X轴转-dz角度
		else if (c == '@') {
			curState.dir = MyRotate::RotateZ(curState.dir, -dz);
			continue;
		}
		//将当前的状态压入栈中保存
		//将当前的长度和体积按比例减小
		else if (c == '[') {
			stacks.push(curState);
			curState.length *= lengthFactor;
			curState.radius *= radiusFactor;
			continue;
		}
		//出栈
		else if (c == ']') {
			curState = stacks.top();
			stacks.pop();
			continue;
		}
		//叶子节点
		else if (c == 'L') {
			Trunk t = trunks[trunks.size() - 1];
			Leaf l;
			l.dir = curState.dir;//t.end - t.start;
			l.dir = glm::normalize(l.dir);
			l.pos = curState.pos;//t.end;
			l.end = l.pos + curState.dir * (float)leafLength;
			
			/*l.end.x = l.pos.x + l.dir.x * leafLength;
			l.end.y = l.pos.y + l.dir.y * leafLength;
			l.end.z = l.pos.z + l.dir.z * leafLength;*/
			leafs.push_back(l);

			Sphere leaf(leafLength, leafColor);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, l.pos);
			model = glm::translate(model, pos);

			leaf.draw(shader_, projection, view, model, *texture2);
			//printf("Leaf's pos is (%f, %f, %f), end is (%f, %f, %f)\n", l.pos[0], l.pos[1], l.pos[2], l.end[0], l.end[1], l.end[2]);
		}
		else {
			//do nothing
		}
	}
}

glm::mat4 calculateRotationMatrix(const glm::vec3& originalVec, const glm::vec3& targetVec) {
	glm::vec3 normOriginalVec = glm::normalize(originalVec); // 确保原始向量是单位向量
	glm::vec3 normTargetVec = glm::normalize(targetVec);     // 确保目标向量是单位向量

	float cosTheta = glm::dot(normOriginalVec, normTargetVec);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		// 特殊情况，当两个向量方向相反时
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), normOriginalVec);
		if (glm::length(rotationAxis) < 0.01) // 当原始向量与Z轴平行时
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), normOriginalVec);

		rotationAxis = glm::normalize(rotationAxis);
		return glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = glm::cross(normOriginalVec, normTargetVec);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::mat4(
		1 + (rotationAxis.x * rotationAxis.x * invs - cosTheta),
		(rotationAxis.x * rotationAxis.y * invs - rotationAxis.z * s),
		(rotationAxis.x * rotationAxis.z * invs + rotationAxis.y * s),
		0.0f,

		(rotationAxis.y * rotationAxis.x * invs + rotationAxis.z * s),
		1 + (rotationAxis.y * rotationAxis.y * invs - cosTheta),
		(rotationAxis.y * rotationAxis.z * invs - rotationAxis.x * s),
		0.0f,

		(rotationAxis.z * rotationAxis.x * invs - rotationAxis.y * s),
		(rotationAxis.z * rotationAxis.y * invs + rotationAxis.x * s),
		1 + (rotationAxis.z * rotationAxis.z * invs - cosTheta),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}